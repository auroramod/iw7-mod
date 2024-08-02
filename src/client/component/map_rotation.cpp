#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "command.hpp"
#include "scheduler.hpp"
#include <utils/hook.hpp>
#include <utils/string.hpp>
#include "game/game.hpp"
#include <game/dvars.hpp>

namespace map_rotation
{
	namespace
	{
		DWORD previous_priority{};

		void set_dvar(const std::string& dvar, const std::string& value)
		{
			command::execute(utils::string::va("%s \"%s\"", dvar.data(), value.data()), true);
		}

		void set_gametype(const std::string& gametype)
		{
			set_dvar("g_gametype", gametype);
		}

		void launch_map(const std::string& mapname)
		{
			command::execute(utils::string::va("map %s", mapname.data()), false);
		}

		void launch_default_map()
		{
			auto* mapname = game::Dvar_FindVar("mapname");
			if (mapname && mapname->current.string && strlen(mapname->current.string) && 
				mapname->current.string != "mp_frontend"s && 
				mapname->current.string != "cp_frontend"s)
			{
				launch_map(mapname->current.string);
			}
			else
			{
				launch_map(game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_CP ? "cp_zmb" : "mp_frontier");
			}
		}

		std::string load_current_map_rotation()
		{
			auto* rotation = game::Dvar_FindVar("sv_mapRotationCurrent");
			if (!strlen(rotation->current.string))
			{
				rotation = game::Dvar_FindVar("sv_mapRotation");
				set_dvar("sv_mapRotationCurrent", rotation->current.string);
			}

			return rotation->current.string;
		}

		std::vector<std::string> parse_current_map_rotation()
		{
			const auto rotation = load_current_map_rotation();
			return utils::string::split(rotation, ' ');
		}

		void store_new_rotation(const std::vector<std::string>& elements, const size_t index)
		{
			std::string value{};

			for (auto i = index; i < elements.size(); ++i)
			{
				if (i != index)
				{
					value.push_back(' ');
				}

				value.append(elements[i]);
			}

			set_dvar("sv_mapRotationCurrent", value);
		}

		void change_process_priority()
		{
			auto* const dvar = game::Dvar_FindVar("sv_autoPriority");
			if (dvar && dvar->current.enabled)
			{
				scheduler::on_game_initialized([]()
				{
					//printf("=======================setting OLD priority=======================\n");
					SetPriorityClass(GetCurrentProcess(), previous_priority);
				}, scheduler::pipeline::main, 1s);

				previous_priority = GetPriorityClass(GetCurrentProcess());
				//printf("=======================setting NEW priority=======================\n");
				SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
			}
		}

		void perform_map_rotation()
		{
			if (game::Live_SyncOnlineDataFlags(0) != 0)
			{
				scheduler::on_game_initialized(perform_map_rotation, scheduler::pipeline::main, 1s);
				return;
			}

			const auto rotation = parse_current_map_rotation();

			for (size_t i = 0; !rotation.empty() && i < (rotation.size() - 1); i += 2)
			{
				const auto& key = rotation[i];
				const auto& value = rotation[i + 1];

				if (key == "gametype")
				{
					set_gametype(value);
				}
				else if (key == "map")
				{
					store_new_rotation(rotation, i + 2);
					change_process_priority();
					if (!game::SV_MapExists(value.data()))
					{
						printf("map_rotation: '%s' map doesn't exist!\n", value.data());
						launch_default_map();
						return;
					}
					launch_map(value);
					return;
				}
				else
				{
					printf("Invalid map rotation key: %s\n", key.data());
				}
			}

			launch_default_map();
		}

		void trigger_map_rotation()
		{
			scheduler::schedule([]()
			{
				if (game::CL_IsGameClientActive(0))
				{
					return scheduler::cond_continue;
				}

				command::execute("map_rotate", false);
				return scheduler::cond_end;
			}, scheduler::pipeline::main, 1s);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (!game::environment::is_dedi())
			{
				return;
			}

			scheduler::once([]()
			{
				game::Dvar_RegisterString("sv_mapRotation", "", game::DVAR_FLAG_NONE, "");
				game::Dvar_RegisterString("sv_mapRotationCurrent", "", game::DVAR_FLAG_NONE, "");
				game::Dvar_RegisterString("sv_autoPriority", "", game::DVAR_FLAG_NONE, "Lowers the process priority during map changes to not cause lags on other servers.");
			}, scheduler::pipeline::main);

			command::add("map_rotate", &perform_map_rotation);

			// Hook GScr_ExitLevel 
			utils::hook::jump(0x140B52E50, &trigger_map_rotation, true);

			previous_priority = GetPriorityClass(GetCurrentProcess());
		}
	};
}

REGISTER_COMPONENT(map_rotation::component)