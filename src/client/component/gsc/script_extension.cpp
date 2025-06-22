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
	std::uint16_t function_id_start = 806;
	std::uint16_t method_id_start = 1483 + 0x8000;

	builtin_function func_table[func_table_count]{};
	builtin_method meth_table[meth_table_count]{};

	const game::dvar_t* developer_script = nullptr;

	namespace
	{
		std::unordered_map<std::uint16_t, script_function> functions;
		std::unordered_map<std::uint16_t, script_method> methods;

		std::unordered_map<const char*, const char*> vm_execute_hooks;
		const char* target_function = nullptr;

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

		void vm_call_builtin_function_internal()
		{
			const std::uint16_t function_id = get_function_id();
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
				scr_error(utils::string::va("builtin function \"%s\" doesn't exist", gsc_ctx->func_name(function_id).data()), true);
				return;
			}

			func();
		}

		void execute_custom_method(const std::uint16_t id, game::scr_entref_t ent_ref)
		{
			try
			{
				const auto& method = methods[id];
				const auto result = method(ent_ref, get_arguments());
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

		int meth_function_id = 0;
		void vm_call_builtin_method_internal(game::scr_entref_t ent_ref)
		{
			const auto custom_function_id = static_cast<std::uint16_t>(meth_function_id); // cast for gsc-tool & our custom method map
			const auto custom = methods.contains(custom_function_id);
			if (custom)
			{
				execute_custom_method(custom_function_id, ent_ref);
				return;
			}

			builtin_method meth = meth_table[meth_function_id - 0x8000];
			if (meth == nullptr)
			{
				scr_error(utils::string::va("builtin method \"%s\" doesn't exist", gsc_ctx->meth_name(custom_function_id).data()), true);
				return;
			}

			meth(ent_ref);
		}

		void save_meth_func_id([[maybe_unused]] game::scr_entref_t ent_ref, int function_id)
		{
			meth_function_id = function_id;
		}

		void vm_call_builtin_method_stub(utils::hook::assembler& a)
		{
			a.pushad64();
			a.push(rdx);
			a.push(ecx);
			a.mov(rdx, rdi); // function id is stored in rdi
			a.mov(ecx, ebx); // ent ref is stored in ebx
			a.call_aligned(save_meth_func_id);
			a.pop(rdx);
			a.pop(ecx);
			a.popad64();

			a.jmp(0x140C0E8F2);
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

		bool get_replaced_pos(const char* pos)
		{
			if (vm_execute_hooks.contains(pos))
			{
				target_function = vm_execute_hooks[pos];
				return true;
			}
			return false;
		}

		void vm_execute_stub(utils::hook::assembler& a)
		{
			const auto replace = a.newLabel();
			const auto end = a.newLabel();

			a.pushad64();

			a.mov(rcx, rsi);
			a.call_aligned(get_replaced_pos);

			a.cmp(al, 0);
			a.jne(replace);

			a.popad64();
			a.jmp(end);

			a.bind(end);

			a.movzx(r15d, byte_ptr(rsi));
			a.inc(rsi);
			a.mov(dword_ptr(rbp, 0x94), r15d);

			a.jmp(0x140C0D0B2);

			a.bind(replace);

			a.popad64();
			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&target_function)));
			a.mov(rsi, rax);
			a.jmp(end);
		}

		void replace(std::string& str, const std::string& from, const std::string& to)
		{
			const auto start_pos = str.find(from);

			if (start_pos == std::string::npos)
			{
				return;
			}

			str.replace(start_pos, from.length(), to);
		}
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

	std::uint16_t get_function_id()
	{
		const auto pos = game::scr_function_stack->pos;
		return *reinterpret_cast<std::uint16_t*>(
			reinterpret_cast<size_t>(pos - 2));
	}

	class extension final : public component_interface
	{
	public:
		void post_unpack() override
		{
#ifdef DEBUG
			developer_script = game::Dvar_RegisterBool("developer_script", true, 0, "Enable developer script comments");
#else
			developer_script = game::Dvar_RegisterBool("developer_script", false, 0, "Enable developer script comments");
#endif

			utils::hook::set<uint32_t>(0x140BFD16B + 1, func_table_count); // change builtin func count
			utils::hook::set<uint32_t>(0x140BFD172 + 4, static_cast<uint32_t>(reverse_b((&func_table))));
			utils::hook::inject(0x140BFD5A1 + 3, &func_table);
			utils::hook::set<uint32_t>(0x140BFD595 + 2, sizeof(func_table));
			utils::hook::nop(0x140C0E5CE, 7);
			utils::hook::call(0x140C0E5CE, vm_call_builtin_function_internal);

			utils::hook::set<uint32_t>(0x140BFD182 + 4, static_cast<uint32_t>(reverse_b((&meth_table))));
			utils::hook::inject(0x140BFD5AF + 3, &meth_table);
			utils::hook::set<uint32_t>(0x140BFD5B6 + 2, sizeof(meth_table));
			utils::hook::nop(0x140C0E8EB, 14); // nop the lea & call at the end of call_builtin_method
			utils::hook::jump(0x140C0E8EB, utils::hook::assemble(vm_call_builtin_method_stub));
			utils::hook::call(0x140C0E8F2, vm_call_builtin_method_internal);

			utils::hook::jump(0x140C0D0A4, utils::hook::assemble(vm_execute_stub), true);

			scripting::on_shutdown([](bool free_scripts, bool post_shutdown)
			{
				if (free_scripts && post_shutdown)
				{
					vm_execute_hooks.clear();
				}
			});

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

				if (what.type != game::VAR_FUNCTION)
				{
					throw std::runtime_error("parameter 1 must be a function");
				}

				if (with.type != game::VAR_FUNCTION)
				{
					throw std::runtime_error("parameter 2 must be a function");
				}

				vm_execute_hooks[what.u.codePosValue] = with.u.codePosValue;

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

			function::add("say", [](const function_args& args)
			{
				const auto message = args[0].as<std::string>();
				game::SV_GameSendServerCommand(-1, game::SV_CMD_CAN_IGNORE, utils::string::va("%c \"%s\"", 84, message.data()));
				return scripting::script_value{};
			});

			method::add("tell", [](const game::scr_entref_t ent, const function_args& args)
			{
				if (ent.classnum != 0)
				{
					throw std::runtime_error("Invalid entity");
				}

				const auto client = ent.entnum;

				if (game::g_entities[client].client == nullptr)
				{
					throw std::runtime_error("Not a player entity");
				}

				const auto message = args[0].as<std::string>();
				game::SV_GameSendServerCommand(client, game::SV_CMD_CAN_IGNORE, utils::string::va("%c \"%s\"", 84, message.data()));

				return scripting::script_value{};
			});

			method::add("test_method", [](game::scr_entref_t ent_ref, const function_args& args)
			{
				print(args);
				return scripting::script_value{};
			});

			function::add("va", [](const function_args& args)
			{
				auto fmt = args[0].as<std::string>();

				for (auto i = 1u; i < args.size(); i++)
				{
					const auto arg = args[i].to_string();
					replace(fmt, "%s", arg);
				}

				return fmt;
			});

			function::add("strstartswith", [](const function_args& args)
			{
				auto string = args[0].as<std::string>();
				auto substring = args[1].as<std::string>();
				return string.starts_with(substring);
			});
		}
	};
}

REGISTER_COMPONENT(gsc::extension)
