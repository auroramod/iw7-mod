#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "console/console.hpp"
#include "fastfiles.hpp"
#include "localized_strings.hpp"
#include "network.hpp"
#include "party.hpp"
#include "scheduler.hpp"
#include "server_list.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "game/ui_scripting/execution.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace server_list
{
	namespace
	{
		const int server_limit = 128;

		struct server_info
		{
			int clients;
			int max_clients;
			int bots;
			int ping;
			std::string host_name;
			std::string map_name;
			std::string game_type;
			std::string mod_name;
			game::GameModeType play_mode;
			char in_game;
			game::netadr_s address;
			bool is_private;
		};

		struct
		{
			game::netadr_s address{};
			volatile bool requesting = false;
			std::unordered_map<game::netadr_s, int> queued_servers{};
		} master_state;

		std::mutex mutex;
		std::vector<server_info> servers;

		game::dvar_t* master_server_ip;
		game::dvar_t* master_server_port;

		void refresh_server_list()
		{
			{
				std::lock_guard<std::mutex> _(mutex);
				servers.clear();
				master_state.queued_servers.clear();
			}

			party::reset_server_connection_state();

			if (get_master_server(master_state.address))
			{
				master_state.requesting = true;
				network::send(master_state.address, "getservers", utils::string::va("IW7 %i full empty", PROTOCOL));
			}
		}

		void join_server(int, int, const int index)
		{
			std::lock_guard<std::mutex> _(mutex);

			const auto i = static_cast<size_t>(index);
			if (i < servers.size())
			{
				static auto last_index = ~0ull;
				if (last_index != i)
				{
					last_index = i;
				}
				else
				{
					party::connect(servers[i].address);
				}
			}
		}

		unsigned short node_index = 0;

		const char* ui_feeder_item_text(int localClientNum, int feederID, const int index, const int column, 
			float* s0, float* t0, float* s1, float* t1, game::Material** material)
		{
			const auto i = index;
			if (i >= servers.size())
			{
				return "";
			}

			switch (column)
			{
			case 2:
			{
				return servers[i].host_name.empty() ? "" : servers[i].host_name.data();
			}
			case 3:
			{
				const auto& map_name = servers[i].map_name;
				if (map_name.empty())
				{
					return "Unknown";
				}

				auto map_display_name = game::UI_GetMapDisplayName(map_name.data());
				if (!fastfiles::exists(map_name))
				{
					map_display_name = utils::string::va("^1%s", map_display_name);
				}
				return map_display_name;
			}
			case 4:
			{
				const auto client_count = servers[i].clients - servers[i].bots;
				return utils::string::va("%d/%d [%d]", client_count, servers[i].max_clients,
					servers[i].clients);
			}
			case 5:
			{
				return servers[i].game_type.empty() ? "" : servers[i].game_type.data();
			}
			case 6:
			{
				return servers[i].mod_name.empty() ? "" : servers[i].mod_name.data();
			}
			case 7:
			{
				const auto ping = servers[i].ping ? servers[i].ping : 999;
				if (ping < 75)
				{
					return utils::string::va("^2%d", ping);
				}
				else if (ping < 150)
				{
					return utils::string::va("^3%d", ping);
				}
				return utils::string::va("^1%d", ping);
			}
			case 8:
			{
				return servers[i].is_private ? "Yes" : "No";
			}
			case 10:
			{
				// add custom feeder values here
				game::Material* material_[2];
				
				const char* val = nullptr;
				unsigned short n_index = 0;

				val = ui_feeder_item_text(0, 0, i, 6, 0, 0, 0, 0, material_);
				n_index = game::LUI_Model_CreateModelFromPath(node_index, "mod");
				game::LUI_Model_SetString(n_index, val);

				val = ui_feeder_item_text(0, 0, i, 7, 0, 0, 0, 0, material_);
				n_index = game::LUI_Model_CreateModelFromPath(node_index, "ping");
				game::LUI_Model_SetString(n_index, val);

				val = ui_feeder_item_text(0, 0, i, 8, 0, 0, 0, 0, material_);
				n_index = game::LUI_Model_CreateModelFromPath(node_index, "priv");
				game::LUI_Model_SetString(n_index, val);

				return servers[i].in_game ? "0" : "1";
			}
			default:
				return "";
			}
		}

		unsigned short lui_model_create_model_from_path_stub(const unsigned short parentNodeIndex, const char* path)
		{
			node_index = game::LUI_Model_CreateModelFromPath(parentNodeIndex, path);
			return node_index;
		}

		void sort_serverlist()
		{
			std::stable_sort(servers.begin(), servers.end(), [](const server_info& a, const server_info& b)
			{
				const auto a_players = a.clients - a.bots;
				const auto b_players = b.clients - b.bots;

				if (a_players == b_players)
				{
					if (a.clients == b.clients)
					{
						return a.ping < b.ping;
					}

					return a.clients > b.clients;
				}

				return a_players > b_players;
			});
		}

		void insert_server(server_info&& server)
		{
			std::lock_guard<std::mutex> _(mutex);
			servers.emplace_back(std::move(server));
			sort_serverlist();
		}

		void do_frame_work()
		{
			auto& queue = master_state.queued_servers;
			if (queue.empty())
			{
				return;
			}

			std::lock_guard<std::mutex> _(mutex);

			size_t queried_servers = 0;
			const size_t query_limit = 3;

			for (auto i = queue.begin(); i != queue.end();)
			{
				if (i->second)
				{
					const auto now = game::Sys_Milliseconds();
					if (now - i->second > 10'000)
					{
						i = queue.erase(i);
						continue;
					}
				}
				else if (queried_servers++ < query_limit)
				{
					i->second = game::Sys_Milliseconds();
					network::send(i->first, "getInfo", utils::cryptography::random::get_challenge());
				}

				++i;
			}
		}

		bool is_server_list_open()
		{
			return game::Menu_IsMenuOpenAndVisible(0, "SystemLinkMenu");
		}

		utils::hook::detour lui_open_menu_hook;

		void lui_open_menu_stub(int controllerIndex, const char* menuName, int isPopup, int isModal, unsigned int isExclusive)
		{
			if (!strcmp(menuName, "SystemLinkMenu"))
			{
				refresh_server_list();
			}

			lui_open_menu_hook.invoke<void>(controllerIndex, menuName, isPopup, isModal, isExclusive);
		}
	}

	bool get_master_server(game::netadr_s& address)
	{
		return game::NET_StringToAdr(utils::string::va("%s:%s",
			master_server_ip->current.string, master_server_port->current.string), &address);
	}

	void handle_info_response(const game::netadr_s& address, const utils::info_string& info)
	{
		// Don't show servers that aren't using the same protocol!
		const auto protocol = std::atoi(info.get("protocol").data());
		if (protocol != PROTOCOL)
		{
			return;
		}

		// Don't show servers that aren't dedicated!
		const auto dedicated = std::atoi(info.get("dedicated").data());
		if (!dedicated)
		{
			return;
		}

		// Don't show servers that aren't running!
		const auto sv_running = std::atoi(info.get("sv_running").data());
		if (!sv_running)
		{
			return;
		}

		// Only handle servers of the same playmode!
		const auto playmode = game::GameModeType(std::atoi(info.get("playmode").data()));
		if (game::Com_GameMode_GetActiveGameMode() != playmode)
		{
			return;
		}

		if (info.get("gamename") != "IW7")
		{
			return;
		}

		int start_time{};
		const auto now = game::Sys_Milliseconds();

		{
			std::lock_guard<std::mutex> _(mutex);
			const auto entry = master_state.queued_servers.find(address);

			if (entry == master_state.queued_servers.end() || !entry->second)
			{
				return;
			}

			start_time = entry->second;
			master_state.queued_servers.erase(entry);
		}

		server_info server{};
		server.address = address;
		server.host_name = info.get("hostname");
		server.map_name = info.get("mapname");
		server.game_type = game::UI_GetGameTypeDisplayName(info.get("gametype").data());
		server.mod_name = info.get("fs_game");
		server.play_mode = playmode;
		server.clients = atoi(info.get("clients").data());
		server.max_clients = atoi(info.get("sv_maxclients").data());
		server.bots = atoi(info.get("bots").data());
		server.ping = std::min(now - start_time, 999);
		server.is_private = atoi(info.get("isPrivate").data()) == 1;

		server.in_game = 1;

#ifdef DEBUG
		console::debug("inserting server \"%s\"\n", server.host_name.data());
#endif

		insert_server(std::move(server));
	}

	int get_player_count()
	{
		std::lock_guard<std::mutex> _(mutex);
		auto count = 0;
		for (const auto& server : servers)
		{
			count += server.clients - server.bots;
		}
		return count;
	}

	int get_server_count()
	{
		std::lock_guard<std::mutex> _(mutex);
		return static_cast<int>(servers.size());
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::once([]()
			{
				// add dvars to change destination master server ip/port
				master_server_ip = game::Dvar_RegisterString("masterServerIP", "server.alterware.dev", game::DVAR_FLAG_NONE,
					"IP of the destination master server to connect to");
				master_server_port = game::Dvar_RegisterString("masterServerPort", "20810", game::DVAR_FLAG_NONE,
					"Port of the destination master server to connect to");
			}, scheduler::pipeline::main);

			if (game::environment::is_dedi())
			{
				return;
			}

			// hook LUI_OpenMenu to refresh server list for system link menu
			lui_open_menu_hook.create(game::LUI_OpenMenu, lui_open_menu_stub);

			// replace UI_RunMenuScript call in LUI_CoD_LuaCall_RefreshServerList to our refresh_servers
			utils::hook::jump(0x14069E019, utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.pushad64();
				a.call_aligned(refresh_server_list);
				a.popad64();

				a.xor_(eax, eax);
				a.mov(rbx, qword_ptr(rsp, 0x38));
				a.add(rsp, 0x20);
				a.pop(rdi);
				a.ret();
			}), true);

			utils::hook::jump(0x14069E9F7, utils::hook::assemble([](utils::hook::assembler& a)
			{
				a.mov(r8d, edi);
				a.mov(ecx, eax);
				a.mov(ebx, eax);

				a.pushad64();
				a.call_aligned(join_server);
				a.popad64();

				a.jmp(0x14069EA03);
			}), true);

			utils::hook::nop(0x14069EA1D, 5);

			utils::hook::call(0x14069E45E, get_server_count);
			utils::hook::jump(0x140CC5F00, ui_feeder_item_text);
			utils::hook::call(0x14069E4D7, lui_model_create_model_from_path_stub);

			scheduler::loop(do_frame_work, scheduler::pipeline::main);

			network::on("getServersResponse", [](const game::netadr_s& target, const std::string_view& data)
			{
				{
					std::lock_guard<std::mutex> _(mutex);
					if (!master_state.requesting || master_state.address != target)
					{
						return;
					}

					master_state.requesting = false;

					std::optional<size_t> start{};
					for (std::size_t i = 0; i + 6 < data.size(); ++i)
					{
						if (data[i + 6] == '\\')
						{
							start.emplace(i);
							break;
						}
					}

					if (!start.has_value())
					{
						return;
					}

					for (auto i = start.value(); i + 6 < data.size(); i += 7)
					{
						if (data[i + 6] != '\\')
						{
							break;
						}

						game::netadr_s address{};
						address.type = game::NA_IP;
						address.localNetID = game::NS_CLIENT1;
						std::memcpy(&address.ip[0], data.data() + i + 0, 4);
						std::memcpy(&address.port, data.data() + i + 4, 2);

						master_state.queued_servers[address] = 0;
					}
				}
			});
		}
	};
}

REGISTER_COMPONENT(server_list::component)
