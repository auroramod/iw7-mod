#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "command.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "game/scripting/execution.hpp"

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

			auto& com_num_console_lines = *game::com_num_console_lines;
			auto* com_console_lines = game::com_console_lines.get();

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
			auto& com_num_console_lines = *game::com_num_console_lines;
			auto* com_console_lines = game::com_console_lines.get();

			for (int i = 0; i < com_num_console_lines; i++)
			{
				game::Cmd_TokenizeString(com_console_lines[i]);

				// only +set dvar value
				if (game::Cmd_Argc() >= 3 && (game::Cmd_Argv(0) == "set"s || game::Cmd_Argv(0) == "seta"s))
				{
					const std::string& key = game::Cmd_Argv(1);
					const std::string& value = game::Cmd_Argv(2);

					const auto* dvar = game::Dvar_FindVar(key.data());
					if (dvar)
					{
						game::Dvar_SetCommand(key.data(), value.data());
					}
					else
					{
						dvars::callback::on_register(key, [key, value]()
						{
							game::Dvar_SetCommand(key.data(), value.data());
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
					const std::string current = game::Dvar_ValueToString(dvar, dvar->current);
					const std::string reset = game::Dvar_ValueToString(dvar, dvar->reset);

					console::info("\"%s\" is: \"%s\" default: \"%s\" checksum: %d type: %i\n",
						dvars::dvar_get_name(dvar).data(), current.data(), reset.data(), dvar->checksum, dvar->type);

					const auto dvar_info = dvars::dvar_get_description(dvar);

					if (!dvar_info.empty())
						console::info("%s\n", dvar_info.data());

					console::info("   %s\n", dvars::dvar_get_domain(dvar->type, dvar->domain).data());
				}
				else
				{
					char command[0x1000]{};
					game::Dvar_GetCombinedString(command, 1);
					game::Dvar_SetCommand(args[0], command);
				}

				return dvar;
			}

			return 0;
		}

		void cmd_give(const int client_num, const std::vector<std::string>& params)
		{
			if (params.size() < 2)
			{
				game::shared::client_println(client_num, "You did not specify a weapon name");
				return;
			}

			try
			{
				const auto& arg = params[1];
				const auto player = scripting::entity({ static_cast<uint16_t>(client_num), 0 });

				if (arg == "ammo")
				{
					const auto weapon = player.call("getcurrentweapon").as<std::string>();
					player.call("givemaxammo", { weapon });
				}
				else if (arg == "allammo")
				{
					const auto weapons = player.call("getweaponslistall").as<scripting::array>();
					for (auto i = 0; i < weapons.size(); i++)
					{
						player.call("givemaxammo", { weapons[i] });
					}
				}
				else if (arg == "health")
				{
					if (params.size() > 2)
					{
						const auto amount = atoi(params[2].data());
						const auto health = player.get("health").as<int>();
						player.set("health", { health + amount });
					}
					else
					{
						const auto amount = atoi(game::Dvar_FindVar("scr_player_maxhealth")->current.string);
						player.set("health", { amount });
					}
				}
				else if (arg == "all")
				{
					const auto type = game::XAssetType::ASSET_TYPE_WEAPON;
					game::DB_EnumXAssets(type, [&player, type](const game::XAssetHeader header)
					{
						const auto asset = game::XAsset{ type, header };
						const auto asset_name = game::DB_GetXAssetName(&asset);

						player.call("giveweapon", { asset_name });
					});
				}
				else
				{
					player.call("giveweapon", { arg });
					player.call("switchtoweapon", { arg });
				}
			}
			catch (...)
			{
			}
		}

		void cmd_drop_weapon(int client_num)
		{
			try
			{
				const auto player = scripting::entity({ static_cast<uint16_t>(client_num), 0 });
				const auto weapon = player.call("getcurrentweapon");
				player.call("dropitem", { weapon });
			}
			catch (...)
			{
			}
		}

		void cmd_take(int client_num, const std::vector<std::string>& params)
		{
			if (params.size() < 2)
			{
				game::shared::client_println(client_num, "You did not specify a weapon name");
				return;
			}

			const auto& weapon = params[1];

			try
			{
				const auto player = scripting::entity({ static_cast<uint16_t>(client_num), 0 });
				if (weapon == "all"s)
				{
					player.call("takeallweapons");
				}
				else
				{
					player.call("takeweapon", { weapon });
				}
			}
			catch (...)
			{
			}
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
			utils::hook::jump(0x140BB1DC0, dvar_command_stub, true);
			client_command_mp_hook.create(0x140B105D0, &client_command_mp);
			client_command_sp_hook.create(0x140483130, &client_command_sp);

			parse_commandline_hook.create(0x140C039F0, parse_commandline); // SL_Init

			add_commands();
		}

	private:
		static void add_commands()
		{
			add("quit", []()
			{
				*game::g_quitRequested = true;
			});

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

			add("bindlist", []()
			{
				game::Key_Bindlist_f();
			});

			add_sv("god", [](const int client_num, const params_sv&)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 1;
				game::shared::client_println(client_num,
					game::g_entities[client_num].flags & 1
					? "GAME_GODMODE_ON"
					: "GAME_GODMODE_OFF");
			});

			add_sv("demigod", [](const int client_num, const params_sv&)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 2;
				game::shared::client_println(client_num,
					game::g_entities[client_num].flags & 2
					? "GAME_DEMI_GODMODE_ON"
					: "GAME_DEMI_GODMODE_OFF");
			});

			add_sv("notarget", [](const int client_num, const params_sv&)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				game::g_entities[client_num].flags ^= 4;
				game::shared::client_println(client_num,
					game::g_entities[client_num].flags & 4
					? "GAME_NOTARGETON"
					: "GAME_NOTARGETOFF");
			});

			add_sv("noclip", [](const int client_num, const params_sv&)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				game::g_entities[client_num].client->flags ^= 1;
				game::shared::client_println(client_num,
					game::g_entities[client_num].client->flags & 1
					? "GAME_NOCLIPON"
					: "GAME_NOCLIPOFF");
			});

			add_sv("ufo", [](const int client_num, const params_sv&)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				game::g_entities[client_num].client->flags ^= 2;
				game::shared::client_println(client_num,
					game::g_entities[client_num].client->flags & 2
					? "GAME_UFOON"
					: "GAME_UFOOFF");
			});

			add_sv("give", [](const int client_num, const params_sv& params)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				cmd_give(client_num, params.get_all());
			});

			add_sv("dropweapon", [](const int client_num, const params_sv& params)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				cmd_drop_weapon(client_num);
			});

			add_sv("take", [](const int client_num, const params_sv& params)
			{
				if (!game::shared::cheats_ok(client_num, true))
				{
					return;
				}

				cmd_take(client_num, params.get_all());
			});
		}
	};
}

REGISTER_COMPONENT(command::component)