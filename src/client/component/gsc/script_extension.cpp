#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/dvars.hpp"
#include "game/game.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/function.hpp"
#include "game/scripting/functions.hpp"
#include "game/scripting/lua/error.hpp"

#include <utils/hook.hpp>

#include "component/command.hpp"
#include "component/console.hpp"
#include "component/scripting.hpp"
#include "component/logfile.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/resolver.hpp>

#include "script_extension.hpp"
#include "script_error.hpp"

namespace gsc
{
	std::uint16_t function_id_start = 0x30A;
	std::uint16_t method_id_start = 0x8586;

	builtin_function func_table[0x1000];
	builtin_method meth_table[0x1000];

	const game::dvar_t* developer_script = nullptr;

	namespace
	{
		std::unordered_map<std::uint32_t, script_function> functions;
		std::unordered_map<std::uint32_t, script_method> methods;

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

		void execute_custom_function(const std::uint16_t id)
		{
			auto error = false;

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
			catch (const std::exception& e)
			{
				error = true;
				force_error_print = true;
				gsc_error_msg = e.what();
			}

			if (error)
			{
				game::Scr_ErrorInternal();
			}
		}

		void execute_custom_method(const std::uint16_t id)
		{
			auto error = false;

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
			catch (const std::exception& e)
			{
				error = true;
				force_error_print = true;
				gsc_error_msg = e.what();
			}

			if (error)
			{
				game::Scr_ErrorInternal();
			}
		}

		void vm_call_builtin_function_stub(builtin_function function)
		{
			const auto function_id = get_function_id();

			if (!functions.contains(function_id))
			{
				if (function == nullptr)
				{
					force_error_print = true;
					gsc_error_msg = "function doesn't exist";
					game::Scr_ErrorInternal();
				}
				else
				{
					function();
				}
			}
			else
			{
				execute_custom_function(function_id);
			}
		}

		game::scr_entref_t get_entity_id_stub(std::uint32_t ent_id)
		{
			const auto ref = game::Scr_GetEntityIdRef(ent_id);
			saved_ent_ref = ref;
			return ref;
		}

		void vm_call_builtin_method_stub(builtin_method method)
		{
			const auto function_id = get_function_id();

			if (!methods.contains(function_id))
			{
				if (method == nullptr)
				{
					force_error_print = true;
					gsc_error_msg = "method doesn't exist";
					game::Scr_ErrorInternal();
				}
				else
				{
					method(saved_ent_ref);
				}
			}
			else
			{
				execute_custom_method(function_id);
			}
		}

		void builtin_call_error(const std::string& error)
		{
			const auto function_id = get_function_id();

			if (function_id > 0x1000)
			{
				console::warn("in call to builtin method \"%s\"%s",
					xsk::gsc::iw7::resolver::method_name(function_id).data(), error.data());
			}
			else
			{
				console::warn("in call to builtin function \"%s\"%s",
					xsk::gsc::iw7::resolver::function_name(function_id).data(), error.data());
			}
		}

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				return {xsk::gsc::iw7::resolver::opcode_name(opcode)};
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
			if (!developer_script->current.enabled && !force_error_print)
			{
				utils::hook::invoke<void>(0x510C80_b, mark_pos);
				return;
			}

			console::warn("*********** script runtime error *************\n");

			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(0x6B22940_b);
			const std::string error_str = gsc_error_msg.has_value()
				? utils::string::va(": %s", gsc_error_msg.value().data())
				: "";

			if ((opcode_id >= 0x1A && opcode_id <= 0x20) || (opcode_id >= 0xA9 && opcode_id <= 0xAF))
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
			utils::hook::invoke<void*>(0x510C80_b, mark_pos);
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

	namespace function
	{
		void add(const std::string& name, script_function function)
		{
			if (xsk::gsc::iw7::resolver::find_function(name))
			{
				const auto id = xsk::gsc::iw7::resolver::function_id(name);
				functions[id] = function;
			}
			else
			{
				const auto id = ++function_id_start;
				xsk::gsc::iw7::resolver::add_function(name, static_cast<std::uint16_t>(id));
				functions[id] = function;
			}
		}
	}

	namespace method
	{
		void add(const std::string& name, script_method method)
		{
			if (xsk::gsc::iw7::resolver::find_method(name))
			{
				const auto id = xsk::gsc::iw7::resolver::method_id(name);
				methods[id] = method;
			}
			else
			{
				const auto id = ++method_id_start;
				xsk::gsc::iw7::resolver::add_method(name, static_cast<std::uint16_t>(id));
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
			utils::hook::set<uint32_t>(0xBFD16B_b, 0x1000); // change builtin func count

			utils::hook::set<uint32_t>(0xBFD172_b + 4, static_cast<uint32_t>(reverse_b((&func_table))));
			//utils::hook::set<uint32_t>(0xC0E5CE_b + 4, static_cast<uint32_t>(reverse_b((&func_table))));  // maybe wrong
			utils::hook::inject(0xBFD5A1_b + 3, &func_table);
			utils::hook::set<uint32_t>(0xBFD595_b, sizeof(func_table));
			
			//utils::hook::set<uint32_t>(0xC0E8E0_b + 4, static_cast<uint32_t>(reverse_b(&meth_table))); // maybe wrong
			utils::hook::set<uint32_t>(0xBFD182_b + 4, static_cast<uint32_t>(reverse_b((&meth_table)))); 
			utils::hook::inject(0xBFD5AF_b + 3, &meth_table);
			utils::hook::set<uint32_t>(0xBFD5B6_b, sizeof(meth_table));

			developer_script = game::Dvar_RegisterBool("developer_script", false, 0, "Enable developer script comments");

			utils::hook::call(0xC0E8C7_b, get_entity_id_stub);
			
			/* GOT ISSUE WITH THESE - nullsub doesn't exist on IW */
			/*utils::hook::nop(0xC0E5CE_b, 8);
			utils::hook::call(0xC0E5CE_b, vm_call_builtin_function_stub);*/
			//utils::hook::call(SELECT_VALUE(0x3CBA46_b, 0x512AA6_b), vm_call_builtin_method_stub);

			utils::hook::call(0xC0F8C1_b, vm_error_stub);

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
					throw std::runtime_error("replaceFunc: parameter 1 must be a function");
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
				const auto cmd = args[0].as<std::string>();
				command::execute(cmd, false);

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
		}
	};
}

//REGISTER_COMPONENT(gsc::extension)