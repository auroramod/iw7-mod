#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>

namespace fx
{
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// skip "fx/" and "vfx/" name prefix checks
			utils::hook::set<uint8_t>(0x140B34889, 0xEB); // Scr_LoadFx
			utils::hook::nop(0x140D0FBFD, 2); // ParticleSystem_Register
		}
	};
}

REGISTER_COMPONENT(fx::component)