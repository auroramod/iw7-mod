#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>

namespace chat
{
	namespace
	{
		game::GfxFont* ui_get_font_handle_stub()
		{
			const auto font = *reinterpret_cast<game::GfxFont**>(0x1474CC850); // textFont
			return font;
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// use better font
			utils::hook::call(0x14035602C, ui_get_font_handle_stub); // con_drawsay
			utils::hook::call(0x1409A7843, ui_get_font_handle_stub); // cl_keys_draw_text_override_field
			utils::hook::call(0x14077D250, ui_get_font_handle_stub); // cg_drawmp_drawchatmessages
		}
	};
}

REGISTER_COMPONENT(chat::component)