#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "console/console.hpp"

#include "gsc/script_extension.hpp"

#include <utils/hook.hpp>

namespace logprint
{
	namespace
	{
		utils::hook::detour g_say_hook;
		utils::hook::detour scr_player_killed_hook;
		utils::hook::detour scr_player_damage_hook;

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

		std::string get_weapon_name(unsigned short* weapon, bool isAlternate)
		{
			char output[1024] = { 0 };
			game::BG_GetWeaponNameComplete(weapon, isAlternate, output, 1024);
			return output;
		}

		std::string convert_mod(const int meansOfDeath)
		{
			const auto value = reinterpret_cast<game::scr_string_t**>(0x14196AA40)[meansOfDeath];
			const auto string = game::SL_ConvertToString(*value);
			return string;
		}

		void scr_player_damage_stub(game::gentity_s* self, game::gentity_s* inflictor, game::gentity_s* attacker, int damage, int damageFlags,
			unsigned int meansOfDeath, unsigned short* weapon, bool isAlternate, const float* vPoint, const float* vDir,
			game::hitLocation_t hitLoc, int timeOffset, int modelIndex, game::scr_string_t partName)
		{
			auto hit_location = game::g_HitLocNames[hitLoc];
			const auto mod = convert_mod(meansOfDeath);
			const auto weapon_name = get_weapon_name(weapon, isAlternate);

			scheduler::once([=]()
			{
				try
				{
					const scripting::entity self_{ game::Scr_GetEntityId(self->s.number, 0) };
					assert(self && "self must be defined in Scr_PlayerDamage");

					int lpselfnum = self_.call("getentitynumber").as<int>();
					std::string lpselfname = self_.get("name").as<std::string>();
					std::string lpselfteam = self_.get("team").as<std::string>();
					std::string lpselfGuid = self_.call("getxuid").as<std::string>();

					int lpattacknum = -1;
					std::string lpattackname = "";
					std::string lpattackGuid = "";
					std::string lpattackerteam = "world";

					if (attacker)
					{
						const scripting::entity attacker_{ game::Scr_GetEntityId(attacker->s.number, 0) };
						if (scripting::call("isplayer", { attacker_ }).as<bool>())
						{
							lpattacknum = attacker_.call("getentitynumber").as<int>();
							lpattackname = attacker_.get("name").as<std::string>();
							lpattackerteam = attacker_.get("team").as<std::string>();
							lpattackGuid = attacker_.call("getxuid").as<std::string>();
						}
					}

					game::G_LogPrintf("D;%s;%d;%s;%s;%s;%d;%s;%s;%s;%d;%s;%s\n",
						lpselfGuid.data(),
						lpselfnum,
						lpselfteam.data(),
						lpselfname.data(),
						lpattackGuid.data(),
						lpattacknum,
						lpattackerteam.data(),
						lpattackname.data(),
						weapon_name.data(),
						damage,
						mod.data(),
						hit_location);
				}
				catch (std::exception err)
				{
					console::error("%s\n", err.what());
				}
			}, scheduler::pipeline::server);

			return scr_player_damage_hook.invoke<void>(self, inflictor, attacker, damage, damageFlags,
				meansOfDeath, weapon, isAlternate, vPoint, vDir, hitLoc, timeOffset, modelIndex, partName);
		}

		void scr_player_killed_stub(game::gentity_s* self, game::gentity_s* inflictor, game::gentity_s* attacker, int damage, int damageFlags,
			unsigned int meansOfDeath, unsigned short* weapon, bool isAlternate, const float* vDir,
			game::hitLocation_t hitLoc, int timeOffset, int deathAnimDuration)
		{
			auto hit_location = game::g_HitLocNames[hitLoc];
			const auto mod = convert_mod(meansOfDeath);
			const auto weapon_name = get_weapon_name(weapon, isAlternate);

			scheduler::once([=]()
			{
				try
				{
					const scripting::entity self_{ game::Scr_GetEntityId(self->s.number, 0) };
					assert(self && "self must be defined in Scr_PlayerKilled");

					int lpselfnum = self_.call("getentitynumber").as<int>();
					std::string lpselfname = self_.get("name").as<std::string>();
					std::string lpselfteam = self_.get("team").as<std::string>();
					std::string lpselfGuid = self_.call("getxuid").as<std::string>();

					int lpattacknum = -1;
					std::string lpattackname = "";
					std::string lpattackGuid = "";
					std::string lpattackerteam = "world";

					if (attacker)
					{
						const scripting::entity attacker_{ game::Scr_GetEntityId(attacker->s.number, 0) };
						if (scripting::call("isplayer", { attacker_ }).as<bool>())
						{
							lpattacknum = attacker_.call("getentitynumber").as<int>();
							lpattackname = attacker_.get("name").as<std::string>();
							lpattackerteam = attacker_.get("team").as<std::string>();
							lpattackGuid = attacker_.call("getxuid").as<std::string>();
						}
					}

					game::G_LogPrintf("K;%s;%d;%s;%s;%s;%d;%s;%s;%s;%d;%s;%s\n",
						lpselfGuid.data(),
						lpselfnum,
						lpselfteam.data(),
						lpselfname.data(),
						lpattackGuid.data(),
						lpattacknum,
						lpattackerteam.data(),
						lpattackname.data(),
						weapon_name.data(),
						damage,
						mod.data(),
						hit_location);
				}
				catch (std::exception err)
				{
					console::error("%s\n", err.what());
				}
			}, scheduler::pipeline::server);

			return scr_player_killed_hook.invoke<void>(self, inflictor, attacker, damage, damageFlags, 
				meansOfDeath, weapon, isAlternate, vDir, hitLoc, timeOffset, deathAnimDuration);
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

			// Hook player damage and player death so we can logprint them
			scr_player_damage_hook.create(0x140B5E7D0, scr_player_damage_stub);
			scr_player_killed_hook.create(0x140B5E9E0, scr_player_killed_stub);
		}
	};
}

REGISTER_COMPONENT(logprint::component)