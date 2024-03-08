#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "party.hpp"

#include "steam/steam.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console/console.hpp"
#include "network.hpp"
#include "profile_infos.hpp"
#include "scheduler.hpp"
#include "server_list.hpp"

#include <utils/string.hpp>
#include <utils/info_string.hpp>
#include <utils/hook.hpp>
#include <utils/cryptography.hpp>

namespace party
{
	namespace
	{
		connection_state server_connection_state{};
		std::optional<discord_information> server_discord_info{};

		/*
		struct usermap_file
		{
			std::string extension;
			std::string name;
			bool optional;
		};

		// snake case these names before release
		std::vector<usermap_file> usermap_files =
		{
			{".ff", "usermap_hash", false},
			{"_load.ff", "usermap_load_hash", true},
			{".arena", "usermap_arena_hash", true},
			{".pak", "usermap_pak_hash", true},
		};

		std::vector<usermap_file> mod_files =
		{
			{".ff", "mod_hash", false},
			{"_pre_gfx.ff", "mod_pre_gfx_hash", true},
			{".pak", "mod_pak_hash", true},
		};
		*/

		bool preloaded_map = false;

		void perform_game_initialization()
		{
			command::execute("onlinegame 1", true);
			command::execute("xblive_privatematch 1", true);
			//command::execute("xstartprivateparty", true);
			command::execute("xstartprivatematch", true);
			command::execute("uploadstats", true);
		}

		void connect_to_party(const game::netadr_s& target, const std::string& mapname, const std::string& gametype, int sv_maxclients)
		{
			const auto mode = game::Com_GameMode_GetActiveGameMode();
			if (mode != game::GAME_MODE_MP && mode != game::GAME_MODE_CP)
			{
				return;
			}

			if (game::Live_SyncOnlineDataFlags(0) != 0)
			{
				scheduler::once([=]()
				{
					connect_to_party(target, mapname, gametype, sv_maxclients);
				}, scheduler::pipeline::main, 1s);
				return;
			}

			const auto ui_maxclients = game::Dvar_FindVar("ui_maxclients");
			const auto party_maxplayers = game::Dvar_FindVar("party_maxplayers");
			game::Dvar_SetInt(ui_maxclients, sv_maxclients);
			game::Dvar_SetInt(party_maxplayers, sv_maxclients);

			command::execute(utils::string::va("ui_mapname %s", mapname.data()), true);
			command::execute(utils::string::va("ui_gametype %s", gametype.data()), true);

			perform_game_initialization();

			// setup agent count
			utils::hook::invoke<void>(0xC19B00_b, gametype.data());

			preloaded_map = false;

			// connect
			char session_info[0x100] = {};
			game::CL_MainMP_ConnectAndPreloadMap(0, reinterpret_cast<void*>(session_info), &target, mapname.data(), gametype.data());
		}

		void start_map_for_party(std::string map_name)
		{
			[[maybe_unused]]auto* mapname = game::Dvar_FindVar("ui_mapname");
			auto* gametype = game::Dvar_FindVar("ui_gametype");
			auto* clients = game::Dvar_FindVar("ui_maxclients");
			auto* private_clients = game::Dvar_FindVar("ui_privateClients");
			auto* hardcore = game::Dvar_FindVar("ui_hardcore");

			if (game::Com_FrontEnd_IsInFrontEnd())
			{
				// Com_FrontEndScene_ShutdownAndDisable
				utils::hook::invoke<void>(0x5AEFB0_b);
			}

			game::SV_CmdsMP_StartMapForParty(
				map_name.data(),
				gametype->current.string,
				clients->current.integer,
				private_clients->current.integer,
				hardcore->current.enabled,
				false,
				false);
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

		void com_gamestart_beginclient_stub(const char* mapname, const char* gametype, char a3)
		{
			if (preloaded_map)
			{
				// Com_GameStart_BeginClient
				utils::hook::invoke<void>(0x5B0130_b, mapname, gametype, a3);
			}
			else
			{
				// DB_LoadLevelXAssets
				utils::hook::invoke<void>(0x3B9C90_b, mapname, 0);
			}
		}

		void com_restart_for_frontend_stub()
		{
			if (preloaded_map)
			{
				// Com_RestartForFrontend
				utils::hook::invoke<void>(0xBAF0B0_b);
			}
			else
			{
				// Com_Restart
				utils::hook::invoke<void>(0xBAF0A0_b);
			}
		}

		utils::hook::detour sv_start_map_for_party_hook;
		void sv_start_map_for_party_stub(const char* map, const char* game_type, int client_count, int agent_count, bool hardcore,
			bool map_is_preloaded, bool migrate)
		{
			profile_infos::xuid::clear_xuids();

			preloaded_map = map_is_preloaded;
			sv_start_map_for_party_hook.invoke<void>(map, game_type, client_count, agent_count, hardcore, map_is_preloaded, migrate);
		}

		void reset_mem_stuff(game::SvServerInitSettings* init_settings)
		{
			if (init_settings->maxClientCount != *game::svs_numclients)
			{
				game::Com_Error(game::ERR_DROP, reinterpret_cast<const char*>(0x1512140_b));
			}

			if (!init_settings->serverThreadStartup)
			{
				if (!init_settings->isRestart)
				{
					// Nav_AllocNavPower
					memset(&*reinterpret_cast<__int64*>(0x4E3A490_b + 8), 0, 0x78680ui64 - 8);
				}
			}
		}

		void reset_mem_stuff_stub(utils::hook::assembler& a)
		{
			a.pushad64();

			a.mov(rcx, rbx);
			a.call_aligned(reset_mem_stuff);

			a.popad64();

			a.jmp(0xC563E2_b);
		}
	}

	void start_map(const std::string& mapname, bool dev)
	{
		if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
		{
			console::info("Starting sp map: %s\n", mapname.data());
			command::execute(utils::string::va("spmap %s", mapname.data()), false);
			return;
		}

		if (game::Live_SyncOnlineDataFlags(0) != 0)
		{
			scheduler::once([=]()
			{
				start_map(mapname, dev);
			}, scheduler::pipeline::main, 1s);
			return;
		}

		if (mapname.empty())
		{
			console::error("No map specified.\n");
			return;
		}

		if (!game::SV_MapExists(mapname.data()))
		{
			console::error("Map \"%s\" doesn't exist.\n", mapname.data());
			return;
		}

		if (!game::Com_GameMode_SupportsMap(mapname.data()))
		{
			console::error("Cannot load map \"%s\" in current game mode.\n", mapname.data());
			return;
		}

		auto* current_mapname = game::Dvar_FindVar("mapname");

		command::execute((dev ? "seta sv_cheats 1" : "seta sv_cheats 0"), true);

		if (current_mapname && utils::string::to_lower(current_mapname->current.string) ==
			utils::string::to_lower(mapname) && (game::SV_Loaded() && !game::Com_FrontEndScene_IsActive()))
		{
			console::info("Restarting map: %s\n", mapname.data());
			command::execute("map_restart", false);
			return;
		}

		auto* gametype = game::Dvar_FindVar("g_gametype");
		if (gametype && gametype->current.string)
		{
			command::execute(utils::string::va("ui_gametype %s", gametype->current.string), true);
		}

		perform_game_initialization();

		console::info("Starting map: %s\n", mapname.data());

		start_map_for_party(mapname);
		return;
	}

	int get_client_num_by_name(const std::string& name)
	{
		for (unsigned int i = 0; !name.empty() && i < *game::svs_numclients; ++i)
		{
			if (game::g_entities[i].client)
			{
				char client_name[32] = { 0 };
				strncpy_s(client_name, game::g_entities[i].client->name, sizeof(client_name));
				game::I_CleanStr(client_name);

				if (client_name == name)
				{
					return i;
				}
			}
		}
		return -1;
	}

	void reset_server_connection_state()
	{
		server_connection_state = {};
	}

	int get_client_count()
	{
		auto count = 0;
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1)
			{
				++count;
			}
		}

		return count;
	}

	int get_bot_count()
	{
		auto count = 0;
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1 &&
				game::SV_BotIsBot(i))
			{
				++count;
			}
		}

		return count;
	}

	void connect(const game::netadr_s& target)
	{
		command::execute("luiOpenPopup AcceptingInvite", false);

		profile_infos::xuid::clear_xuids();
		profile_infos::clear_profile_infos();

		server_connection_state.host = target;
		server_connection_state.challenge = utils::cryptography::random::get_challenge();
		server_connection_state.hostDefined = true;

		network::send(target, "getInfo", server_connection_state.challenge);
	}

	void info_response_error(const std::string& error)
	{
		console::error("%s\n", error.data());
		command::execute("luiLeaveMenu AcceptingInvite", false);
		game::Com_SetLocalizedErrorMessage(error.data(), "MENU_NOTICE");
	}

	connection_state get_server_connection_state()
	{
		return server_connection_state;
	}

	std::optional<discord_information> get_server_discord_info()
	{
		return server_discord_info;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
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

			utils::hook::set<uint8_t>(0xC562FD_b, 0xEB); // allow mapname to be changed while server is running

			utils::hook::nop(0xA7A8DF_b, 5); // R_SyncRenderThread inside CL_MainMp_PreloadMap ( freezes )

			utils::hook::call(0x9AFE84_b, com_gamestart_beginclient_stub); // blackscreen issue on connect
			utils::hook::call(0x9B4077_b, com_gamestart_beginclient_stub); // may not be necessary (map rotate)
			utils::hook::call(0x9B404A_b, com_restart_for_frontend_stub); // may not be necessary (map rotate)

			sv_start_map_for_party_hook.create(0xC4D150_b, sv_start_map_for_party_stub);

			utils::hook::nop(0xC563C3_b, 12); // far jump = 12 bytes
			utils::hook::jump(0xC563C3_b, utils::hook::assemble(reset_mem_stuff_stub), true);

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

			command::add("reconnect", [](const command::params& argument)
			{
				if (!server_connection_state.hostDefined)
				{
					console::info("Cannot connect to server.\n");
					return;
				}

				if (game::CL_IsGameClientActive(0))
				{
					command::execute("disconnect");
					command::execute("reconnect");
				}
				else
				{
					connect(server_connection_state.host);
				}
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
				info.set("sv_wwwBaseUrl", get_dvar_string("sv_wwwBaseUrl"));
				info.set("sv_discordImageUrl", get_dvar_string("sv_discordImageUrl"));
				info.set("sv_discordImageText", get_dvar_string("sv_discordImageText"));

				/*
				if (!fastfiles::is_stock_map(mapname))
				{
					for (const auto& file : usermap_files)
					{
						const auto path = get_usermap_file_path(mapname, file.extension);
						const auto hash = get_file_hash(path);
						info.set(file.name, hash);
					}
				}

				const auto fs_game = get_dvar_string("fs_game");
				info.set("fs_game", fs_game);

				if (!fs_game.empty())
				{
					for (const auto& file : mod_files)
					{
						const auto hash = get_file_hash(utils::string::va("%s/mod%s", 
							fs_game.data(), file.extension.data()));
						info.set(file.name, hash);
					}
				}
				*/

				network::send(target, "infoResponse", info.build(), '\n');
			});

			if (game::environment::is_dedi())
			{
				return;
			}

			network::on("infoResponse", [](const game::netadr_s& target, const std::string_view& data)
			{
				const utils::info_string info{ data };
				server_list::handle_info_response(target, info);

				if (server_connection_state.host != target)
				{
					return;
				}

				const auto protocol = info.get("protocol");
				if (std::atoi(protocol.data()) != PROTOCOL)
				{
					info_response_error("Connection failed: Invalid protocol.");
					return;
				}

				if (info.get("challenge") != server_connection_state.challenge)
				{
					info_response_error("Connection failed: Invalid challenge.");
					return;
				}

				const auto gamename = info.get("gamename");
				if (gamename != "IW7"s)
				{
					info_response_error("Connection failed: Invalid gamename.");
					return;
				}

				const auto playmode = info.get("playmode");
				if (game::GameModeType(std::atoi(playmode.data())) != game::Com_GameMode_GetActiveGameMode())
				{
					info_response_error("Connection failed: Invalid playmode.");
					return;
				}

				const auto sv_running = info.get("sv_running");
				if (!std::atoi(sv_running.data()))
				{
					info_response_error("Connection failed: Server not running.");
					return;
				}

				const auto mapname = info.get("mapname");
				if (mapname.empty())
				{
					info_response_error("Connection failed: Invalid map.");
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
					info_response_error("Connection failed: Invalid sv_maxclients.");
					return;
				}

				server_connection_state.base_url = info.get("sv_wwwBaseUrl");
				/*
				if (download_files(target, info, false))
				{
					return;
				}
				*/

				server_connection_state.motd = info.get("sv_motd");
				server_connection_state.max_clients = std::stoi(sv_maxclients_str);

				const auto profile_info = profile_infos::get_profile_info();
				if (!profile_info.has_value())
				{
					console::error("profile_info has no value to send, possible undefined behavior ahead\n");
				}

				const auto profile_info_value = profile_info.value_or(profile_infos::profile_info{});
				profile_infos::send_profile_info(target, steam::SteamUser()->GetSteamID().bits, profile_info_value);

				discord_information discord_info{};
				discord_info.image = info.get("sv_discordImageUrl");
				discord_info.image_text = info.get("sv_discordImageText");
				if (!discord_info.image.empty() || !discord_info.image_text.empty())
				{
					server_discord_info.emplace(discord_info);
				}

				connect_to_party(target, mapname, gametype, sv_maxclients);
			});
		}
	};
}

REGISTER_COMPONENT(party::component)