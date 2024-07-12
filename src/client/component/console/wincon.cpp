#include <std_include.hpp>
#include "wincon.hpp"
#include "loader/component_loader.hpp"

#include "console.hpp"

#include "game/game.hpp"

#include <utils/thread.hpp>

namespace wincon
{
	static std::atomic_bool ingame = false;
	static std::atomic_bool exit = false;

	DWORD WINAPI console(LPVOID)
	{
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		SetConsoleTitleA("IW7-Mod Console");

		std::string cmd;
		exit = false;

		while (!exit)
		{
			std::getline(std::cin, cmd);
			if (ingame)
			{
				game::Cbuf_AddText(0, cmd.data());
			}
		}

		return 0;
	}

	class component final : public component_interface
	{
	public:
		component()
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		}

		void post_start() override
		{
			const auto handle = CreateThread(0, 0, wincon::console, 0, 0, 0);
			utils::thread::set_name(handle, "Console");
		}

		void post_unpack() override
		{
			wincon::ingame = true;
		}

		void pre_destroy() override
		{
			wincon::ingame = false;
			wincon::exit = true;
		}
	};
}

namespace wincon
{
	void init()
	{
		// register component
		static component_loader::installer<wincon::component> __component = component_loader::installer<wincon::component>("wincon::component");
	}
}