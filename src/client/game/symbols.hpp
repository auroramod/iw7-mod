#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	/***************************************************************
	 * Functions
	 **************************************************************/

	WEAK symbol<void(int type, VariableUnion u)> AddRefToValue{ 0xC04360 };
	WEAK symbol<void(int type, VariableUnion u)> RemoveRefToValue{ 0xC05DB0 };
	WEAK symbol<void(unsigned int id)> AddRefToObject{ 0xC04350 };
	WEAK symbol<void(unsigned int id)> RemoveRefToObject{ 0xC05CA0 };
	WEAK symbol<unsigned int(unsigned int id)> AllocThread{ 0xC04580 };
	WEAK symbol<ObjectVariableValue* (unsigned int* id)> AllocVariable{ 0xC04650 };
	WEAK symbol<unsigned int(const char* filename)> Scr_LoadScript{ 0xBFD900 };
	WEAK symbol<unsigned int(const char* filename, unsigned int handle)> Scr_GetFunctionHandle{ 0xBFD780 };
	WEAK symbol<unsigned int(int handle, int num_param)> Scr_ExecThread{ 0xC0ACD0 };

	WEAK symbol<float()> BG_GetGravity{ 0x68DD0 };

	WEAK symbol<void(errorParm code, const char* message, ...)> Com_Error{ 0xB8D830 };

	WEAK symbol<void()> Com_Quit_f{ 0xBADC90 };
	WEAK symbol<void()> j_Com_Quit_f{ 0xD33D10 };

	WEAK symbol<bool()> Com_FrontEnd_IsInFrontEnd{ 0x5AE6C0 };
	WEAK symbol<void()> Com_FrontEnd_ExitFrontEnd{ 0x5AE4F0 };
	WEAK symbol<bool()> Com_FrontEndScene_IsActive{ 0x5AEBA0 };
	WEAK symbol<void()> Com_FrontEndScene_ShutdownAndDisable{ 0x5AEFB0 };
	WEAK symbol<void()> Com_FrontEndScene_Shutdown{ 0x5AED00 };

	WEAK symbol<int> s_frontEndScene_state{ 0x4BFF608 };

	WEAK symbol<void(GameModeType)> Com_GameMode_SetDesiredGameMode{ 0x5AFDA0 };
	WEAK symbol<GameModeType()> Com_GameMode_GetActiveGameMode{ 0x5AFD50 };
	WEAK symbol<bool(const char* name)> Com_GameMode_SupportsMap{ 0x5AFE10 };

	WEAK symbol<bool()> Com_IsAnyLocalServerStarting{ 0xBAD9C0 };
	WEAK symbol<bool()> Com_IsAnyLocalServerRunning{ 0xBAD9A0 };

	WEAK symbol<void(const char* localizedMessage, const char* localizedTitle)> Com_SetLocalizedErrorMessage{ 0xBAF300 };

	WEAK symbol<void()> Com_SyncThreads{ 0xBB02D0 };

	WEAK symbol<void(const char* finalmsg)> Com_Shutdown{ 0xBAFEA0 };

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{ 0xB7C290 };
	WEAK symbol<void(int localClientNum, void(*))> Cbuf_AddCall{ 0xB7C220 };
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* buffer)> Cbuf_ExecuteBuffer{ 0xB7C3B0 };
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* buffer, void(int, int, const char*))> Cbuf_ExecuteBufferInternal{ 0xB7C3C0 };

	WEAK symbol<void(int localClientNum, const char* msg, int flags)> CG_Utils_GameMessage{ 0x1D7FC0 };
	WEAK symbol<void(int localClientNum, const char* msg, int flags)> CG_Utils_BoldGameMessage{ 0x1D7F10 };

	WEAK symbol<void(int localClientNum, void* hostInfo, const netadr_s* addr, const char* mapname, const char* gametype)>
		CL_MainMP_ConnectAndPreloadMap{ 0x9AED80 };

	WEAK symbol<void(int localClientNum, int andMask)> CL_Keys_RemoveCatcher{ 0x9A9B00 };

	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{ 0xB7D040 };
	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{ 0xB7C8F0 };
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{ 0xB7D630 };
	WEAK symbol<void(const char* text_in)> Cmd_TokenizeString{ 0xB7D850 };
	WEAK symbol<void()> Cmd_EndTokenizeString{ 0xB7CC90 };

	WEAK symbol<__int64(void* stream, int flush)> db_inflate{ 0xE77380 };
	WEAK symbol<__int64(void* stream, const char* version, int stream_size)> db_inflateInit_{ 0xE77980 };
	WEAK symbol<__int64(void* stream)> db_inflateEnd{ 0xE777F0 };

	WEAK symbol<void(void* stream, void* memory, int size)> DB_Zip_InitThreadMemory{ 0xE78290 };
	WEAK symbol<void(void* stream)> DB_Zip_ShutdownThreadMemory{ 0xE782D0 };

	WEAK symbol<void(XZoneInfo* zoneInfo, unsigned int zoneCount, char syncMode)> DB_LoadXAssets{ 0xA78630 };
	WEAK symbol<int(XAssetType type, const char* name)> DB_XAssetExists{ 0xA7C3A0 };
	WEAK symbol<int(const RawFile* rawfile, char* buf, int size)> DB_GetRawBuffer{ 0xA77AB0 };

	WEAK symbol<const char* (XAssetType type, XAssetHeader header)> DB_GetXAssetHeaderName{ 0x9E5BA0 };
	WEAK symbol<bool(std::int32_t, void(__cdecl*)(XAssetHeader, void*), const void*)> DB_EnumXAssets_FastFile{ 0xA76CE0 };
	WEAK symbol<bool(XAssetType type, const char* name)> DB_IsXAssetDefault{ 0xA780D0 };
	WEAK symbol<XAssetHeader(XAssetType type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{ 0xA76E00 };
	WEAK symbol<bool(const char* zoneName)> DB_IsLocalized{ 0x3BC500 };
	WEAK symbol<char* (const char* filename, char* buf, int size)> DB_ReadRawFile{ 0xA79E30 };
	WEAK symbol<int(const RawFile* rawfile)> DB_GetRawFileLen{ 0xF20AF0 };

	WEAK symbol<const char* (const DDLState* state, int enumValue)> DDL_Lookup_GetEnumString{ 0x30430 };
	WEAK symbol<bool(const DDLState* state)> DDL_StateIsLeaf{ 0x2E3C0 };
	WEAK symbol<DDLType(const DDLState* state)> DDL_GetType{ 0x2E5A0 };
	WEAK symbol<DDLValue(const DDLState* state, const DDLContext* ddlContext)> DDL_GetValue{ 0x2F5E0 };

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

	WEAK symbol<dvar_t* (unsigned int checksum, const char* string, DvarSetSource source)> Dvar_SetFromStringByChecksum{ 0xCECDB0 };
	WEAK symbol<dvar_t* (const char* name, const char* string, DvarSetSource source)> Dvar_SetFromStringByName{ 0xCECF30 };
	WEAK symbol<void(dvar_t* dvar, const char* string, DvarSetSource source)> Dvar_SetFromStringFromSource{ 0xCECFF0 };

	WEAK symbol<void(const char* name, const char* string)> Dvar_SetCommand{ 0xCECB30 };
	WEAK symbol<dvar_t* (const char* name)> Dvar_FindVar{ 0xCEA460 };
	WEAK symbol<void(void* callback, void* userData)> Dvar_ForEach{ 0xCEA480 };
	WEAK symbol<dvar_t* (unsigned int checksum)> Dvar_FindMalleableVar{ 0xCEA3C0 };
	WEAK symbol<void(const dvar_t* dvar)> Dvar_ClearModified{ 0xCE9E90 };
	WEAK symbol<const char* (const dvar_t* dvar)> Dvar_DisplayableLatchedValue{ 0xCEA1D0 };
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{ 0xBB1F30 };
	WEAK symbol<const char* (dvar_t* dvar, DvarValue value)> Dvar_ValueToString{ 0xCEED00 };
	WEAK symbol<void(dvar_t* dvar, DvarSetSource source)> Dvar_Reset{ 0xCEC490 };
	WEAK symbol<unsigned int(const char* name)> Dvar_GenerateChecksum{ 0xCEA520 };
	WEAK symbol<void(dvar_t* dvar, int value)> Dvar_SetInt{ 0xCED3D0 };
	WEAK symbol<void(bool cheatOverride)> Dvar_OverrideCheatProtection{ 0XCEB250 };

	WEAK symbol<__int64(const char* qpath, char** buffer)> FS_ReadFile{ 0xCDE200 };
	WEAK symbol<void(char* buffer)> FS_FreeFile{ 0xCDE1F0 };
	WEAK symbol<void(int h, const char* fmt, ...)> FS_Printf{ 0xCDD1C0 };

	WEAK symbol<unsigned int(unsigned int)> GetObjectType{ 0xC059E0 };
	WEAK symbol<unsigned int(unsigned int, unsigned int)> GetVariable{ 0xC05A90 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int unsignedValue)> GetNewVariable{ 0xC05660 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int unsignedValue)> GetNewArrayVariable{ 0xC054E0 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int name)> FindVariable{ 0xC05100 };
	WEAK symbol<unsigned int(int entnum, unsigned int classnum)> FindEntityId{ 0xC05000 };
	WEAK symbol<void(unsigned int parentId, unsigned int index)> RemoveVariableValue{ 0xC05E90 };
	WEAK symbol<void(VariableValue* result, unsigned int classnum,
		int entnum, int offset)> GetEntityFieldValue{ 0xC09CC0 };

	WEAK symbol<int(int clientNum)> G_MainMP_GetClientScore{ 0xB20550 };
	WEAK symbol<void* (void*, const char* name)> G_GetWeaponForName { 0x733D40 };
	WEAK symbol<int (void*, const void* weapon, int dualWield, int startInAltMode, int usedBefore)> G_GivePlayerWeapon{ 0x733D40 };
	WEAK symbol<int(void*, const void* weapon, const bool isAlternate, int hadWeapon)> G_InitializeAmmo{ 0x733D40 };

	WEAK symbol<char* (char* string)> I_CleanStr{ 0xCFACC0 };

	WEAK symbol<const char* (int, int, int)> Key_KeynumToString{ 0x9A95E0 };

	WEAK symbol<bool()> LUI_CoD_InFrontEnd{ 0x615080 };
	WEAK symbol<void(bool frontend, bool errorRecovery)> LUI_CoD_Init{ 0x615090 };
	WEAK symbol<void()> LUI_CoD_Shutdown{ 0x617610 };
	WEAK symbol<void(int localClientNum, const char* menuName, int isPopup, int isModal, int isExclusive)> LUI_OpenMenu{ 0xCC0CA0 };
	WEAK symbol<void(int localClientNum, const char* menuName, int immediate)> LUI_CloseMenu{ 0xCC0C40 };
	WEAK symbol<void(int localClientNum)> LUI_CoD_CLoseAll{ 0x6135C0 };

	WEAK symbol<unsigned int(int controllerIndex)> Live_SyncOnlineDataFlags{ 0xDC5CE0 };
	WEAK symbol<std::uint64_t(int controllerIndex)> Live_GetXuid{ 0xD32A20 };

	WEAK symbol<PartyData* ()> Lobby_GetPartyData{ 0x9C3E20 };

	WEAK symbol<void()> LUI_EnterCriticalSection{ 0x600080 };
	WEAK symbol<void()> LUI_LeaveCriticalSection{ 0x602280 };

	WEAK symbol<Material* (const char* material)> Material_RegisterHandle{ 0xE11CE0 };

	WEAK symbol<void(netadr_s*, sockaddr*)> NetadrToSockadr{ 0xCE6B90 };
	WEAK symbol<void(netsrc_t, netadr_s*, const char*)> NET_OutOfBandPrint{ 0xBB4EE0 };
	WEAK symbol<void(netsrc_t sock, int length, const void* data, const netadr_s* to)> NET_SendLoopPacket{ 0xBB50A0 };
	WEAK symbol<bool(const char* s, netadr_s* a)> NET_StringToAdr{ 0xBB5180 };
	WEAK symbol<int(netadr_s a, netadr_s b)> NET_CompareAdr{ 0xBB49B0 };
	WEAK symbol<int(netadr_s a, netadr_s b)> NET_CompareBaseAdr{ 0xBB4A00 };

	WEAK symbol<PartyData* ()> Party_GetActiveParty{ 0x9CC010 };

	WEAK symbol<void(const unsigned int controllerIndex, XUID xuid)> PlayercardCache_AddToDownload{ 0xDB72E0 };

	WEAK symbol<GfxFont* (const char* font, int size)> R_RegisterFont{ 0xDFC670 };
	WEAK symbol<int(const char* text, int maxChars, GfxFont* font)> R_TextWidth{ 0xDFC770 };
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

	WEAK symbol<std::uint64_t(const void* session, const int clientNum)> Session_GetXuid{ 0xC72AB0 };
	WEAK symbol<bool(const SessionData* session, const int memberIndex)> Session_IsHost{ 0xD9B470 };

	WEAK symbol<int(const char* str, std::uint64_t* xuid)> StringToXUID{ 0xCE6C40 };

	WEAK symbol<char* ()> Sys_Cwd{ 0xCFE5A0 };
	
	WEAK symbol<int()> Sys_Milliseconds{ 0xD58110 };

	WEAK symbol<HANDLE(Sys_Folder folder, const char* baseFilename)> Sys_CreateFile{ 0xCFDF50 };

	WEAK symbol<int(int length, void const* data, const netadr_s* to)> Sys_SendPacket{ 0xD57DE0 };
	WEAK symbol<int(netadr_s* net_from, msg_t* net_message)> Sys_GetPacket{ 0xD57D50 };

	WEAK symbol<bool()> Sys_IsDatabaseThread{ 0xBB7B30 };

	WEAK symbol<const char* ()> SEH_GetCurrentLanguageCode{ 0xCBAF50 };
	WEAK symbol<char* ()> SEH_GetCurrentLanguageName{ 0xCBB090 };
	WEAK symbol<char* (int code)> SEH_GetLanguageName{ 0xCBB140 };

	WEAK symbol<void(const char* name, int allocDir)> PMem_BeginAlloc{ 0xCF0E10 };
	WEAK symbol<void(const char* name, int allocDir)> PMem_EndAlloc{ 0xCF1070 };
	WEAK symbol<char* (const size_t size, unsigned int alignment, int type, int source)> PMem_AllocFromSource_NoDebug{ 0xCF0A90 };
	WEAK symbol<void(const char* name, int allocDir)> PMem_Free{ 0xCF10D0 };

	WEAK symbol<unsigned int(unsigned int localId, const char* pos,
		unsigned int paramcount)> VM_Execute{ 0xC0CDB0 };

	WEAK symbol<void(unsigned int id, scr_string_t stringValue,
		unsigned int paramcount)> Scr_NotifyId{ 0xC0C2B0 };
	WEAK symbol<const float* (const float* v)> Scr_AllocVector{ 0xC06960 };
	WEAK symbol<void(int)> Scr_AddInt{ 0xC0A580 };
	WEAK symbol<bool(VariableValue* value)> Scr_CastString{ 0xC06AE0 };
	WEAK symbol<void()> Scr_ClearOutParams{ 0xC0ABC0 };
	WEAK symbol<unsigned int(int classnum, unsigned int entnum)> Scr_GetEntityId{ 0xC08FA0 };
	WEAK symbol<scr_entref_t(unsigned int entnum)> Scr_GetEntityIdRef{ 0xC09050 };
	WEAK symbol<int(unsigned int classnum, int entnum, int offset)> Scr_SetObjectField{ 0x40B6E0 };
	WEAK symbol<int()> Scr_GetInt{ 0xC0B950 };
	WEAK symbol<void()> Scr_ErrorInternal{ 0xC0AC30 };

	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetViewPlacement{ 0x9E4090 };

	WEAK symbol<const char* (const StringTable* table, const int comparisonColumn, const char* value, const int valueColumn)> StringTable_Lookup{ 0xCE7950 };
	WEAK symbol<const char* (const StringTable* table, const int row, const int column)> StringTable_GetColumnValueForRow{ 0xCE78E0 };

	WEAK symbol<scr_string_t(const char* str, unsigned int user)> SL_GetString{ 0xC037E0 };
	WEAK symbol<const char*(scr_string_t stringValue)> SL_ConvertToString{ 0xC03300 };
	WEAK symbol<unsigned int(const char* str)> SL_GetCanonicalString{ 0xBFD340 };

	WEAK symbol<void(const char* string)> SV_Cmd_TokenizeString{ 0xB7DD00 };
	WEAK symbol<void()> SV_Cmd_EndTokenizedString{ 0xB7DCC0 };
	WEAK symbol<void(const char* map, const char* gameType, int clientCount, int agentCount, bool hardcore,
		bool mapIsPreloaded, bool migrate)> SV_CmdsMP_StartMapForParty{ 0xC4D150 };
	WEAK symbol<void()> SV_CmdsMP_CheckLoadGame{ 0xC4C9E0 };
	WEAK symbol<void()> SV_CmdsSP_MapRestart_f{ 0xC12B30 };
	WEAK symbol<void()> SV_CmdsSP_FastRestart_f{ 0xC12AF0 };
	WEAK symbol<int (int clientNum)> SV_ClientMP_GetClientPing{ 0xC507D0 };
	WEAK symbol<char* (int entNum)> SV_GameMP_GetGuid{ 0xC12410 };
	WEAK symbol<void()> SV_MainMP_KillLocalServer{ 0xC58DF0 };
	WEAK symbol<SessionData* ()> SV_MainMP_GetServerLobby{ 0xC58DA0 };
	WEAK symbol<void(int clientNum, svscmd_type type, const char* text)> SV_GameSendServerCommand{ 0xC54780 };
	WEAK symbol<void(client_t* drop, const char* reason, bool tellThem)> SV_DropClient{ 0xC4FBA0 };
	WEAK symbol<bool()> SV_Loaded{ 0xC114C0 };
	WEAK symbol<bool(const char* name)> SV_MapExists{ 0xCDB620 };
	WEAK symbol<bool(int clientNum)> SV_BotIsBot{ 0xC3BC90 };
	WEAK symbol<void* (int num)> SV_GetPlayerstateForClientNum{ 0xC123A0 };

	WEAK symbol<void(int)> SND_StopSounds{ 0xCA06E0 };
	WEAK symbol<void(const char*)> SND_SetMusicState{ 0xC9E110 };

	WEAK symbol<const char* (const char*)> UI_GetMapDisplayName{ 0xCC6270 };
	WEAK symbol<const char* (const char*)> UI_GetGameTypeDisplayName{ 0xCC61C0 };
	WEAK symbol<void(unsigned int localClientNum, const char** args)> UI_RunMenuScript{ 0xCC9710 };
	WEAK symbol<const char* (const char* string)> UI_SafeTranslateString{ 0xCC9790 };

	WEAK symbol<void* (jmp_buf* Buf, int Value)> longjmp{ 0x12C0758 };
	WEAK symbol<int(jmp_buf* Buf)> _setjmp{ 0x1423110 };

	/***************************************************************
	 * Variables
	 **************************************************************/

	//WEAK symbol<int> client_actives_something{ 0x2246C51 }; // 0x140995A8B in IW7 idb shows this to be cgameinitialized times the local client number

	WEAK symbol<int> g_script_error_level{ 0x6B16298 };
	WEAK symbol<jmp_buf> g_script_error{ 0x6B162A0 };

	WEAK symbol<unsigned int> levelEntityId{ 0x665A120 };

	WEAK symbol<CmdArgs> sv_cmd_args{ 0x5D65C20 };
	WEAK symbol<CmdArgs> cmd_args{ 0x5D65B70 };
	WEAK symbol<cmd_function_s*> cmd_functions{ 0x5D65CC8 };
	WEAK symbol<const char*> command_whitelist{ 0x14D1B70 };

	WEAK symbol<PLAYERCARD_CACHE_TASK_STAGE> g_DWPlayercardCacheDownloadTaskStage{ 0x80AE414 };
	WEAK symbol<CachedPlayerProfile> cached_playercards{ 0x80AE420 };

	WEAK symbol<GfxDrawMethod> gfxDrawMethod{ 0x83E86A8 };

	WEAK symbol<int> keyCatchers{ 0x2246C34 };
	WEAK symbol<PlayerKeyState> playerKeys{ 0x523BA0C };

	WEAK symbol<int> dvarCount{ 0x7595E54 };
	WEAK symbol<dvar_t*> dvarPool{ 0x7595E60 };

	WEAK symbol<char> isCheatOverride{ 0x1BBEF28 };

	WEAK symbol<gentity_s> g_entities{ 0x3D22610 };

	WEAK symbol<unsigned int> svs_numclients{ 0x6B229E0 };
	WEAK symbol<client_t*> svs_clients{ 0x6B22950 };

	WEAK symbol<SessionData> g_serverSession{ 0x6B4E080 };

	WEAK symbol<clientUIActive_t> clientUIActives{ 0x2246C30 };

	WEAK symbol<connection_data*> cl_con_data{ 0x1FE58B8 };

	WEAK symbol<int> sv_map_restart{ 0x6B2C9D4 };
	WEAK symbol<int> sv_loadScripts{ 0x6B2C9D8 };
	WEAK symbol<int> sv_migrate{ 0x6B2C9DC };

	WEAK symbol<SOCKET> query_socket{ 0x779FDC8 };

	WEAK symbol<DWORD> threadIds{ 0x602BAB0 };

	WEAK symbol<scrVarGlob_t> scr_VarGlob{ 0x6691180 };
	WEAK symbol<scrVmPub_t> scr_VmPub{ 0x6B183B0 };
	WEAK symbol<function_stack_t> scr_function_stack{ 0x6B22908 };

	WEAK symbol<PhysicalMemory> g_mem{ 0x7685560 };
	WEAK symbol<PhysicalMemory> g_scriptmem{ 0x7685FC0 };

	WEAK symbol<searchpath_s> fs_searchpaths{ 0x756DEE0 };

	WEAK symbol<DB_LoadData> g_load{ 0x52A8010 };
	WEAK symbol<int> g_authLoad_isSecure{ 0x529DD90 };
	WEAK symbol<DB_ReadStream> db_stream{ 0x52A8050 };
	WEAK symbol<db_z_stream_s> db_zip_stream{ 0x529DD30 };
	WEAK symbol<char*> db_zip_memory{ 0x525B500 };

	WEAK symbol<unsigned __int64> g_streamPos{ 0x5687E30 };

	WEAK symbol<bool> g_quitRequested{ 0x779CD44 };
  
	WEAK symbol<unsigned int> gameEntityId{ 0x665A124 };
	WEAK symbol<int> level_time{ 0x3C986D8 };

	namespace hks
	{
		WEAK symbol<lua_State*> lua_state{ 0x4FC35F0 };
		WEAK symbol<void(lua_State* s, const char* str, unsigned int l)> hksi_lua_pushlstring{ 0x309E0 };
		WEAK symbol<HksObject* (HksObject* result, lua_State* s, const HksObject* table, const HksObject* key)> hks_obj_getfield{ 0x11E14D0 };
		WEAK symbol<HksObject* (HksObject* result, lua_State* s, const HksObject* table, const HksObject* key)> hks_obj_gettable{ 0x11E19B0 };
		WEAK symbol<void(lua_State* s, const HksObject* tbl, const HksObject* key, const HksObject* val)> hks_obj_settable{ 0x11E26F0 };
		WEAK symbol<void(lua_State* s, int nargs, int nresults, const unsigned int* pc)> vm_call_internal{ 0x120CCE0 };
		WEAK symbol<HashTable* (lua_State* s, unsigned int arraySize, unsigned int hashSize)> Hashtable_Create{ 0x11D0590 };
		WEAK symbol<cclosure* (lua_State* s, lua_function function, int num_upvalues,
			int internal_, int profilerTreatClosureAsFunc)> cclosure_Create{ 0x11D07B0 };
		WEAK symbol<int(lua_State* s, int t)> hksi_luaL_ref{ 0x11EAE10 };
		WEAK symbol<int(lua_State* s, const HksCompilerSettings* options, const char* buff,
			unsigned __int64 sz, const char* name)> hksi_hksL_loadbuffer{ 0x11E2F50 };
		WEAK symbol<int(lua_State* s, const char* what, lua_Debug* ar)> hksi_lua_getinfo{ 0x11E48C0 };
		WEAK symbol<int(lua_State* s, int level, lua_Debug* ar)> hksi_lua_getstack{ 0x11E4AA0 };
		WEAK symbol<void(lua_State* s, const char* fmt, ...)> hksi_luaL_error{ 0x11EA860 };
		WEAK symbol<void(lua_State* s, int what, int data)> hksi_lua_gc{ 0x11EAF00 };
		WEAK symbol<void(lua_State* s, int t, int ref)> hksi_luaL_unref{ 0x11E4460 };
		WEAK symbol<const char*> s_compilerTypeName{ 0x1BDEEF0 };
	}
}
