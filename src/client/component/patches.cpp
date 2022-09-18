#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		void dvar_write_single_variable_stub(const game::dvar_t* dvar, int* user_data)
		{
			if ((dvar->flags & game::DVAR_FLAG_SAVED) != 0)
			{
				const char* val = game::Dvar_DisplayableLatchedValue(dvar);
				auto h = *user_data;
				auto dvar_name = dvars::dvar_get_name(dvar);
				if (!dvar_name.empty())
				{
					game::FS_Printf(h, "seta %s \"%s\"\n", dvar_name.data(), val);
				}
				else
				{
					game::FS_Printf(h, "setcl %d \"%s\"\n", dvar->checksum, val);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// write better config
			utils::hook::jump(0xBB2A50_b, dvar_write_single_variable_stub);
		}
	};
}

REGISTER_COMPONENT(patches::component)