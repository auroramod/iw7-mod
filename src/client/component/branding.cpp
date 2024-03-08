#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "localized_strings.hpp"
#include "scheduler.hpp"
#include "version.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "utils/hook.hpp"
#include <utils/string.hpp>

namespace branding
{
	namespace
	{
		utils::hook::detour ui_get_formatted_build_number_hook;
		const char* ui_get_formatted_build_number_stub()
		{
			const auto build_num = ui_get_formatted_build_number_hook.invoke<const char*>();
			return utils::string::va("%s (%s)", VERSION, build_num);
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

			localized_strings::override("LUA_MENU_LEGAL_COPYRIGHT", "IW7-MOD");

			//localized_strings::override("MENU_SP_CAMPAIGN", "IW7-MOD: CAMPAIGN");
			//localized_strings::override("LUA_MENU_MULTIPLAYER_CAPS", "IW7-MOD: MULTIPLAYER");
			//localized_strings::override("LUA_MENU_ALIENS_CAPS", "IW7-MOD: ZOMBIES");

			scheduler::once([]()
			{
				dvars::branding = game::Dvar_RegisterBool("branding", true, game::DvarFlags::DVAR_FLAG_SAVED, "Show branding in the top left corner");
			}, scheduler::pipeline::renderer);

			scheduler::loop([]()
			{
				if (dvars::branding && dvars::branding->current.enabled)
				{
					const auto font = game::R_RegisterFont("fonts/fira_mono_bold.ttf", 15);
					if (font)
					{
						static float text_color[4] = { 0.860f, 0.459f, 0.925f, 0.400f };
						game::R_AddCmdDrawText("iw7-mod: " VERSION, 0x7FFFFFFF, font, 10.f,
							5.f + static_cast<float>(font->pixelHeight), 1.f, 1.f, 0.0f, text_color, 0);
					}
				}
			}, scheduler::pipeline::renderer);

			ui_get_formatted_build_number_hook.create(0xCD1170_b, ui_get_formatted_build_number_stub);
		}
	};
}

REGISTER_COMPONENT(branding::component)
