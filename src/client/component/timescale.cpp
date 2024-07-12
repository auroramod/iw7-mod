#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "dvars.hpp"

#include "gsc/script_extension.hpp"

#include <utils/hook.hpp>

namespace timescale
{
	namespace
	{
		template <typename T, typename T2>
		T get_timescale_safe(const scripting::value_wrap& arg)
		{
			if (arg.is<T>())
			{
				return arg.as<T>();
			}

			return static_cast<T>(arg.as<T2>());
		}

		void com_init_dobj_stub()
		{
			utils::hook::invoke<void>(0x140BB17E0); // call original

			if (game::Com_GameMode_SupportsFeature(game::Com_GameMode_Feature::FEATURE_TIMESCALE))
			{
				auto com_timescale = game::Dvar_FindVar("com_timescale");
				game::Dvar_SetFloat(com_timescale, 1.0f);
			}
		}

		utils::hook::detour com_set_slow_motion_hook;
		void com_set_slow_motion_stub(float startTimescale, float endTimescale, int deltaMsec)
		{
			com_set_slow_motion_hook.invoke<void>(startTimescale, endTimescale, deltaMsec);

			if (game::Com_GameMode_SupportsFeature(game::Com_GameMode_Feature::FEATURE_TIMESCALE))
			{
				if (endTimescale == startTimescale)
				{
					auto com_timescale = game::Dvar_FindVar("com_timescale");
					game::Dvar_SetFloat(com_timescale, endTimescale);
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::override::register_float("timescale", 1.0f, 0.1f, 1000.0f, game::DVAR_FLAG_CHEAT);
			dvars::override::register_float("com_timescale", 1.0f, 0.1f, 1000.0f, game::DVAR_FLAG_CHEAT | 0x40 | game::DVAR_FLAG_READ);

			// reset com_timeScale dvar in Com_Restart
			utils::hook::call(0x140BAF16D, com_init_dobj_stub);

			// set com_timeScale in Com_SetSlowMotion
			com_set_slow_motion_hook.create(game::Com_SetSlowMotion, com_set_slow_motion_stub);

			gsc::function::add("setslowmotion", [](const gsc::function_args& args)
			{
				if (args.size() == 0)
				{
					return scripting::script_value{};
				}

				const auto start = get_timescale_safe<float, int>(args[0]);
				const auto end = (args.size() > 0 ? get_timescale_safe<float, int>(args[1]) : 1.0f);
				const auto duration = (args.size() > 1 ? get_timescale_safe<int, float>(args[2]) : 1) * 1000;

				game::SV_SetConfigString(game::CS_TIMESCALE, utils::string::va("%i %i %g %g", *game::gameTime, duration, start, end));
				game::Com_SetSlowMotion(start, end, duration);

				return scripting::script_value{};
			});
		}
	};
}

REGISTER_COMPONENT(timescale::component)