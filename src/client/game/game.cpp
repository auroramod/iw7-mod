#include <std_include.hpp>
#include "game.hpp"

#include <utils/flags.hpp>

namespace game
{
	uint64_t base_address;

	void load_base_address()
	{
		const auto module = GetModuleHandleA(0);
		base_address = reinterpret_cast<uint64_t>(module);
	}

	namespace environment
	{
		bool is_dedi()
		{
			static const auto dedicated = utils::flags::has_flag("dedicated");
			return dedicated;
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
		"soundbankpatch",
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
		"impactfx",
		"surfacefx",
		"aitype",
		"mptype",
		"character",
		"xmodelalias",
		"ASSET_TYPE_UNKNOWN",
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
}

uintptr_t operator"" _b(const uintptr_t ptr)
{
	return game::base_address + ptr;
}