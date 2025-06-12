#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include "dvars.hpp"

namespace backflips
{
	//returning 1 makes the game use the dvar value instead of hardcoded value
	int use_view_pitch_dvars()
	{
		return 1;
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

			utils::hook::call(0x140B6E70A, use_view_pitch_dvars);

			utils::hook::call(0x1406F5CF0, use_view_pitch_dvars);
			utils::hook::call(0x1406F5D1A, use_view_pitch_dvars);

			utils::hook::call(0x1406E6823, use_view_pitch_dvars);

			utils::hook::call(0x1405B5BBB, use_view_pitch_dvars);
			utils::hook::call(0x1405B5BE0, use_view_pitch_dvars);

			utils::hook::call(0x140154E81, use_view_pitch_dvars);
			utils::hook::call(0x140154EA1, use_view_pitch_dvars);

			utils::hook::call(0x14005651E, use_view_pitch_dvars);
			utils::hook::call(0x140056555, use_view_pitch_dvars);

			dvars::override::register_float("player_view_pitch_down", 85, 0, 360, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("player_view_pitch_up", 85, 0, 360, game::DVAR_FLAG_SAVED);
		}
	};
}

REGISTER_COMPONENT(backflips::component)
