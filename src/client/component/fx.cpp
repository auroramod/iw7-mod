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
			utils::hook::set<uint8_t>(0xB34889_b, 0xEB); // Scr_LoadFx
			utils::hook::nop(0xD0FBFD_b, 2); // ParticleSystem_Register
		}
	};
}

REGISTER_COMPONENT(fx::component)