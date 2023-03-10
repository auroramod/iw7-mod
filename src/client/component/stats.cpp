#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "console/console.hpp"
#include "command.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace stats
{
	void unlock_stats()
	{
		if (game::CL_IsGameClientActive(0))
		{
			console::info("Not allowed while ingame.");
			return;
		}

		if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_MP && game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_CP)
		{
			console::info("Must be in multiplayer or coop.");
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
	}

	namespace
	{
		utils::hook::detour is_unlocked1_hook;
		utils::hook::detour is_unlocked2_hook;
		utils::hook::detour item_quantity_hook;

		bool is_unlocked1(__int64 a1, int a2, const char* unlock_table, unsigned __int8* value)
		{
			if (dvars::cg_unlockall_items && dvars::cg_unlockall_items->current.enabled)
			{
				return true;
			}

			return is_unlocked1_hook.invoke<bool>(a1, a2, unlock_table, value);
		}

		bool is_unlocked2(__int64 a1, int a2, const char* unlock_table, unsigned __int8* value)
		{
			if (dvars::cg_unlockall_items && dvars::cg_unlockall_items->current.enabled)
			{
				return true;
			}

			return is_unlocked2_hook.invoke<bool>(a1, a2, unlock_table, value);
		}

		int item_quantity(__int64 a1, unsigned int a2, int a3)
		{
			if (dvars::cg_unlockall_loot && dvars::cg_unlockall_loot->current.enabled)
			{
				return 1;
			}
			
			return item_quantity_hook.invoke<int>(a1, a2, a3);
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
				case game::DDL_UINT_TYPE:
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
					console::info("%s\n", game::DDL::DDL_Lookup_GetEnumString(state, value.intValue));
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

			// register dvars
			scheduler::once([]()
			{
				dvars::cg_unlockall_items = game::Dvar_RegisterBool("cg_unlockall_items", false, game::DVAR_FLAG_SAVED, "Unlocks all items.");
				dvars::cg_unlockall_loot = game::Dvar_RegisterBool("cg_unlockall_loot", false, game::DVAR_FLAG_SAVED, "Unlocks all loot.");
			}, scheduler::main);

			// unlockables
			is_unlocked1_hook.create(0x34E020_b, is_unlocked1);
			is_unlocked2_hook.create(0x34CF40_b, is_unlocked2);

			// loot
			item_quantity_hook.create(0x51DBE0_b, item_quantity);

			// GetPlayerData print
			utils::hook::jump(0xB84F00_b, com_ddl_print_state); // Com_DDL_PrintState
		}
	};
}

REGISTER_COMPONENT(stats::component)