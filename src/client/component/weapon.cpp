#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "command.hpp"
#include "console/console.hpp"
#include "scheduler.hpp"

#include "gsc/script_extension.hpp"
#include "gsc/script_error.hpp"

#include <utils/hook.hpp>

#define WEAP_ANIM_INSPECT WEAP_ANIM_RELOAD_MULTIPLE_FAST_8
#define WEAP_STATE_INSPECT 34 // night vision wear?

#define WEAP_FLAG2_INSPECT 0x4

namespace weapon
{
	namespace
	{
#ifdef DEBUG
		template <typename T>
		void set_weapon_field(const std::string& weapon_name, unsigned int field, T value)
		{
			auto weapon = game::DB_FindXAssetHeader(game::ASSET_TYPE_WEAPON, weapon_name.data(), false).weapon;
			if (weapon)
			{
				if (field && field < (sizeof(game::WeaponDef)))
				{
					*reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(weapon->weapDef) + field) = value;
				}
				else
				{
					console::warn("weapon field: %d is higher than the size of weapon struct!\n", field);
				}
			}
			else
			{
				console::warn("weapon %s not found!\n", weapon_name.data());
			}
		}

		void set_weapon_field_float(const std::string& weapon_name, unsigned int field, float value)
		{
			set_weapon_field<float>(weapon_name, field, value);
		}

		void set_weapon_field_int(const std::string& weapon_name, unsigned int field, int value)
		{
			set_weapon_field<int>(weapon_name, field, value);
		}

		void set_weapon_field_bool(const std::string& weapon_name, unsigned int field, bool value)
		{
			set_weapon_field<bool>(weapon_name, field, value);
		}
#endif

		game::WeaponDef* GetWeaponDef(unsigned __int16* weapon)
		{
			return game::bg_weaponDefs[*weapon];
		}

		int XAnimPartsGetLengthMsec(game::XAnimParts* parts)
		{
			return (int)(((float)parts->numframes / parts->framerate) * 1000.0f);
		}

		bool HasAnimState(int num, int compare)
		{
			return (num == compare || (num | 0x80) == compare);
		}

		int BG_GetWeaponInspectTime(game::playerState_s* ps, unsigned __int16* weapon, char isAlternate, char isDualWielding)
		{
			int result = 0;

			auto* weapon_def = GetWeaponDef(weapon);
			if (!weapon_def->szXAnims)
			{
				return result;
			}

			result = weapon_def->szXAnims->pad;

			if (!result)
			{
				if (weapon_def->szXAnims->anims[game::WEAP_ANIM_INSPECT])
				{
					result = XAnimPartsGetLengthMsec(weapon_def->szXAnims->anims[game::WEAP_ANIM_INSPECT]) + 33;
				}
			}

			return result;
		}

		utils::hook::detour CG_Weapons_GetWeaponAnimRate_hook;
		float CG_Weapons_GetWeaponAnimRate_stub(int localClientNum, game::playerState_s* ps, int hand, __int64* weapon, unsigned __int8 isAlternate,
			unsigned __int8 isDualWielding, game::XAnim_s* anims, game::weapAnimFiles_t animIndex, game::weapAnimFiles_t animIndex2)
		{
			// inspect animation time
			if (animIndex == game::WEAP_ANIM_INSPECT)
			{
				animIndex2 = animIndex;

				auto time = BG_GetWeaponInspectTime(ps, (unsigned __int16*)weapon, isAlternate, isDualWielding);
				if (!time)
					return 0.0f;
				auto anim_time = game::XAnimGetLengthMsec(anims, animIndex2);
				return (float)anim_time / (float)time;
			}

			return CG_Weapons_GetWeaponAnimRate_hook.invoke<float>(localClientNum, ps, hand, weapon, 
				isAlternate, isDualWielding, anims, animIndex, animIndex2);
		}

		utils::hook::detour BG_MapWeaponAnimStateToAnimIndex_hook;
		game::weapAnimFiles_t BG_MapWeaponAnimStateToAnimIndex_stub(__int64 a1, game::playerState_s* ps, int animState, unsigned __int64* weapon, unsigned __int8 isAlternate,
			int handIndex, unsigned __int64 pmoveHandler)
		{
			if (HasAnimState(game::WEAP_INSPECT, animState) && ps->weapState[handIndex].weaponState == WEAP_STATE_INSPECT)
			{
				return game::WEAP_ANIM_INSPECT;
			}

			return BG_MapWeaponAnimStateToAnimIndex_hook.invoke<game::weapAnimFiles_t>(a1, ps, animState, weapon, isAlternate, handIndex, pmoveHandler);
		}

		bool BG_GetWeaponInspectEnabled()
		{
			return true;
		}

		void PM_Weap_Inspect(game::pmove_t* pm, game::playerState_s* ps, unsigned short* weapon, int hand)
		{
			if (pm->ps->weapState[hand].weaponState != game::WEAPON_READY)
			{
				return;
			}

			if (pm->ps->weapState[hand].weaponState == WEAP_STATE_INSPECT)
			{
				return;
			}

			if (!game::BG_ViewModelAnimExists(pm->ps, game::WEAP_ANIM_INSPECT, hand, pm->handler))
			{
				return;
			}

			ps->weapState[hand].weaponState = WEAP_STATE_INSPECT;
			ps->weapState[hand].weaponTime = BG_GetWeaponInspectTime(ps, weapon, false, false);
			ps->weapState[hand].weaponDelay = 0;

			if (ps->pm_type < 7)
				ps->weapState[hand].weapAnim = ~(unsigned __int8)ps->weapState[hand].weapAnim & 0x80 | game::WEAP_INSPECT;

			//PM_BuildWeaponAnimArrays(*(__int64*)((char*)pm + 568), pm->ps, *(char*)((char*)pm + 576));
		}

		void PM_Weapon_CheckForInspect(game::pmove_t* pm, game::pml_t* pml)
		{
			if ((pm->ps->weapFlags.m_flags[1] & WEAP_FLAG2_INSPECT) != 0)
			{
				pm->ps->weapFlags.m_flags[1] = ~pm->ps->weapFlags.m_flags[1] & WEAP_FLAG2_INSPECT;
			}
			else
			{
				return;
			}

			if (!BG_GetWeaponInspectEnabled())
			{
				return;
			}

			auto v3 = *(__int64*)((char*)pm + 568) + 16i64 * *(unsigned __int16*)((char*)pm->ps + 2176);
			unsigned short* weapon = (unsigned short*)((char*)v3 + 10);

			unsigned int hand = 0;
			if (game::BG_PlayerLastWeaponHandForViewWeapon(weapon, pm->ps) >= 0)
			{
				do
				{
					PM_Weap_Inspect(pm, pm->ps, weapon, hand++);
				} while (hand <= game::BG_PlayerLastWeaponHandForViewWeapon(weapon, pm->ps));
			}
		}

		utils::hook::detour PM_Weapon_Check_hook;
		void PM_Weapon_Check_stub(game::pmove_t* pm, game::pml_t* pml)
		{
			PM_Weapon_Check_hook.invoke<void>(pm, pml);

			PM_Weapon_CheckForInspect(pm, pml);
		}

		utils::hook::detour PM_Weapon_IsInInterruptibleState_hook;
		bool PM_Weapon_IsInInterruptibleState_stub(__int64 weaponMap, game::playerState_s* ps, unsigned int hand, unsigned __int64 pmoveHandler)
		{
			auto result = PM_Weapon_IsInInterruptibleState_hook.invoke<bool>(weaponMap, ps, hand, pmoveHandler);

			if (HasAnimState(game::WEAP_INSPECT, ps->weapState[hand].weapAnim) && ps->weapState[hand].weaponState == WEAP_STATE_INSPECT)
			{
				return true;
			}
			
			return result;
		}

		utils::hook::detour PM_WeaponProcessState_hook;
		unsigned int PM_WeaponProcessState_stub(game::pmove_t* pm, game::pml_t* pml, unsigned int delayedAction, int hand, unsigned __int8 interruptable)
		{
			if (HasAnimState(game::WEAP_INSPECT, pm->ps->weapState[hand].weapAnim) && pm->ps->weapState[hand].weaponState == WEAP_STATE_INSPECT)
			{
				if (pm->ps->weapState[hand].weaponTime > 0)
				{
					return 1;
				}

				pm->ps->weapState->weaponState = game::WEAPON_READY;
				if (pm->ps->pm_type < 7)
					pm->ps->weapState[hand].weapAnim = ~(unsigned __int8)pm->ps->weapState[hand].weapAnim & 0x80;
				
				return 1;
			}

			return PM_WeaponProcessState_hook.invoke<unsigned int>(pm, pml, delayedAction, hand, interruptable);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
#ifdef DEBUG
			command::add("setWeaponFieldFloat", [](const command::params& params)
			{
				if (params.size() <= 3)
				{
					console::info("usage: setWeaponFieldInt <weapon> <field> <value>\n");
					return;
				}
				set_weapon_field_float(params.get(1), atoi(params.get(2)), static_cast<float>(atof(params.get(3))));
			});

			command::add("setWeaponFieldInt", [](const command::params& params)
			{
				if (params.size() <= 3)
				{
					console::info("usage: setWeaponFieldInt <weapon> <field> <value>\n");
					return;
				}
				set_weapon_field_int(params.get(1), atoi(params.get(2)), static_cast<int>(atoi(params.get(3))));
			});

			command::add("setWeaponFieldBool", [](const command::params& params)
			{
				if (params.size() <= 3)
				{
					console::info("usage: setWeaponFieldBool <weapon> <field> <value>\n");
					return;
				}
				set_weapon_field_bool(params.get(1), atoi(params.get(2)), static_cast<bool>(atoi(params.get(3))));
			});
#endif

			CG_Weapons_GetWeaponAnimRate_hook.create(0x140932550, CG_Weapons_GetWeaponAnimRate_stub);
			BG_MapWeaponAnimStateToAnimIndex_hook.create(0x14074A300, BG_MapWeaponAnimStateToAnimIndex_stub);
			PM_Weapon_Check_hook.create(0x140723350, PM_Weapon_Check_stub);
			PM_Weapon_IsInInterruptibleState_hook.create(0x140728210, PM_Weapon_IsInInterruptibleState_stub);
			PM_WeaponProcessState_hook.create(0x14072C3B0, PM_WeaponProcessState_stub);

			gsc::method::add("startweaponinspection", [](game::scr_entref_t ent_ref, const gsc::function_args& args)
			{
				auto* ent = game::GetEntity(ent_ref);
				if (!ent->client)
				{
					gsc::scr_error(utils::string::va("entity %i is not player", ent_ref));
					return scripting::script_value{};
				}

				ent->client->ps.weapFlags.m_flags[1] |= WEAP_FLAG2_INSPECT; // lets hope the game doesn't use this flag already...
				return scripting::script_value{};
			});

			gsc::method::add("isinspectingweapon", [](game::scr_entref_t ent_ref, const gsc::function_args& args)
			{
				auto* ent = game::GetEntity(ent_ref);
				if (!ent->client)
				{
					gsc::scr_error(utils::string::va("entity %i is not player", ent_ref));
					return scripting::script_value{ false };
				}

				if (ent->client->ps.weapState[0].weaponState == WEAP_STATE_INSPECT && HasAnimState(game::WEAP_INSPECT, ent->client->ps.weapState[0].weapAnim))
				{
					return scripting::script_value{ true };
				}
				return scripting::script_value{ false };
			});
		}
	};
}

REGISTER_COMPONENT(weapon::component)