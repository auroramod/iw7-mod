#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console.hpp"

#include <utils/hook.hpp>

namespace logger
{
	namespace
	{
		void nullsub_6_stub(const char* msg, ...)
		{
			char buffer[2048];

			{
				va_list ap;
				va_start(ap, msg);

				vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

				va_end(ap);

				console::info("%s", buffer);
			}
		}

		void nullsub_6()
		{
			utils::hook::call(0xC6E57A_b, nullsub_6_stub);
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
			//nullsub_6();

			if (!game::environment::is_dedi())
			{
				//utils::hook::call(0xE4B121_b, R_WarnOncePerFrame_print_stub);
			}
		}
	};
}

REGISTER_COMPONENT(logger::component)