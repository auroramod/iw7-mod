#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

//#include "dvars.hpp"
#include "console.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			
		}
	};
}

REGISTER_COMPONENT(patches::component)