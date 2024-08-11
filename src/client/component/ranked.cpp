#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/dvars.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/flags.hpp>

namespace ranked
{
	namespace
	{
		bool bots_enabled()
		{
			return !game::Com_FrontEndScene_IsActive() && game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP;
		}
	}

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

			// Always run bots, even if xblive_privatematch is 0
			utils::hook::jump(0x1406E6940, bots_enabled); // BG_BotSystemEnabled
			utils::hook::jump(0x1406E6510, bots_enabled); // BG_AISystemEnabled
			utils::hook::jump(0x1406E68F0, bots_enabled); // BG_BotFastFileEnabled
			utils::hook::jump(0x140C546F0, bots_enabled); // BG_BotsUsingTeamDifficulty
		}
	};
}

REGISTER_COMPONENT(ranked::component)
