#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"

#include <utils/hook.hpp>

namespace logger
{
	namespace
	{
		void sys_print_stub(const char* msg)
		{
			console::info(msg);
		}

		void print_info(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::info(buffer);
		}

		void print_warn(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::warn(buffer);
		}

		void print_error(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::error(buffer);
		}

		void print_debug(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::debug(buffer);
		}

		void r_warn_once_per_frame_vsnprintf_stub(char* buffer, size_t buffer_length, char* msg, va_list va)
		{
			vsnprintf(buffer, buffer_length, msg, va);
			console::warn(buffer);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Sys_Print
			utils::hook::call(0xC6E57A_b, sys_print_stub); // SV_SpawnServer: completed\n
			utils::hook::call(0xC13641_b, sys_print_stub); // SV_CmdsSP_MapRestart: completed\n
			utils::hook::jump(0x519772_b, sys_print_stub); // OnlineAutoTest:: Map load success. Server is listen.\n
			utils::hook::call(0xB712BA_b, sys_print_stub); // G_SaveError

			// Com_Printf
			utils::hook::jump(0x343080_b, print_info);

			if (!game::environment::is_dedi())
			{
				// LUI_Interface_DebugPrint
				utils::hook::jump(0x61C430_b, print_debug);

				// R_WarnOncePerFrame
				utils::hook::call(0xE4B121_b, r_warn_once_per_frame_vsnprintf_stub);
			}
		}
	};
}

REGISTER_COMPONENT(logger::component)