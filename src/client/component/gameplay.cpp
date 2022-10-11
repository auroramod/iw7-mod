#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

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
			if (dvars::g_playerEjection->current.enabled)
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

		void* bg_gravity_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::bg_gravity)));
				a.mov(eax, dword_ptr(rax, 0x10));
				a.mov(dword_ptr(rdi, 0x78), eax);
				
				a.call(0xBB3030_b);
				a.mov(ptr(rdi, 0x32C), eax);

				a.jmp(0xAFA342_b);
			});
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Implement bounces
			dvars::bg_bounces = game::Dvar_RegisterBool("bg_bounces", false, game::DVAR_FLAG_REPLICATED, "Enables bounces");
			utils::hook::jump(0x70FBB7_b, bg_bounces_stub(), true);

			// Implement gravity dvar
			dvars::bg_gravity = game::Dvar_RegisterInt("bg_gravity", 800, 0, 1000, game::DVAR_FLAG_REPLICATED, "Game gravity in inches per second squared");
			utils::hook::nop(0xAFA330_b, 13);
			utils::hook::jump(0xAFA330_b, bg_gravity_stub(), true);

			dvars::g_playerEjection = game::Dvar_RegisterBool("g_playerEjection", true, game::DVAR_FLAG_REPLICATED, "Flag whether player ejection is on or off");
			stuck_in_client_hook.create(0xAFD9B0_b, stuck_in_client_stub);
		}
	};
}

REGISTER_COMPONENT(gameplay::component)