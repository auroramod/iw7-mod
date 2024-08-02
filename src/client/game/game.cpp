#include <std_include.hpp>
#include "game.hpp"
#include "dvars.hpp"

#include "component/console/console.hpp"

#include <utils/flags.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace game
{
	uint64_t base_address;

	namespace environment
	{
		bool is_dedi()
		{
			static const auto dedicated = utils::flags::has_flag("dedicated");
			return dedicated;
		}
	}

	namespace shared
	{
		void client_println(int client_num, const std::string& text)
		{
			if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
			{
				game::CG_Utils_GameMessage(client_num, text.data(), 0); // why is nothing printed?
			}
			else
			{
				game::SV_GameSendServerCommand(client_num, game::SV_CMD_RELIABLE,
					utils::string::va("f \"%s\"", text.data()));
			}
		}

		bool cheats_ok(int client_num, bool print)
		{
			if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_SP)
			{
				return true;
			}

			const auto sv_cheats = game::Dvar_FindVar("sv_cheats");
			if (!sv_cheats || !sv_cheats->current.enabled)
			{
				if(print)
					client_println(client_num, "GAME_CHEATSNOTENABLED");
				return false;
			}

			return true;
		}
	}

	int Cmd_Argc()
	{
		return cmd_args->argc[cmd_args->nesting];
	}

	const char* Cmd_Argv(const int index)
	{
		return cmd_args->argv[cmd_args->nesting][index];
	}

	int SV_Cmd_Argc()
	{
		return sv_cmd_args->argc[sv_cmd_args->nesting];
	}

	const char* SV_Cmd_Argv(const int index)
	{
		return sv_cmd_args->argv[sv_cmd_args->nesting][index];
	}

	const char* G_GAME_MODE_STRINGS[] =
	{
		"none",
		"sp",
		"mp",
		"cp",
	};

	const char* G_GAME_MODE_STRINGS_FORMATTED[] =
	{
		"None",
		"Singleplayer",
		"Multiplayer",
		"Zombies",
	};

	const char* Com_GameMode_GetActiveGameModeStr()
	{
		return G_GAME_MODE_STRINGS[game::Com_GameMode_GetActiveGameMode()];
	}

	const char* Com_GameMode_GetGameModeStr(GameModeType gameMode)
	{
		return G_GAME_MODE_STRINGS[gameMode];
	}

	bool CL_IsGameClientActive(const int localClientNum)
	{
		if (game::clientUIActives[localClientNum].frontEndSceneState[0])
		{
			return false;
		}
		return game::clientUIActives[localClientNum].cgameInitialized;
	}

	const char* g_assetNames[ASSET_TYPE_COUNT] =
	{
		"physicslibrary",
		"physicssfxeventasset",
		"physicsvfxeventasset",
		"physicsasset",
		"physicsfxpipeline",
		"physicsfxshape",
		"xanim",
		"xmodelsurfs",
		"xmodel",
		"mayhem",
		"material",
		"computeshader",
		"vertexshader",
		"hullshader",
		"domainshader",
		"pixelshader",
		"vertexdecl",
		"techset",
		"image",
		"soundglobals",
		"soundbank",
		"soundpatch",
		"col_map",
		"com_map",
		"glass_map",
		"aipaths",
		"navmesh",
		"map_ents",
		"fx_map",
		"gfx_map",
		"gfx_map_trzone",
		"iesprofile",
		"lightdef",
		"ui_map",
		"animclass",
		"playeranim",
		"gesture",
		"localize",
		"attachment",
		"weapon",
		"vfx",
		"fx",
		"impactfx",
		"surfacefx",
		"aitype",
		"mptype",
		"character",
		"xmodelalias",
		"rawfile",
		"scriptfile",
		"stringtable",
		"leaderboarddef",
		"virtualleaderboarddef",
		"structureddatadef",
		"ddl",
		"tracer",
		"vehicle",
		"addon_map_ents",
		"netconststrings",
		"luafile",
		"scriptable",
		"equipsndtable",
		"vectorfield",
		"particlesimanimation",
		"streaminginfo",
		"laser",
		"ttf",
		"suit",
		"suitanimpackage",
		"spaceshiptarget",
		"rumble",
		"rumblegraph",
		"animpkg",
		"sfxpkg",
		"vfxpkg",
		"behaviortree",
		"animarchetype",
		"proceduralbones",
		"reticle",
		"gfxlightmap",
	};

	const char* DB_GetXAssetName(const XAsset* asset)
	{
		return DB_GetXAssetHeaderName(asset->type, asset->header);
	}

	void DB_EnumXAssets(const std::int32_t type, const std::function<void(XAssetHeader)>& callback)
	{
		DB_EnumXAssets_FastFile(type, static_cast<void(*)(XAssetHeader, void*)>([](XAssetHeader header, void* data)
		{
			const auto& cb = *static_cast<const std::function<void(XAssetHeader)>*>(data);
			cb(header);
		}), &callback);
	}

	void SV_CmdsMP_RequestMapRestart(bool loadScripts, bool migrate)
	{
		if (!Com_IsAnyLocalServerStarting())
		{
			SND_StopSounds(2);
			SND_SetMusicState("\0");
			*sv_map_restart = 1;
			*sv_loadScripts = loadScripts;
			*sv_migrate = migrate;
			Cbuf_AddCall(0, SV_CmdsMP_CheckLoadGame);
		}
	}

	bool SV_ClientIsBot(client_t* client)
	{
		return client->remoteAddress.type == NA_BOT;
	}

	bool SV_ClientIsBot(unsigned int client_num)
	{
		return svs_clients[client_num]->remoteAddress.type == NA_BOT;
	}

	void G_LogPrintf(const char* fmt, ...)
	{
		if (!dvars::logfile->current.enabled)
		{
			return;
		}

		char va_buffer[0x400] = { 0 };

		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(va_buffer, fmt, ap);
		va_end(ap);

		const auto file = dvars::g_log->current.string;
		const auto time = *game::gameTime / 1000;

		utils::io::write_file(file, utils::string::va("%3i:%i%i %s",
			time / 60,
			time % 60 / 10,
			time % 60 % 10,
			va_buffer
		), true);
	}

	void Key_Bindlist_f()
	{
		for (int keynum = 0; keynum < 256; ++keynum)
		{
			auto value = game::Key_GetActiveBinding(0, keynum);
			if (value)
			{
				auto* binding = game::Key_GetCmdForBinding(0, value);
				if (*binding)
				{
					console::info("%s \"%s\"\n", game::Key_KeynumToString(keynum, 0, 0), binding);
				}
			}
		}
	}
}

size_t reverse_b(const size_t ptr)
{
	return ptr - game::base_address;
}

size_t reverse_b(const void* ptr)
{
	return reverse_b(reinterpret_cast<size_t>(ptr));
}
