#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"
#include "command.hpp"
#include "console/console.hpp"
#include "network.hpp"
#include "party.hpp"
#include "profile_infos.hpp"
#include "scheduler.hpp"

#include <utils/concurrency.hpp>

#include "../steam/steam.hpp"
#include <utils/io.hpp>

#include "game/fragment_handler.hpp"

namespace profile_infos
{
	namespace xuid
	{
		client_xuid_array client_xuids{};

		void add_client_xuid(const std::uint32_t& player_id, const std::uint64_t& xuid)
		{
			console::debug("[add_client_xuid] adding xuid %llX for client num %u\n", xuid, player_id);
			client_xuids[player_id] = xuid;
		}

		std::uint64_t get_client_xuid(const std::uint32_t& player_id)
		{
			if (client_xuids[player_id])
			{
				// returns xuid for player. this must be on both the client & server
				// client recieves data for this via playerXuid packet
				return client_xuids[player_id];
			}
			
			console::error("returning XUID 0 for %u\n", player_id);
			return static_cast<std::uint64_t>(0);
		}

		// TODO
		void remove_client_xuid(const std::uint32_t& player_id)
		{
			const auto xuid = client_xuids[player_id];
			console::debug("[remove_client_xuid] removing xuid %llX for client num %u\n", xuid, player_id);
			client_xuids[player_id] = 0;
		}

		void clear_xuids()
		{
			for (auto& xuid : client_xuids)
			{
				xuid = 0;
			}
		}

		client_xuid_array get_xuids()
		{
			return client_xuids;
		}
	}

	namespace
	{
		using profile_map = std::unordered_map<std::uint64_t, profile_info>;
		utils::concurrency::container<profile_map, std::recursive_mutex> profile_mapping{};

		std::optional<profile_info> load_profile_info()
		{
			std::string data{};
			if (!utils::io::read_file("players2/user/profile_info", &data))
			{
				console::error("[load_profile_info] failed to load profile_info for self!\n");
				return {};
			}

			profile_info info{};
			info.m_memberplayer_card.assign(data);

			return {std::move(info)};
		}

		std::unordered_set<std::uint64_t> get_connected_client_xuids()
		{
			if (!game::SV_Loaded()) // is_host()
			{
				return {};
			}

			std::unordered_set<std::uint64_t> xuids{};

			const auto server_session = game::SV_MainMP_GetServerLobby();
			const auto* svs_clients = *game::svs_clients;
			for (unsigned int i = 0; i < *game::svs_numclients; ++i)
			{
				if (svs_clients[i].header.state >= 1)
				{
					xuids.emplace(xuid::get_client_xuid(i));
				}
			}

			return xuids;
		}

		auto last_clear = std::chrono::high_resolution_clock::now();
		void clear_invalid_mappings()
		{
			const auto time = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration_cast<std::chrono::seconds>(time - last_clear).count() < 15) // add a grace period of 15 seconds since last clear
			{
				return;
			}
			last_clear = time;

			profile_mapping.access([](profile_map& profiles)
			{
				const auto xuids = get_connected_client_xuids();

				for (auto i = profiles.begin(); i != profiles.end();)
				{
					if (xuids.contains(i->first))
					{
						++i;
					}
					else
					{
						console::debug("[clear_invalid_mappings] erasing profile info: %llX\n", i->first);
						i = profiles.erase(i);
					}
				}
			});
		}
	}

	profile_info::profile_info(utils::byte_buffer& buffer)
	{
		this->m_memberplayer_card = buffer.read_string();
	}

	void profile_info::serialize(utils::byte_buffer& buffer) const
	{
		buffer.write_string(this->m_memberplayer_card);
	}

	void send_profile_info(const game::netadr_s& address, const std::string& data)
	{
		game::fragment_handler::fragment_data(data.data(), data.size(), [&address](const utils::byte_buffer& buffer)
		{
			network::send(address, "profileInfo", buffer.get_buffer());
		});
	}

	void send_profile_info(const game::netadr_s& address, const std::uint64_t user_id, const profile_info& info)
	{
		utils::byte_buffer buffer{};
		buffer.write(user_id);
		info.serialize(buffer);

		const std::string data = buffer.move_buffer();

		send_profile_info(address, data);
	}

	std::optional<profile_info> get_profile_info()
	{
		return load_profile_info();
	}

	std::optional<profile_info> get_profile_info(const uint64_t user_id)
	{
		const auto my_xuid = steam::SteamUser()->GetSteamID().bits;
		console::debug("get_profile_info: (%llX) == (%llX)\n", user_id, my_xuid);
		if (user_id == my_xuid)
		{
			return get_profile_info();
		}

		return profile_mapping.access<std::optional<profile_info>>([user_id](const profile_map& profiles)
		{
			std::optional<profile_info> result{};

			const auto profile_entry = profiles.find(user_id);
			if (profile_entry != profiles.end())
			{
				result = profile_entry->second;

#ifdef DEBUG
				console::debug("[get_profile_info] requesting profile info for %llX (good)\n", user_id);
#endif
			}
#ifdef DEBUG
			else
			{
				console::error("[get_profile_info] requesting profile info for %llX (bad)\n", user_id);
			}
#endif

			return result;
		});
	}

	void update_profile_info(const profile_info& info)
	{
		std::string data{};
		data.reserve(info.m_memberplayer_card.size());
		data.append(info.m_memberplayer_card);
		utils::io::write_file("players2/user/profile_info", data);
	}

	void send_all_profile_infos(const game::netadr_s& sender_addr)
	{
		profile_mapping.access([&](const profile_map& profiles)
		{
			for (const auto& entry : profiles)
			{
				send_profile_info(sender_addr, entry.first, entry.second);
			}
		});
	}

	void send_profile_info_to_all_clients(const std::uint64_t user_id, const profile_info& info)
	{
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1 && !game::Party_IsHost(game::SV_MainMP_GetServerLobby(), i))
			{
				send_profile_info(svs_clients[i].remoteAddress, user_id, info);
			}
		}
	}

	void send_self_profile(const game::netadr_s& addr)
	{
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1 && game::Party_IsHost(game::SV_MainMP_GetServerLobby(), i))
			{
				auto self = load_profile_info();
				if (self.has_value())
				{
					console::debug("[send_self_profile] sending self profile info\n");
					send_profile_info(addr, xuid::get_client_xuid(i), self.value());
					break;
				}
			}
		}
	}

	void send_xuid_to_all_clients(const std::uint64_t user_id, const std::uint32_t& num_client)
	{
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1 && !game::Party_IsHost(game::SV_MainMP_GetServerLobby(), i))
			{
				utils::byte_buffer buffer{};
				buffer.write(num_client);
				buffer.write(user_id);

				const std::string data = buffer.move_buffer();

				game::fragment_handler::fragment_data(data.data(), data.size(), [&](const utils::byte_buffer& buffer)
				{
					network::send(svs_clients[i].remoteAddress, "playerXuid", buffer.get_buffer());
				});
			}
		}
	}

	void add_profile_info(const game::netadr_s& sender_addr, const std::uint64_t user_id, const profile_info& info)
	{
		if (user_id == steam::SteamUser()->GetSteamID().bits)
		{
			return;
		}

#ifdef DEBUG
		console::debug("[add_profile_info] adding profile info for %llX\n", user_id);
#endif

		if (game::SV_Loaded()) // is_host()
		{
			const auto num_client = 1; // cgArray->snap->numClients
			xuid::add_client_xuid(num_client, user_id);

			// send new player xuid to all clients (TODO: find better way to just get xuid from client num lmfao)
			send_xuid_to_all_clients(user_id, num_client);

			// clear disconnected players
			clear_invalid_mappings(); // clear BEFORE adding connecting user

			send_all_profile_infos(sender_addr);	// send all stored profile infos to the new player
			send_self_profile(sender_addr);			// send self profile info to the new player too

			// send new player info to all clients
			send_profile_info_to_all_clients(user_id, info);
		}

		profile_mapping.access([&](profile_map& profiles)
		{
			profiles[user_id] = info;
		});

		game::XUID xuid{user_id};
		game::PlayercardCache_AddToDownload(0, xuid);
		command::execute("download_playercard", false);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::override::register_int("playercard_cache_validity_life", 5, 1, 60, 0x0);

			network::on("profileInfo", [](const game::netadr_s& client_addr, const std::string_view& data)
			{
				utils::byte_buffer buffer(data);
				std::string final_packet{};
				if (game::fragment_handler::handle(client_addr, buffer, final_packet))
				{
					buffer = utils::byte_buffer(final_packet);

					const auto user_id = buffer.read<uint64_t>();

					const profile_info info(buffer);
					add_profile_info(client_addr, user_id, info);
				}
			});

			network::on("playerXuid", [](const game::netadr_s& server_addr, const std::string_view& data)
			{
				utils::byte_buffer buffer(data);
				std::string final_packet{};
				if (game::fragment_handler::handle(server_addr, buffer, final_packet))
				{
					buffer = utils::byte_buffer(final_packet);

					const auto num_client = buffer.read<std::uint32_t>();
					const auto user_id = buffer.read<std::uint64_t>();
					xuid::add_client_xuid(num_client, user_id);
				}
			});
		}
	};
}

REGISTER_COMPONENT(profile_infos::component)
