#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "party.hpp"

#include "steam/steam.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console.hpp"
#include "network.hpp"
#include "scheduler.hpp"

#include <utils/string.hpp>
#include <utils/info_string.hpp>
#include <utils/hook.hpp>
#include <utils/cryptography.hpp>

namespace party
{
	namespace
	{
		struct
		{
			game::netadr_s host{};
			std::string challenge{};
			bool hostDefined{ false };
		} connect_state;

		void perform_game_initialization()
		{
			command::execute("onlinegame 1", true);
			//command::execute("exec default_xboxlive.cfg", true);
			command::execute("xblive_privatematch 1", true);
			//command::execute("xstopparty 0", true);
			//command::execute("xstartprivateparty 0", true);
			command::execute("xstartprivatematch 0", true);
			command::execute("uploadstats", true);
			//command::execute("entitlements_delay 0", true);
		}

		void connect_to_party(const game::netadr_s& target, const std::string& mapname, const std::string& gametype, int sv_maxclients)
		{
			if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_MP &&
				game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_CP)
			{
				return;
			}

			if (game::Live_SyncOnlineDataFlags(0) != 0)
			{
				// initialize the game after onlinedataflags is 32 (workaround)
				if (game::Live_SyncOnlineDataFlags(0) == 32)
				{
					scheduler::once([=]()
					{
						command::execute("xstartprivateparty", true);
						command::execute("disconnect", true); // 32 -> 0

						connect_to_party(target, mapname, gametype, sv_maxclients);
					}, scheduler::pipeline::main, 1s);
					return;
				}
				else
				{
					scheduler::once([=]()
					{
						connect_to_party(target, mapname, gametype, sv_maxclients);
					}, scheduler::pipeline::main, 1s);
					return;
				}
			}

			const auto ui_maxclients = game::Dvar_FindVar("ui_maxclients");
			game::Dvar_SetInt(ui_maxclients, sv_maxclients);

			perform_game_initialization();

			// setup agent count
			utils::hook::invoke<void>(0xC19B00_b, gametype.data());

			// connect
			char session_info[0x100] = {};
			game::CL_MainMP_ConnectAndPreloadMap(0, reinterpret_cast<void*>(session_info), &target, mapname.data(), gametype.data());
		}

		std::string get_dvar_string(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.string)
			{
				return dvar_value->current.string;
			}

			return {};
		}

		int get_dvar_int(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.integer)
			{
				return dvar_value->current.integer;
			}

			return -1;
		}

		bool get_dvar_bool(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.enabled)
			{
				return dvar_value->current.enabled;
			}

			return false;
		}
	}

	void start_map_for_party()
	{
		auto* mapname = game::Dvar_FindVar("ui_mapname");
		auto* gametype = game::Dvar_FindVar("ui_gametype");
		auto* clients = game::Dvar_FindVar("ui_maxclients");
		auto* private_clients = game::Dvar_FindVar("ui_privateClients");
		auto* hardcore = game::Dvar_FindVar("ui_hardcore");

		//utils::hook::invoke<void>(0x9D8900_b, game::Party_GetActiveParty(), true);

		utils::hook::invoke<void>(0xE4DDC0_b); // Sys_WaitWorkerCmds
		utils::hook::invoke<void>(0xBAFFD0_b, ""); // Com_ShutdownInternal
		game::SV_CmdsMP_StartMapForParty(
			mapname->current.string,
			gametype->current.string,
			clients->current.integer,
			private_clients->current.integer,
			hardcore->current.enabled,
			false,
			false);
	}

	bool start_map(const std::string& mapname, bool dev)
	{
		if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
		{
			console::info("Starting sp map: %s\n", mapname.data());
			command::execute(utils::string::va("spmap %s", mapname.data()), false);
			return true;
		}

		if (mapname.empty())
		{
			console::error("No map specified.\n");
			return false;
		}

		if (!game::SV_MapExists(mapname.data()))
		{
			console::error("Map \"%s\" doesn't exist.\n", mapname.data());
			return false;
		}

		if (!game::Com_GameMode_SupportsMap(mapname.data()))
		{
			console::error("Cannot load map \"%s\" in current game mode.\n", mapname.data());
			return false;
		}

		auto* current_mapname = game::Dvar_FindVar("mapname");

		command::execute((dev ? "seta sv_cheats 1" : "seta sv_cheats 0"), true);

		if (current_mapname && utils::string::to_lower(current_mapname->current.string) ==
			utils::string::to_lower(mapname) && (game::SV_Loaded() && !game::Com_FrontEndScene_IsActive()))
		{
			console::info("Restarting map: %s\n", mapname.data());
			command::execute("map_restart", false);
			return true;
		}

		command::execute(utils::string::va("ui_mapname %s", mapname.data()), true);

		console::info("Starting map: %s\n", mapname.data());

		perform_game_initialization();

		game::Cbuf_AddCall(0, start_map_for_party);
		return true;
	}

	int get_client_num_by_name(const std::string& name)
	{
		return 0;
	}

	int get_client_count()
	{
		return 0;
	}

	int get_bot_count()
	{
		return 0;
	}

	void connect(const game::netadr_s& target)
	{
		//command::execute("lui_open_popup popup_acceptinginvite", false);

		printf("connect called...\n");

		connect_state.host = target;
		connect_state.challenge = utils::cryptography::random::get_challenge();
		connect_state.hostDefined = true;

		network::send(target, "getInfo", connect_state.challenge);
	}

	void info_response_error(const std::string& error)
	{
		console::error("%s\n", error.data());
		//if (game::Menu_IsMenuOpenAndVisible(0, "popup_acceptinginvite"))
		//{
		//	command::execute("lui_close popup_acceptinginvite", false);
		//}

		game::Com_SetLocalizedErrorMessage(error.data(), "MENU_NOTICE");
		//*reinterpret_cast<int*>(0x0) = 1;
	}


	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("live", []()
			{
				console::info("%d\n", game::Live_SyncOnlineDataFlags(0));
			});

			command::add("connstate", []()
			{
				console::info("%d\n", game::clientUIActives[0].connectionState);
			});

			static const char* a1 = "map_sp";
			static const char* a2 = "map_restart_sp";
			static const char* a3 = "fast_restart_sp";

			// patch singleplayer "map" -> "map_sp"
			utils::hook::set(0x1BBA800_b + 0, a1);
			utils::hook::set(0x1BBA800_b + 24, a1);
			utils::hook::set(0x1BBA800_b + 56, a1);

			// patch singleplayer map_restart -> "map_restart_sp"
			utils::hook::set(0x1BBA740_b + 0, a2);
			utils::hook::set(0x1BBA740_b + 24, a2);
			utils::hook::set(0x1BBA740_b + 56, a2);

			// patch singleplayer fast_restart -> "fast_restart_sp"
			utils::hook::set(0x1BBA700_b + 0, a3);
			utils::hook::set(0x1BBA700_b + 24, a3);
			utils::hook::set(0x1BBA700_b + 56, a3);

			command::add("map", [](const command::params& args)
			{
				if (args.size() != 2)
				{
					return;
				}

				if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
				{
					command::execute(utils::string::va("spmap %s", args.get(1)));
					return;
				}

				start_map(args.get(1), false);
			});

			command::add("devmap", [](const command::params& args)
			{
				if (args.size() != 2)
				{
					return;
				}

				if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
				{
					command::execute(utils::string::va("spmap %s", args.get(1)));
					return;
				}

				start_map(args.get(1), true);
			});

			command::add("map_restart", []()
			{
				if (!game::SV_Loaded() || game::Com_FrontEnd_IsInFrontEnd())
				{
					return;
				}

				if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
				{
					game::Cbuf_AddCall(0, game::SV_CmdsSP_MapRestart_f);
					return;
				}

				game::SV_CmdsMP_RequestMapRestart(1, 0);
			});

			command::add("fast_restart", []()
			{
				if (!game::SV_Loaded() || game::Com_FrontEnd_IsInFrontEnd())
				{
					return;
				}

				if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
				{
					game::Cbuf_AddCall(0, game::SV_CmdsSP_FastRestart_f);
					return;
				}

				game::SV_CmdsMP_RequestMapRestart(0, 0);
			});

			command::add("connect", [](const command::params& argument)
			{
				if (argument.size() != 2)
				{
					return;
				}

				if (game::CL_IsGameClientActive(0))
				{
					console::info("Cannot use \"connect\" command while ingame.\n");
					return;
				}

				game::netadr_s target{};
				if (game::NET_StringToAdr(argument[1], &target))
				{
					connect(target);
				}
			});

			network::on("getInfo", [](const game::netadr_s& target, const std::string_view& data)
			{
				printf("getInfo called...\n");

				utils::info_string info{};
				info.set("challenge", std::string{ data });
				info.set("gamename", "IW7");
				info.set("hostname", get_dvar_string("sv_hostname"));
				info.set("gametype", get_dvar_string("g_gametype"));
				info.set("sv_motd", get_dvar_string("sv_motd"));
				info.set("xuid", utils::string::va("%llX", steam::SteamUser()->GetSteamID().bits));
				info.set("mapname", get_dvar_string("mapname"));
				info.set("isPrivate", get_dvar_string("g_password").empty() ? "0" : "1");
				info.set("clients", utils::string::va("%i", get_client_count()));
				info.set("bots", utils::string::va("%i", get_bot_count()));
				info.set("sv_maxclients", utils::string::va("%i", *game::svs_numclients));
				info.set("protocol", utils::string::va("%i", PROTOCOL));
				info.set("playmode", utils::string::va("%i", game::Com_GameMode_GetActiveGameMode()));
				info.set("sv_running", utils::string::va("%i", get_dvar_bool("sv_running") && !game::Com_FrontEndScene_IsActive()));
				info.set("dedicated", utils::string::va("%i", get_dvar_bool("dedicated")));

				network::send(target, "infoResponse", info.build(), '\n');
			});

			network::on("infoResponse", [](const game::netadr_s& target, const std::string_view& data)
			{
				printf("infoResponse called...\n");

				const utils::info_string info{ data };
				//server_list::handle_info_response(target, info);

				if (connect_state.host != target)
				{
					return;
				}

				if (info.get("challenge") != connect_state.challenge)
				{
					info_response_error("Invalid challenge.");
					return;
				}

				const auto gamename = info.get("gamename");
				if (gamename != "IW7"s)
				{
					info_response_error("Invalid gamename.");
					return;
				}

				const auto playmode = info.get("playmode");
				if (game::GameModeType(std::atoi(playmode.data())) != game::Com_GameMode_GetActiveGameMode())
				{
					info_response_error("Invalid playmode.");
					return;
				}

				const auto sv_running = info.get("sv_running");
				if (!std::atoi(sv_running.data()))
				{
					info_response_error("Server not running.");
					return;
				}

				const auto mapname = info.get("mapname");
				if (mapname.empty())
				{
					info_response_error("Invalid map.");
					return;
				}

				const auto gametype = info.get("gametype");
				if (gametype.empty())
				{
					info_response_error("Connection failed: Invalid gametype.");
					return;
				}

				const auto sv_maxclients_str = info.get("sv_maxclients");
				const auto sv_maxclients = std::atoi(sv_maxclients_str.data());
				if (!sv_maxclients)
				{
					info_response_error("Invalid sv_maxclients.");
					return;
				}

				//party::sv_motd = info.get("sv_motd");
				//party::sv_maxclients = std::stoi(info.get("sv_maxclients"));

				connect_to_party(target, mapname, gametype, sv_maxclients);
			});
		}
	};
}

REGISTER_COMPONENT(party::component)