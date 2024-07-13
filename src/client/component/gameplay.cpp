#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "dvars.hpp"

#include <utils/nt.hpp>
#include <utils/hook.hpp>
#include <utils/flags.hpp>

namespace gameplay
{
	namespace
	{
		utils::hook::detour stuck_in_client_hook;

		void stuck_in_client_stub(void* entity)
		{
			if (dvars::bg_playerEjection->current.enabled)
			{
				stuck_in_client_hook.invoke<void>(entity);
			}
		}

		void* bg_bounces_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				const auto no_bounce = a.newLabel();
				const auto loc_70FB6F = a.newLabel();

				a.push(rax);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::bg_bounces)));
				a.mov(al, byte_ptr(rax, 0x10));
				a.cmp(ptr(rbp, -0x66), al);

				a.pop(rax);
				a.jz(no_bounce);
				a.jmp(0x14070FBF0);

				a.bind(no_bounce);
				a.cmp(ptr(rsp, 0x44), r14d);
				a.jnz(loc_70FB6F);
				a.jmp(0x14070FBE1);

				a.bind(loc_70FB6F);
				a.jmp(0x14070FB6F);
			});
		}

		int get_gravity()
		{
			return static_cast<int>(game::BG_GetGravity());
		}

		void* bg_gravity_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				// do moveSpeedScaleMultiplier first (xmm0)
				a.call(0x140BB3030);
				a.mov(ptr(rdi, 0x32C), eax);

				// get bg_gravity as int
				a.pushad64();
				a.push(rdi);
				a.call_aligned(get_gravity);
				a.pop(rdi);
				a.mov(dword_ptr(rdi, 0x78), eax);
				a.popad64();

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
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Implement ejection dvar
			dvars::bg_playerEjection = game::Dvar_RegisterBool("bg_playerEjection", true, game::DVAR_FLAG_REPLICATED, "Flag whether player ejection is on or off");
			stuck_in_client_hook.create(0x140AFD9B0, stuck_in_client_stub);

			// Implement bounces dvar
			dvars::bg_bounces = game::Dvar_RegisterBool("bg_bounces", false, game::DVAR_FLAG_REPLICATED, "Enables bounces");
			utils::hook::jump(0x14070FBB7, bg_bounces_stub(), true);

			// Modify gravity dvar
			dvars::override::register_float("bg_gravity", 800, 0, 1000, 0xC0 | game::DVAR_FLAG_REPLICATED);
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
		}
	};
}

REGISTER_COMPONENT(gameplay::component)