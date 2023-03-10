#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "command.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "console/console.hpp"
#include "game_console.hpp"
#include "scheduler.hpp"
#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/memory.hpp>
#include <utils/io.hpp>

namespace command
{
	namespace
	{
		utils::hook::detour client_command_mp_hook;
		utils::hook::detour client_command_sp_hook;
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

		void client_command_mp(const int client_num)
		{
			params_sv params = {};

			const auto command = utils::string::to_lower(params[0]);
			if (handlers_sv.find(command) != handlers_sv.end())
			{
				handlers_sv[command](client_num, params);
			}

			client_command_mp_hook.invoke<void>(client_num);
		}

		void client_command_sp(const int client_num, const char* s)
		{
			game::SV_Cmd_TokenizeString(s);
			params_sv params = {};

			const auto command = utils::string::to_lower(s);
			if (handlers_sv.find(command) != handlers_sv.end())
			{
				handlers_sv[command](client_num, params);
			}
			game::SV_Cmd_EndTokenizedString();

			client_command_sp_hook.invoke<void>(client_num, s);
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

		void parse_startup_variables()
		{
			auto& com_num_console_lines = *reinterpret_cast<int*>(0x6006DB0_b);
			auto* com_console_lines = reinterpret_cast<char**>(0x6006DC0_b);

			for (int i = 0; i < com_num_console_lines; i++)
			{
				game::Cmd_TokenizeString(com_console_lines[i]);

				// only +set dvar value
				if (game::Cmd_Argc() >= 3 && (game::Cmd_Argv(0) == "set"s || game::Cmd_Argv(0) == "seta"s))
				{
					const std::string& dvar_name = game::Cmd_Argv(1);
					const std::string& value = game::Cmd_Argv(2);

					const auto* dvar = game::Dvar_FindVar(dvar_name.data());
					if (dvar)
					{
						game::Dvar_SetCommand(dvar_name.data(), value.data());
					}
					else
					{
						dvars::callback::on_register(dvar_name, [dvar_name, value]()
						{
							game::Dvar_SetCommand(dvar_name.data(), value.data());
						});
					}
				}

				game::Cmd_EndTokenizeString();
			}
		}

		void parse_commandline()
		{
			parse_command_line();
			parse_startup_variables();

			parse_commandline_hook.invoke<void>();
		}

		game::dvar_t* dvar_command_stub()
		{
			const params args;

			if (args.size() <= 0)
			{
				return 0;
			}

			auto* dvar = game::Dvar_FindVar(args[0]);
			if (dvar == nullptr)
			{
				dvar = game::Dvar_FindMalleableVar(atoi(args[0]));
			}

			if (dvar)
			{
				if (args.size() == 1)
				{
					const auto current = game::Dvar_ValueToString(dvar, dvar->current);
					const auto reset = game::Dvar_ValueToString(dvar, dvar->reset);

					console::info("\"%s\" is: \"%s\" default: \"%s\" checksum: %d type: %i\n",
						dvars::dvar_get_name(dvar).data(), current, reset, dvar->checksum, dvar->type);

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
			if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
			{
				game::CG_Utils_GameMessage(client_num, text.data(), 0); // why is nothing printed?
			}
			else
			{
				game::SV_GameSendServerCommand(client_num, game::SV_CMD_RELIABLE,
					utils::string::va("f \"%s\"", text.data()));
			}
		}

		bool cheats_ok(int client_num)
		{
			if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
			{
				return true;
			}

			const auto sv_cheats = game::Dvar_FindVar("sv_cheats");
			if (!sv_cheats || !sv_cheats->current.enabled)
			{
				client_println(client_num, "GAME_CHEATSNOTENABLED");
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
			client_command_mp_hook.create(0xB105D0_b, &client_command_mp);
			client_command_sp_hook.create(0x483130_b, &client_command_sp);

			parse_commandline_hook.create(0xF2F67B_b, parse_commandline);

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

			add("noMode", []()
			{
				game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_NONE);
			});

			add("spMode", []()
			{
				game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_SP);
			});

			add("mpMode", []()
			{
				game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_MP);
			});

			add("cpMode", []()
			{
				game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_CP);
			});

			add("director_cut", [](const params& params)
			{
				if (params.size() == 2)
				{
					const auto value = static_cast<int>(atoi(params.get(1)));
					utils::hook::set<int>(0x5ECB830_b, value);
				}
				else
				{
					const auto command = params.get(0);
					console::error("Incorrect number of arguments for \"%s\".\n", command);
				}
			});

			add_sv("god", [](const int client_num, const params_sv&)
			{
				if (!cheats_ok(client_num))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 1;
				client_println(client_num,
					game::g_entities[client_num].flags & 1
					? "GAME_GODMODE_ON"
					: "GAME_GODMODE_OFF");
			});

			add_sv("demigod", [](const int client_num, const params_sv&)
			{
				if (!cheats_ok(client_num))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 2;
				client_println(client_num,
					game::g_entities[client_num].flags & 2
					? "GAME_DEMI_GODMODE_ON"
					: "GAME_DEMI_GODMODE_OFF");
			});

			add_sv("notarget", [](const int client_num, const params_sv&)
			{
				if (!cheats_ok(client_num))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 4;
				client_println(client_num,
					game::g_entities[client_num].flags & 4
					? "GAME_NOTARGETON"
					: "GAME_NOTARGETOFF");
			});

			add_sv("noclip", [](const int client_num, const params_sv&)
			{
				if (!cheats_ok(client_num))
				{
					return;
				}

				game::g_entities[client_num].client->flags ^= 1;
				client_println(client_num,
					game::g_entities[client_num].client->flags & 1
					? "GAME_NOCLIPON"
					: "GAME_NOCLIPOFF");
			});

			add_sv("ufo", [](const int client_num, const params_sv&)
			{
				if (!cheats_ok(client_num))
				{
					return;
				}

				game::g_entities[client_num].client->flags ^= 2;
				client_println(client_num,
					game::g_entities[client_num].client->flags & 2
					? "GAME_UFOON"
					: "GAME_UFOOFF");
			});
		}
	};
}

REGISTER_COMPONENT(command::component)