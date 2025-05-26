#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "dvars.hpp"

#include <utils/hook.hpp>

namespace fov
{
	constexpr float max_fov = 160.0f;

	namespace
	{
		game::dvar_t* cg_fov;
		game::dvar_t* cg_fovScale;
		game::dvar_t* cg_use_fov_comp;

		float cg_view_calc_fov_compensation_stub(game::cg_s* blob)
		{
			if (!cg_use_fov_comp->current.enabled)
			{
				return 0.0f;
			}

			return utils::hook::invoke<float>(0x140889B60, blob);
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
			
			cg_fov = game::Dvar_RegisterFloat("cg_fov", 65.0f, 1.0f, 160.f, game::DVAR_ARCHIVE,
				"The field of view angle in degrees");
			cg_fovScale = game::Dvar_RegisterFloat("cg_fovScale", 1.0f, 0.1f, 2.0f, game::DVAR_ARCHIVE,
				"Scale applied to the field of view");

			*reinterpret_cast<game::dvar_t**>(0x141FA6DA0) = cg_fov;
			dvars::disable::re_register("cg_fov");
			dvars::disable::de_register("cg_fov");

			*reinterpret_cast<game::dvar_t**>(0x141FA6DB0) = cg_fovScale;
			dvars::disable::re_register("cg_fovScale");
			dvars::disable::de_register("cg_fovScale");

			// don't reset our fov
			utils::hook::set<uint8_t>(0x1408A6160, 0xC3);

			// don't set cg_fov value
			utils::hook::nop(0x1408B5659, 5);
			utils::hook::set<uint8_t>(0x1408B5659, 0xC3);

			// disable FOV compensation by default
			cg_use_fov_comp = game::Dvar_RegisterBool("cg_use_fov_comp", false, game::DVAR_ARCHIVE, "Use FOV offset compensation for the viewmodel");
			utils::hook::call(0x140186FC4, cg_view_calc_fov_compensation_stub);
			utils::hook::call(0x1408D5940, cg_view_calc_fov_compensation_stub);

			// patch max fov values
			utils::hook::inject(0x14087E08B + 4, &max_fov);
			utils::hook::inject(0x140898C4B + 4, &max_fov); // mp max
			utils::hook::inject(0x140898C55 + 4, &max_fov); // sp max
		}
	};
}

REGISTER_COMPONENT(fov::component)