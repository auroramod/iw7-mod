#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

namespace renderer
{
	namespace
	{
		utils::hook::detour r_init_draw_method_hook;

		int get_fullbright_technique()
		{
			switch (dvars::r_fullbright->current.integer)
			{
			case 2:
				return game::TECHNIQUE_LIT;
			default:
				return game::TECHNIQUE_UNLIT;
			}
		}

		void gfxdrawmethod()
		{
			game::gfxDrawMethod->drawScene = game::GFX_DRAW_SCENE_STANDARD;
			game::gfxDrawMethod->baseTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : game::TECHNIQUE_LIT;
			game::gfxDrawMethod->emissiveTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : game::TECHNIQUE_EMISSIVE;
			game::gfxDrawMethod->forceTechType = dvars::r_fullbright->current.integer ? get_fullbright_technique() : game::TECHNIQUE_LIT;
		}

		void r_init_draw_method_stub()
		{
			gfxdrawmethod();
		}

		bool r_update_front_end_dvar_options_stub()
		{
			if (dvars::r_fullbright->modified)
			{
				game::Dvar_ClearModified(dvars::r_fullbright);
				game::R_SyncRenderThread();

				gfxdrawmethod();
			}

			return utils::hook::invoke<bool>(0x140E28B60);
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

			dvars::r_fullbright = game::Dvar_RegisterInt("r_fullbright", 0, 0, 2, game::DVAR_NOFLAG, "Toggles rendering without lighting");

			r_init_draw_method_hook.create(0x140DE9260, r_init_draw_method_stub);
			utils::hook::call(0x140E264B3, r_update_front_end_dvar_options_stub);
		}
	};
}

REGISTER_COMPONENT(renderer::component)