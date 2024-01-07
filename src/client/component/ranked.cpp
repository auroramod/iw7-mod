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
			dvars::override::register_bool("xblive_privatematch", false, game::DVAR_FLAG_REPLICATED);

			if (game::environment::is_dedi())
			{
				dvars::override::register_bool("xblive_privatematch", false, game::DVAR_FLAG_REPLICATED | game::DVAR_FLAG_WRITE);
				game::Dvar_RegisterBool("onlinegame", true, game::DVAR_FLAG_READ, "Current game is an online game with stats, custom classes, unlocks");
			}
		}

		component_priority priority() override
		{
			return component_priority::ranked;
		}
	};
}

REGISTER_COMPONENT(ranked::component)
