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
		/*struct moddl_file
		{
			std::string extension;
			std::string name;
			bool optional;
		};

		std::vector<moddl_file> mod_files =
		{
			{".ff", "mod_hash", false},
		};

		std::unordered_map<std::string, std::string> hash_cache;

		std::string get_file_hash(const std::string& file)
		{
			const auto iter = hash_cache.find(file);
			if (iter != hash_cache.end())
			{
				return iter->second;
			}

			const auto hash = utils::hash::get_file_hash(file);
			if (!hash.empty())
			{
				hash_cache.insert(std::make_pair(file, hash));
			}

			return hash;
		}

		// generate hashes so they are cached
		void generate_hashes(const std::string& mapname)
		{
			// mod
			const auto fs_game = get_dvar_string("fs_game");
			if (!fs_game.empty())
			{
				for (const auto& file : mod_files)
				{
					const auto path = std::format("{}\\mod{}", fs_game, file.extension);
					get_file_hash(path);
				}
			}
		}

		bool check_download_mod(const utils::info_string& info, std::vector<download::file_t>& files)
		{
			static const auto fs_game = game::Dvar_FindVar("fs_game");
			const auto client_fs_game = utils::string::to_lower(fs_game->current.string);
			const auto server_fs_game = utils::string::to_lower(info.get("fs_game"));

			if (server_fs_game.empty() && client_fs_game.empty())
			{
				return false;
			}

			if (server_fs_game.empty() && !client_fs_game.empty())
			{
				mods::set_mod("");
				return true;
			}

			if (!server_fs_game.starts_with("mods/") || server_fs_game.contains('.') || server_fs_game.contains("::"))
			{
				throw std::runtime_error(utils::string::va("Invalid server fs_game value '%s'", server_fs_game.data()));
			}

			auto needs_restart = false;
			for (const auto& file : mod_files)
			{
				const auto source_hash = info.get(file.name);
				if (source_hash.empty())
				{
					if (file.optional)
					{
						continue;
					}

					throw std::runtime_error(
						utils::string::va("Server '%s' is empty", file.name.data()));
				}

				const auto file_path = server_fs_game + "/mod" + file.extension;
				auto has_to_download = !utils::io::file_exists(file_path);

				if (!has_to_download)
				{
					const auto hash = get_file_hash(file_path);
					console::debug("has_to_download = %s != %s\n", source_hash.data(), hash.data());
					has_to_download = source_hash != hash;
				}

				if (has_to_download)
				{
					// unload mod before downloading it
					if (client_fs_game == server_fs_game)
					{
						mods::set_mod("", true);
						return true;
					}
					else
					{
						files.emplace_back(file_path, source_hash);
					}
				}
				else if (client_fs_game != server_fs_game)
				{
					mods::set_mod(server_fs_game);
					needs_restart = true;
				}
			}

			return needs_restart;
		}*/
	}

	namespace
	{
		connection_state server_connection_state{};
		std::optional<discord_information> server_discord_info{};

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
			utils::hook::invoke<void>(0x140C19B00, gametype.data());

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
				utils::hook::invoke<void>(0x1405AEFB0);
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
				utils::hook::invoke<void>(0x1405B0130, mapname, gametype, a3);
			}
			else
			{
				// DB_LoadLevelXAssets
				utils::hook::invoke<void>(0x1403B9C90, mapname, 0);
			}
		}

		void com_restart_for_frontend_stub()
		{
			if (preloaded_map)
			{
				// Com_RestartForFrontend
				utils::hook::invoke<void>(0x140BAF0B0);
			}
			else
			{
				// Com_Restart
				utils::hook::invoke<void>(0x140BAF0A0);
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
				game::Com_Error(game::ERR_DROP, reinterpret_cast<const char*>(0x141512140));
			}

			if (!init_settings->serverThreadStartup)
			{
				if (!init_settings->isRestart)
				{
					// Nav_AllocNavPower
					memset(&*reinterpret_cast<__int64*>(0x144E3A490 + 8), 0, 0x78680ui64 - 8);
				}
			}
		}

		void reset_mem_stuff_stub(utils::hook::assembler& a)
		{
			a.pushad64();

			a.mov(rcx, rbx);
			a.call_aligned(reset_mem_stuff);

			a.popad64();

			a.jmp(0x140C563E2);
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

		if (dev)
		{
			command::execute("seta sv_cheats 1", true);
		}

		if (current_mapname && utils::string::to_lower(current_mapname->current.string) ==
			utils::string::to_lower(mapname) && (game::SV_Loaded() && !game::Com_FrontEndScene_IsActive()))
		{
			console::info("Restarting map: %s\n", mapname.data());
			command::execute("map_restart", false);
			return;
		}

		command::execute(utils::string::va("seta ui_mapname %s", mapname.data()), true);

		auto* gametype = game::Dvar_FindVar("g_gametype");
		if (gametype && gametype->current.string)
		{
			command::execute(utils::string::va("seta ui_gametype %s", gametype->current.string), true);
		}

		auto* hardcore = game::Dvar_FindVar("g_hardcore");
		if (hardcore)
		{
			command::execute(utils::string::va("seta ui_hardcore %d", hardcore->current.enabled), true);
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
				strncpy_s(client_name, game::g_entities[i].client->sess.name, sizeof(client_name));
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

	unsigned int get_client_count()
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

	unsigned int get_bot_count()
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
			utils::hook::set(0x141BBA800 + 0, a1);
			utils::hook::set(0x141BBA800 + 24, a1);
			utils::hook::set(0x141BBA800 + 56, a1);

			// patch singleplayer map_restart -> "map_restart_sp"
			utils::hook::set(0x141BBA740 + 0, a2);
			utils::hook::set(0x141BBA740 + 24, a2);
			utils::hook::set(0x141BBA740 + 56, a2);

			// patch singleplayer fast_restart -> "fast_restart_sp"
			utils::hook::set(0x141BBA700 + 0, a3);
			utils::hook::set(0x141BBA700 + 24, a3);
			utils::hook::set(0x141BBA700 + 56, a3);

			utils::hook::set<uint8_t>(0x140C562FD, 0xEB); // allow mapname to be changed while server is running

			utils::hook::nop(0x140A7A8DF, 5); // R_SyncRenderThread inside CL_MainMp_PreloadMap ( freezes )

			utils::hook::call(0x1409AFE84, com_gamestart_beginclient_stub); // blackscreen issue on connect
			utils::hook::call(0x1409B4077, com_gamestart_beginclient_stub); // may not be necessary (map rotate)
			utils::hook::call(0x1409B404A, com_restart_for_frontend_stub); // may not be necessary (map rotate)

			sv_start_map_for_party_hook.create(0x140C4D150, sv_start_map_for_party_stub);

			utils::hook::nop(0x140C563C3, 12); // far jump = 12 bytes
			utils::hook::jump(0x140C563C3, utils::hook::assemble(reset_mem_stuff_stub), true);

			// show custom drop reason
			utils::hook::set<uint8_t>(0x1409B0AFF, 0xEB);

			// enable custom kick reason in GScr_KickPlayer
			utils::hook::set<uint8_t>(0x140B5377E, 0xEB);

			// disable this, maybe causes no issues, but fixes Session unregister on map change/restart
			utils::hook::set<uint8_t>(0x140851B50, 0xC3); // CG_ServerCmdMP_ParsePlayerInfos

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

			command::add("kickClient", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("usage: kickClient <num>, <reason>(optional)\n");
					return;
				}

				if (!game::SV_Loaded() || game::Com_FrontEnd_IsInFrontEnd())
				{
					return;
				}

				std::string reason;
				if (params.size() > 2)
				{
					reason = params.join(2);
				}
				if (reason.empty())
				{
					reason = "EXE_PLAYERKICKED";
				}

				const auto client_num = atoi(params.get(1));
				if (client_num < 0 || static_cast<unsigned int>(client_num) >= *game::svs_numclients)
				{
					return;
				}

				scheduler::once([client_num, reason]()
				{
					game::SV_CmdsMP_KickClientNum(client_num, reason.data(), false);
				}, scheduler::pipeline::server);
			});

			command::add("kick", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("usage: kick <name>, <reason>(optional)\n");
					return;
				}

				if (!game::SV_Loaded() || game::Com_FrontEnd_IsInFrontEnd())
				{
					return;
				}

				std::string reason;
				if (params.size() > 2)
				{
					reason = params.join(2);
				}
				if (reason.empty())
				{
					reason = "EXE_PLAYERKICKED";
				}

				const std::string name = params.get(1);
				if (name == "all"s)
				{
					for (auto i = 0u; i < *game::svs_numclients; ++i)
					{
						scheduler::once([i, reason]()
						{
							game::SV_CmdsMP_KickClientNum(i, reason.data(), false);
						}, scheduler::pipeline::server);
					}
					return;
				}

				const auto client_num = get_client_num_by_name(name);
				if (client_num < 0 || static_cast<unsigned int>(client_num) >= *game::svs_numclients)
				{
					return;
				}

				scheduler::once([client_num, reason]()
				{
					game::SV_CmdsMP_KickClientNum(client_num, reason.data(), false);
				}, scheduler::pipeline::server);
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

				/*const auto fs_game = get_dvar_string("fs_game");
				info.set("fs_game", fs_game);

				if (!fs_game.empty())
				{
					for (const auto& file : mod_files)
					{
						const auto hash = get_file_hash(utils::string::va("%s/mod%s", 
							fs_game.data(), file.extension.data()));
						info.set(file.name, hash);
					}
				}*/

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