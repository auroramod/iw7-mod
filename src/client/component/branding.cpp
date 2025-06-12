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
				dvars::branding = game::Dvar_RegisterBool("branding", true, game::DvarFlags::DVAR_FLAG_SAVED, "Show branding");
			}, scheduler::pipeline::renderer);

#if GIT_DIRTY == 1
			static char version_buffer[0x100]{};
			sprintf_s(version_buffer, sizeof(version_buffer), "%s %s build %s %s %s", BUILD_NAME, VERSION, __DATE__, __TIME__, TARGET_ARCHITECTURE);
#endif

			scheduler::loop([]()
			{
				if (dvars::branding && dvars::branding->current.enabled)
				{
#if GIT_DIRTY == 1
					const auto* placement = game::ScrPlace_GetViewPlacement();
					const auto font = game::UI_GetFontHandle(placement, game::FONT_TYPE_HUD_BIG_FONT, 1.0f);
					if (!font) return;

					static const auto offset_from_corner = 75.0f;
					static float text_color[4] = { 0.4f, 0.69f, 1.0f, 0.69f };
					
					const auto x = (placement->realViewportSize[0] - offset_from_corner) - (game::R_TextWidth(version_buffer, std::numeric_limits<int>::max(), font));
					const auto height = (placement->realViewportSize[1] - offset_from_corner) + 5.0f; // remove some off the offset

					game::R_AddCmdDrawText(version_buffer, std::numeric_limits<int>::max(), font, static_cast<float>(x),
						height + static_cast<float>(font->pixelHeight),
						1.0f, 1.0f, 0.0f, text_color, game::FONT_STYLE_SHADOW);
#else
					const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", 16);
					if (!font) return;

					static float text_color[4] = { 0.860f, 0.459f, 0.925f, 0.400f };
					game::R_AddCmdDrawText("iw7-mod: " VERSION, std::numeric_limits<int>::max(), font, 10.f,
						5.f + static_cast<float>(font->pixelHeight), 1.f, 1.f, 0.0f, text_color, 0);
#endif
				}
			}, scheduler::pipeline::renderer);

			ui_get_formatted_build_number_hook.create(0x140CD1170, ui_get_formatted_build_number_stub);
		}
	};
}

REGISTER_COMPONENT(branding::component)
