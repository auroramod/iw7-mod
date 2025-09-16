#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "scheduler.hpp"
#include "party.hpp"
#include "scripting.hpp"

#include "game/game.hpp"
#include "game/scripting/execution.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>

namespace bots
{
	namespace
	{
		bool can_add()
		{
			return (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP || game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_CP) &&
				party::get_client_count() < *game::svs_numclients
				&& game::SV_Loaded() && !game::Com_FrontEnd_IsInFrontEnd();
		}

		utils::hook::detour get_bot_name_hook;
		std::vector<std::string> bot_names{};

		void load_bot_data()
		{
			static const char* bots_txt = "iw7-mod/bots.txt";

			std::string bots_content;
			if (!utils::io::read_file(bots_txt, &bots_content))
			{
				return;
			}

			auto names = utils::string::split(bots_content, '\n');
			for (auto& name : names)
			{
				name = utils::string::replace(name, "\r", "");
				if (!name.empty())
				{
					bot_names.emplace_back(name);
				}
			}
		}

		size_t bot_id = 0;

		const char* get_random_bot_name()
		{
			if (bot_names.empty())
			{
				load_bot_data();
			}

			// only use bot names once, no dupes in names
			if (!bot_names.empty() && bot_id < bot_names.size())
			{
				bot_id %= bot_names.size();
				const auto& entry = bot_names.at(bot_id++);
				return utils::string::va("%.*s", static_cast<int>(entry.size()), entry.data());
			}

			return get_bot_name_hook.invoke<const char*>();
		}

		utils::hook::detour sv_kick_client_num_hook;
		void sv_kick_client_num_stub(const int client_num, const char* reason, bool kicked_for_inactivity)
		{
			// Don't kick bot to equalize team balance.
			if (reason == "EXE_PLAYERKICKED_BOT_BALANCE"s)
			{
				return;
			}
			return sv_kick_client_num_hook.invoke<void>(client_num, reason, kicked_for_inactivity);
		}

		void add_bots(unsigned int num_bots)
		{
			scripting::call_script_function(0, "scripts/mp/bots/bots", "spawn_bots", { num_bots, "autoassign" });
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// register bot dvars
			game::Dvar_RegisterBool("bots_enabled", true, game::DVAR_FLAG_READ, "Enable bots and activate bot management systems");
			game::Dvar_RegisterInt("bot_difficulty", 0, 0, 5, game::DVAR_FLAG_NONE, "Bot difficulty. 0: Mixed, 1: Recruit, 2: Regular, 3: Hardened, 4: Veteran");

			sv_kick_client_num_hook.create(game::SV_CmdsMP_KickClientNum, sv_kick_client_num_stub);

			get_bot_name_hook.create(game::SV_BotGetRandomName, get_random_bot_name);

			command::add("spawnBot", [](const command::params& params)
			{
				if (!can_add())
				{
					return;
				}

				auto num_bots = 1u;
				if (params.size() == 2)
				{
					num_bots = static_cast<unsigned int>(atoi(params.get(1)));
				}

				num_bots = std::min(num_bots, *game::svs_numclients);

				scheduler::once([num_bots]()
				{
					add_bots(num_bots);
				}, scheduler::pipeline::server);
			});

			// Clear bot names and reset ID on game shutdown to allow new names to be added without restarting
			scripting::on_shutdown([](bool /*free_scripts*/, bool post_shutdown)
			{
				if (!post_shutdown)
				{
					bot_names.clear();
					bot_id = 0;
				}
			});
		}
	};
}

REGISTER_COMPONENT(bots::component)