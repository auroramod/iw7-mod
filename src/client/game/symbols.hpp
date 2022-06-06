#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	/***************************************************************
	 * Functions
	 **************************************************************/

	WEAK symbol<void()> Com_Quit_f{ 0xBADC90 };

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{ 0xB7C290 };
	WEAK symbol<void(int localClientNum, void(*))> Cbuf_AddCall{ 0xB7C220 };

	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{ 0xB7D040 };
	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{ 0xB7C8F0 };
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{ 0xB7D630 };
	WEAK symbol<void(const char* text_in)> Cmd_TokenizeString{ 0xB7D850 };
	WEAK symbol<void()> Cmd_EndTokenizeString{ 0xB7CC90 };

	WEAK symbol<const char* (XAssetType type, XAssetHeader header)> DB_GetXAssetHeaderName{ 0x9E5BA0 };
	WEAK symbol<bool(std::int32_t, void(__cdecl*)(XAssetHeader, void*), const void*)> DB_EnumXAssets_FastFile{ 0xA76CE0 };
	WEAK symbol<bool(XAssetType type, const char* name)> DB_IsXAssetDefault{ 0xA780D0 };
	WEAK symbol<XAssetHeader(XAssetType type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{ 0xA76E00 };

	WEAK symbol<dvar_t* (const char* name, bool value,
		unsigned int flags, const char* description)> Dvar_RegisterBool{ 0xCEB380 };
	WEAK symbol<dvar_t* (const char* name, int value, int min, int max,
		unsigned int flags, const char* description)> Dvar_RegisterInt{ 0xCEB920 };
	WEAK symbol<dvar_t* (const char* name, float value, float min,
		float max, unsigned int flags, const char* description)> Dvar_RegisterFloat{ 0xCEB890 };
	WEAK symbol<dvar_t* (const char* name, const char* value,
		unsigned int flags, const char* description)> Dvar_RegisterString{ 0xCEBD50 };
	WEAK symbol<dvar_t* (const char* name, float x, float y,
		float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec2{ 0xCEBF50 };
	WEAK symbol<dvar_t* (const char* name, float x, float y, float z,
		float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec3{ 0xCEBFE0 };
	WEAK symbol<dvar_t* (const char* name, float x, float y, float z,
		float w, float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec4{ 0xCEC110 };

	WEAK symbol<void(const char* name, const char* string)> Dvar_SetCommand{ 0xCECB30 };
	WEAK symbol<dvar_t* (const char* name)> Dvar_FindVar{ 0xCEA460 };
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{ 0xBB1F30 };
	WEAK symbol<const char* (dvar_t* dvar, dvar_value value)> Dvar_ValueToString{ 0xCEED00 };
	WEAK symbol<int(const char* name)> Dvar_GenerateChecksum{ 0xCEA520 };
#define Dvar_GenerateHash(name) \
	Dvar_GenerateChecksum(name);

	WEAK symbol<unsigned int(int controllerIndex)> Live_SyncOnlineDataFlags{ 0xDC5CE0 };

	WEAK symbol<Material* (const char* material)> Material_RegisterHandle{ 0xE11CE0 };

	WEAK symbol<Font_s* (const char* font, int size)> R_RegisterFont{ 0xDFC670 };
	WEAK symbol<int(const char* text, int maxChars, Font_s* font)> R_TextWidth{ 0xDFC770 };
	WEAK symbol<int(void* font)> R_GetFontHeight{ 0x12727B0 };
	WEAK symbol<void* (int a1)> R_DrawSomething{ 0xDFBD00 };
	WEAK symbol<void()> R_SyncRenderThread{ 0xE27EE0 };
	WEAK symbol<void(float x, float y, float width, float height, float s0, float t0, float s1, float t1,
		float* color, Material* material, int unk)> R_AddCmdDrawStretchPic{ 0xE24DC0 };
	WEAK symbol<void* (const char* text, int maxChars, void* font, int fontHeight, float x,
		float y, float xScale, float yScale, float rotation, float* color,
		int cursor_pos, char cursor_char, void* style_unk, int a14, int a15, int a16, int a17)> IW7_AddBaseDrawTextCmd{ 0xE23D90 };
#define R_AddCmdDrawText(TXT, MC, F, X, Y, XS, YS, R, C, S) \
	IW7_AddBaseDrawTextCmd(TXT, MC, F, game::R_GetFontHeight(F), X, Y, XS, YS, R, C,-1, 0, game::R_DrawSomething(S), 0, 0, 0, 0)
#define R_AddCmdDrawTextWithCursor(TXT, MC, F, UNK, X, Y, XS, YS, R, C, S, CP, CC) \
	IW7_AddBaseDrawTextCmd(TXT, MC, F, game::R_GetFontHeight(F), X, Y, XS, YS, R, C, CP, CC, game::R_DrawSomething(S), 0, 0, 0, 0)

	WEAK symbol<int()> Sys_Milliseconds{ 0xD58110 };

	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetViewPlacement{ 0x9E4090 };

	WEAK symbol<void(int clientNum, svscmd_type type, const char* text)> SV_GameSendServerCommand{ 0xC54780 };

	/***************************************************************
	 * Variables
	 **************************************************************/

	WEAK symbol<CmdArgs> sv_cmd_args{ 0x5D65C20 };
	WEAK symbol<CmdArgs> cmd_args{ 0x5D65B70 };
	WEAK symbol<cmd_function_s*> cmd_functions{ 0x5D65CC8 };

	WEAK symbol<int> keyCatchers{ 0x2246C34 };
	WEAK symbol<PlayerKeyState> playerKeys{ 0x523BA0C };

	WEAK symbol<int> dvarCount{ 0x7595E54 };
	WEAK symbol<dvar_t*> dvarPool{ 0x7595E60 };
}
