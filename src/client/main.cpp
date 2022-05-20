#include <std_include.hpp>
#include "loader/loader.hpp"
#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/flags.hpp>
#include <utils/io.hpp>

DECLSPEC_NORETURN void WINAPI exit_hook(const int code)
{
	component_loader::pre_destroy();
	exit(code);
}

BOOL WINAPI system_parameters_info_a(const UINT uiAction, const UINT uiParam, const PVOID pvParam, const UINT fWinIni)
{
	component_loader::post_unpack();
	return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
}

FARPROC load_binary(uint64_t* base_address)
{
	loader loader;
	utils::nt::library self;

	loader.set_import_resolver([self](const std::string& library, const std::string& function) -> void*
	{
		if (library == "steam_api64.dll"
			&& function != "SteamAPI_Shutdown")
		{
			return self.get_proc<FARPROC>(function);
		}
		else if (function == "ExitProcess")
		{
			return exit_hook;
		}
		else if (function == "SystemParametersInfoA")
		{
			return system_parameters_info_a;
		}

		return component_loader::load_import(library, function);
	});

	std::string binary = "iw7_ship.exe";

	std::string data;
	if (!utils::io::read_file(binary, &data))
	{
		throw std::runtime_error(utils::string::va(
			"Failed to read game binary (%s)!\nPlease copy the iw7-mod.exe into your Call of Duty: Infinite Warfare installation folder and run it from there.",
			binary.data()));
	}
 
#ifdef INJECT_HOST_AS_LIB
	return loader.load_library(binary, base_address);
#else
	*base_address = 0x140000000;
	return loader.load(self, data); // this gives memory errors
#endif
}

void remove_crash_file()
{
	utils::io::remove_file("__iw7-mod");
	utils::io::remove_file("__iw7_ship");
}

void enable_dpi_awareness()
{
	const utils::nt::library user32{"user32.dll"};
	const auto set_dpi = user32
		                     ? user32.get_proc<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>("SetProcessDpiAwarenessContext")
		                     : nullptr;
	if (set_dpi)
	{
		set_dpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	}
}

void limit_parallel_dll_loading()
{
	const utils::nt::library self;
	const auto registry_path = R"(Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\)" + self.
		get_name();

	HKEY key = nullptr;
	if (RegCreateKeyA(HKEY_LOCAL_MACHINE, registry_path.data(), &key) == ERROR_SUCCESS)
	{
		RegCloseKey(key);
	}

	key = nullptr;
	if (RegOpenKeyExA(
		HKEY_LOCAL_MACHINE, registry_path.data(), 0,
		KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
	{
		return;
	}

	DWORD value = 1;
	RegSetValueExA(key, "MaxLoaderThreads", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));

	RegCloseKey(key);
}

// solution for other processes that may launch the mod
void apply_proper_directory()
{
	char module_path[MAX_PATH];
	GetModuleFileNameA(nullptr, module_path, MAX_PATH);
	PathRemoveFileSpecA(module_path);
	SetCurrentDirectoryA(module_path);
	SetDllDirectoryA(module_path);
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	FARPROC entry_point;
	enable_dpi_awareness();

	// This requires admin privilege, but I suppose many
	// people will start with admin rights if it crashes.
	limit_parallel_dll_loading();

	srand(uint32_t(time(nullptr)));
	remove_crash_file();

	{
		auto premature_shutdown = true;
		const auto _ = gsl::finally([&premature_shutdown]()
		{
			if (premature_shutdown)
			{
				component_loader::pre_destroy();
			}
		});

		try
		{
			//apply_proper_directory();
			//remove_crash_file();

			if (!component_loader::post_start()) return 0;

			uint64_t base_address{};
			entry_point = load_binary(&base_address);
			if (!entry_point)
			{
				throw std::runtime_error("Unable to load binary into memory");
			}

			if (base_address != 0x140000000)
			{
				throw std::runtime_error(utils::string::va(
					"Base address was (%p) and not (%p)\nThis should not be possible!",
					base_address, 0x140000000));
			}
			game::base_address = base_address;

			if (!component_loader::post_load()) return 0;

			premature_shutdown = false;
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "ERROR", MB_ICONERROR);
			return 1;
		}
	}

	return static_cast<int>(entry_point());
}

int __stdcall WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	return main();
}
