#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"

#include <utils/hook.hpp>

namespace thirdperson
{
	namespace
	{
		void* cg_offset_third_person_view_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.push(rax);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::cg_thirdPersonAngle)));
				a.movss(xmm11, dword_ptr(rax, 0x10));

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::cg_thirdPersonRange)));
				a.movss(xmm10, dword_ptr(rax, 0x10));

				a.pop(rax);

				// original code

				a.mulss(xmm7, xmm0);
				a.mulss(xmm6, xmm0);
				a.addss(xmm7, qword_ptr(rdi));

				a.jmp(0x140274596);
			});
		}

		void* cg_offset_chase_cam_view_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.push(rax);

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::cg_thirdPersonAngle)));
				a.movss(xmm8, dword_ptr(rax, 0x10));

				a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&dvars::cg_thirdPersonRange)));
				a.movss(xmm7, dword_ptr(rax, 0x10));
				
				a.pop(rax);

				// original code

				a.mulss(xmm2, xmm2);
				a.mulss(xmm3, xmm3);
				a.addss(xmm2, xmm3);

				a.jmp(0x140272069);
			});
		}

		utils::hook::detour cg_should_render_third_person_hook;
		int cg_should_render_third_person_stub(int localClientNum, game::cg_s* cgameGlob)
		{
			auto result = cg_should_render_third_person_hook.invoke<int>(localClientNum, cgameGlob);
			if (!result && cgameGlob->predictedPlayerState.pm_type != 3)
			{
				result = static_cast<int>(dvars::cg_thirdPerson->current.enabled);
				if (result)
				{
					cgameGlob->m_deathCameraFailsafeLock = false; // disable this, acts up for some reason
				}
			}
			return result;
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

			scheduler::once([]()
			{
				dvars::cg_thirdPerson = game::Dvar_RegisterBool("cg_thirdPerson", false, game::DVAR_FLAG_CHEAT, "Use third person view");

				dvars::cg_thirdPersonAngle = game::Dvar_RegisterFloat("cg_thirdPersonAngle", 356.0f, -180.0f, 360.0f, game::DVAR_FLAG_CHEAT,
					"The angle of the camera from the player in third person view");

				dvars::cg_thirdPersonRange = game::Dvar_RegisterFloat("cg_thirdPersonRange", 120.0f, 0.0f, 1024, game::DVAR_FLAG_CHEAT,
					"The range of the camera from the player in third person view");
			}, scheduler::main);

			utils::hook::jump(0x14027205D, cg_offset_chase_cam_view_stub(), true);
			utils::hook::jump(0x14027458A, cg_offset_third_person_view_stub(), true);

			cg_should_render_third_person_hook.create(0x140275B50, cg_should_render_third_person_stub);
		}
	};
}

REGISTER_COMPONENT(thirdperson::component)