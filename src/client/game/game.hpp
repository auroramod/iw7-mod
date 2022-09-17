#pragma once

#include "structs.hpp"

#define SERVER_CD_KEY "IW7MOD-CD-Key"

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

	extern const char* g_assetNames[ASSET_TYPE_COUNT];
	const char* DB_GetXAssetName(const XAsset* asset);
	void DB_EnumXAssets(const std::int32_t type, const std::function<void(XAssetHeader)>& callback);
}

uintptr_t operator"" _b(const uintptr_t ptr);

#include "symbols.hpp"
