#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console/console.hpp"

#include <utils/hook.hpp>

#include "version.h"

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

		void com_init_stub()
		{
			console::info("%s %s build %s %s\n", "IW7", VERSION, "win64", __DATE__);

			console::info("--- Common Initialization ---\n");
			utils::hook::invoke<void>(0xB8EF90_b);
			console::info("--- Common Initialization Complete ---\n");

			console::info("Working directory: %s\n", game::Sys_Cwd());
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

			utils::hook::call(0xD4D8D8_b, com_init_stub);

			if (!game::environment::is_dedi())
			{
				// R_WarnOncePerFrame
				utils::hook::call(0xE4B121_b, r_warn_once_per_frame_vsnprintf_stub);
			}
		}
	};
}

REGISTER_COMPONENT(logger::component)