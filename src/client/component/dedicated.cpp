#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "dvars.hpp"
#include "command.hpp"
#include "console/console.hpp"
#include "scheduler.hpp"
#include "filesystem.hpp"
#include "server_list.hpp"
#include "network.hpp"

#include <utils/json.hpp>

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/flags.hpp>
#include <utils/io.hpp>

namespace dedicated
{
	namespace
	{
		utils::hook::detour com_quit_f_hook;

		const game::dvar_t* sv_lanOnly;

		const char* sv_get_game_type_stub()
		{
			return game::Dvar_FindVar("g_gametype")->current.string;
		}

		bool sv_is_hardcore_mode_stub()
		{
			return game::Dvar_FindVar("g_hardcore")->current.enabled;
		}

		void kill_server()
		{
			game::SV_MainMP_KillLocalServer();

			com_quit_f_hook.invoke<void>();
		}

		std::vector<std::string>& get_startup_command_queue()
		{
			static std::vector<std::string> startup_command_queue;
			return startup_command_queue;
		}

		void execute_startup_command(int client, int /*controllerIndex*/, const char* command)
		{
			if (game::Live_SyncOnlineDataFlags(0) == 0)
			{
				game::Cbuf_ExecuteBufferInternal(0, 0, command, game::Cmd_ExecuteSingleCommand);
			}
			else
			{
				get_startup_command_queue().emplace_back(command);
			}
		}

		void execute_startup_command_queue()
		{
			const auto queue = get_startup_command_queue();
			get_startup_command_queue().clear();

			for (const auto& command : queue)
			{
				game::Cbuf_ExecuteBufferInternal(0, 0, command.data(), game::Cmd_ExecuteSingleCommand);
			}
		}

		bool party_is_server_dedicated_stub()
		{
			return true;
		}

		void sync_gpu_stub(void* a1)
		{
			// R_SyncGpu
			utils::hook::invoke<void>(0x140E08AE0, a1);

			const auto frame_time = *game::com_frameTime;
			const auto sys_msec = game::Sys_Milliseconds();
			const auto msec = frame_time - sys_msec;

			std::this_thread::sleep_for(std::chrono::milliseconds(msec));
		}

		void gscr_is_using_match_rules_data_stub()
		{
			game::Scr_AddInt(0);
		}

		void send_heartbeat()
		{
			if (sv_lanOnly->current.enabled)
			{
				return;
			}

			game::netadr_s target{};
			if (server_list::get_master_server(target))
			{
				network::send(target, "heartbeat", "IW7");
			}
		}

		void sys_error_stub(const char* msg, ...)
		{
			char buffer[2048]{};

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, _TRUNCATE, msg, ap);

			va_end(ap);

			scheduler::once([]
			{
				command::execute("map_rotate");
			}, scheduler::main, 3s);

			game::Com_Error(game::ERR_DROP, "%s", buffer);
		}

		void init_dedicated_server()
		{
			// R_RegisterDvars
			utils::hook::invoke<void>(0x140DF62C0);

			// R_RegisterCmds
			utils::hook::invoke<void>(0x140DD7E50);

			// RB_Tonemap_RegisterDvars
			utils::hook::invoke<void>(0x1404B1320);

			static bool initialized = false;
			if (initialized) return;
			initialized = true;

			// R_LoadGraphicsAssets
			utils::hook::invoke<void>(0x140E06220);
		}

		void initialize()
		{
			const auto initialize_gamemode = []() -> void
			{
				if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_CP)
				{
					command::execute("exec default_systemlink_cp.cfg", true);
					command::execute("exec default_cp.cfg", true);
				}
				else if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP)
				{
					command::execute("exec default_systemlink_mp.cfg", true);
					command::execute("exec default_mp.cfg", true);
				}
			};

			initialize_gamemode();
		}

		nlohmann::json snd_alias_length_data;

		nlohmann::json get_snd_alias_length_data(const char* mapname, const std::string& game_mode = "")
		{
			const auto path = "sounddata/"s + game_mode + "/"s + mapname + ".json"s;
			const auto buffer = filesystem::read_file(path);
			if (!buffer.empty())
			{
				try
				{
					return nlohmann::json::parse(buffer);
				}
				catch (const std::exception& e)
				{
					console::error("[SND]: failed to read sound lengths file \"%s\"\n    %s\n", path.data(), e.what());
				}
			}
			else
			{
				console::error("[SND]: failed to find sound lengths file \"%s\"\n", path.data());
			}
			return{};
		}

		nlohmann::json get_snd_alias_length_data_for_map(const char* mapname)
		{
			return get_snd_alias_length_data(mapname, game::Com_GameMode_GetActiveGameModeStr());
		}

		int get_snd_alias_length(const char* alias)
		{
			if (snd_alias_length_data.is_object() && snd_alias_length_data.contains(alias))
			{
				return snd_alias_length_data[alias];
			}
			else
			{
				//console::error("[SND]: failed to find sound length soundalias \"%s\"\n", alias);
				return 0;
			}
		}

		utils::hook::detour snd_lookup_sound_length_hook;
		int snd_lookup_sound_length_stub(const char* alias)
		{
			return get_snd_alias_length(alias);
		}

		utils::hook::detour start_server_hook;
		void start_server_stub(game::SvServerInitSettings* init_settings)
		{
			snd_alias_length_data = get_snd_alias_length_data_for_map(init_settings->mapName);
			start_server_hook.invoke<void>(init_settings);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (!game::environment::is_dedi())
			{
				return;
			}
			
#ifdef DEBUG
			printf("Starting dedicated server\n");
#endif
			
			// Register dedicated dvar
			game::Dvar_RegisterBool("dedicated", true, game::DVAR_FLAG_READ, "Dedicated server");

			// Add lanonly mode
			sv_lanOnly = game::Dvar_RegisterBool("sv_lanOnly", false, game::DVAR_FLAG_NONE, "Don't send heartbeat");

			// Disable frontend
			//dvars::override::register_bool("frontEndSceneEnabled", false, game::DVAR_FLAG_READ);
			utils::hook::set<uint8_t>(0x1405AC8F0, 0xC3); // Com_FastFile_Frame_FrontEnd

			// Disable shader preload
			dvars::override::register_bool("r_preloadShaders", false, game::DVAR_FLAG_READ);

			// Disable load for renderer
			dvars::override::register_bool("r_loadForRenderer", false, game::DVAR_FLAG_READ);

			dvars::override::register_bool("intro", false, game::DVAR_FLAG_READ);

			// Stop crashing from sys_errors
			utils::hook::jump(0x140D34180, sys_error_stub, true);

			// Is party dedicated
			utils::hook::jump(0x1405DFC10, party_is_server_dedicated_stub);

			// Make GScr_IsUsingMatchRulesData return 0 so the game doesn't override the cfg
			utils::hook::jump(0x140B53950, gscr_is_using_match_rules_data_stub);

			// Hook R_SyncGpu
			utils::hook::call(0x1403428B1, sync_gpu_stub);

			utils::hook::jump(0x140341B60, init_dedicated_server, true);

			// delay startup commands until the initialization is done
			utils::hook::call(0x140B8D20F, execute_startup_command);

			utils::hook::set<uint32_t>(0x140B21107 + 2, 0x482); // g_gametype flags
			utils::hook::set<uint32_t>(0x140B21137 + 2, 0x480); // g_hardcore flags
			utils::hook::jump(0x140C12400, sv_get_game_type_stub);
			utils::hook::jump(0x140C12660, sv_is_hardcore_mode_stub);

			utils::hook::nop(0x140CDD5D3, 5); // don't load config file
			utils::hook::nop(0x140B7CE46, 5); // ^
			utils::hook::set<uint8_t>(0x140BB0930, 0xC3); // don't save config file

			utils::hook::set<uint8_t>(0x1409D49C0, 0xC3); // disable self-registration
			utils::hook::set<uint8_t>(0x140E574E0, 0xC3); // render thread
			utils::hook::set<uint8_t>(0x1403471A0, 0xC3); // called from Com_Frame, seems to do renderer stuff
			utils::hook::set<uint8_t>(0x1409AA9A0, 0xC3); // CL_CheckForResend, which tries to connect to the local server constantly
			utils::hook::set<uint8_t>(0x140D2EBB0, 0xC3); // recommended settings check

			utils::hook::nop(0x140C5007B, 6); // unknown check in SV_ExecuteClientMessage
			utils::hook::nop(0x140C4F407, 3); // allow first slot to be occupied
			utils::hook::nop(0x1403429A7, 2); // properly shut down dedicated servers
			utils::hook::nop(0x14034296F, 2); // ^
			utils::hook::set<uint8_t>(0x140E08360, 0xC3); // don't shutdown renderer

			// SOUND patches
			//utils::hook::nop(0x140C93213, 5); // snd stream thread
			//utils::hook::set<uint8_t>(0x140C93206, 0); // snd_active
			//utils::hook::set<uint8_t>(0x140CB9150, 0xC3); // sound queue thing
			//utils::hook::set<uint8_t>(0x140C75550, 0xC3); // SD_AllocInit
			//utils::hook::set<uint8_t>(0x140C75CA0, 0xC3); // SD_Init

			utils::hook::set<uint8_t>(0x140D597C0, 0xC3); // Voice_Init
			utils::hook::set(0x140CFDC40, 0xC3C033); // sound stream reading

			// lookup the length from our list
			snd_lookup_sound_length_hook.create(0x140C9BCE0, snd_lookup_sound_length_stub);

			// check the sounddata when server is launched
			start_server_hook.create(0x140C56050, start_server_stub);

			// IMAGE patches
			// image stream (pak)
			utils::hook::set<uint8_t>(0x140A7DB10, 0xC3); // DB_CreateGfxImageStreamInternal

			// UI patches
			utils::hook::set<uint8_t>(0x140615090, 0xC3); // LUI_CoD_Init

			// IW7 patches
			utils::hook::set(0x140E06060, 0xC3C033); // directx
			utils::hook::set(0x140E05B80, 0xC3C033); // ^
			utils::hook::set(0x140DD2760, 0xC3C033); // ^
			utils::hook::set(0x140E05E20, 0xC3C033); // ^ buffer
			utils::hook::set(0x140E11270, 0xC3C033); // ^
			utils::hook::set(0x140DD3C50, 0xC3C033); // ^
			utils::hook::set(0x1400C1210, 0xC3C033); // ^ idk
			utils::hook::set(0x1400C12B0, 0xC3C033); // ^ idk
			utils::hook::set(0x140E423A0, 0xC3C033); // directx
			utils::hook::set(0x140E04680, 0xC3C033); // ^

			utils::hook::set(0x140E00ED0, 0xC3C033); // Image_Create1DTexture_PC
			utils::hook::set(0x140E00FC0, 0xC3C033); // Image_Create2DTexture_PC
			utils::hook::set(0x140E011A0, 0xC3C033); // Image_Create3DTexture_PC
			utils::hook::set(0x140E015C0, 0xC3C033); // Image_CreateCubeTexture_PC
			utils::hook::set(0x140E01300, 0xC3C033); // Image_CreateArrayTexture_PC

			utils::hook::set(0x1405F1EA0, 0xC3C033); // renderer
			utils::hook::set(0x1400C1370, 0xC3C033); // ^
			utils::hook::set(0x140DD26E0, 0xC3C033); // directx
			utils::hook::set(0x1405F0610, 0xC3C033); // ^
			utils::hook::set(0x1405F0580, 0xC3C033); // ^
			utils::hook::set(0x1405F0820, 0xC3C033); // ^
			utils::hook::set(0x1405F0790, 0xC3C033); // ^

			utils::hook::set(0x140DD42A0, 0xC3C033); // shutdown
			utils::hook::set(0x140DD42E0, 0xC3C033); // ^
			utils::hook::set(0x140DD4280, 0xC3C033); // ^

			utils::hook::set(0x140DD4230, 0xC3C033); // ^

			// r_loadForRenderer
			//utils::hook::set<uint8_t>(0x140E114A0, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140E11380, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140E113D0, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140E476F0, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140E11420, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140DD2300, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140DD2610, 0xC3); // ^
			//utils::hook::set<uint8_t>(0x140E11F40, 0xC3); // ^

			// skip R_GetFrameIndex check in DB_LoadLevelXAssets
			utils::hook::set<uint8_t>(0x1403B9E72, 0xEB);

			// don't release buffer
			utils::hook::set<uint8_t>(0x140DD4430, 0xC3);

			// R_LoadWorld
			utils::hook::set<uint8_t>(0x140DD14C0, 0xC3);

			// something to do with vls?
			utils::hook::set<uint8_t>(0x140D02CB0, 0xC3);

			// recipe save threads
			utils::hook::set<uint8_t>(0x140E7C970, 0xC3);

			// set game mode
			scheduler::once([]()
			{
				if (utils::flags::has_flag("cpMode") || utils::flags::has_flag("zombies"))
				{
					game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_CP);
				}
				else
				{
					game::Com_GameMode_SetDesiredGameMode(game::GAME_MODE_MP);
				}
			}, scheduler::pipeline::main);

			// initialization
			scheduler::on_game_initialized([]()
			{
				initialize();

				console::info("==================================\n");
				console::info("Server started!\n");
				console::info("==================================\n");

				// remove disconnect command
				game::Cmd_RemoveCommand("disconnect");

				execute_startup_command_queue();

				// Send heartbeat to dpmaster
				scheduler::once(send_heartbeat, scheduler::pipeline::server);
				scheduler::loop(send_heartbeat, scheduler::pipeline::server, 10min);
				command::add("heartbeat", send_heartbeat);

			}, scheduler::pipeline::main, 1s);

			// dedicated info
			scheduler::loop([]()
			{
				auto* sv_running = game::Dvar_FindVar("sv_running");
				if (!sv_running || !sv_running->current.enabled)
				{
					console::set_title("IW7-Mod Dedicated Server");
					return;
				}

				auto* const sv_hostname = game::Dvar_FindVar("sv_hostname");
				auto* const mapname = game::Dvar_FindVar("mapname");

				if (!sv_hostname || !mapname)
				{
					return;
				}

				std::string cleaned_hostname;
				cleaned_hostname.resize(static_cast<int>(strlen(sv_hostname->current.string) + 1));

				utils::string::strip(sv_hostname->current.string, cleaned_hostname.data(),
					static_cast<int>(strlen(sv_hostname->current.string)) + 1);

				console::set_title(utils::string::va("%s on %s", cleaned_hostname.data(), mapname->current.string));
			}, scheduler::pipeline::main, 1s);

			scheduler::once([]()
			{
				command::add("killserver", kill_server);
				com_quit_f_hook.create(game::Com_Quit_f, kill_server);
			}, scheduler::server);
		}
	};
}

REGISTER_COMPONENT(dedicated::component)
