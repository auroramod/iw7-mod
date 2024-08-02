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
			console::info("%s", msg);
		}

		void print_info(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::info("%s", buffer);
		}

		void print_warn(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::warn("%s", buffer);
		}

		void print_error(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::error("%s", buffer);
		}

		void print_debug(const char* msg, ...)
		{
			char buffer[0x1000]{ 0 };

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			va_end(ap);

			console::debug("%s", buffer);
		}

		void r_warn_once_per_frame_vsnprintf_stub(char* buffer, size_t buffer_length, char* msg, va_list va)
		{
			vsnprintf(buffer, buffer_length, msg, va);
			console::warn("%s", buffer);
		}

		void com_init_pre()
		{
			console::info("%s %s build %s %s\n", BUILD_NAME, VERSION, TARGET_ARCHITECTURE, __DATE__);
			console::info("--- Common Initialization ---\n");
		}

		void com_init_post()
		{
			console::info("--- Common Initialization Complete ---\n");
			console::info("Working directory: %s\n", game::Sys_Cwd());
		}

		void com_init_stub(utils::hook::assembler& a)
		{
			// arxan doesn't like his registers/stack getting touched
			a.pushad64();
			a.call_aligned(com_init_pre);
			a.popad64();

			a.call(0x140B8EF90);

			a.pushad64();
			a.call_aligned(com_init_post);
			a.popad64();

			a.jmp(0x140D4D8DD);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Sys_Print
			utils::hook::call(0x140C6E57A, sys_print_stub); // SV_SpawnServer: completed\n
			utils::hook::call(0x140C13641, sys_print_stub); // SV_CmdsSP_MapRestart: completed\n
			utils::hook::jump(0x140519772, sys_print_stub); // OnlineAutoTest:: Map load success. Server is listen.\n
			utils::hook::call(0x140B712BA, sys_print_stub); // G_SaveError

			// Com_Printf
			utils::hook::jump(0x140343080, print_info);

			utils::hook::jump(0x140D4D8D8, utils::hook::assemble(com_init_stub), false);

			if (!game::environment::is_dedi())
			{
				// R_WarnOncePerFrame
				utils::hook::call(0x140E4B121, r_warn_once_per_frame_vsnprintf_stub);
			}
		}
	};
}

REGISTER_COMPONENT(logger::component)