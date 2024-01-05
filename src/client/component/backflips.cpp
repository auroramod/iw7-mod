#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include "dvars.hpp"

namespace backflips
{
    //returning 1 makes the game use the dvar value instead of hardcoded value
	int UseViewPitchDvars()
    {
        return 1;
    }

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0xB6E70A_b, UseViewPitchDvars);

            utils::hook::call(0x6F5CF0_b, UseViewPitchDvars);
            utils::hook::call(0x6F5D1A_b, UseViewPitchDvars);

            utils::hook::call(0x6E6823_b, UseViewPitchDvars);

            utils::hook::call(0x5B5BBB_b, UseViewPitchDvars);
            utils::hook::call(0x5B5BE0_b, UseViewPitchDvars);

            utils::hook::call(0x154E81_b, UseViewPitchDvars);
            utils::hook::call(0x154EA1_b, UseViewPitchDvars);

            utils::hook::call(0x5651E_b, UseViewPitchDvars);
            utils::hook::call(0x56555_b, UseViewPitchDvars);

            dvars::override::register_float("player_view_pitch_down", 85, 0, 360, game::DVAR_FLAG_SAVED);
            dvars::override::register_float("player_view_pitch_up", 85, 0, 360, game::DVAR_FLAG_SAVED);
		}
	};
}

REGISTER_COMPONENT(backflips::component)