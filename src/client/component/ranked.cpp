#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/dvars.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/flags.hpp>
#include <utils/string.hpp>

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
			game::Cbuf_ExecuteBuffer(0, controller, "exec playlist_overide.cfg\n");
			running_playlist_rules = false;
		}

		int bg_get_bots_team_limit(int team)
		{
			const auto* partyMax = game::Dvar_FindVar("party_maxplayers");
			const int maxPlayers = std::min(partyMax->current.integer, 18);

			auto* alliesVar = game::Dvar_FindVar("bot_allies");
			auto* enemiesVar = game::Dvar_FindVar("bot_enemies");

			int allies = std::clamp(alliesVar->current.integer,  0, maxPlayers);
			int enemies = std::clamp(enemiesVar->current.integer, 0, maxPlayers);

			const int total = allies + enemies;

			if (total > maxPlayers)
			{
				const int overflow = total - maxPlayers;

				if (team == 0)
				{
					allies = std::max(0, allies - overflow);
				}
				else
				{
					enemies = std::max(0, enemies - overflow);
				}
			}

			return team == 0 ? allies : enemies;
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

				utils::hook::nop(0x140036FFB, 0xD); // BG_Bots_IsBotMatchMakingAllowedForPlaylist. enables bots in public match

				// allow up to 18 players in any mode. 
				utils::hook::jump(0x1406E6A60, bg_get_bots_team_limit); // allow unbalanced bot teams
				utils::hook::nop(0x140C3E518, 53); // SV_BotTeamLimit
				utils::hook::nop(0x140C3E576, 0xC); // SV_BotTeamLimit remove 9 player per team check

				// fix team assignment that lead to gsc problems with invalid sessionteam
				utils::hook::set(0x14037E030, 0xC300000001B8); // Party_UsingPartyBasedTeams
				utils::hook::set(0x1409DB330, 0xC300000001B8); // PartyHost_UsingAssignedTeams

				utils::hook::nop(0x140E7ADC4, 26); // allow saving recipes in onlinegame
			}
		}
	};
}

REGISTER_COMPONENT(ranked::component)
