#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "console.hpp"
#include "game_console.hpp"
//#include "fastfiles.hpp"
#include "scheduler.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/memory.hpp>
#include <utils/io.hpp>

namespace command
{
	namespace
	{
		utils::hook::detour client_command_hook;
		utils::hook::detour parse_commandline_hook;

		std::unordered_map<std::string, std::function<void(params&)>> handlers;
		std::unordered_map<std::string, std::function<void(int, params_sv&)>> handlers_sv;

		void main_handler()
		{
			params params = {};

			const auto command = utils::string::to_lower(params[0]);
			if (handlers.find(command) != handlers.end())
			{
				handlers[command](params);
			}
		}

		void client_command(const int client_num)
		{
			params_sv params = {};

			const auto command = utils::string::to_lower(params[0]);
			if (handlers_sv.find(command) != handlers_sv.end())
			{
				handlers_sv[command](client_num, params);
			}

			client_command_hook.invoke<void>(client_num);
		}

		// Shamelessly stolen from Quake3
		// https://github.com/id-Software/Quake-III-Arena/blob/dbe4ddb10315479fc00086f08e25d968b4b43c49/code/qcommon/common.c#L364
		void parse_command_line()
		{
			static auto parsed = false;
			if (parsed)
			{
				return;
			}

			static std::string comand_line_buffer = GetCommandLineA();
			auto* command_line = comand_line_buffer.data();

			auto& com_num_console_lines = *reinterpret_cast<int*>(0x6006DB0_b);
			auto* com_console_lines = reinterpret_cast<char**>(0x6006DC0_b);

			auto inq = false;
			com_console_lines[0] = command_line;
			com_num_console_lines = 0;

			while (*command_line)
			{
				if (*command_line == '"')
				{
					inq = !inq;
				}
				// look for a + separating character
				// if commandLine came from a file, we might have real line seperators
				if ((*command_line == '+' && !inq) || *command_line == '\n' || *command_line == '\r')
				{
					if (com_num_console_lines == 0x20) // MAX_CONSOLE_LINES
					{
						break;
					}
					com_console_lines[com_num_console_lines] = command_line + 1;
					com_num_console_lines++;
					*command_line = '\0';
				}
				command_line++;
			}
			parsed = true;
		}

		game::dvar_t* dvar_command_stub()
		{
			const params args;

			if (args.size() <= 0)
			{
				return 0;
			}

			const auto dvar = game::Dvar_FindVar(args[0]);

			if (dvar)
			{
				if (args.size() == 1)
				{
					const auto current = game::Dvar_ValueToString(dvar, dvar->current);
					const auto reset = game::Dvar_ValueToString(dvar, dvar->reset);

					console::info("\"%s\" is: \"%s\" default: \"%s\" hash: 0x%08lX type: %i\n",
						args[0], current, reset, dvar->hash, dvar->type);

					const auto dvar_info = dvars::dvar_get_description(dvar);

					if (!dvar_info.empty())
						console::info("%s\n", dvar_info.data());

					console::info("   %s\n", dvars::dvar_get_domain(dvar->type, dvar->domain).data());
				}
				else
				{
					char command[0x1000] = { 0 };
					game::Dvar_GetCombinedString(command, 1);
					game::Dvar_SetCommand(args[0], command);
				}

				return dvar;
			}

			return 0;
		}

		void client_println(int client_num, const std::string& text)
		{
			game::SV_GameSendServerCommand(client_num, game::SV_CMD_RELIABLE,
				utils::string::va("f \"%s\"", text.data()));
		}

		bool check_cheats(int client_num)
		{
			if (!game::Dvar_FindVar("sv_cheats")->current.enabled)
			{
				client_println(client_num, "Cheats are not enabled on this server");
				return false;
			}

			return true;
		}
	}

	params::params()
		: nesting_(game::cmd_args->nesting)
	{
	}

	int params::size() const
	{
		return game::cmd_args->argc[this->nesting_];
	}

	const char* params::get(const int index) const
	{
		if (index >= this->size())
		{
			return "";
		}

		return game::cmd_args->argv[this->nesting_][index];
	}

	std::string params::join(const int index) const
	{
		std::string result = {};

		for (auto i = index; i < this->size(); i++)
		{
			if (i > index) result.append(" ");
			result.append(this->get(i));
		}
		return result;
	}

	std::vector<std::string> params::get_all() const
	{
		std::vector<std::string> params_;
		for (auto i = 0; i < this->size(); i++)
		{
			params_.push_back(this->get(i));
		}
		return params_;
	}

	params_sv::params_sv()
		: nesting_(game::sv_cmd_args->nesting)
	{
	}

	int params_sv::size() const
	{
		return game::sv_cmd_args->argc[this->nesting_];
	}

	const char* params_sv::get(const int index) const
	{
		if (index >= this->size())
		{
			return "";
		}

		return game::sv_cmd_args->argv[this->nesting_][index];
	}

	std::string params_sv::join(const int index) const
	{
		std::string result = {};

		for (auto i = index; i < this->size(); i++)
		{
			if (i > index) result.append(" ");
			result.append(this->get(i));
		}
		return result;
	}

	std::vector<std::string> params_sv::get_all() const
	{
		std::vector<std::string> params_;
		for (auto i = 0; i < this->size(); i++)
		{
			params_.push_back(this->get(i));
		}
		return params_;
	}

	void add_raw(const char* name, void (*callback)())
	{
		game::Cmd_AddCommandInternal(name, callback, utils::memory::get_allocator()->allocate<game::cmd_function_s>());
	}

	void add(const char* name, const std::function<void(const params&)>& callback)
	{
		const auto command = utils::string::to_lower(name);

		if (handlers.find(command) == handlers.end())
			add_raw(name, main_handler);

		handlers[command] = callback;
	}

	void add(const char* name, const std::function<void()>& callback)
	{
		add(name, [callback](const params&)
		{
			callback();
		});
	}

	void add_sv(const char* name, std::function<void(int, const params_sv&)> callback)
	{
		// doing this so the sv command would show up in the console
		add_raw(name, nullptr);

		const auto command = utils::string::to_lower(name);

		if (handlers_sv.find(command) == handlers_sv.end())
			handlers_sv[command] = std::move(callback);
	}

	void execute(std::string command, const bool sync)
	{
		command += "\n";

		if (sync)
		{
			game::Cmd_ExecuteSingleCommand(0, 0, command.data());
		}
		else
		{
			game::Cbuf_AddText(0, command.data());
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0xBB1DC0_b, dvar_command_stub, true);
			client_command_hook.create(0xB105D0_b, &client_command);

			add_commands();
		}

	private:
		static void add_commands()
		{
			add("quit", game::Com_Quit_f);
			add("crash", []()
			{
				*reinterpret_cast<int*>(1) = 0;
			});
		}
	};
}

REGISTER_COMPONENT(command::component)