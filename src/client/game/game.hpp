#pragma once

#include "structs.hpp"

namespace game
{
	extern uint64_t base_address;
	void load_base_address();

	template <typename T>
	class symbol
	{
	public:
		symbol(const size_t address)
			: address_(reinterpret_cast<T*>(address))
		{
		}

		T* get() const
		{
			return reinterpret_cast<T*>(reinterpret_cast<uint64_t>(address_) + base_address);
		}

		operator T* () const
		{
			return this->get();
		}

		T* operator->() const
		{
			return this->get();
		}

	private:
		T* address_;
	};

	namespace environment
	{
		bool is_dedi();
	}

	int Cmd_Argc();
	const char* Cmd_Argv(int index);

	int SV_Cmd_Argc();
	const char* SV_Cmd_Argv(int index);

	const char* Com_GameMode_GetActiveGameModeStr();
	const char* Com_GameMode_GetGameModeStr(GameModeType gameMode);

	bool CL_IsGameClientActive(const int localClientNum);

	extern const char* g_assetNames[ASSET_TYPE_COUNT];
	const char* DB_GetXAssetName(const XAsset* asset);
	void DB_EnumXAssets(const std::int32_t type, const std::function<void(XAssetHeader)>& callback);

	void SV_CmdsMP_RequestMapRestart(bool loadScripts, bool migrate);
}

size_t operator"" _b(const size_t ptr);
size_t reverse_b(const size_t ptr);
size_t reverse_b(const void* ptr);

#include "symbols.hpp"
