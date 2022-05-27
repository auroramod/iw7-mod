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

		void sys_print_stubs()
		{
			utils::hook::call(0xC6E57A_b, sys_print_stub); // SV_SpawnServer: completed\n
			utils::hook::call(0xC13641_b, sys_print_stub); // SV_CmdsSP_MapRestart: completed\n
			utils::hook::jump(0x519772_b, sys_print_stub); // OnlineAutoTest:: Map load success. Server is listen.\n
			utils::hook::call(0xB712BA_b, sys_print_stub); // G_SaveError
		}

		void R_WarnOncePerFrame_print_stub(char* buffer, size_t buffer_length, char* msg, va_list va)
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
			sys_print_stubs();

			if (!game::environment::is_dedi())
			{
				utils::hook::call(0xE4B121_b, R_WarnOncePerFrame_print_stub);
			}
		}
	};
}

//REGISTER_COMPONENT(logger::component)