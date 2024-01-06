#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "component/command.hpp"
#include "component/console/console.hpp"
#include "component/dvars.hpp"
#include "component/scripting.hpp"

#include "script_error.hpp"
#include "script_extension.hpp"
#include "script_loading.hpp"

#include <utils/hook.hpp>

namespace gsc
{
	std::uint16_t function_id_start = 0x326;
	std::uint16_t method_id_start = 0x85DB;

	constexpr size_t func_table_count = 0x1000;
	constexpr size_t meth_table_count = 0x1000;

	builtin_function func_table[func_table_count]{};
	builtin_method meth_table[meth_table_count]{};

	const game::dvar_t* developer_script = nullptr;

	namespace
	{
		std::unordered_map<std::uint16_t, script_function> functions;
		std::unordered_map<std::uint16_t, script_method> methods;

		bool force_error_print = false;
		std::optional<std::string> gsc_error_msg;
		game::scr_entref_t saved_ent_ref;

		function_args get_arguments()
		{
			std::vector<scripting::script_value> args;

			for (auto i = 0; static_cast<std::uint32_t>(i) < game::scr_VmPub->outparamcount; ++i)
			{
				const auto value = game::scr_VmPub->top[-i];
				args.push_back(value);
			}

			return args;
		}

		void return_value(const scripting::script_value& value)
		{
			if (game::scr_VmPub->outparamcount)
			{
				game::Scr_ClearOutParams();
			}

			scripting::push_value(value);
		}

		std::uint16_t get_function_id()
		{
			const auto pos = game::scr_function_stack->pos;
			return *reinterpret_cast<std::uint16_t*>(
				reinterpret_cast<size_t>(pos - 2));
		}

		game::scr_entref_t get_entity_id_stub(std::uint32_t ent_id)
		{
			const auto ref = game::Scr_GetEntityIdRef(ent_id);
			saved_ent_ref = ref;
			return ref;
		}

		void execute_custom_function(const std::uint16_t id)
		{
			try
			{
				const auto& function = functions[id];
				const auto result = function(get_arguments());
				const auto type = result.get_raw().type;

				if (type)
				{
					return_value(result);
				}
			}
			catch (const std::exception& ex)
			{
				scr_error(ex.what());
			}
		}

		void vm_call_builtin_function_internal(int function_id)
		{
			const auto custom_function_id = static_cast<std::uint16_t>(function_id); // cast for gsc-tool & our custom func map
			const auto custom = functions.contains(custom_function_id);
			if (custom)
			{
				execute_custom_function(custom_function_id);
				return;
			}

			builtin_function func = func_table[function_id - 1]; // game does this for the stock func table
			if (func == nullptr)
			{
				console::error(utils::string::va("builtin function \"%s\" doesn't exist", gsc_ctx->func_name(custom_function_id).data())); // scr_error crashes
				return;
			}

			func();
		}

		void* vm_call_builtin_function_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.pushad64();
				a.push(rcx);
				a.mov(rcx, rcx); // function id is already in rcx
				a.call_aligned(vm_call_builtin_function_internal);
				a.pop(rcx);
				a.popad64();

				a.jmp(0xC0E8F9_b);
			});
		}

		void execute_custom_method(const std::uint16_t id)
		{
			try
			{
				const auto& method = methods[id];
				const auto result = method(saved_ent_ref, get_arguments());
				const auto type = result.get_raw().type;

				if (type)
				{
					return_value(result);
				}
			}
			catch (const std::exception& ex)
			{
				scr_error(ex.what());
			}
		}

		void vm_call_builtin_method_stub(builtin_method meth)
		{
			const auto method_id = get_function_id();
			const auto custom = methods.contains(static_cast<std::uint16_t>(method_id));
			if (custom)
			{
				execute_custom_method(method_id);
				return;
			}

			if (meth == nullptr)
			{
				console::error(utils::string::va("builtin method \"%s\" doesn't exist", gsc_ctx->meth_name(method_id).data()), true); // scr_error crashes
				return;
			}

			meth(saved_ent_ref);
		}

		void builtin_call_error(const std::string& error)
		{
			const auto function_id = get_function_id();

			if (function_id > 0x1000)
			{
				console::warn("in call to builtin method \"%s\"%s", gsc_ctx->meth_name(function_id).data(), error.data());
			}
			else
			{
				console::warn("in call to builtin function \"%s\"%s", gsc_ctx->func_name(function_id).data(), error.data());
			}
		}

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				const auto index = gsc_ctx->opcode_enum(opcode);
				return { gsc_ctx->opcode_name(index) };
			}
			catch (...)
			{
				return {};
			}
		}

		void print_callstack()
		{
			for (auto frame = game::scr_VmPub->function_frame; frame != game::scr_VmPub->function_frame_start; --frame)
			{
				const auto pos = frame == game::scr_VmPub->function_frame ? game::scr_function_stack->pos : frame->fs.pos;
				const auto function = find_function(frame->fs.pos);

				if (function.has_value())
				{
					console::warn("\tat function \"%s\" in file \"%s.gsc\"\n", function.value().first.data(), function.value().second.data());
				}
				else
				{
					console::warn("\tat unknown location %p\n", pos);
				}
			}
		}

		void vm_error_stub(int mark_pos)
		{
			const bool dev_script = developer_script ? developer_script->current.enabled : false;

			if (!dev_script && !force_error_print)
			{
				utils::hook::invoke<void>(0x510C80_b, mark_pos);
				return;
			}

			console::warn("*********** script runtime error *************\n");

			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(0x6B22940_b);
			const std::string error_str = gsc_error_msg.has_value()
				? utils::string::va(": %s", gsc_error_msg.value().data())
				: "";

			if ((opcode_id >= 0x1A && opcode_id <= 0x20) || (opcode_id >= 0xA8 && opcode_id <= 0xAE))
			{
				builtin_call_error(error_str);
			}
			else
			{
				const auto opcode = get_opcode_name(opcode_id);
				if (opcode.has_value())
				{
					console::warn("while processing instruction %s%s\n", opcode.value().data(), error_str.data());
				}
				else
				{
					console::warn("while processing instruction 0x%X%s\n", opcode_id, error_str.data());
				}
			}

			force_error_print = false;
			gsc_error_msg = {};

			print_callstack();
			console::warn("**********************************************\n");
			utils::hook::invoke<void>(0x510C80_b, mark_pos);
		}

		void print(const function_args& args)
		{
			std::string buffer{};

			for (auto i = 0u; i < args.size(); ++i)
			{
				const auto str = args[i].to_string();
				buffer.append(str);
				buffer.append("\t");
			}
			console::info("%s\n", buffer.data());
		}

		scripting::script_value typeof(const function_args& args)
		{
			return args[0].type_name();
		}
	}

	void scr_error(const char* error, const bool force_print)
	{
		force_error_print = force_print;
		gsc_error_msg = error;

		game::Scr_ErrorInternal();
	}

	namespace function
	{
		void add(const std::string& name, script_function function)
		{
			if (gsc_ctx->func_exists(name))
			{
				const auto id = gsc_ctx->func_id(name);
				functions[id] = function;
			}
			else
			{
				const auto id = ++function_id_start;
				gsc_ctx->func_add(name, id);
				functions[id] = function;
			}
		}
	}

	namespace method
	{
		void add(const std::string& name, script_method method)
		{
			if (gsc_ctx->meth_exists(name))
			{
				const auto id = gsc_ctx->meth_id(name);
				methods[id] = method;
			}
			else
			{
				const auto id = ++method_id_start;
				gsc_ctx->meth_add(name, static_cast<std::uint16_t>(id));
				methods[id] = method;
			}
		}
	}

	function_args::function_args(std::vector<scripting::script_value> values)
		: values_(values)
	{
	}

	std::uint32_t function_args::size() const
	{
		return static_cast<std::uint32_t>(this->values_.size());
	}

	std::vector<scripting::script_value> function_args::get_raw() const
	{
		return this->values_;
	}

	scripting::value_wrap function_args::get(const int index) const
	{
		if (index >= this->values_.size())
		{
			throw std::runtime_error(utils::string::va("parameter %d does not exist", index));
		}

		return {this->values_[index], index};
	}

	class extension final : public component_interface
	{
	public:
		void post_unpack() override
		{
			developer_script = game::Dvar_RegisterBool("developer_script", true, 0, "Enable developer script comments"); // enable by default for now

			utils::hook::set<uint32_t>(0xBFD16B_b + 1, func_table_count); // change builtin func count
			utils::hook::set<uint32_t>(0xBFD172_b + 4, static_cast<uint32_t>(reverse_b((&func_table))));
			utils::hook::nop(0xC0E5CE_b, 12); // nop the call & jmp at the end of call_builtin
			utils::hook::jump(0xC0E5CE_b, vm_call_builtin_function_stub(), true);

			utils::hook::inject(0xBFD5A1_b + 3, &func_table);
			utils::hook::set<uint32_t>(0xBFD595_b + 2, sizeof(func_table));

			function::add("print", [](const function_args& args)
			{
				console::info(args[0].as<std::string>().data());
				return scripting::script_value{};
			});
			
			//utils::hook::set<uint32_t>(0xBFD182_b + 4,
			//	static_cast<uint32_t>(reverse_b((&meth_table))));
			//utils::hook::set<uint32_t>(0xC0E8F2_b + 3, // could be wrong..
			//	static_cast<uint32_t>(reverse_b(&meth_table)));
			//utils::hook::inject(0xBFD5AF_b + 3, &meth_table);
			//utils::hook::set<uint32_t>(0xBFD5B6_b + 2, sizeof(meth_table));

			/*
			utils::hook::call(SELECT_VALUE(0x3CBA12_b, 0x512A72_b), get_entity_id_stub);
			utils::hook::nop(SELECT_VALUE(0x3CBA46_b, 0x512AA6_b), 6);
			utils::hook::nop(SELECT_VALUE(0x3CBA4E_b, 0x512AAE_b), 2);
			utils::hook::call(SELECT_VALUE(0x3CBA46_b, 0x512AA6_b), vm_call_builtin_method_stub);
			*/

			utils::hook::call(0xC0F8C1_b, vm_error_stub); // LargeLocalResetToMark

			/*
			if (game::environment::is_dedi())
			{
				function::add("isusingmatchrulesdata", [](const function_args& args)
				{
					// return 0 so the game doesn't override the cfg
					return 0;
				});
			}

			function::add("print", [](const function_args& args)
			{
				print(args);
				return scripting::script_value{};
			});

			function::add("println", [](const function_args& args)
			{
				print(args);
				return scripting::script_value{};
			});

			function::add("assert", [](const function_args& args)
			{
				const auto expr = args[0].as<int>();
				if (!expr)
				{
					throw std::runtime_error("assert fail");
				}

				return scripting::script_value{};
			});

			function::add("assertex", [](const function_args& args)
			{
				const auto expr = args[0].as<int>();
				if (!expr)
				{
					const auto error = args[1].as<std::string>();
					throw std::runtime_error(error);
				}

				return scripting::script_value{};
			});

			function::add("getfunction", [](const function_args& args)
			{
				const auto filename = args[0].as<std::string>();
				const auto function = args[1].as<std::string>();

				if (!scripting::script_function_table[filename].contains(function))
				{
					throw std::runtime_error("function not found");
				}

				return scripting::function{scripting::script_function_table[filename][function]};
			});

			function::add("replacefunc", [](const function_args& args)
			{
				const auto what = args[0].get_raw();
				const auto with = args[1].get_raw();

				if (what.type != game::VAR_FUNCTION || with.type != game::VAR_FUNCTION)
				{
					throw std::runtime_error("replacefunc: parameter 1 must be a function");
				}

				logfile::set_gsc_hook(what.u.codePosValue, with.u.codePosValue);

				return scripting::script_value{};
			});

			function::add("toupper", [](const function_args& args)
			{
				const auto string = args[0].as<std::string>();
				return utils::string::to_upper(string);
			});

			function::add("logprint", [](const function_args& args)
			{
				std::string buffer{};

				for (auto i = 0u; i < args.size(); ++i)
				{
					const auto string = args[i].as<std::string>();
					buffer.append(string);
				}

				game::G_LogPrintf("%s", buffer.data());

				return scripting::script_value{};
			});

			function::add("executecommand", [](const function_args& args)
			{
				command::execute(args[0].as<std::string>(), false);

				return scripting::script_value{};
			});

			function::add("typeof", typeof);
			function::add("type", typeof);
			*/
		}
	};
}

REGISTER_COMPONENT(gsc::extension)
