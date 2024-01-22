#include <std_include.hpp>
#include "loader/component_loader.hpp"

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
	namespace
	{
		using profile_map = std::unordered_map<std::uint64_t, profile_info>;
		utils::concurrency::container<profile_map, std::recursive_mutex> profile_mapping{};

		int get_max_client_count()
		{
			static const auto com_maxclients = game::Dvar_FindVar("com_maxclients");
			if (com_maxclients)
			{
				return com_maxclients->current.integer;
			}
			return 0;
		}

		template <typename T>
		static void foreach_client(T* client_states, const std::function<void(game::client_t&, int index)>& callback)
		{
			if (!client_states || !callback)
			{
				return;
			}

			for (int i = 0; i < get_max_client_count(); ++i)
			{
				callback(client_states[i], i);
			}
		}

		void foreach_client(const std::function<void(game::client_t&, int index)>& callback)
		{
			foreach_client(*game::svs_clients, callback);
		}

		std::optional<profile_info> load_profile_info()
		{
			std::string data{};
			if (!utils::io::read_file("players2/user/profile_info", &data))
			{
				return {};
			}

			profile_info info{};
			constexpr auto version_size = sizeof(info.version);

			if (data.size() < sizeof(version_size))
			{
				return {};
			}

			memcpy(&info.version, data.data(), version_size);
			info.ddl.assign(data.begin() + version_size, data.end());

			return {std::move(info)};
		}

		void send_profile_info(const game::netadr_s& address, const std::string& data)
		{
			game::fragment_handler::fragment_data(data.data(), data.size(), [&address](const utils::byte_buffer& buffer)
			{
				network::send(address, "profileInfo", buffer.get_buffer());
			});
		}

		void distribute_profile_info(const uint64_t user_id, const profile_info& info)
		{
			if (user_id == steam::SteamUser()->GetSteamID().bits)
			{
				return;
			}

			utils::byte_buffer buffer{};
			buffer.write(user_id);
			info.serialize(buffer);

			const std::string data = buffer.move_buffer();

			foreach_connected_client([&](const game::client_t& client, const int)
			{
				send_profile_info(client.remoteAddress, data);
			});
		}

		std::unordered_set<std::uint64_t> get_connected_client_xuids()
		{
			std::unordered_set<std::uint64_t> connected_clients{};
			connected_clients.reserve(get_max_client_count());

			foreach_connected_client([&](const game::client_t&, const int index)
			{
				const auto guid = game::SV_GameMP_GetGuid(index);
				connected_clients.emplace(party::get_xuid_from_guid(guid)); // TODO
			});

			return connected_clients;
		}

		void clean_cached_profile_infos()
		{
			static const auto sv_running = game::Dvar_FindVar("sv_running");
			if (sv_running == nullptr || !sv_running->current.enabled)
			{
				return;
			}

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
#ifdef DEBUG
						console::debug("Erasing profile info: %llX\n", i->first);
#endif

						i = profiles.erase(i);
					}
				}
			});
		}
	}

	profile_info::profile_info(utils::byte_buffer& buffer)
	{
		this->version = buffer.read<int32_t>();
		this->ddl = buffer.read_string();
	}

	void profile_info::serialize(utils::byte_buffer& buffer) const
	{
		buffer.write(this->version);
		buffer.write_string(this->ddl);
	}

	void add_profile_info(const uint64_t user_id, const profile_info& info)
	{
		if (user_id == steam::SteamUser()->GetSteamID().bits)
		{
			return;
		}

#ifdef DEBUG
		console::debug("Adding profile info: %llX\n", user_id);
#endif

		profile_mapping.access([&](profile_map& profiles)
		{
			profiles[user_id] = info;
		});
	}

	void distribute_profile_info_to_user(const game::netadr_s& addr, const uint64_t user_id, const profile_info& info)
	{
		utils::byte_buffer buffer{};
		buffer.write(user_id);
		info.serialize(buffer);

		send_profile_info(addr, buffer.get_buffer());
	}

	void distribute_profile_infos_to_user(const game::netadr_s& addr)
	{
		profile_mapping.access([&](const profile_map& profiles)
		{
			for (const auto& entry : profiles)
			{
				distribute_profile_info_to_user(addr, entry.first, entry.second);
			}
		});

		if (!game::environment::is_dedi())
		{
			const auto info = get_profile_info();
			if (info)
			{
				distribute_profile_info_to_user(addr, steam::SteamUser()->GetSteamID().bits, *info);
			}
		}
	}

	void add_and_distribute_profile_info(const game::netadr_s& addr, const uint64_t user_id, const profile_info& info)
	{
		console::debug("add_and_distribute_profile_info\n");
		distribute_profile_infos_to_user(addr); // send profiles stored in map & our profile to host
		add_profile_info(user_id, info);		// add host's profile info
		distribute_profile_info(user_id, info);	// send host's profile info to all clients
	}

	void clear_profile_infos()
	{
		profile_mapping.access([&](profile_map& profiles)
		{
			profiles = {};
		});
	}

	std::unique_lock<std::recursive_mutex> accquire_profile_lock()
	{
		return profile_mapping.accquire_lock();
	}

	std::optional<profile_info> get_profile_info()
	{
		return load_profile_info();
	}

	std::optional<profile_info> get_profile_info(const uint64_t user_id)
	{
		if (user_id == steam::SteamUser()->GetSteamID().bits)
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
				console::debug("Requesting profile info: %llX - good\n", user_id);
#endif
			}
#ifdef DEBUG
			else
			{
				console::debug("Requesting profile info: %llX - bad\n", user_id);
			}
#endif

			return result;
		});
	}

	void update_profile_info(const profile_info& info)
	{
		std::string data{};
		data.reserve(4 + info.ddl.size());

		data.append(reinterpret_cast<const char*>(&info.version), sizeof(info.version));
		data.append(info.ddl);

		utils::io::write_file("players2/user/profile_info", data);
	}

	void foreach_connected_client(const std::function<void(game::client_t&, int index)>& callback)
	{
		foreach_client([&](game::client_t& client, const int index)
		{
			if (client.header.state != 0x0) // CS_FREE
			{
				callback(client, index);
			}
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(clean_cached_profile_infos, scheduler::main, 5s);

			if (!game::environment::is_dedi())
			{
				// server sends this to client, contains a fragmented packet for profile info
				network::on("profileInfo", [](const game::netadr_s& server, const std::string_view& data)
				{
					if (party::get_server_connection_state().host != server) // only allow from servers
					{
						return;
					}

					utils::byte_buffer buffer(data);

					std::string final_packet{};
					if (game::fragment_handler::handle(server, buffer, final_packet))
					{
						buffer = utils::byte_buffer(final_packet);
						const auto user_id = buffer.read<uint64_t>();
						const profile_info info(buffer);
						add_profile_info(user_id, info);
					}
				});
			}
		}
	};
}

REGISTER_COMPONENT(profile_infos::component)
