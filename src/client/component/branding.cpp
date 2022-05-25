#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "version.hpp"

#include "game/game.hpp"

// Fonts used in game:
// fonts/blender_pro_bold.ttf, fonts/blender_pro_book.ttf, fonts/blender_pro_heavy.ttf, fonts/blender_pro_medium.ttf
// fonts/changelingneo-regular.ttf, fonts/dev.ttf, fonts/fira_mono_bold.ttf, fonts/fira_mono_regular.ttf, fonts/iw6_digital.ttf

namespace branding
{
	namespace
	{
		float color[4] = { 0.666f, 0.666f, 0.666f, 0.666f };

		game::dvar_t* branding;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			localized_strings::override("LUA_MENU_LEGAL_COPYRIGHT", "IW7-MOD");

			localized_strings::override("LUA_MENU_MULTIPLAYER_CAPS", "IW7-MOD: MULTIPLAYER");
			localized_strings::override("LUA_MENU_ALIENS_CAPS", "IW7-MOD: ZOMBIES");

			branding = game::Dvar_RegisterBool("branding", true, game::DvarFlags::DVAR_FLAG_SAVED, "Show branding in the top right corner");

			scheduler::loop([]()
			{
				if (branding && branding->current.enabled)
				{
					const auto font = game::R_RegisterFont("fonts/fira_mono_bold.ttf", 20);
					if (font)
					{
						game::R_AddCmdDrawText("IW7-Mod: " VERSION, 0x7FFFFFFF, font, 10.f,
							5.f + static_cast<float>(font->pixelHeight), 1.f, 1.f, 0.0f, color, 0);
					}
				}
			}, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(branding::component)