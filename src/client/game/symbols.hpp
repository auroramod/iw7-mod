#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	/***************************************************************
	 * Functions
	 **************************************************************/

	WEAK symbol<void(int type, VariableUnion u)> AddRefToValue{ 0x140C04360 };
	WEAK symbol<void(int type, VariableUnion u)> RemoveRefToValue{ 0x140C05DB0 };
	WEAK symbol<void(unsigned int id)> AddRefToObject{ 0x140C04350 };
	WEAK symbol<void(unsigned int id)> RemoveRefToObject{ 0x140C05CA0 };
	WEAK symbol<unsigned int(unsigned int id)> AllocThread{ 0x140C04580 };
	WEAK symbol<ObjectVariableValue* (unsigned int* id)> AllocVariable{ 0x140C04650 };
	WEAK symbol<unsigned int(const char* filename)> Scr_LoadScript{ 0x140BFD900 };
	WEAK symbol<unsigned int(const char* filename, unsigned int handle)> Scr_GetFunctionHandle{ 0x140BFD780 };
	WEAK symbol<unsigned int(int handle, int num_param)> Scr_ExecThread{ 0x140C0ACD0 };

	WEAK symbol<float()> BG_GetGravity{ 0x140068DD0 };
	WEAK symbol<unsigned int(unsigned short* weapon, playerState_s* ps)> BG_PlayerLastWeaponHandForViewWeapon{ 0x14074B410 };
	WEAK symbol<bool(playerState_s* ps, weapAnimFiles_t anim, int hand, unsigned char pmoveHandler)> BG_ViewModelAnimExists{ 0x140751200 };
	WEAK symbol<char* (const unsigned short* weapon, bool isAlternate, char* output, unsigned int maxStringLen)> BG_GetWeaponNameComplete{ 0x140733D50 };

	WEAK symbol<void(errorParm code, const char* message, ...)> Com_Error{ 0x140B8D830 };

	WEAK symbol<void()> Com_Quit_f{ 0x140BADC90 };
	WEAK symbol<void()> j_Com_Quit_f{ 0x140D33D10 };

	WEAK symbol<bool()> Com_FrontEnd_IsInFrontEnd{ 0x1405AE6C0 };
	WEAK symbol<void()> Com_FrontEnd_ExitFrontEnd{ 0x1405AE4F0 };
	WEAK symbol<bool()> Com_FrontEndScene_IsActive{ 0x1405AEBA0 };
	WEAK symbol<void()> Com_FrontEndScene_ShutdownAndDisable{ 0x1405AEFB0 };
	WEAK symbol<void()> Com_FrontEndScene_Shutdown{ 0x1405AED00 };

	WEAK symbol<void(GameModeType)> Com_GameMode_SetDesiredGameMode{ 0x1405AFDA0 };
	WEAK symbol<GameModeType()> Com_GameMode_GetActiveGameMode{ 0x1405AFD50 };
	WEAK symbol<bool(const char* name)> Com_GameMode_SupportsMap{ 0x1405AFE10 };
	WEAK symbol<bool(Com_GameMode_Feature)> Com_GameMode_SupportsFeature{ 0x1405AFDE0 };

	WEAK symbol<bool()> Com_IsAnyLocalServerStarting{ 0x140BAD9C0 };
	WEAK symbol<bool()> Com_IsAnyLocalServerRunning{ 0x140BAD9A0 };

	WEAK symbol<void(const char* localizedMessage, const char* localizedTitle)> Com_SetLocalizedErrorMessage{ 0x140BAF300 };

	WEAK symbol<void(float startTimescale, float endTimescale, int deltaMsec)> Com_SetSlowMotion{ 0x140BAFD70 };

	WEAK symbol<void()> Com_SyncThreads{ 0x140BB02D0 };

	WEAK symbol<void(const char* finalmsg)> Com_Shutdown{ 0x140BAFEA0 };

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{ 0x140B7C290 };
	WEAK symbol<void(int localClientNum, void(*))> Cbuf_AddCall{ 0x140B7C220 };
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* buffer)> Cbuf_ExecuteBuffer{ 0x140B7C3B0 };
	WEAK symbol<void(int localClientNum, int controllerIndex, const char* buffer, void(int, int, const char*))> Cbuf_ExecuteBufferInternal{ 0x140B7C3C0 };

	WEAK symbol<void(int localClientNum, const char* msg, int flags)> CG_Utils_GameMessage{ 0x1401D7FC0 };
	WEAK symbol<void(int localClientNum, const char* msg, int flags)> CG_Utils_BoldGameMessage{ 0x1401D7F10 };

	WEAK symbol<void(int localClientNum, void* hostInfo, const netadr_s* addr, const char* mapname, const char* gametype)>
		CL_MainMP_ConnectAndPreloadMap{ 0x1409AED80 };
	WEAK symbol<void(int localClientNum, uiClientState_t* state)> CL_GetClientState{ 0x1409BFFF0 };

	WEAK symbol<void(int localClientNum, int andMask)> CL_Keys_RemoveCatcher{ 0x1409A9B00 };

	WEAK symbol<void(int localClientNum, int controllerIndex, const char* text)> Cmd_ExecuteSingleCommand{ 0x140B7D040 };
	WEAK symbol<void(const char* cmdName, void(), cmd_function_s* allocedCmd)> Cmd_AddCommandInternal{ 0x140B7C8F0 };
	WEAK symbol<void(const char* cmdName)> Cmd_RemoveCommand{ 0x140B7D630 };
	WEAK symbol<void(const char* text_in)> Cmd_TokenizeString{ 0x140B7D850 };
	WEAK symbol<void()> Cmd_EndTokenizeString{ 0x140B7CC90 };

	WEAK symbol<__int64(void* stream, int flush)> db_inflate{ 0x140E77380 };
	WEAK symbol<__int64(void* stream, const char* version, int stream_size)> db_inflateInit_{ 0x140E77980 };
	WEAK symbol<__int64(void* stream)> db_inflateEnd{ 0x140E777F0 };

	WEAK symbol<void(void* stream, void* memory, int size)> DB_Zip_InitThreadMemory{ 0x140E78290 };
	WEAK symbol<void(void* stream)> DB_Zip_ShutdownThreadMemory{ 0x140E782D0 };

	WEAK symbol<void(XZoneInfo* zoneInfo, unsigned int zoneCount, char syncMode)> DB_LoadXAssets{ 0x140A78630 };
	WEAK symbol<int(XAssetType type, const char* name)> DB_XAssetExists{ 0x140A7C3A0 };
	WEAK symbol<int(const RawFile* rawfile, char* buf, int size)> DB_GetRawBuffer{ 0x140A77AB0 };

	WEAK symbol<const char* (XAssetType type, XAssetHeader header)> DB_GetXAssetHeaderName{ 0x1409E5BA0 };
	WEAK symbol<bool(std::int32_t, void(__cdecl*)(XAssetHeader, void*), const void*)> DB_EnumXAssets_FastFile{ 0x140A76CE0 };
	WEAK symbol<bool(XAssetType type, const char* name)> DB_IsXAssetDefault{ 0x140A780D0 };
	WEAK symbol<XAssetHeader(XAssetType type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{ 0x140A76E00 };
	WEAK symbol<bool(const char* zoneName)> DB_IsLocalized{ 0x1403BC500 };
	WEAK symbol<char* (const char* filename, char* buf, int size)> DB_ReadRawFile{ 0x140A79E30 };
	WEAK symbol<int(const RawFile* rawfile)> DB_GetRawFileLen{ 0x140F20AF0 };
	WEAK symbol<void(int flags)> DB_UnloadFastfilesByZoneFlags{ 0x140A7BCE0 };
	WEAK symbol<int()> DB_Zones_GetInUseFlags{ 0x1403BC160 };

	WEAK symbol<const char* (const DDLState* state, int enumValue)> DDL_Lookup_GetEnumString{ 0x140030430 };
	WEAK symbol<bool(const DDLState* state)> DDL_StateIsLeaf{ 0x14002E3C0 };
	WEAK symbol<DDLType(const DDLState* state)> DDL_GetType{ 0x14002E5A0 };
	WEAK symbol<DDLValue(const DDLState* state, const DDLContext* ddlContext)> DDL_GetValue{ 0x14002F5E0 };

	WEAK symbol<dvar_t* (const char* name, bool value,
		unsigned int flags, const char* description)> Dvar_RegisterBool{ 0x140CEB380 };
	WEAK symbol<dvar_t* (const char* name, int value, int min, int max,
		unsigned int flags, const char* description)> Dvar_RegisterInt{ 0x140CEB920 };
	WEAK symbol<dvar_t* (const char* name, float value, float min,
		float max, unsigned int flags, const char* description)> Dvar_RegisterFloat{ 0x140CEB890 };
	WEAK symbol<dvar_t* (const char* name, const char* value,
		unsigned int flags, const char* description)> Dvar_RegisterString{ 0x140CEBD50 };
	WEAK symbol<dvar_t* (const char* name, float x, float y,
		float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec2{ 0x140CEBF50 };
	WEAK symbol<dvar_t* (const char* name, float x, float y, float z,
		float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec3{ 0x140CEBFE0 };
	WEAK symbol<dvar_t* (const char* name, float x, float y, float z,
		float w, float min, float max, unsigned int flags, const char* description)> Dvar_RegisterVec4{ 0x140CEC110 };

	WEAK symbol<dvar_t* (unsigned int checksum, const char* string, DvarSetSource source)> Dvar_SetFromStringByChecksum{ 0x140CECDB0 };
	WEAK symbol<dvar_t* (const char* name, const char* string, DvarSetSource source)> Dvar_SetFromStringByName{ 0x140CECF30 };
	WEAK symbol<void(dvar_t* dvar, const char* string, DvarSetSource source)> Dvar_SetFromStringFromSource{ 0x140CECFF0 };

	WEAK symbol<void(const char* name, const char* string)> Dvar_SetCommand{ 0x140CECB30 };
	WEAK symbol<dvar_t* (const char* name)> Dvar_FindVar{ 0x140CEA460 };
	WEAK symbol<void(void* callback, void* userData)> Dvar_ForEach{ 0x140CEA480 };
	WEAK symbol<dvar_t* (unsigned int checksum)> Dvar_FindMalleableVar{ 0x140CEA3C0 };
	WEAK symbol<void(const dvar_t* dvar)> Dvar_ClearModified{ 0x140CE9E90 };
	WEAK symbol<const char* (const dvar_t* dvar)> Dvar_DisplayableLatchedValue{ 0x140CEA1D0 };
	WEAK symbol<void(char* buffer, int index)> Dvar_GetCombinedString{ 0x140BB1F30 };
	WEAK symbol<const char* (dvar_t* dvar, DvarValue value)> Dvar_ValueToString{ 0x140CEED00 };
	WEAK symbol<void(dvar_t* dvar, DvarSetSource source)> Dvar_Reset{ 0x140CEC490 };
	WEAK symbol<unsigned int(const char* name)> Dvar_GenerateChecksum{ 0x140CEA520 };
	WEAK symbol<void(dvar_t* dvar, int value)> Dvar_SetInt{ 0x140CED3D0 };
	WEAK symbol<void(dvar_t* dvar, float value)> Dvar_SetFloat{ 0x140CECD90 };
	WEAK symbol<void(bool cheatOverride)> Dvar_OverrideCheatProtection{ 0x140CEB250 };

	WEAK symbol<__int64(const char* qpath, char** buffer)> FS_ReadFile{ 0x140CDE200 };
	WEAK symbol<void(char* buffer)> FS_FreeFile{ 0x140CDE1F0 };
	WEAK symbol<void(int h, const char* fmt, ...)> FS_Printf{ 0x140CDD1C0 };

	WEAK symbol<gentity_s* (scr_entref_t ent)> GetEntity{ 0x140B50EA0 };
	WEAK symbol<unsigned int(unsigned int)> GetObjectType{ 0x140C059E0 };
	WEAK symbol<unsigned int(unsigned int, unsigned int)> GetVariable{ 0x140C05A90 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int unsignedValue)> GetNewVariable{ 0x140C05660 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int unsignedValue)> GetNewArrayVariable{ 0x140C054E0 };
	WEAK symbol<unsigned int(unsigned int parentId, unsigned int name)> FindVariable{ 0x140C05100 };
	WEAK symbol<unsigned int(int entnum, unsigned int classnum)> FindEntityId{ 0x140C05000 };
	WEAK symbol<void(unsigned int parentId, unsigned int index)> RemoveVariableValue{ 0x140C05E90 };
	WEAK symbol<void(VariableValue* result, unsigned int classnum,
		int entnum, int offset)> GetEntityFieldValue{ 0x140C09CC0 };

	WEAK symbol<int(int clientNum)> G_MainMP_GetClientScore{ 0x140B20550 };
	WEAK symbol<void* (void*, const char* name)> G_GetWeaponForName { 0x140B4FCB0 };
	WEAK symbol<int (void*, const void* weapon, int dualWield, int startInAltMode, int usedBefore)> G_GivePlayerWeapon{ 0x140B69400 };
	WEAK symbol<int(void*, const void* weapon, const bool isAlternate, int hadWeapon)> G_InitializeAmmo{ 0x140B1B0E0 };
	WEAK symbol<scr_string_t(hitLocation_t hitLoc)> G_GetHitLocationString{ 0x1403F5D70 };

	WEAK symbol<char* (char* string)> I_CleanStr{ 0x140CFACC0 };

	WEAK symbol<const char* (int keynum, int translate, int gamePadOnly)> Key_KeynumToString{ 0x1409A95E0 };
	WEAK symbol<int (int localClientNum, int keynum)> Key_GetActiveBinding{ 0x1409A8E80 };
	WEAK symbol<const char* (int localClientNum, int keyBinding)> Key_GetCmdForBinding{ 0x1409A91D0 };
	WEAK symbol<const char* (const char* cmd)> Key_GetBindingForCmd{ 0x1409A8EA0 };

	WEAK symbol<bool()> LUI_CoD_InFrontEnd{ 0x140615080 };
	WEAK symbol<void(bool frontend, bool errorRecovery)> LUI_CoD_Init{ 0x140615090 };
	WEAK symbol<void()> LUI_CoD_Shutdown{ 0x140617610 };
	WEAK symbol<void(int localClientNum, const char* menuName, int isPopup, int isModal, int isExclusive)> LUI_OpenMenu{ 0x140CC0CA0 };
	WEAK symbol<void(int localClientNum, const char* menuName, int immediate)> LUI_CloseMenu{ 0x140CC0C40 };
	WEAK symbol<void(int localClientNum)> LUI_CoD_CLoseAll{ 0x1406135C0 };
	WEAK symbol<void(const char* fmt, ...)> LUI_Interface_DebugPrint{ 0x14061C43F };
	WEAK symbol<void()> LUI_EnterCriticalSection{ 0x140600080 };
	WEAK symbol<void()> LUI_LeaveCriticalSection{ 0x140602280 };

	WEAK symbol<unsigned int(int controllerIndex)> Live_SyncOnlineDataFlags{ 0x140DC5CE0 };
	WEAK symbol<std::uint64_t(int controllerIndex)> Live_GetXuid{ 0x140D32A20 };

	WEAK symbol<float()> LoadBar_GetLoadedFraction{ 0x1405DE190 };

	WEAK symbol<PartyData* ()> Lobby_GetPartyData{ 0x1409C3E20 };

	WEAK symbol<Material* (const char* material)> Material_RegisterHandle{ 0x140E11CE0 };

	WEAK symbol<bool(int localClientNum, const char* menuName)> Menu_IsMenuOpenAndVisible{ 0x14061CA70 };

	WEAK symbol<void(netadr_s*, sockaddr*)> NetadrToSockadr{ 0x140CE6B90 };
	WEAK symbol<void(netsrc_t, netadr_s*, const char*)> NET_OutOfBandPrint{ 0x140BB4EE0 };
	WEAK symbol<void(netsrc_t sock, int length, const void* data, const netadr_s* to)> NET_SendLoopPacket{ 0x140BB50A0 };
	WEAK symbol<bool(const char* s, netadr_s* a)> NET_StringToAdr{ 0x140BB5180 };
	WEAK symbol<int(netadr_s a, netadr_s b)> NET_CompareAdr{ 0x140BB49B0 };
	WEAK symbol<int(netadr_s a, netadr_s b)> NET_CompareBaseAdr{ 0x140BB4A00 };

	WEAK symbol<PartyData* ()> Party_GetActiveParty{ 0x1409CC010 };

	WEAK symbol<void(const unsigned int controllerIndex, XUID xuid)> PlayercardCache_AddToDownload{ 0x140DB72E0 };

	WEAK symbol<GfxFont* (const char* font, int size)> R_RegisterFont{ 0x140DFC670 };
	WEAK symbol<int(const char* text, int maxChars, GfxFont* font)> R_TextWidth{ 0x140DFC770 };
	WEAK symbol<int(GfxFont* font)> R_GetFontHeight{ 0x1412727B0 };
	WEAK symbol<FontGlowStyle* (int style)> R_Font_GetLegacyFontStyle{ 0x140DFBD00 };
	WEAK symbol<void()> R_SyncRenderThread{ 0x140E27EE0 };
	WEAK symbol<void(float x, float y, float width, float height, float s0, float t0, float s1, float t1,
		float* color, Material* material, int unk)> R_AddCmdDrawStretchPic{ 0x140E24DC0 };
	WEAK symbol<void* (const char* text, int maxChars, GfxFont* font, int fontHeight, float x,
		float y, float xScale, float yScale, float rotation, float* color,
		int cursorPos, char cursor, FontGlowStyle* glowStyle, int a14, int a15, int a16, int a17)> AddBaseDrawTextCmd{ 0x140E23D90 };
#define R_AddCmdDrawText(TXT, MC, F, X, Y, XS, YS, R, C, S) \
	AddBaseDrawTextCmd(TXT, MC, F, game::R_GetFontHeight(F), X, Y, XS, YS, R, C,-1, 0, game::R_Font_GetLegacyFontStyle(S), 0, 0, 0, 0)
#define R_AddCmdDrawTextWithCursor(TXT, MC, F, UNK, X, Y, XS, YS, R, C, S, CP, CC) \
	AddBaseDrawTextCmd(TXT, MC, F, game::R_GetFontHeight(F), X, Y, XS, YS, R, C, CP, CC, game::R_Font_GetLegacyFontStyle(S), 0, 0, 0, 0)

	WEAK symbol<std::uint64_t(const void* session, const int clientNum)> Session_GetXuid{ 0x140C72AB0 };
	WEAK symbol<bool(const SessionData* session, const int memberIndex)> Session_IsHost{ 0x140D9B470 };

	WEAK symbol<int(const char* str, std::uint64_t* xuid)> StringToXUID{ 0x140CE6C40 };

	WEAK symbol<char* ()> Sys_Cwd{ 0x140CFE5A0 };
	
	WEAK symbol<int()> Sys_Milliseconds{ 0x140D58110 };

	WEAK symbol<HANDLE(Sys_Folder folder, const char* baseFilename)> Sys_CreateFile{ 0x140CFDF50 };

	WEAK symbol<int(int length, void const* data, const netadr_s* to)> Sys_SendPacket{ 0x140D57DE0 };
	WEAK symbol<int(netadr_s* net_from, msg_t* net_message)> Sys_GetPacket{ 0x140D57D50 };

	WEAK symbol<bool()> Sys_IsDatabaseThread{ 0x140BB7B30 };

	WEAK symbol<const char* ()> SEH_GetCurrentLanguageCode{ 0x140CBAF50 };
	WEAK symbol<char* ()> SEH_GetCurrentLanguageName{ 0x140CBB090 };
	WEAK symbol<char* (int code)> SEH_GetLanguageName{ 0x140CBB140 };

	WEAK symbol<void(const char* name, int allocDir)> PMem_BeginAlloc{ 0x140CF0E10 };
	WEAK symbol<void(const char* name, int allocDir)> PMem_EndAlloc{ 0x140CF1070 };
	WEAK symbol<char* (const size_t size, unsigned int alignment, int type, int source)> PMem_AllocFromSource_NoDebug{ 0x140CF0A90 };
	WEAK symbol<void(const char* name, int allocDir)> PMem_Free{ 0x140CF10D0 };

	WEAK symbol<bool(__int64 weaponMap, playerState_s* ps, unsigned int hand, unsigned char pmoveHandler)> PM_Weapon_IsInInterruptibleState{ 0x140728210 };
	WEAK symbol<void(__int64 weaponMap, playerState_s* ps, unsigned char pmoveHandler)> PM_BuildWeaponAnimArrays{ 0x14071AC50 };
	WEAK symbol<void(pmove_t* pm, int hand)> PM_Weapon_Idle{ 0x140727910 };

	WEAK symbol<unsigned int(unsigned int localId, const char* pos,
		unsigned int paramcount)> VM_Execute{ 0x140C0CDB0 };

	WEAK symbol<void(unsigned int id, scr_string_t stringValue,
		unsigned int paramcount)> Scr_NotifyId{ 0x140C0C2B0 };
	WEAK symbol<const float* (const float* v)> Scr_AllocVector{ 0x140C06960 };
	WEAK symbol<void(int)> Scr_AddInt{ 0x140C0A580 };
	WEAK symbol<bool(VariableValue* value)> Scr_CastString{ 0x140C06AE0 };
	WEAK symbol<void()> Scr_ClearOutParams{ 0x140C0ABC0 };
	WEAK symbol<unsigned int(int classnum, unsigned int entnum)> Scr_GetEntityId{ 0x140C08FA0 };
	WEAK symbol<scr_entref_t(unsigned int entnum)> Scr_GetEntityIdRef{ 0x140C09050 };
	WEAK symbol<int(unsigned int classnum, int entnum, int offset)> Scr_SetObjectField{ 0x14040B6E0 };
	WEAK symbol<int()> Scr_GetInt{ 0x140C0B950 };
	WEAK symbol<void()> Scr_ErrorInternal{ 0x140C0AC30 };
	WEAK symbol<void(const char* str)> Scr_AllocGlobalString{ 0x140C03C70 };

	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetViewPlacement{ 0x1409E4090 };
	WEAK symbol<ScreenPlacement* ()> ScrPlace_GetActivePlacement{ 0x1409E4060 };

	WEAK symbol<const char* (const StringTable* table, const int comparisonColumn, const char* value, const int valueColumn)> StringTable_Lookup{ 0x140CE7950 };
	WEAK symbol<const char* (const StringTable* table, const int row, const int column)> StringTable_GetColumnValueForRow{ 0x140CE78E0 };

	WEAK symbol<scr_string_t(const char* str)> SL_FindString{ 0x140C035F0 };
	WEAK symbol<scr_string_t(const char* str, unsigned int user)> SL_GetString{ 0x140C037E0 };
	WEAK symbol<const char*(scr_string_t stringValue)> SL_ConvertToString{ 0x140C03300 };
	WEAK symbol<unsigned int(const char* str)> SL_GetCanonicalString{ 0x140BFD340 };

	WEAK symbol<void(const char* string)> SV_Cmd_TokenizeString{ 0x140B7DD00 };
	WEAK symbol<void()> SV_Cmd_EndTokenizedString{ 0x140B7DCC0 };
	WEAK symbol<void(int clientNum, const char* reason, bool kickedForInactivity)> SV_CmdsMP_KickClientNum{ 0x140C4CCC0 };
	WEAK symbol<void(const char* map, const char* gameType, int clientCount, int agentCount, bool hardcore,
		bool mapIsPreloaded, bool migrate)> SV_CmdsMP_StartMapForParty{ 0x140C4D150 };
	WEAK symbol<void()> SV_CmdsMP_CheckLoadGame{ 0x140C4C9E0 };
	WEAK symbol<void()> SV_CmdsSP_MapRestart_f{ 0x140C12B30 };
	WEAK symbol<void()> SV_CmdsSP_FastRestart_f{ 0x140C12AF0 };
	WEAK symbol<int (int clientNum)> SV_ClientMP_GetClientPing{ 0x140C507D0 };
	WEAK symbol<char* (int entNum)> SV_GameMP_GetGuid{ 0x140C12410 };
	WEAK symbol<void()> SV_MainMP_KillLocalServer{ 0x140C58DF0 };
	WEAK symbol<SessionData* ()> SV_MainMP_GetServerLobby{ 0x140C58DA0 };
	WEAK symbol<void(int clientNum, svscmd_type type, const char* text)> SV_GameSendServerCommand{ 0x140C54780 };
	WEAK symbol<void(client_t* drop, const char* reason, bool tellThem)> SV_DropClient{ 0x140C4FBA0 };
	WEAK symbol<bool()> SV_Loaded{ 0x140C114C0 };
	WEAK symbol<bool(const char* name)> SV_MapExists{ 0x140CDB620 };
	WEAK symbol<playerState_s* (int num)> SV_GetPlayerstateForClientNum{ 0x140C123A0 };
	WEAK symbol<void(unsigned int index, const char* val)> SV_SetConfigString{ 0x140C11CD0 };

	WEAK symbol<gentity_s* (const char* bot_name, unsigned int head, unsigned int body, unsigned int helmet)> SV_AddBot{ 0x140C4E340 };
	WEAK symbol<bool(int clientNum)> SV_BotIsBot{ 0x140C3BC90 };
	WEAK symbol<const char* ()> SV_BotGetRandomName{ 0x140C3B4E0 };
	WEAK symbol<int(gentity_s* ent)> SV_SpawnTestClient{ 0x140C51DE0 };

	WEAK symbol<void(int)> SND_StopSounds{ 0x140CA06E0 };
	WEAK symbol<void(const char*)> SND_SetMusicState{ 0x140C9E110 };

	WEAK symbol<const char* (const char*)> UI_GetMapDisplayName{ 0x140CC6270 };
	WEAK symbol<const char* (const char*)> UI_GetGameTypeDisplayName{ 0x140CC61C0 };
	WEAK symbol<void(unsigned int localClientNum, const char** args)> UI_RunMenuScript{ 0x140CC9710 };
	WEAK symbol<const char* (const char* string)> UI_SafeTranslateString{ 0x140CC9790 };
	WEAK symbol<const char* (const char* sourceString, int replaceInt)> UI_ReplaceConversionInt{ 0x140CC91A0 };
	WEAK symbol<GfxFont*(const ScreenPlacement* scrPlace, int fontEnum, float scale)> UI_GetFontHandle{ 0x140CD1240 };
	WEAK symbol<void(const ScreenPlacement* scrPlace, float x, float y, float width, float height,
		int horzAlign, int vertAlign, const vec4_t* color)> UI_FillRect{ 0x140CBDE30 };
	WEAK symbol<void(const ScreenPlacement* scrPlace, float x, float y, float w, float h,
		int horzAlign, int vertAlign, const vec4_t* color, Material* material)> UI_DrawLoadBar{ 0x140CBDC40 };
	WEAK symbol<void(const ScreenPlacement* scrPlace, const char* text, int maxChars, GfxFont* font, float x, float y,
		int horzAlign, int vertAlign, float scale, const vec4_t* color, int style)> UI_DrawText{ 0x140CC5C00 };
	WEAK symbol<int(const char* text, int maxChars, GfxFont* font, float scale)> UI_TextWidth{ 0x140CC9FA0 };

	WEAK symbol<float(XAnim_s* anims, unsigned int anim)> XAnimGetLengthMsec{ 0x140D761C0 };

	WEAK symbol<void* (jmp_buf* Buf, int Value)> longjmp{ 0x1412C0758 };
	WEAK symbol<int(jmp_buf* Buf)> _setjmp{ 0x141423110 };

	/***************************************************************
	 * Variables
	 **************************************************************/

	WEAK symbol<int> g_script_error_level{ 0x146B16298 };
	WEAK symbol<jmp_buf> g_script_error{ 0x146B162A0 };

	WEAK symbol<unsigned int> levelEntityId{ 0x14665A120 };

	WEAK symbol<CmdArgs> sv_cmd_args{ 0x145D65C20 };
	WEAK symbol<CmdArgs> cmd_args{ 0x145D65B70 };
	WEAK symbol<cmd_function_s*> cmd_functions{ 0x145D65CC8 };
	WEAK symbol<const char*> command_whitelist{ 0x1414D1B70 };

	WEAK symbol<PLAYERCARD_CACHE_TASK_STAGE> g_DWPlayercardCacheDownloadTaskStage{ 0x1480AE414 };
	WEAK symbol<CachedPlayerProfile> cached_playercards{ 0x1480AE420 };

	WEAK symbol<GfxDrawMethod> gfxDrawMethod{ 0x1483E86A8 };

	WEAK symbol<int> keyCatchers{ 0x142246C34 };
	WEAK symbol<PlayerKeyState> playerKeys{ 0x14523BA0C };

	WEAK symbol<int> dvarCount{ 0x147595E54 };
	WEAK symbol<dvar_t*> dvarPool{ 0x147595E60 };

	WEAK symbol<char> isCheatOverride{ 0x141BBEF28 };

	WEAK symbol<gentity_s> g_entities{ 0x143D22610 };

	WEAK symbol<unsigned int> svs_numclients{ 0x146B229E0 };
	WEAK symbol<client_t*> svs_clients{ 0x146B22950 };

	WEAK symbol<SessionData> g_serverSession{ 0x146B4E080 };

	WEAK symbol<clientUIActive_t> clientUIActives{ 0x142246C30 };

	WEAK symbol<connection_data*> cl_con_data{ 0x141FE58B8 };

	WEAK symbol<int> sv_map_restart{ 0x146B2C9D4 };
	WEAK symbol<int> sv_loadScripts{ 0x146B2C9D8 };
	WEAK symbol<int> sv_migrate{ 0x146B2C9DC };

	WEAK symbol<SOCKET> query_socket{ 0x14779FDC8 };

	WEAK symbol<DWORD> threadIds{ 0x14602BAB0 };

	WEAK symbol<scrVarGlob_t> scr_VarGlob{ 0x146691180 };
	WEAK symbol<scrVmPub_t> scr_VmPub{ 0x146B183B0 };
	WEAK symbol<function_stack_t> scr_function_stack{ 0x146B22908 };

	WEAK symbol<PhysicalMemory> g_mem{ 0x147685560 };
	WEAK symbol<PhysicalMemory> g_scriptmem{ 0x147685FC0 };

	WEAK symbol<searchpath_s> fs_searchpaths{ 0x14756DEE0 };

	WEAK symbol<DB_LoadData> g_load{ 0x1452A8010 };
	WEAK symbol<int> g_authLoad_isSecure{ 0x14529DD90 };
	WEAK symbol<DB_ReadStream> db_stream{ 0x1452A8050 };
	WEAK symbol<db_z_stream_s> db_zip_stream{ 0x14529DD30 };
	WEAK symbol<char*> db_zip_memory{ 0x14525B500 };

	WEAK symbol<unsigned __int64> g_streamPos{ 0x145687E30 };

	WEAK symbol<bool> g_quitRequested{ 0x14779CD44 };
  
	WEAK symbol<unsigned int> gameEntityId{ 0x14665A124 };
	WEAK symbol<int> gameTime{ 0x143C986D8 };

	WEAK symbol<int> com_frameTime{ 0x1460053C0 };

	WEAK symbol<int> s_frontEndScene_state{ 0x144BFF608 };

	WEAK symbol<WeaponDef*> bg_weaponDefs{ 0x145210120 };

	WEAK symbol<const char*> g_HitLocNames{ 0x14196AAF0 };

	WEAK symbol<bool> cl_serverLoadingMap{ 0x141FE5D74 };
	WEAK symbol<bool> g_waitingForServer{ 0x14523ED91 };

	WEAK symbol<Material*> whiteMaterial{ 0x141FE5ED0 };
	WEAK symbol<vec4_t> colorWhite{ 0x141524800 };

	namespace hks
	{
		WEAK symbol<lua_State*> lua_state{ 0x144FC35F0 };
		WEAK symbol<void(lua_State* s, const char* str, unsigned int l)> hksi_lua_pushlstring{ 0x1400309E0 };
		WEAK symbol<HksObject* (HksObject* result, lua_State* s, const HksObject* table, const HksObject* key)> hks_obj_getfield{ 0x1411E14D0 };
		WEAK symbol<HksObject* (HksObject* result, lua_State* s, const HksObject* table, const HksObject* key)> hks_obj_gettable{ 0x1411E19B0 };
		WEAK symbol<void(lua_State* s, const HksObject* tbl, const HksObject* key, const HksObject* val)> hks_obj_settable{ 0x1411E26F0 };
		WEAK symbol<void(lua_State* s, int nargs, int nresults, const unsigned int* pc)> vm_call_internal{ 0x14120CCE0 };
		WEAK symbol<HashTable* (lua_State* s, unsigned int arraySize, unsigned int hashSize)> Hashtable_Create{ 0x1411D0590 };
		WEAK symbol<cclosure* (lua_State* s, lua_function function, int num_upvalues,
			int internal_, int profilerTreatClosureAsFunc)> cclosure_Create{ 0x1411D07B0 };
		WEAK symbol<int(lua_State* s, int t)> hksi_luaL_ref{ 0x1411EAE10 };
		WEAK symbol<int(lua_State* s, const HksCompilerSettings* options, const char* buff,
			unsigned __int64 sz, const char* name)> hksi_hksL_loadbuffer{ 0x1411E2F50 };
		WEAK symbol<int(lua_State* s, const char* what, lua_Debug* ar)> hksi_lua_getinfo{ 0x1411E48C0 };
		WEAK symbol<int(lua_State* s, int level, lua_Debug* ar)> hksi_lua_getstack{ 0x1411E4AA0 };
		WEAK symbol<void(lua_State* s, const char* fmt, ...)> hksi_luaL_error{ 0x1411EA860 };
		WEAK symbol<void(lua_State* s, int what, int data)> hksi_lua_gc{ 0x1411EAF00 };
		WEAK symbol<void(lua_State* s, int t, int ref)> hksi_luaL_unref{ 0x1411E4460 };
		WEAK symbol<const char*> s_compilerTypeName{ 0x141BDEEF0 };
	}
}
