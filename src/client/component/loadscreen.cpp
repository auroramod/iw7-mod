#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace loadscreen
{
	namespace
	{
		bool CG_IsShowingProgress_FastFile()
		{
			return game::LoadBar_GetLoadedFraction() > 0.0;
		}

		void UI_DrawLoadScreenInfo()
		{
			auto* mapname = game::Dvar_FindVar("ui_mapname")->current.string;
			auto* gametype = game::Dvar_FindVar("ui_gametype")->current.string;

			auto* mapname_ui = game::UI_GetMapDisplayName(mapname);
			auto* gametype_ui = game::UI_GetGameTypeDisplayName(gametype);

			const auto* placement = game::ScrPlace_GetActivePlacement();

			{
				const auto scale_mapname = 0.4f;
				const auto scale_gametype = 0.3f;

				auto* font_mapname = game::UI_GetFontHandle(placement, game::FONT_TYPE_OBJECTIVE_FONT, scale_mapname);
				auto* font_gametype = game::UI_GetFontHandle(placement, game::FONT_TYPE_OBJECTIVE_FONT, scale_gametype);

				auto text_width_mapname = game::UI_TextWidth(mapname_ui, 0, font_mapname, scale_mapname);
				auto text_width_gametype = game::UI_TextWidth(gametype_ui, 0, font_gametype, scale_gametype);

				game::UI_DrawText(placement, mapname_ui, std::numeric_limits<int>::max(), font_mapname, (640.0f - (float)text_width_mapname) * 0.5f, 89.0f,
					0, 0, scale_mapname, game::colorWhite, game::FONT_STYLE_SHADOW);

				game::UI_DrawText(placement, gametype_ui, std::numeric_limits<int>::max(), font_gametype, (640.0f - (float)text_width_gametype) * 0.5f, 89.0f + 8.0f + font_mapname->pixelHeight * scale_mapname,
					0, 0, scale_gametype, game::colorWhite, game::FONT_STYLE_SHADOW);
			}

			game::uiClientState_t cstate{};
			game::CL_GetClientState(0, &cstate);

			bool displayConnectionInfo = true;
			if (!_stricmp(cstate.servername, "localhost"))
			{
				displayConnectionInfo = false;
			}
			if (game::LoadBar_GetLoadedFraction() >= 1.0f)
			{
				displayConnectionInfo = true;
			}

			if (displayConnectionInfo)
			{
				const char* s = nullptr;
				switch (cstate.connState)
				{
				case game::CA_CONNECTING:
					s = game::UI_SafeTranslateString("EXE_AWAITINGCONNECTION");
					s = game::UI_ReplaceConversionInt(s, cstate.connectPacketCount);
					break;
				case game::CA_CHALLENGING:
					s = game::UI_SafeTranslateString("EXE_AWAITINGCHALLENGE");
					s = game::UI_ReplaceConversionInt(s, cstate.connectPacketCount);
					break;
				case game::CA_CONNECTED:
					s = game::UI_SafeTranslateString("EXE_AWAITINGGAMESTATE");
					break;
				case game::CA_SENDINGSTATS:
					s = game::UI_SafeTranslateString("EXE_UPLOADINGSTATS");
					break;
				case game::CA_LOADING:
				case game::CA_PRIMED:
					s = game::UI_SafeTranslateString("EXE_AWAITINGHOST");
					break;
				default:
					break;
				}

				auto scale = 0.25f;
				auto* font = game::UI_GetFontHandle(placement, game::FONT_TYPE_TEXT_FONT, scale);
				auto width = game::UI_TextWidth(s, 0, font, scale);

				game::UI_DrawText(
					placement,
					s,
					std::numeric_limits<int>::max(),
					font,
					(640.0f - (float)width) * 0.5f,
					420.0f,
					0,
					0,
					scale,
					game::colorWhite,
					game::FONT_STYLE_SHADOW);
			}

			if (!CG_IsShowingProgress_FastFile() && *game::cl_serverLoadingMap && *game::g_waitingForServer)
			{
				const char* s = nullptr;
				const char* dots = "";

				const auto val = (game::Sys_Milliseconds() / 500) & 3;
				switch (val)
				{
				case 1:
					dots = ".";
					break;
				case 2:
					dots = "..";
					break;
				case 3:
					dots = "...";
					break;
				default:
					dots = "";
					break;
				}

				s = game::UI_SafeTranslateString("CGAME_WAITINGFORSERVERLOAD");
				const char* sa = utils::string::va("%s%s", s, dots);

				auto scale = 0.25f;
				auto* font = game::UI_GetFontHandle(placement, game::FONT_TYPE_TEXT_FONT, scale);
				auto width = game::UI_TextWidth(sa, 0, font, scale);

				game::UI_DrawText(
					placement,
					sa,
					std::numeric_limits<int>::max(),
					font,
					(640.0f - (float)width) * 0.5f,
					400.0f,
					0,
					0,
					scale,
					game::colorWhite,
					game::FONT_STYLE_SHADOW);
			}
		}

		void UI_DrawLoadScreenMaterial()
		{
			auto* mapname = game::Dvar_FindVar("ui_mapname")->current.string;
			auto* loadscreen_name = utils::string::va("loadscreen_%s", mapname);
			auto* material = game::Material_RegisterHandle(loadscreen_name);
			if (!material || game::DB_IsXAssetDefault(game::ASSET_TYPE_MATERIAL, loadscreen_name))
			{
				return;
			}

			const auto* placement = game::ScrPlace_GetActivePlacement();

			game::R_AddCmdDrawStretchPic(0.0f, 0.0f, placement->realViewportSize[0], placement->realViewportSize[1], 0.0f, 0.0f, 1.0f, 1.0f, (float*)game::colorWhite.get(), material, 0);

			auto* vignette_name = "load_vignette";
			auto* vignette = game::Material_RegisterHandle(vignette_name);
			if (vignette && !game::DB_IsXAssetDefault(game::ASSET_TYPE_MATERIAL, vignette_name))
			{
				static const game::vec4_t colorBlack{ 0.0f, 0.0f, 0.0f, 1.0f };
				game::R_AddCmdDrawStretchPic(0.0f, 0.0f, placement->realViewportSize[0], placement->realViewportSize[1], 0.0f, 0.0f, 1.0f, 1.0f, (float*)&colorBlack, vignette, 0);
			}
		}

		void UI_DrawLoadBarState()
		{
			const auto* placement = game::ScrPlace_GetActivePlacement();

			static const game::vec4_t color{ 0.2f, 0.2f, 0.2f, 1.0f };

			game::UI_FillRect(placement, 192.0f, 430.0f, 256.0f, 2.0f, 0, 0, &color);
			game::UI_DrawLoadBar(placement, 192.0f, 430.0f, 256.0f, 2.0f, 0, 0, game::colorWhite, *game::whiteMaterial);
		}

		void UI_DrawTempConnectScreen([[maybe_unused]] int localClientNum)
		{
			UI_DrawLoadScreenMaterial();
			UI_DrawLoadScreenInfo();
			UI_DrawLoadBarState();
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

			//utils::hook::jump(0x1409BC7D7, UI_DrawTempConnectScreen);
			utils::hook::call(0x1409BC84C, UI_DrawTempConnectScreen);

			// Allows loading of _load fastfiles
			dvars::override::register_bool("frontEndUseLoadFastfiles", true, game::DVAR_FLAG_NONE);
		}
	};
}

REGISTER_COMPONENT(loadscreen::component)