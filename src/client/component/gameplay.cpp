#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "console/console.hpp"

#include <utils/nt.hpp>
#include <utils/hook.hpp>
#include <utils/flags.hpp>

namespace gameplay
{
	namespace
	{
		utils::hook::detour pm_weapon_use_ammo_hook;

		void stuck_in_client_stub(void* entity)
		{
			if (dvars::bg_playerEjection && dvars::bg_playerEjection->current.enabled)
			{
				utils::hook::invoke<void>(0x140AFD9B0, entity);
			}
		}

		void* bg_bounces_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				const auto compare = a.newLabel();
				const auto original = a.newLabel();

				a.push(rax);
				a.push(rcx);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::bg_bounces)));
				a.mov(al, byte_ptr(rax, 0x10));
				a.test(al, al);
				a.jz(compare);

				a.mov(rcx, reinterpret_cast<std::uint64_t>(&dvars::bg_bounceMinFallSpeed));
				a.mov(rcx, qword_ptr(rcx));
				a.mov(ecx, dword_ptr(rcx, 0x10));
				a.or_(ecx, 0x80000000);
				a.cmp(dword_ptr(r15, 0x3C), ecx);
				a.jae(compare);

				a.xor_(al, al);

				a.bind(compare);
				a.cmp(byte_ptr(rbp, -0x66), al);

				a.pop(rcx);
				a.pop(rax);
				a.jz(original);
				a.jmp(0x14070FBF0);

				a.bind(original);
				a.jmp(0x14070FBBD);
			});
		}

		void* force_bounce_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				const auto bounce = a.newLabel();

				// check dvar value
				a.push(rax);
				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::bg_bounces)));
				a.mov(al, byte_ptr(rax, 0x10));
				a.test(al, al);
				a.pop(rax);
				a.jnz(bounce);

				// original code
				a.push(rax);
				a.mov(rax, 0x14143E5A0);
				a.comiss(xmm0, dword_ptr(rax));
				a.pop(rax);
				a.jb(bounce);

				a.jmp(0x14070FBEA);

				a.bind(bounce);
				a.jmp(0x14070FB6F);
			});
		}

		void* bg_gravity_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&*reinterpret_cast<game::dvar_t**>(0x145209290))));

				a.cvtss2si(eax, dword_ptr(rax, 0x10));
				a.mov(dword_ptr(rdi, 0x78), eax);

				a.mov(rcx, rsi);
				a.mov(r8d, 0xC);
				a.call(0x140BB3030);
				a.mov(dword_ptr(rdi, 0x32C), eax);

				a.jmp(0x140AFA342);
			});
		}

		void* g_speed_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.push(rax);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&*reinterpret_cast<game::dvar_t**>(0x143C98330))));
				a.mov(eax, dword_ptr(rax, 0x10));

				a.mov(dword_ptr(rdi, 0x7C), eax);

				a.pop(rax);

				// original code
				a.mov(eax, ptr(rdi, 0x1FD4));
				a.add(eax, ptr(rdi, 0x1FD0));

				a.jmp(0x140AFB1EC);
			});
		}

		void cg_calculate_weapon_movement_debug_stub(game::cg_s* glob, float* origin)
		{
			// CG_View_CalcFovCompensation
			float value = utils::hook::invoke<float>(0x140889B60, glob, origin);

			// Initialize values
			float valueX = -6.0f * value;
			float valueY = 0.0f * value;
			float valueZ = 0.0f * value;

			// Apply values
			origin[0] += valueX * glob->viewModelAxis[0][0];
			origin[1] += valueX * glob->viewModelAxis[0][1];
			origin[2] += valueX * glob->viewModelAxis[0][2];

			origin[0] += valueY * glob->viewModelAxis[1][0];
			origin[1] += valueY * glob->viewModelAxis[1][1];
			origin[2] += valueY * glob->viewModelAxis[1][2];

			origin[0] += valueZ * glob->viewModelAxis[2][0];
			origin[1] += valueZ * glob->viewModelAxis[2][1];
			origin[2] += valueZ * glob->viewModelAxis[2][2];

			if (!dvars::cg_gun_x || !dvars::cg_gun_y || !dvars::cg_gun_z) return;

			// Apply dvar values
			origin[0] += dvars::cg_gun_x->current.value * glob->viewModelAxis[0][0];
			origin[1] += dvars::cg_gun_x->current.value * glob->viewModelAxis[0][1];
			origin[2] += dvars::cg_gun_x->current.value * glob->viewModelAxis[0][2];

			origin[0] += dvars::cg_gun_y->current.value * glob->viewModelAxis[1][0];
			origin[1] += dvars::cg_gun_y->current.value * glob->viewModelAxis[1][1];
			origin[2] += dvars::cg_gun_y->current.value * glob->viewModelAxis[1][2];

			origin[0] += dvars::cg_gun_z->current.value * glob->viewModelAxis[2][0];
			origin[1] += dvars::cg_gun_z->current.value * glob->viewModelAxis[2][1];
			origin[2] += dvars::cg_gun_z->current.value * glob->viewModelAxis[2][2];
		}

		void pm_weapon_use_ammo_stub(void* ps, const void* weapon, bool isAlternate, int amount, int hand)
		{
			if (!dvars::player_sustain_ammo || !dvars::player_sustain_ammo->current.enabled)
			{
				pm_weapon_use_ammo_hook.invoke<void>(ps, weapon, isAlternate, amount, hand);
			}
		}

		utils::hook::detour pm_crashland_hook;
		void pm_crashland_stub(void* ps, void* pml)
		{
			if (dvars::jump_enableFallDamage && dvars::jump_enableFallDamage->current.enabled)
			{
				pm_crashland_hook.invoke<void>(ps, pml);
			}
		}

		utils::hook::detour pmove_single_hook;
		void pmove_single_stub(game::pmove_t* pm, void* a2, unsigned int a3, int a4, int a5)
		{
			if (dvars::bg_disable_barrier_clips && dvars::bg_disable_barrier_clips->current.enabled && pm)
			{
				pm->tracemask &= ~0x10000;
				pm->tracemask |= 0x400;
			}

			pmove_single_hook.invoke<void>(pm, a2, a3, a4, a5);
		}

		constexpr auto mantle_surface_flags 	= 0x6000000;   	// SURF_MANTLEON | SURF_MANTLEOVER
		constexpr auto mantle_legacy_contents 	= 0x1000000; 	// IW6 CONTENTS_MANTLE

		constexpr auto mantle_angle_limit 	= 0x1414B8D8C; 	// acosf limit: 75 in IW7, 60 in IW6
		constexpr auto mantle_reach_base 	= 0x1414B8D84;  // 20.0 -> 34.9 reach; IW6 reaches 54.9
		constexpr auto mantle_reach_bias 	= 14.9f;        // the 15.0 - 0.1 the game adds to it
		constexpr auto mantle_stock_angle 	= 75.0f;
		constexpr auto mantle_stock_reach 	= 34.9f;

		void mantle_pm_tracehandler_stub(__int64 handler, game::trace_t* results, float* start, float* end, 
			game::Bounds* bounds, int passEntityNum, int contentMask, game::playerState_s* ps)
		{
			const auto legacy = dvars::mantle_legacy && dvars::mantle_legacy->current.enabled;

			const auto mask = legacy ? mantle_legacy_contents : contentMask;
			utils::hook::invoke<void>(0x140707C90, handler, results, start, end, bounds, passEntityNum, mask, ps);

			const auto surface_flags = results->surfaceFlags;
			const auto mantleable = (surface_flags & mantle_surface_flags) != 0;

			if (legacy && !mantleable)
			{
				results->fraction = 1.0f;
			}

			if (dvars::mantle_legacyMaxAngle && dvars::mantle_legacyReach)
			{
				const auto angle = legacy ? dvars::mantle_legacyMaxAngle->current.value : mantle_stock_angle;
				const auto reach = (legacy ? dvars::mantle_legacyReach->current.value : mantle_stock_reach)
					- mantle_reach_bias;

				static auto written_angle = mantle_stock_angle;
				static auto written_reach = mantle_stock_reach - mantle_reach_bias;

				if (angle != written_angle)
				{
					written_angle = angle;
					utils::hook::set<float>(mantle_angle_limit, angle);
				}

				if (reach != written_reach)
				{
					written_reach = reach;
					utils::hook::set<float>(mantle_reach_base, reach);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Implement ejection dvar
			dvars::bg_playerEjection = game::Dvar_RegisterBool("bg_playerEjection", true, game::DVAR_FLAG_REPLICATED, "Flag whether player ejection is on or off");
			utils::hook::call(0x140AFA739, stuck_in_client_stub);

			dvars::bg_bounces = game::Dvar_RegisterBool("bg_bounces", false, game::DVAR_FLAG_REPLICATED, "Keep your velocity when landing on a walkable surface at speed");
			dvars::bg_bounceMinFallSpeed = game::Dvar_RegisterFloat("bg_bounceMinFallSpeed", 200.0f, 0.0f, 1000.0f, game::DVAR_FLAG_REPLICATED, "Minimum downward speed before bg_bounces takes effect");
			utils::hook::jump(0x14070FBB7, bg_bounces_stub());
			utils::hook::jump(0x14070FBE1, force_bounce_stub());

			// Modify gravity dvar
			dvars::override::register_float("bg_gravity", 800.0f, 1.0f, 1000.0f, 0xC0 | game::DVAR_FLAG_REPLICATED);
			utils::hook::nop(0x140AFA330, 18);
			utils::hook::jump(0x140AFA330, bg_gravity_stub(), true);

			// Modify speed dvar
			dvars::override::register_int("g_speed", 190, 0x80000000, 0x7FFFFFFF, 0xC0 | game::DVAR_FLAG_REPLICATED);
			utils::hook::nop(0x140AFB1DF, 13);
			utils::hook::jump(0x140AFB1DF, g_speed_stub(), true);

			// Implement gun position dvars
			dvars::cg_gun_x = game::Dvar_RegisterFloat("cg_gun_x", 0.0f, -800.0f, 800.0f, game::DvarFlags::DVAR_FLAG_NONE, "Forward position of the viewmodel");
			dvars::cg_gun_y = game::Dvar_RegisterFloat("cg_gun_y", 0.0f, -800.0f, 800.0f, game::DvarFlags::DVAR_FLAG_NONE, "Right position of the viewmodel");
			dvars::cg_gun_z = game::Dvar_RegisterFloat("cg_gun_z", 0.0f, -800.0f, 800.0f, game::DvarFlags::DVAR_FLAG_NONE, "Up position of the viewmodel");
			utils::hook::jump(0x1408D5930, cg_calculate_weapon_movement_debug_stub);

			// Modify limits
			dvars::override::register_float("cl_yawspeed", 140.0f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), game::DVAR_FLAG_SAVED);

			// Add toggle for keeping your clip ammo
			dvars::player_sustain_ammo = game::Dvar_RegisterBool("player_sustainAmmo", false, game::DVAR_FLAG_REPLICATED, "Firing weapon will not decrease clip ammo");
			pm_weapon_use_ammo_hook.create(0x1407330E0, pm_weapon_use_ammo_stub);

			// Implement fall damage dvar
			dvars::jump_enableFallDamage = game::Dvar_RegisterBool("jump_enableFallDamage", true, game::DVAR_FLAG_REPLICATED, "Enable fall damage");
			pm_crashland_hook.create(0x1406F9860, pm_crashland_stub);

			// Make min/max falldamage dvars work on all gamemodes
			utils::hook::nop(0x1406F6265, 2);
			utils::hook::nop(0x1406F6285, 2);

			// Add a feature to toggle barrier clips on maps
			dvars::bg_disable_barrier_clips = game::Dvar_RegisterBool("bg_disableBarrierClips", false, game::DVAR_FLAG_REPLICATED, "(Experimental) Disables barrier clips in maps to access things easily");
			pmove_single_hook.create(0x14070F530, pmove_single_stub);

			// Make ladder velocity 0.5 for each gamemode
			utils::hook::nop(0x1406FD240, 2);

			// Make mantle_enable work on all gamemodes
			utils::hook::nop(0x1406E2676, 2); // Mantle_CanMantle
			utils::hook::nop(0x1406E4CDE, 2); // Mantle_Update

			// Make mantle behave like older games
			dvars::mantle_legacy = game::Dvar_RegisterBool("mantle_legacy", false, game::DVAR_FLAG_REPLICATED, "Enable legacy mantle behavior");
			dvars::mantle_legacyMaxAngle = game::Dvar_RegisterFloat("mantle_legacyMaxAngle", 60.0f, 0.0f, 90.0f, game::DVAR_FLAG_REPLICATED, "Ledge angle limit while legacy mantling (IW6 uses 60, IW7 75)");
			dvars::mantle_legacyReach = game::Dvar_RegisterFloat("mantle_legacyReach", 54.9f, 16.0f, 128.0f, game::DVAR_FLAG_REPLICATED, "Forward reach of the mantle sweep while legacy mantling (IW6 uses 54.9, IW7 34.9)");
			utils::hook::call(0x1406E398D, mantle_pm_tracehandler_stub);
			utils::hook::set<uint32_t>(0x1406E3AB8 + 4, mantle_surface_flags);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)
