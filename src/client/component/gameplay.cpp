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
				a.jmp(0x70FBF0_b);

				a.bind(no_bounce);
				a.cmp(ptr(rsp, 0x44), r14d);
				a.jnz(loc_70FB6F);
				a.jmp(0x70FBE1_b);

				a.bind(loc_70FB6F);
				a.jmp(0x70FB6F_b);
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
				a.call(0xBB3030_b);
				a.mov(ptr(rdi, 0x32C), eax);

				// get bg_gravity as int
				a.pushad64();
				a.push(rdi);
				a.call_aligned(get_gravity);
				a.pop(rdi);
				a.mov(dword_ptr(rdi, 0x78), eax);
				a.popad64();

				a.jmp(0xAFA342_b);
			});
		}

		void* g_speed_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				// original code
				a.add(eax, ptr(rdi, 0x1FD0));

				a.push(rax);
				a.push(eax);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&*reinterpret_cast<game::dvar_t**>(0x3C98330_b))));
				a.mov(eax, dword_ptr(rax, 0x10));

				a.mov(dword_ptr(rdi, 0x7C), eax);

				a.pop(rax);
				a.pop(eax);

				a.jmp(0xAFB1EC_b);
			});
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Implement ejection dvar
			dvars::bg_playerEjection = game::Dvar_RegisterBool("bg_playerEjection", true, game::DVAR_FLAG_REPLICATED, "Flag whether player ejection is on or off");
			stuck_in_client_hook.create(0xAFD9B0_b, stuck_in_client_stub);

			// Implement bounces dvar
			dvars::bg_bounces = game::Dvar_RegisterBool("bg_bounces", false, game::DVAR_FLAG_REPLICATED, "Enables bounces");
			utils::hook::jump(0x70FBB7_b, bg_bounces_stub(), true);

			// Modify gravity dvar
			dvars::override::register_float("bg_gravity", 800, 0, 1000, 0xC0 | game::DVAR_FLAG_REPLICATED);
			utils::hook::nop(0xAFA330_b, 18);
			utils::hook::jump(0xAFA330_b, bg_gravity_stub(), true);

			// Modify speed dvar
			dvars::override::register_int("g_speed", 190, 0x80000000, 0x7FFFFFFF, 0xC0 | game::DVAR_FLAG_REPLICATED);
			utils::hook::nop(0xAFB1DF_b, 13);
			utils::hook::jump(0xAFB1DF_b, g_speed_stub(), true);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)