#include <std_include.hpp>
#include "steam.hpp"

#include <utils/nt.hpp>
#include <utils/io.hpp>
#include <utils/flags.hpp>

#include "loader/component_loader.hpp"

#include "../component/steam_proxy.hpp"

namespace steam
{
	namespace
	{
		struct ldr_data
		{
			ULONG length;
			BOOLEAN initialized;
			HANDLE ss_handle;
			LIST_ENTRY in_load_order_module_list;
			LIST_ENTRY in_memory_order_module_list;
			LIST_ENTRY in_initialization_order_module_list;
		};

		struct ldr_data_table_entry
		{
			LIST_ENTRY in_load_order_links;
			LIST_ENTRY in_memory_order_links;
			LIST_ENTRY in_initialization_order_links;
			void* dll_base;
			void* entry_point;
			ULONG size_of_image;
			UNICODE_STRING full_dll_name;
			UNICODE_STRING base_dll_name;
			ULONG flags;
			USHORT obsolete_load_count;
			USHORT tls_index;
			LIST_ENTRY hash_links;
			ULONG time_date_stamp;
			void* entry_point_activation_context;
			void* lock;
			LIST_ENTRY* ddag_node; // starts with the node's module list
			LIST_ENTRY node_module_link;
			std::uint8_t unused[0x200]; // rest of the loader's entry, left zeroed
		};

		static_assert(offsetof(ldr_data_table_entry, dll_base) == 0x30);
		static_assert(offsetof(ldr_data_table_entry, base_dll_name) == 0x58);
		static_assert(offsetof(ldr_data_table_entry, hash_links) == 0x70);
		static_assert(offsetof(ldr_data_table_entry, ddag_node) == 0x98);

		void insert_tail(LIST_ENTRY* head, LIST_ENTRY* entry)
		{
			entry->Flink = head;
			entry->Blink = head->Blink;
			head->Blink->Flink = entry;
			head->Blink = entry;
		}

		UNICODE_STRING make_unicode_string(const std::wstring& value)
		{
			auto* buffer = new wchar_t[value.size() + 1]{};
			std::memcpy(buffer, value.data(), value.size() * sizeof(wchar_t));

			UNICODE_STRING result{};
			result.Buffer = buffer;
			result.Length = static_cast<USHORT>(value.size() * sizeof(wchar_t));
			result.MaximumLength = static_cast<USHORT>(result.Length + sizeof(wchar_t));
			return result;
		}

		// A minimal read-only image: PE headers whose debug directory holds a CodeView record naming `pdb_name`
		void* create_stand_in_image(const char* pdb_name)
		{
			auto* base = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
			if (!base) return nullptr;

			auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
			dos->e_magic = IMAGE_DOS_SIGNATURE;
			dos->e_lfanew = 0x40;

			auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
			nt->Signature = IMAGE_NT_SIGNATURE;
			nt->FileHeader.Machine = IMAGE_FILE_MACHINE_AMD64;
			nt->FileHeader.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER64);
			nt->FileHeader.Characteristics = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_LARGE_ADDRESS_AWARE | IMAGE_FILE_DLL;
			nt->OptionalHeader.Magic = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
			nt->OptionalHeader.ImageBase = reinterpret_cast<std::uint64_t>(base);
			nt->OptionalHeader.SectionAlignment = 0x1000;
			nt->OptionalHeader.FileAlignment = 0x200;
			nt->OptionalHeader.SizeOfImage = 0x1000;
			nt->OptionalHeader.SizeOfHeaders = 0x1000;
			nt->OptionalHeader.NumberOfRvaAndSizes = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;

			auto* debug = reinterpret_cast<IMAGE_DEBUG_DIRECTORY*>(base + 0x200);
			nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG] = { 0x200, sizeof(IMAGE_DEBUG_DIRECTORY) };

			// RSDS: signature, guid, age, pdb path
			auto* codeview = base + 0x240;
			const auto pdb_length = std::strlen(pdb_name) + 1;
			std::memcpy(codeview, "RSDS", 4);
			*reinterpret_cast<DWORD*>(codeview + 20) = 1;
			std::memcpy(codeview + 24, pdb_name, pdb_length);

			debug->Type = IMAGE_DEBUG_TYPE_CODEVIEW;
			debug->SizeOfData = static_cast<DWORD>(24 + pdb_length);
			debug->AddressOfRawData = 0x240;
			debug->PointerToRawData = 0x240;

			DWORD old_protect{};
			VirtualProtect(base, 0x1000, PAGE_READONLY, &old_protect);
			return base;
		}

		void add_ldr_entry(void* base, const ULONG size_of_image, const std::filesystem::path& path)
		{
			if (!base) return;

			auto* entry = new ldr_data_table_entry{};
			entry->dll_base = base;
			entry->size_of_image = size_of_image;
			entry->full_dll_name = make_unicode_string(path.wstring());
			entry->base_dll_name = make_unicode_string(path.filename().wstring());
			entry->flags = 0x4 | 0x40 | 0x4000 | 0x40000; // ImageDll, InLegacyLists, EntryProcessed, DontCallForThreads
			entry->obsolete_load_count = 0xFFFF;
			entry->in_initialization_order_links.Flink = entry->in_initialization_order_links.Blink = &entry->in_initialization_order_links;
			entry->hash_links.Flink = entry->hash_links.Blink = &entry->hash_links;

			// LdrpInitializeThread reads every entry's DDAG node, a zeroed one has state "not ready" so it's skipped
			auto* ddag_node = reinterpret_cast<LIST_ENTRY*>(new std::uint8_t[0x100]{});
			ddag_node->Flink = ddag_node->Blink = &entry->node_module_link;
			entry->node_module_link.Flink = entry->node_module_link.Blink = ddag_node;
			entry->ddag_node = ddag_node;

			// only the load/memory order lists, so the loader never initializes, notifies or unloads it
			const ::utils::nt::library ntdll("ntdll.dll");
			void* cookie{};
			ntdll.invoke<NTSTATUS>("LdrLockLoaderLock", 0ul, static_cast<ULONG*>(nullptr), &cookie);
			auto* ldr = reinterpret_cast<ldr_data*>(NtCurrentTeb()->ProcessEnvironmentBlock->Ldr);
			insert_tail(&ldr->in_load_order_module_list, &entry->in_load_order_links);
			insert_tail(&ldr->in_memory_order_module_list, &entry->in_memory_order_links);
			ntdll.invoke<NTSTATUS>("LdrUnlockLoaderLock", 0ul, cookie);
		}
	}

	uint64_t callbacks::call_id_ = 0;
	std::recursive_mutex callbacks::mutex_;
	std::map<uint64_t, bool> callbacks::calls_;
	std::map<uint64_t, callbacks::base*> callbacks::result_handlers_;
	std::vector<callbacks::result> callbacks::results_;
	std::vector<callbacks::base*> callbacks::callback_list_;

	uint64_t callbacks::register_call()
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);
		calls_[++call_id_] = false;
		return call_id_;
	}

	void callbacks::register_callback(base* handler, const int callback)
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);
		handler->set_i_callback(callback);
		callback_list_.push_back(handler);
	}

	void callbacks::unregister_callback(base* handler)
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);
		for (auto i = callback_list_.begin(); i != callback_list_.end();)
		{
			if (*i == handler)
			{
				i = callback_list_.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

	void callbacks::register_call_result(const uint64_t call, base* result)
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);
		result_handlers_[call] = result;
	}

	void callbacks::unregister_call_result(const uint64_t call, base* /*result*/)
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);
		const auto i = result_handlers_.find(call);
		if (i != result_handlers_.end())
		{
			result_handlers_.erase(i);
		}
	}

	void callbacks::return_call(void* data, const int size, const int type, const uint64_t call)
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);

		result result{};
		result.call = call;
		result.data = data;
		result.size = size;
		result.type = type;

		calls_[call] = true;

		results_.emplace_back(result);
	}

	void callbacks::run_callbacks()
	{
		std::lock_guard<std::recursive_mutex> _(mutex_);

		for (const auto& result : results_)
		{
			if (result_handlers_.find(result.call) != result_handlers_.end())
			{
				result_handlers_[result.call]->run(result.data, false, result.call);
			}

			// someone concluded this can cause issues and from my knowledge, removing this doesn't break anything (unless it does?)
			// if it does and is discovered, please make a issue <3
			/*
			for (const auto& callback : callback_list_)
			{
				if (callback && callback->get_i_callback() == result.type)
				{
					callback->run(result.data, false, 0);
				}
			}
			*/

			if (result.data)
			{
				std::free(result.data);
			}
		}

		results_.clear();
	}

	extern "C" {

	bool SteamAPI_RestartAppIfNecessary()
	{
		return false;
	}

	bool SteamAPI_Init()
	{
		const std::filesystem::path steam_path = steam::SteamAPI_GetSteamInstallPath();
		if (steam_path.empty() || ::utils::flags::has_flag("nosteam"))
		{
			// Without steamclient64.dll or gameoverlayrenderer64.dll WinMain quietly returns 0 right after Com_Init,
			// and the teardown races the fastfile thread into "Memory Error 15 402". Only the name and PDB name matter.
			// They're added this late and without a directory on purpose, added earlier (or in the game folder) they fail.
			add_ldr_entry(create_stand_in_image("steamclient64.pdb"), 0x1000, "steamclient64.dll");
			add_ldr_entry(create_stand_in_image("GameOverlayRenderer64.pdb"), 0x1000, "gameoverlayrenderer64.dll");
			return true;
		}

		const auto app_id = std::to_string(SteamUtils()->GetAppID());
		SetEnvironmentVariableA("SteamAppId", app_id.data());
		SetEnvironmentVariableA("SteamGameId", app_id.data());
		SetEnvironmentVariableA("SteamOverlayGameId", app_id.data());

		::utils::nt::library::load(steam_path / "tier0_s64.dll");
		::utils::nt::library::load(steam_path / "vstdlib_s64.dll");
		::utils::nt::library::load(steam_path / "gameoverlayrenderer64.dll");
		::utils::nt::library::load(steam_path / "steamclient64.dll");

		steam_proxy::initialize();

		return true;
	}

	void SteamAPI_RegisterCallResult(callbacks::base* result, const uint64_t call)
	{
		callbacks::register_call_result(call, result);
	}

	void SteamAPI_RegisterCallback(callbacks::base* handler, const int callback)
	{
		callbacks::register_callback(handler, callback);
	}

	void SteamAPI_RunCallbacks()
	{
		callbacks::run_callbacks();
	}

	void SteamAPI_Shutdown()
	{
	}

	void SteamAPI_UnregisterCallResult(callbacks::base* result, const uint64_t call)
	{
		callbacks::unregister_call_result(call, result);
	}

	void SteamAPI_UnregisterCallback(callbacks::base* handler)
	{
		callbacks::unregister_callback(handler);
	}

	const char* SteamAPI_GetSteamInstallPath()
	{
		static std::string install_path{};
		if (!install_path.empty())
		{
			return install_path.data();
		}

		std::string path_str;
		if (::utils::io::read_file("steam_path.txt", &path_str)) // steam_path.txt in root for directory, manually fixes issues lol
		{
			install_path = path_str;
			return install_path.data();
		}

		// check if Steam contains information in registry for the install path
		HKEY reg_key;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\WOW6432Node\\Valve\\Steam", 0, KEY_QUERY_VALUE, &reg_key) == ERROR_SUCCESS)
		{
			char path[MAX_PATH]{};
			DWORD length = sizeof(path);
			RegQueryValueExA(reg_key, "InstallPath", nullptr, nullptr, reinterpret_cast<BYTE*>(path), &length);
			RegCloseKey(reg_key);
			install_path = path;
		}

		return install_path.data();
	}

	void* SteamGameServer_GetHSteamPipe()
	{
		return reinterpret_cast<void*>(1);
	}

	void* SteamGameServer_GetHSteamUser()
	{
		return reinterpret_cast<void*>(1);
	}

	void* SteamAPI_GetHSteamUser()
	{
		return reinterpret_cast<void*>(1);
	}

	void* SteamAPI_GetHSteamPipe()
	{
		return reinterpret_cast<void*>(1);
	}

	void* SteamInternal_CreateInterface(const char* interfacename)
	{
		if (std::string(interfacename) == "SteamClient017")
		{
			static client c;
			return &c;
		}

		MessageBoxA(0, interfacename, __FUNCTION__, 0);
		return nullptr;
	}

	bool SteamInternal_GameServer_Init()
	{
		return true;
	}

	bool SteamGameServer_Init()
	{
		return true;
	}

	void SteamGameServer_RunCallbacks()
	{
	}

	void SteamGameServer_Shutdown()
	{
	}

	friends* SteamFriends()
	{
		static friends friends;
		return &friends;
	}

	matchmaking* SteamMatchmaking()
	{
		static matchmaking matchmaking;
		return &matchmaking;
	}

	game_server* SteamGameServer()
	{
		static game_server game_server;
		return &game_server;
	}

	networking* SteamNetworking()
	{
		static networking networking;
		return &networking;
	}

	remote_storage* SteamRemoteStorage()
	{
		static remote_storage remote_storage;
		return &remote_storage;
	}

	user* SteamUser()
	{
		static user user;
		return &user;
	}

	utils* SteamUtils()
	{
		static utils utils;
		return &utils;
	}

	apps* SteamApps()
	{
		static apps apps;
		return &apps;
	}

	user_stats* SteamUserStats()
	{
		static user_stats user_stats;
		return &user_stats;
	}

	}
}
