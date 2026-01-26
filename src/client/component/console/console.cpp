#include <std_include.hpp>
#include "console.hpp"

#include "game/game.hpp"

#include "component/rcon.hpp"
#include "component/scheduler.hpp"

#include <utils/flags.hpp>

#include "wincon.hpp"
#include "terminal.hpp"
#include "syscon.hpp"

#include <utils/io.hpp>

namespace game_console
{
	void print(int type, const std::string& data);
}

namespace console
{
	enum console_type
	{
		con_type_none,
		con_type_wincon,
		con_type_terminal,
		con_type_syscon,
		con_type_game = con_type_syscon,
		con_type_default = con_type_terminal,
	} con_type;

	game::dvar_t* console_log = nullptr;

	void init_console_type()
	{
		con_type = con_type_default;

		const auto noconsole_flag = utils::flags::has_flag("noconsole");
		if (!game::environment::is_dedi() && noconsole_flag)
		{
			con_type = con_type_none;
			return;
		}

		const auto wincon_flag = utils::flags::has_flag("wincon");
		if (wincon_flag)
		{
			con_type = con_type_wincon;
			return;
		}

		const auto terminal_flag = utils::flags::has_flag("terminal");
		if (terminal_flag)
		{
			con_type = con_type_terminal;
			return;
		}

		const auto syscon_flag = utils::flags::has_flag("syscon");
		if (syscon_flag)
		{
			con_type = con_type_syscon;
			return;
		}
	}

	auto get_console_type()
	{
		return con_type;
	}

	bool is_enabled()
	{
		return get_console_type() != console_type::con_type_none;
	}

	namespace wincon
	{
		bool is_enabled()
		{
			return get_console_type() == console_type::con_type_wincon;
		}
	}

	namespace terminal
	{
		bool is_enabled()
		{
			return get_console_type() == console_type::con_type_terminal;
		}
	}

	namespace syscon
	{
		bool is_enabled()
		{
			return get_console_type() == console_type::con_type_syscon;
		}
	}

	std::string format(va_list* ap, const char* message)
	{
		static thread_local char buffer[0x1000];

		const auto count = _vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer), message, *ap);

		if (count < 0) return {};
		return { buffer, static_cast<size_t>(count) };
	}

	void dispatch_message(const int type, const std::string& message)
	{
		if (rcon::message_redirect(message) || !console_log)
		{
			return;
		}

		auto out = message;
		if (out.empty() || out.back() != '\n')
		{
			out.push_back('\n');
		}

		utils::io::write_file(console_log->current.string, out, true);

		if (console::is_enabled())
		{
			if (wincon::is_enabled())
			{
				printf(message.data());
			}
			else if (terminal::is_enabled())
			{
				::terminal::dispatch_message(type, message);
				return;
			}
			else if (syscon::is_enabled())
			{
				::syscon::Sys_Print(message.data());
			}
		}

		game_console::print(type, message);
	}

	void print(const int type, const char* fmt, ...)
	{
		if (type == console::print_type_demonware)
		{
			static bool has_demonware_debug = utils::flags::has_flag("demonware_debug");
			if (!has_demonware_debug)
				return;
		}

		va_list ap;
		va_start(ap, fmt);
		const auto result = format(&ap, fmt);
		va_end(ap);

		dispatch_message(type, result);
	}

	void set_title(const std::string& title)
	{
		if (console::is_enabled())
		{
			if (wincon::is_enabled() || terminal::is_enabled())
			{
				SetConsoleTitleA(title.data());
			}
			else if (syscon::is_enabled())
			{
				::syscon::set_title(title);
			}
		}
	}

	void init()
	{
		static auto initialized = false;
		if (initialized) return;
		initialized = true;

		init_console_type();
		if (get_console_type() == con_type_none)
		{
			return;
		}
		else if (get_console_type() == con_type_wincon)
		{
			::wincon::init();
		}
		else if (get_console_type() == con_type_terminal)
		{
			::terminal::init();
		}
		else if (get_console_type() == con_type_syscon)
		{
			::syscon::init();
		}

		scheduler::once([]()
		{
			console_log = game::Dvar_RegisterString("g_consoleLog", "iw7-mod/logs/console.log", game::DVAR_FLAG_SAVED, "Where to write the console log");
		});
	}
}
