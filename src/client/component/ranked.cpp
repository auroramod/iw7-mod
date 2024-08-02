#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/dvars.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/flags.hpp>

namespace ranked
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_dedi())
			{
				dvars::override::register_bool("xblive_privatematch", false, game::DVAR_FLAG_REPLICATED | game::DVAR_FLAG_WRITE);

				dvars::override::register_bool("onlinegame", true, game::DVAR_FLAG_READ);

				// Fix sessionteam always returning none (SV_ClientMP_HasAssignedTeam_Internal)
				utils::hook::set(0x140C50BC0, 0xC300B0);
			}
			else
			{
				dvars::override::register_bool("xblive_privatematch", true, game::DVAR_FLAG_REPLICATED);
			}
		}
	};
}

REGISTER_COMPONENT(ranked::component)
