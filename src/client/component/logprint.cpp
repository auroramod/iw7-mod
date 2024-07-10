#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"

#include "gsc/script_extension.hpp"

#include <utils/hook.hpp>

namespace logprint
{
	namespace
	{
		utils::hook::detour g_say_hook;
		void g_say_stub(game::gentity_s* ent, game::gentity_s* target, int mode, const char* chat_text)
		{
			const char* cmd = mode == game::SAY_TEAM ? "say_team" : "say";
			if (mode == game::SAY_TEAM && (ent->client->sess.cs.team - 1 > 1 || game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_CP))
			{
				cmd = "say";
			}

			game::G_LogPrintf("%s;%s;%i;%s;%s\n",
				cmd,
				game::SV_GameMP_GetGuid(ent->s.number),
				ent->s.number,
				ent->client->sess.cs.name,
				&chat_text[1]);

			g_say_hook.invoke<void>(ent, target, mode, chat_text);
		}

		void touch_log_pickup_stub(char* buffer, const char* guid, const unsigned int ent_number, const char* name, const char* weapon_name)
		{
			game::G_LogPrintf(buffer, guid, ent_number, name, weapon_name);
			game::LUI_Interface_DebugPrint(buffer, guid, ent_number, name, weapon_name);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			gsc::function::add("logprint", [](const gsc::function_args& args)
			{
				std::string buffer{};

				for (auto i = 0u; i < args.size(); ++i)
				{
					const auto string = args[i].as<std::string>();
					buffer.append(string);
				}

				game::G_LogPrintf("%s", buffer.data());

				return scripting::script_value{};
			});

			// Reimplement game log
			scheduler::once([]()
			{
				dvars::logfile = game::Dvar_RegisterBool("logfile", true, game::DVAR_FLAG_NONE, "Enable game logging");
				dvars::g_log = game::Dvar_RegisterString("g_log", "iw7-mod\\logs\\games_mp.log", game::DVAR_FLAG_NONE, "Log file path");
			}, scheduler::pipeline::main);

			// Hook say client command
			g_say_hook.create(0x140B10C00, g_say_stub);

			// Hook LUI_Interface_DebugPrint call in GItemsMP::TouchLogPickup
			utils::hook::call(0x140448F1D, touch_log_pickup_stub);
		}
	};
}

REGISTER_COMPONENT(logprint::component)