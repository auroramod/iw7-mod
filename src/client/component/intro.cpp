#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>

namespace intro
{
	namespace
	{
		void cinematic_start_playback(const char* name, const int playbackFlags, const int startOffsetMsec, 
			const bool fillerBink, const int pauseState)
		{
			if (name == "startup"s)
			{
#ifdef DEV_BUILD
				return;
#endif
				const auto* intro_dvar = game::Dvar_FindVar("intro");
				if (intro_dvar && !intro_dvar->current.enabled)
				{
					return;
				}
			}

			utils::hook::invoke<void>(0x140DD6A10, name, playbackFlags, startOffsetMsec, fillerBink, pauseState);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::call(0x140DD69FF, cinematic_start_playback);
			utils::hook::call(0x140DD69CF, cinematic_start_playback);
		}
	};
}

REGISTER_COMPONENT(intro::component)