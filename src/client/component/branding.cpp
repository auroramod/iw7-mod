#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "version.hpp"

#include "game/game.hpp"
//#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

// Fonts are used in game:
// fonts/blender_pro_bold.ttf, fonts/blender_pro_book.ttf, fonts/blender_pro_heavy.ttf, fonts/blender_pro_medium.ttf
// fonts/changelingneo-regular.ttf, fonts/dev.ttf, fonts/fira_mono_bold.ttf, fonts/fira_mono_regular.ttf, fonts/iw6_digital.ttf

namespace branding
{
	namespace
	{
		utils::hook::detour ui_get_formatted_build_number_hook;

		float color[4] = { 0.666f, 0.666f, 0.666f, 0.666f };

		const char* ui_get_formatted_build_number_stub()
		{
			const auto* const build_num = ui_get_formatted_build_number_hook.invoke<const char*>();
			return utils::string::va("%s (%s)", VERSION, build_num);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			localized_strings::override("LUA_MENU_MULTIPLAYER_CAPS", "IW7-MOD: MULTIPLAYER");
			localized_strings::override("LUA_MENU_ALIENS_CAPS", "IW7-MOD: ZOMBIES");

			//dvars::override::set_string("version", utils::string::va("IW7-Mod %s", VERSION));

			//ui_get_formatted_build_number_hook.create(0x1DF300_b, ui_get_formatted_build_number_stub); can't find

			scheduler::loop([]()
			{
				const auto font = game::R_RegisterFont("fonts/fira_mono_bold.ttf", 20);
				if (font)
				{
					game::R_AddCmdDrawText("IW7-Mod: " VERSION, 0x7FFFFFFF, font, 10.f,
						5.f + static_cast<float>(font->pixelHeight), 1.f, 1.f, 0.0f, color, 0);
				}
			}, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(branding::component)