#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "party.hpp"

#include "steam/steam.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console.hpp"
#include "network.hpp"
#include "scheduler.hpp"

#include <utils/string.hpp>
#include <utils/info_string.hpp>
#include <utils/hook.hpp>
#include <utils/cryptography.hpp>

namespace party
{
	namespace
	{
		struct
		{
			game::netadr_s host{};
			std::string challenge{};
			bool hostDefined{ false };
		} connect_state;

		void connect_to_party(const game::netadr_s& target, const std::string& mapname, const std::string& gametype)
		{
			if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_MP &&
				game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_CP)
			{
				return;
			}

			/*if (game::Live_SyncOnlineDataFlags(0) != 0)
			{
				// initialize the game after onlinedataflags is 32 (workaround)
				if (game::Live_SyncOnlineDataFlags(0) == 32)
				{
					scheduler::once([=]()
					{
						command::execute("xstartprivateparty", true);
						command::execute("disconnect", true); // 32 -> 0

						connect_to_party(target, mapname, gametype);
					}, scheduler::pipeline::main, 1s);
					return;
				}
				else
				{
					scheduler::once([=]()
					{
						connect_to_party(target, mapname, gametype);
					}, scheduler::pipeline::main, 1s);
					return;
				}
			}*/

			//perform_game_initialization();

			// shutdown frontend
			game::Com_FrontEndScene_ShutdownAndDisable();

			// connect
			char session_info[0x100] = {};
			game::CL_MainMP_ConnectAndPreloadMap(0, reinterpret_cast<void*>(session_info), &target, mapname.data(), gametype.data());
		}

		std::string get_dvar_string(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.string)
			{
				return dvar_value->current.string;
			}

			return {};
		}

		int get_dvar_int(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.integer)
			{
				return dvar_value->current.integer;
			}

			return -1;
		}

		bool get_dvar_bool(const std::string& dvar)
		{
			auto* dvar_value = game::Dvar_FindVar(dvar.data());
			if (dvar_value && dvar_value->current.enabled)
			{
				return dvar_value->current.enabled;
			}

			return false;
		}
	}

	int get_client_count()
	{
		return 0;
	}

	int get_bot_count()
	{
		return 0;
	}

	void connect(const game::netadr_s& target)
	{
		//command::execute("lui_open_popup popup_acceptinginvite", false);

		connect_state.host = target;
		connect_state.challenge = utils::cryptography::random::get_challenge();
		connect_state.hostDefined = true;

		network::send(target, "getInfo", connect_state.challenge);
	}

	void info_response_error(const std::string& error)
	{
		console::error("%s\n", error.data());
		//if (game::Menu_IsMenuOpenAndVisible(0, "popup_acceptinginvite"))
		//{
		//	command::execute("lui_close popup_acceptinginvite", false);
		//}

		game::Com_SetLocalizedErrorMessage(error.data(), "MENU_NOTICE");
		//*reinterpret_cast<int*>(0x0) = 1;
	}


	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("connect", [](const command::params& argument)
			{
				if (argument.size() != 2)
				{
					return;
				}

				game::netadr_s target{};
				if (game::NET_StringToAdr(argument[1], &target))
				{
					connect(target);
				}
			});

			network::on("getInfo", [](const game::netadr_s& target, const std::string_view& data)
			{
				utils::info_string info{};
				info.set("challenge", std::string{ data });
				info.set("gamename", "IW7");
				info.set("hostname", get_dvar_string("sv_hostname"));
				info.set("gametype", get_dvar_string("g_gametype"));
				info.set("sv_motd", get_dvar_string("sv_motd"));
				info.set("xuid", utils::string::va("%llX", steam::SteamUser()->GetSteamID().bits));
				info.set("mapname", get_dvar_string("mapname"));
				info.set("isPrivate", get_dvar_string("g_password").empty() ? "0" : "1");
				info.set("clients", utils::string::va("%i", get_client_count()));
				info.set("bots", utils::string::va("%i", get_bot_count()));
				info.set("sv_maxclients", utils::string::va("%i", *game::svs_numclients));
				info.set("protocol", utils::string::va("%i", PROTOCOL));
				info.set("playmode", utils::string::va("%i", game::Com_GameMode_GetActiveGameMode()));
				info.set("sv_running", utils::string::va("%i", get_dvar_bool("sv_running") && !game::Com_FrontEndScene_IsActive()));
				info.set("dedicated", utils::string::va("%i", get_dvar_bool("dedicated")));

				network::send(target, "infoResponse", info.build(), '\n');
			});

			network::on("infoResponse", [](const game::netadr_s& target, const std::string_view& data)
			{
				const utils::info_string info{ data };
				//server_list::handle_info_response(target, info);

				if (connect_state.host != target)
				{
					return;
				}

				if (info.get("challenge") != connect_state.challenge)
				{
					info_response_error("Invalid challenge.");
					return;
				}

				const auto gamename = info.get("gamename");
				if (gamename != "IW7"s)
				{
					info_response_error("Invalid gamename.");
					return;
				}

				const auto playmode = info.get("playmode");
				if (game::GameModeType(std::atoi(playmode.data())) != game::Com_GameMode_GetActiveGameMode())
				{
					info_response_error("Invalid playmode.");
					return;
				}

				const auto sv_running = info.get("sv_running");
				if (!std::atoi(sv_running.data()))
				{
					info_response_error("Server not running.");
					return;
				}

				const auto mapname = info.get("mapname");
				if (mapname.empty())
				{
					info_response_error("Invalid map.");
					return;
				}

				const auto gametype = info.get("gametype");
				if (gametype.empty())
				{
					info_response_error("Connection failed: Invalid gametype.");
					return;
				}

				//party::sv_motd = info.get("sv_motd");
				//party::sv_maxclients = std::stoi(info.get("sv_maxclients"));

				connect_to_party(target, mapname, gametype);
			});
		}
	};
}

REGISTER_COMPONENT(party::component)