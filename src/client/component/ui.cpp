#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"

#include <utils/hook.hpp>

namespace ui
{
	namespace
	{
		utils::hook::detour cg_draw2d_hook;
		void cg_draw2d_stub(__int64 a1)
		{
			if (dvars::cg_draw2D && !dvars::cg_draw2D->current.enabled)
			{
				return;
			}
			
			cg_draw2d_hook.invoke<void>(a1);
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
				dvars::cg_draw2D = game::Dvar_RegisterBool("cg_draw2D", true, game::DVAR_FLAG_NONE, "Draw 2D screen elements");
			}, scheduler::main);

			cg_draw2d_hook.create(0x140781D90, cg_draw2d_stub);
		}
	};
}

REGISTER_COMPONENT(ui::component)