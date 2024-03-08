#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "steam_proxy.hpp"

#include "game/game.hpp"

#include "scheduler.hpp"

#include <utils/nt.hpp>
#include <utils/flags.hpp>
#include <utils/string.hpp>
#include <utils/binary_resource.hpp>

#include "steam/interface.hpp"
#include "steam/steam.hpp"

namespace steam_proxy
{
	namespace
	{
		utils::binary_resource runner_file(RUNNER, "runner.exe");

		utils::nt::library steam_client_module_{};
#ifdef LOAD_STEAM_OVERLAY
		utils::nt::library steam_overlay_module_{};
#endif

		steam::interface client_engine_ {};
		steam::interface client_user_ {};
		steam::interface client_utils_ {};

		steam::HSteamPipe steam_pipe_ = 0;
		steam::HSteamUser global_user_ = 0;

		steam::client* steam_client_ {};

		enum class ownership_state
		{
			success,
			unowned,
			nosteam,
			error,
		};

		bool is_disabled()
		{
			static const auto disabled = utils::flags::has_flag("nosteam"); // fixes issues for offline Steam play
			return disabled;
		}

		void* load_client_engine()
		{
			if (!steam_client_module_) return nullptr;

			for (auto i = 1; i > 0; ++i)
			{
				std::string name = utils::string::va("CLIENTENGINE_INTERFACE_VERSION%03i", i);
				auto* const temp_client_engine = steam_client_module_
					.invoke<void*>("CreateInterface", name.data(), nullptr);
				if (temp_client_engine) return temp_client_engine;
			}

			return nullptr;
		}

		void load_client()
		{
			SetEnvironmentVariableA("SteamAppId", std::to_string(steam::SteamUtils()->GetAppID()).data());

			const std::filesystem::path steam_path = steam::SteamAPI_GetSteamInstallPath();
			if (steam_path.empty()) return;

			utils::nt::library::load(steam_path / "tier0_s64.dll");
			utils::nt::library::load(steam_path / "vstdlib_s64.dll");
#ifdef LOAD_STEAM_OVERLAY
			steam_overlay_module_ = utils::nt::library::load(steam_path / "gameoverlayrenderer64.dll");
#endif
			steam_client_module_ = utils::nt::library::load(steam_path / "steamclient64.dll");
			if (!steam_client_module_) return;

			client_engine_ = load_client_engine();
			if (!client_engine_) return;

			steam_pipe_ = steam_client_module_.invoke<steam::HSteamPipe>("Steam_CreateSteamPipe");
			global_user_ = steam_client_module_.invoke<steam::HSteamUser>(
				"Steam_ConnectToGlobalUser", steam_pipe_);
			client_user_ = client_engine_.invoke<void*>(8, steam_pipe_, global_user_);
			// GetIClientUser
			client_utils_ = client_engine_.invoke<void*>(14, steam_pipe_); // GetIClientUtils
		}

		void do_cleanup()
		{
			client_engine_ = nullptr;
			client_user_ = nullptr;
			client_utils_ = nullptr;

			steam_pipe_ = 0;
			global_user_ = 0;

			steam_client_ = nullptr;
		}

		bool perform_cleanup_if_needed()
		{
			if (steam_client_) return true;

			if (steam_client_module_
				&& steam_pipe_
				&& global_user_
				&& steam_client_module_.invoke<bool>("Steam_BConnected", global_user_, steam_pipe_)
				&& steam_client_module_.invoke<bool>("Steam_BLoggedOn", global_user_, steam_pipe_)
				)
			{
				return false;
			}

			do_cleanup();
			return true;
		}

		void clean_up_on_error()
		{	
			scheduler::schedule([]()
			{
				if (perform_cleanup_if_needed())
				{
					return scheduler::cond_end;
				}

				return scheduler::cond_continue;
			}, scheduler::main);
		}

		ownership_state start_mod_unsafe(const std::string& title, size_t app_id)
		{
			if (!client_utils_ || !client_user_)
			{
				return ownership_state::nosteam;
			}

			if (!client_user_.invoke<bool>("BIsSubscribedApp", app_id))
			{
				//app_id = 480; // Spacewar
				return ownership_state::unowned;
			}

			client_utils_.invoke<void>("SetAppIDForCurrentPipe", app_id, false);

			char our_directory[MAX_PATH] = { 0 };
			GetCurrentDirectoryA(sizeof(our_directory), our_directory);

			const auto path = runner_file.get_extracted_file();
			const std::string cmdline = utils::string::va("\"%s\" -proc %d", path.data(), GetCurrentProcessId());

			steam::game_id game_id;
			game_id.raw.type = 1; // k_EGameIDTypeGameMod
			game_id.raw.app_id = app_id & 0xFFFFFF;

			const auto* mod_id = "iw7";
			game_id.raw.mod_id = *reinterpret_cast<const unsigned int*>(mod_id) | 0x80000000;

			client_user_.invoke<bool>("SpawnProcess", path.data(), cmdline.data(), our_directory,
				&game_id.bits, title.data(), 0, 0, 0);

			return ownership_state::success;
		}

		ownership_state start_mod(const std::string& title, const size_t app_id)
		{
			__try
			{
				return start_mod_unsafe(title, app_id);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				do_cleanup();
				return ownership_state::error;
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_load() override
		{
			if (game::environment::is_dedi() || is_disabled() || !FindWindowA(0, "Steam"))
			{
				return;
			}

			load_client();
			perform_cleanup_if_needed();
		}

		void post_unpack() override
		{
			if (game::environment::is_dedi() || is_disabled())
			{
				return;
			}

#ifndef DEV_BUILD
			try
			{
				const auto res = start_mod("\xF0\x9F\x8C\xA0" "IW7-Mod", steam::SteamUtils()->GetAppID());

				switch (res)
				{
				case ownership_state::nosteam:
					throw std::runtime_error("Steam must be running to play this game!");
				case ownership_state::unowned:
					throw std::runtime_error("You must own the game on steam to play this mod!");
				case ownership_state::error:
					throw std::runtime_error("Failed to verify ownership of the game!");
				case ownership_state::success:
					break;
				}
			}
			catch (std::exception& e)
			{
				printf("Steam: %s\n", e.what());
				MessageBoxA(GetForegroundWindow(), e.what(), "Error", MB_ICONERROR);
				TerminateProcess(GetCurrentProcess(), 1234);
			}
#endif
			clean_up_on_error();
		}

		void pre_destroy() override
		{
			if (steam_client_)
			{
				if (global_user_)
				{
					steam_client_->ReleaseUser(steam_pipe_, global_user_);
				}

				steam_client_->BReleaseSteamPipe(steam_pipe_);
			}
			else if (steam_client_module_ && steam_pipe_)
			{
				if (global_user_)
				{
					steam_client_module_.invoke<void>("Steam_ReleaseUser", steam_pipe_,
						global_user_);
				}

				steam_client_module_.invoke<bool>("Steam_BReleaseSteamPipe", steam_pipe_);
			}
		}

		component_priority priority() override
		{
			return component_priority::steam_proxy;
		}
	};

#ifdef LOAD_STEAM_OVERLAY
	const utils::nt::library& get_overlay_module()
	{
		return steam_overlay_module_;
	}
#endif

	void initialize()
	{
		if (client_engine_ || !steam_client_module_) return;

		steam_client_ = steam_client_module_.invoke<steam::client*>("CreateInterface", "SteamClient017", nullptr);
		if (!steam_client_) return;

		steam_pipe_ = steam_client_->CreateSteamPipe();
		global_user_ = steam_client_->ConnectToGlobalUser(steam_pipe_);
	}
}

REGISTER_COMPONENT(steam_proxy::component)