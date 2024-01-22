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
			// This must be registered as 'true' to avoid crash when starting a private match
			dvars::override::register_bool("xblive_privatematch", true, game::DVAR_FLAG_REPLICATED);

			if (game::environment::is_dedi() && !utils::flags::has_flag("unranked"))
			{
				dvars::override::register_bool("xblive_privatematch", false, game::DVAR_FLAG_REPLICATED | game::DVAR_FLAG_WRITE); // DVAR_FLAG_REPLICATED needed?

				game::Dvar_RegisterBool("onlinegame", true, game::DVAR_FLAG_READ, "Current game is an online game with stats, custom classes, unlocks");

				// Fix sessionteam always returning none (SV_ClientMP_HasAssignedTeam_Internal)
				utils::hook::set(0xC50BC0_b, 0xC300B0);
			}
		}

		component_priority priority() override
		{
			return component_priority::ranked;
		}
	};
}

REGISTER_COMPONENT(ranked::component)
