#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "command.hpp"
#include "console/console.hpp"
#include "dvars.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace stats
{
	namespace
	{
		utils::hook::detour is_item_unlocked_hook;
		utils::hook::detour is_item_unlocked_hook2;
		utils::hook::detour item_quantity_hook;

		game::dvar_t* cg_loot_count = nullptr;

		bool is_item_unlocked_stub(__int64 a1, int a2, const char* unlock_table, unsigned __int8* value)
		{
			if (dvars::cg_unlockall_items && dvars::cg_unlockall_items->current.enabled)
			{
				return true;
			}

			return is_item_unlocked_hook.invoke<bool>(a1, a2, unlock_table, value);
		}

		bool is_item_unlocked_stub2(__int64 a1, int a2, const char* unlock_table, unsigned __int8* value)
		{
			if (dvars::cg_unlockall_items && dvars::cg_unlockall_items->current.enabled)
			{
				return true;
			}

			return is_item_unlocked_hook2.invoke<bool>(a1, a2, unlock_table, value);
		}

		int item_quantity_stub(__int64 a1, int a2, int id)
		{
			auto result = item_quantity_hook.invoke<int>(a1, a2, id);

			// 30000 crashes
			if (id != 30000 && dvars::cg_unlockall_loot && dvars::cg_unlockall_loot->current.enabled)
			{
				if (cg_loot_count)
				{
					return cg_loot_count->current.integer;
				}
			}

			return result;
		}

		void com_ddl_print_state(const game::DDLState* state, const game::DDLContext* context)
		{
			if (game::DDL_StateIsLeaf(state))
			{
				const auto type = game::DDL_GetType(state);
				const auto value = game::DDL_GetValue(state, context);
				switch (type)
				{
				case game::DDL_BYTE_TYPE:
				case game::DDL_SHORT_TYPE:
				case game::DDL_BOOL_TYPE:
				case game::DDL_INT_TYPE:
					console::info("%d\n", value.intValue);
					break;
				case game::DDL_UINT64_TYPE:
					console::info("%zu\n", value.uint64Value);
					break;
				case game::DDL_FLOAT_TYPE:
					console::info("%f\n", value.floatValue);
					break;
				case game::DDL_STRING_TYPE:
					console::info("%s\n", value.stringPtr);
					break;
				case game::DDL_ENUM_TYPE:
					console::info("%s\n", game::DDL_Lookup_GetEnumString(state, value.intValue));
					break;
				default:
					console::info("Unknown type (%d).\n", type);
					break;
				}
			}
			else
			{
				console::info("non leaf node named \"%s\"\n", state->member->name);
			}
		}

		bool can_run_command()
		{
			if (game::CL_IsGameClientActive(0))
			{
				console::error("Not allowed while ingame.");
				return false;
			}

			if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_MP && game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_CP)
			{
				console::error("Must be in multiplayer or coop.");
				return false;
			}

			return true;
		}

		void unlock_stats()
		{
			if (!can_run_command())
			{
				return;
			}

			// experience & prestige
			command::execute("setRankedPlayerData progression playerLevel xp 1457200", true);
			command::execute("setRankedPlayerData progression playerLevel prestige 30", true);

			command::execute("setCoopPlayerData progression playerLevel xp 95297348", true);
			command::execute("setCoopPlayerData progression playerLevel prestige 20", true);

			// weapon experience
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_nrg mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_g18 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_emc mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_revolver mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_erad mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_crb mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ripper mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ump45 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_fhr mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ar57 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ake mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m4 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_aracc mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_fmg mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdfar mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_kbs mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_cheytac mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m8 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m1 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_devastator mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_spas mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sonic mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdfshotty mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mauler mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdflmg mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_lmg03 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_g18c mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ump45c mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_cheytacc mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m1c mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_spasc mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_arclassic mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_rvn mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_udm45 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_crdb mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_vr mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mp28 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_minilmg mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mod2187 mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ba50cal mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_gauss mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_longshot mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mag mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_unsalmg mpXP 54299", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_tacburst mpXP 54299", true);

			// weapon prestige
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_nrg prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_g18 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_emc prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_revolver prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_erad prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_crb prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ripper prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ump45 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_fhr prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ar57 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ake prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m4 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_aracc prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_fmg prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdfar prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_kbs prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_cheytac prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m8 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m1 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_devastator prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_spas prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sonic prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdfshotty prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mauler prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_sdflmg prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_lmg03 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_g18c prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ump45c prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_cheytacc prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_m1c prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_spasc prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_arclassic prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_rvn prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_udm45 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_crdb prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_vr prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mp28 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_minilmg prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mod2187 prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_ba50cal prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_gauss prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_longshot prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_mag prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_unsalmg prestige 3", true);
			command::execute("setCommonPlayerData sharedProgression weaponLevel iw7_tacburst prestige 3", true);

			// classic weapons
			command::execute("setCommonPlayerData sharedProgression classicWeapons iw7_g18c 1", true);
			command::execute("setCommonPlayerData sharedProgression classicWeapons iw7_ump45c 1", true);
			command::execute("setCommonPlayerData sharedProgression classicWeapons iw7_cheytacc 1", true);
			command::execute("setCommonPlayerData sharedProgression classicWeapons iw7_spasc 1", true);
			command::execute("setCommonPlayerData sharedProgression classicWeapons iw7_m1c 1", true);

			// Unlock challenges
			game::StringTable* challenge_table = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_STRINGTABLE, "mp/allchallengestable.csv", false).stringTable;
			if (challenge_table)
			{
				for (int i = 0; i < challenge_table->rowCount; i++)
				{
					// Find challenge
					const char* challenge = game::StringTable_GetColumnValueForRow(challenge_table, i, 0);

					int max_state = 0;
					int max_progress = 0;

					// Find correct tier and progress
					for (int j = 0; j < 10; j++) // iterate through states (max 10)
					{
						// progress, xp_reward, challenge_score

						int progress = atoi(game::StringTable_GetColumnValueForRow(challenge_table, i, 10 + (j * 3)));
						if (!progress) break;

						max_state = j + 1;
						max_progress = progress;
					}

					command::execute(utils::string::va("setRankedPlayerData challengeState %s %d", challenge, max_state), true);
					command::execute(utils::string::va("setRankedPlayerData challengeProgress %s %d", challenge, max_progress), true);
				}
			}

			game::StringTable* merit_table = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_STRINGTABLE, "cp/allmeritstable.csv", false).stringTable;
			if (merit_table)
			{
				for (int i = 0; i < merit_table->rowCount; i++)
				{
					// Find challenge
					const char* challenge = game::StringTable_GetColumnValueForRow(merit_table, i, 0);

					int max_state = 0;
					int max_progress = 0;

					// Find correct tier and progress
					for (int j = 0; j < 10; j++) // iterate through states (max 10)
					{
						// progress, xp_reward, challenge_score

						int progress = atoi(game::StringTable_GetColumnValueForRow(merit_table, i, 10 + (j * 3)));
						if (!progress) break;

						max_state = j + 1;
						max_progress = progress;
					}

					command::execute(utils::string::va("setCoopPlayerData meritState %s %d", challenge, max_state), true);
					command::execute(utils::string::va("setCoopPlayerData meritProgress %s %d", challenge, max_progress), true);
				}
			}

			command::execute("uploadstats", true); // needed to update stats i think
			console::debug("unlocked all normal stats!\n");
		}
		
		void unlock_stats_ee()
		{
			if (!can_run_command())
			{
				return;
			}

			// soul keys (secret characters)
			command::execute("setCoopPlayerData haveSoulKeys any_soul_key 1", true); // useless stat?
			command::execute("setCoopPlayerData haveSoulKeys soul_key_1 1", true);
			command::execute("setCoopPlayerData haveSoulKeys soul_key_2 1", true);
			command::execute("setCoopPlayerData haveSoulKeys soul_key_3 1", true);
			command::execute("setCoopPlayerData haveSoulKeys soul_key_4 1", true);
			command::execute("setCoopPlayerData haveSoulKeys soul_key_5 1", true);

			// secret character 5 on cp_zmb
			command::execute("setCoopPlayerData meritState mt_dlc4_troll2 1", true); // Conditions.HasBeatenMeph
			command::execute("setCoopPlayerData meritState mt_dc_camo 1", true);

			// lobby songs unlocked
			command::execute("setCoopPlayerData hasSongsUnlocked any_song 1", true);
			for (int index = 1; index < 11; index++)
			{
				command::execute(utils::string::va("setCoopPlayerData hasSongsUnlocked song_%d 1", index), true);
			}

			command::execute("uploadstats", true); // needed to update stats i think
			console::debug("unlocked all easter egg stats!\n");
		}

		void director_cut(const command::params& params)
		{
			if (!can_run_command())
			{
				return;
			}

			if (params.size() < 2)
			{
				console::info("usage: \"/director_cut 0/1\"\n");
				return;
			}

			const auto is_enabled = params.get(1);
			command::execute(utils::string::va("setCoopPlayerData dc %s", is_enabled), true);
			command::execute(utils::string::va("setCoopPlayerData dc_available %s", is_enabled), true);

			command::execute("uploadstats", true); // needed to update stats i think
			console::debug("directors cut set to %s\n", is_enabled);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_dedi())
			{
				return;
			}

			command::add("unlockstats", unlock_stats);
			command::add("unlockall", unlock_stats);
			command::add("unlockstatsEE", unlock_stats_ee);
			command::add("unlockallEE", unlock_stats_ee);
			command::add("director_cut", director_cut);

			// register dvars
			scheduler::once([]()
			{
				dvars::cg_unlockall_items = game::Dvar_RegisterBool("cg_unlockall_items", false, game::DVAR_FLAG_SAVED, "Whether items should be locked based on the player's stats or always unlocked.");
				game::Dvar_RegisterBool("cg_unlockall_classes", false, game::DVAR_FLAG_SAVED, "Whether classes should be locked based on the player's stats or always unlocked."); // TODO: need LUI scripting
				dvars::cg_unlockall_loot = game::Dvar_RegisterBool("cg_unlockall_loot", false, game::DVAR_FLAG_SAVED, "Whether loot should be locked based on the player's stats or always unlocked.");
				
				cg_loot_count = game::Dvar_RegisterInt("cg_loot_count", 1, 1, std::numeric_limits<int>::max(), game::DVAR_FLAG_SAVED, "Amount of loot to give for items");
			}, scheduler::main);

			// unlockables
			is_item_unlocked_hook.create(0x14034E020, is_item_unlocked_stub);
			is_item_unlocked_hook2.create(0x14034CF40, is_item_unlocked_stub2);

			// loot
			item_quantity_hook.create(0x14051DBE0, item_quantity_stub);

			// GetPlayerData print
			utils::hook::jump(0x140B84F00, com_ddl_print_state); // Com_DDL_PrintState
		}
	};
}

REGISTER_COMPONENT(stats::component)