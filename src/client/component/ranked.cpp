#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/command.hpp"
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

		constexpr auto max_clients = 18;

		struct bot_settings
		{
			uint8_t connect_type;
			uint8_t difficulty[2];
			uint8_t team_limit[2];
		};

		std::optional<bot_settings> saved_bot_settings{};
		std::atomic_bool has_saved_bot_settings = false;
		std::mutex saved_bot_settings_mutex;

		bool in_combat_training()
		{
			return ui_combat_training && ui_combat_training->current.enabled
				&& game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP;
		}

		bool use_private_game_state()
		{
			return !running_playlist_rules && in_combat_training();
		}

		game::GameStateInfo* get_private_game_state()
		{
			return utils::hook::invoke<game::GameStateInfo*>(0x140E7AEB0, 0, 0); // GameStateInfo_GetInstance(type, no reset?) 0 is private match
		}

		void update_combat_training_state()
		{
			const auto active = in_combat_training();
			if (active == has_saved_bot_settings)
			{
				return;
			}

			std::lock_guard _(saved_bot_settings_mutex);
			if (active == saved_bot_settings.has_value())
			{
				return;
			}

			auto* state = get_private_game_state();
			if (!state)
			{
				return;
			}

			if (active)
			{
				saved_bot_settings = bot_settings{
					state->usingBotsConnectType,
					{state->usingBotsDifficulty[0], state->usingBotsDifficulty[1]},
					{state->usingBotsTeamLimit[0], state->usingBotsTeamLimit[1]},
				};
				has_saved_bot_settings = true;
				return;
			}

			state->usingBotsConnectType = saved_bot_settings->connect_type;
			state->usingBotsDifficulty[0] = saved_bot_settings->difficulty[0];
			state->usingBotsDifficulty[1] = saved_bot_settings->difficulty[1];
			state->usingBotsTeamLimit[0] = saved_bot_settings->team_limit[0];
			state->usingBotsTeamLimit[1] = saved_bot_settings->team_limit[1];
			saved_bot_settings.reset();
			has_saved_bot_settings = false;
		}

		void* game_state_info_get_stub()
		{
			update_combat_training_state();

			if (use_private_game_state())
			{
				return get_private_game_state();
			}
			return game_state_info_get_hook.invoke<void*>();
		}

		int game_state_info_is_public_stub()
		{
			update_combat_training_state();

			if (use_private_game_state())
			{
				return 0;
			}
			return game_state_info_is_public_hook.invoke<int>();
		}

		int get_dvar_int(const char* name)
		{
			const auto* dvar = game::Dvar_FindVar(name);
			return dvar ? dvar->current.integer : 0;
		}

		bool is_team_based_gametype()
		{
			const auto* gametype = game::Dvar_FindVar("ui_gametype");
			const auto* table = game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, "mp/gametypestable.csv", false).stringTable;
			if (!gametype || !table)
			{
				return true;
			}

			const auto* value = game::StringTable_Lookup(table, 0, gametype->current.string, 4);
			return !value || *value != '0';
		}

		void sync_combat_training_bots()
		{
			update_combat_training_state();

			if (!in_combat_training())
			{
				return;
			}

			const auto max_bots = max_clients - 1;
			const auto team_based = is_team_based_gametype();

			const auto allies = std::clamp(get_dvar_int(team_based ? "bot_allies" : "bot_free"), 0, max_bots);
			const auto enemies = team_based ? std::clamp(get_dvar_int("bot_enemies"), 0, max_bots - allies) : 0;

			const auto allies_difficulty = std::clamp(get_dvar_int(team_based ? "bot_difficulty_allies" : "bot_difficulty_free"), 0, 4);
			const auto enemies_difficulty = team_based ? std::clamp(get_dvar_int("bot_difficulty_enemies"), 0, 4) : allies_difficulty;

			game::BG_SetBotsConnectType((allies + enemies) > 0 ? 1 : 0);
			game::BG_SetBotsTeamLimit(0, allies);
			game::BG_SetBotsTeamLimit(1, enemies);
			game::BG_SetBotsDifficulty(0, allies_difficulty);
			game::BG_SetBotsDifficulty(1, enemies_difficulty);

			auto* party = game::Lobby_GetPartyData();
			if (party && game::Party_AreWeHost(party))
			{
				game::PartyHost_GamestateChanged(party);
			}
		}

		int party_get_num_game_slots_stub(game::SessionData* session)
		{
			if (in_combat_training())
			{
				return max_clients;
			}
			return utils::hook::invoke<int>(0x140C72500, session); // Party_GetNumGameSlots
		}

		void playlist_run_rules_stub(const int controller, const int rule)
		{
			running_playlist_rules = true;
			playlist_run_rules_hook.invoke<void>(controller, rule);
			running_playlist_rules = false;

			sync_combat_training_bots();
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

				// override Party_GetNumGameSlots to allow 18 players in combat training
				utils::hook::call(0x1409D9868, party_get_num_game_slots_stub);

				// enables bots in public match
				utils::hook::nop(0x140036FFB, 0xD); // BG_Bots_IsBotMatchMakingAllowedForPlaylist

				// allow up to 18 players in any mode
				utils::hook::nop(0x140C3E518, 53);	// SV_BotTeamLimit
				utils::hook::nop(0x140C3E576, 0xC); // ^ remove 9 player per team check

				// fix team assignment that lead to gsc problems with invalid sessionteam
				utils::hook::set(0x14037E030, 0xC300000001B8); // Party_UsingPartyBasedTeams
				utils::hook::set(0x1409DB330, 0xC300000001B8); // PartyHost_UsingAssignedTeams

				utils::hook::nop(0x140E7ADC4, 26); // allow saving recipes in onlinegame

				command::add("xsyncbots", sync_combat_training_bots);
			}
		}
	};
}

REGISTER_COMPONENT(ranked::component)
