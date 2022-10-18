#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "filesystem.hpp"

#include "game/game.hpp"

#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace filesystem
{
	namespace
	{
		
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::override::register_string("fs_basegame", "iw7-mod", 2048);
		}
	};
}

REGISTER_COMPONENT(filesystem::component)