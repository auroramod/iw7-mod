#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/dvars.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/flags.hpp>

namespace ranked
{
	namespace
	{
		game::dvar_t* ui_combat_training = nullptr;

		utils::hook::detour game_state_info_get_hook;
		utils::hook::detour playlist_run_rules_hook;
		utils::hook::detour game_state_info_is_public_hook;

		bool running_playlist_rules = false;

		bool use_private_game_state()
		{
			return !running_playlist_rules && ui_combat_training && ui_combat_training->current.enabled
				&& game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP;
		}

		void* game_state_info_get_stub()
		{
			if (use_private_game_state())
			{
				return utils::hook::invoke<void*>(0x140E7AEB0, 0, 0);
			}
			return game_state_info_get_hook.invoke<void*>();
		}

		// MatchRules.SetData and Lobby.SetBotsConnectType bail out when this says we're on the public game state
		int game_state_info_is_public_stub()
		{
			if (use_private_game_state())
			{
				return 0;
			}
			return game_state_info_is_public_hook.invoke<int>();
		}

		void playlist_run_rules_stub(const int controller, const int rule)
		{
			running_playlist_rules = true;
			playlist_run_rules_hook.invoke<void>(controller, rule);
			running_playlist_rules = false;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_dedi())
			{
				dvars::override::register_bool("xblive_privatematch", false, game::DVAR_FLAG_REPLICATED | game::DVAR_FLAG_WRITE);

				dvars::override::register_bool("onlinegame", true, game::DVAR_FLAG_READ);

				// Fix sessionteam always returning none (SV_ClientMP_HasAssignedTeam_Internal)
				utils::hook::set(0x140C50BC0, 0xC300B0);
			}
			else
			{
				dvars::override::register_bool("xblive_privatematch", true, game::DVAR_FLAG_REPLICATED);

				ui_combat_training = game::Dvar_RegisterBool("ui_combat_training", false, game::DVAR_FLAG_NONE,
					"Hosting a combat training match, use the private match game rules");
				game_state_info_get_hook.create(0x140E7AD70, game_state_info_get_stub);
				game_state_info_is_public_hook.create(0x140E7ADC0, game_state_info_is_public_stub);
				playlist_run_rules_hook.create(0x140CCD840, playlist_run_rules_stub);
			}
		}
	};
}

REGISTER_COMPONENT(ranked::component)
