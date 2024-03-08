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
			utils::hook::invoke<void>(0xE08AE0_b, a1);

			std::this_thread::sleep_for(1ms);
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
			utils::hook::invoke<void>(0xDF62C0_b);

			// R_RegisterCmds
			utils::hook::invoke<void>(0xDD7E50_b);

			// RB_Tonemap_RegisterDvars
			utils::hook::invoke<void>(0x4B1320_b);

			static bool initialized = false;
			if (initialized) return;
			initialized = true;

			// R_LoadGraphicsAssets
			utils::hook::invoke<void>(0xE06220_b);
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
			utils::hook::set<uint8_t>(0x5AC8F0_b, 0xC3); // Com_FastFile_Frame_FrontEnd

			// Disable shader preload
			dvars::override::register_bool("r_preloadShaders", false, game::DVAR_FLAG_READ);

			// Disable load for renderer
			dvars::override::register_bool("r_loadForRenderer", false, game::DVAR_FLAG_READ);

			dvars::override::register_bool("intro", false, game::DVAR_FLAG_READ);

			// Stop crashing from sys_errors
			utils::hook::jump(0xD34180_b, sys_error_stub, true);

			// Is party dedicated
			utils::hook::jump(0x5DFC10_b, party_is_server_dedicated_stub);

			// Make GScr_IsUsingMatchRulesData return 0 so the game doesn't override the cfg
			utils::hook::jump(0xB53950_b, gscr_is_using_match_rules_data_stub);

			// Hook R_SyncGpu
			utils::hook::call(0x3428B1_b, sync_gpu_stub);

			utils::hook::jump(0x341B60_b, init_dedicated_server, true);

			// delay startup commands until the initialization is done
			utils::hook::call(0xB8D20F_b, execute_startup_command);

			utils::hook::nop(0xCDD5D3_b, 5); // don't load config file
			utils::hook::nop(0xB7CE46_b, 5); // ^
			utils::hook::set<uint8_t>(0xBB0930_b, 0xC3); // don't save config file

			utils::hook::set<uint8_t>(0x9D49C0_b, 0xC3); // disable self-registration
			utils::hook::set<uint8_t>(0xE574E0_b, 0xC3); // render thread
			utils::hook::set<uint8_t>(0x3471A0_b, 0xC3); // called from Com_Frame, seems to do renderer stuff
			utils::hook::set<uint8_t>(0x9AA9A0_b, 0xC3); // CL_CheckForResend, which tries to connect to the local server constantly
			utils::hook::set<uint8_t>(0xD2EBB0_b, 0xC3); // recommended settings check

			utils::hook::nop(0xC5007B_b, 6); // unknown check in SV_ExecuteClientMessage
			utils::hook::nop(0xC4F407_b, 3); // allow first slot to be occupied
			utils::hook::nop(0x3429A7_b, 2); // properly shut down dedicated servers
			utils::hook::nop(0x34296F_b, 2); // ^
			utils::hook::set<uint8_t>(0xE08360_b, 0xC3); // don't shutdown renderer

			// SOUND patches
			//utils::hook::nop(0xC93213_b, 5); // snd stream thread
			//utils::hook::set<uint8_t>(0xC93206_b, 0); // snd_active
			//utils::hook::set<uint8_t>(0xCB9150_b, 0xC3); // sound queue thing
			//utils::hook::set<uint8_t>(0xC75550_b, 0xC3); // SD_AllocInit
			//utils::hook::set<uint8_t>(0xC75CA0_b, 0xC3); // SD_Init

			utils::hook::set<uint8_t>(0xD597C0_b, 0xC3); // Voice_Init
			utils::hook::set(0xCFDC40_b, 0xC3C033); // sound stream reading

			// lookup the length from our list
			snd_lookup_sound_length_hook.create(0xC9BCE0_b, snd_lookup_sound_length_stub);

			// check the sounddata when server is launched
			start_server_hook.create(0xC56050_b, start_server_stub);

			// IMAGE patches
			// image stream (pak)
			utils::hook::set<uint8_t>(0xA7DB10_b, 0xC3); // DB_CreateGfxImageStreamInternal

			// UI patches
			utils::hook::set<uint8_t>(0x615090_b, 0xC3); // LUI_CoD_Init

			// IW7 patches
			utils::hook::set(0xE06060_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE06060_b, 0xC3); // directx
			utils::hook::set(0xE05B80_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE05B80_b, 0xC3); // ^
			utils::hook::set(0xDD2760_b, 0xC3C033); //utils::hook::set<uint8_t>(0xDD2760_b, 0xC3); // ^
			utils::hook::set(0xE05E20_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE05E20_b, 0xC3); // ^ buffer
			utils::hook::set(0xE11270_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE11270_b, 0xC3); // ^
			utils::hook::set(0xDD3C50_b, 0xC3C033); //utils::hook::set<uint8_t>(0xDD3C50_b, 0xC3); // ^
			utils::hook::set(0x0C1210_b, 0xC3C033); //utils::hook::set<uint8_t>(0x0C1210_b, 0xC3); // ^ idk
			utils::hook::set(0x0C12B0_b, 0xC3C033); //utils::hook::set<uint8_t>(0x0C12B0_b, 0xC3); // ^ idk
			utils::hook::set(0xE423A0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE423A0_b, 0xC3); // directx
			utils::hook::set(0xE04680_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE04680_b, 0xC3); // ^

			utils::hook::set(0xE00ED0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE00ED0_b, 0xC3); // Image_Create1DTexture_PC
			utils::hook::set(0xE00FC0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE00FC0_b, 0xC3); // Image_Create2DTexture_PC
			utils::hook::set(0xE011A0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE011A0_b, 0xC3); // Image_Create3DTexture_PC
			utils::hook::set(0xE015C0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE015C0_b, 0xC3); // Image_CreateCubeTexture_PC
			utils::hook::set(0xE01300_b, 0xC3C033); //utils::hook::set<uint8_t>(0xE01300_b, 0xC3); // Image_CreateArrayTexture_PC

			utils::hook::set(0x5F1EA0_b, 0xC3C033); //utils::hook::set<uint8_t>(0x5F1EA0_b, 0xC3); // renderer
			utils::hook::set(0x0C1370_b, 0xC3C033); //utils::hook::set<uint8_t>(0x0C1370_b, 0xC3); // ^
			utils::hook::set(0xDD26E0_b, 0xC3C033); //utils::hook::set<uint8_t>(0xDD26E0_b, 0xC3); // directx
			utils::hook::set(0x5F0610_b, 0xC3C033); //utils::hook::set<uint8_t>(0x5F0610_b, 0xC3); // ^
			utils::hook::set(0x5F0580_b, 0xC3C033); //utils::hook::set<uint8_t>(0x5F0580_b, 0xC3); // ^
			utils::hook::set(0x5F0820_b, 0xC3C033); //utils::hook::set<uint8_t>(0x5F0820_b, 0xC3); // ^
			utils::hook::set(0x5F0790_b, 0xC3C033); //utils::hook::set<uint8_t>(0x5F0790_b, 0xC3); // ^

			utils::hook::set(0xDD42A0_b, 0xC3C033); // shutdown
			utils::hook::set(0xDD42E0_b, 0xC3C033); // ^
			utils::hook::set(0xDD42E0_b, 0xC3C033); // ^
			utils::hook::set(0xDD4280_b, 0xC3C033); // ^

			utils::hook::set(0xDD4230_b, 0xC3C033); // ^

			// r_loadForRenderer
			//utils::hook::set<uint8_t>(0xE114A0_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xE11380_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xE113D0_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xE476F0_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xE11420_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xDD2300_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xDD2610_b, 0xC3); // ^
			//utils::hook::set<uint8_t>(0xE11F40_b, 0xC3); // ^

			// skip R_GetFrameIndex check in DB_LoadLevelXAssets
			utils::hook::set<uint8_t>(0x3B9E72_b, 0xEB);

			// don't release buffer
			utils::hook::set<uint8_t>(0xDD4430_b, 0xC3);

			// R_LoadWorld
			utils::hook::set<uint8_t>(0xDD14C0_b, 0xC3);

			// something to do with vls?
			utils::hook::set<uint8_t>(0xD02CB0_b, 0xC3);

			// recipe save threads
			utils::hook::set<uint8_t>(0xE7C970_b, 0xC3);

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
