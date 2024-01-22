#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "console/console.hpp"
#include "network.hpp"
#include "party.hpp"
#include "profile_infos.hpp"
#include "scheduler.hpp"

#include <utils/concurrency.hpp>

#include "../steam/steam.hpp"
#include <utils/io.hpp>

//#include "game/fragment_handler.hpp"

/*

	const auto lobby = SV_MainMP_GetServerLobby();
	Session_GetXuidEvenIfInactive(lobby, client_num);

	our XUID = steam::SteamUser()->GetSteamID().bits

*/

namespace profile_infos
{
	namespace
	{
		using profile_map = std::unordered_map<std::uint64_t, profile_info>;
		utils::concurrency::container<profile_map, std::recursive_mutex> profile_mapping{};

		profile_map server_profile_info_cache;

		std::optional<profile_info> load_profile_info()
		{
			std::string data{};
			if (!utils::io::read_file("players2/user/profile_info", &data))
			{
				return {};
			}

			profile_info info{};
			info.m_memberplayer_card = data;

			return {std::move(info)};
		}

		int get_max_client_count()
		{
			static const auto com_maxclients = game::Dvar_FindVar("com_maxclients");
			if (com_maxclients)
			{
				return com_maxclients->current.integer;
			}
			return 0;
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

	void send_profile_info(const game::netadr_s& address, const std::uint64_t user_id, const profile_info& info)
	{
		utils::byte_buffer buffer{};
		buffer.write(user_id);
		info.serialize(buffer);

		const std::string data = buffer.move_buffer();

		network::send(address, "add_profile_info_server", buffer.get_buffer());
		/*
		game::fragment_handler::fragment_data(data.data(), data.size(), [&address](const utils::byte_buffer& buffer)
		{
			network::send(address, "add_profile_info_server", buffer.get_buffer());
		});
		*/
	}

	void add_profile_info(const std::uint64_t user_id, const profile_info& info)
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
		data.reserve(info.m_memberplayer_card.size());
		data.append(info.m_memberplayer_card);
		utils::io::write_file("players2/user/profile_info", data);
	}

	/*
	bool handle_fragmented_packet(const game::netadr_s& address, const std::string_view& data, utils::byte_buffer* buffer_)
	{
		utils::byte_buffer buffer(data);

		std::string final_packet{};
		if (!game::fragment_handler::handle(address, buffer, final_packet))
		{
			console::error("failed to handle fragmented packet\n");
			return false;
		}

		*buffer_ = utils::byte_buffer(final_packet);
		return true;
	}
	*/

	void handle_profile_info_data(const game::netadr_s& address, const std::string_view& data)
	{
		utils::byte_buffer buffer(data);
		//if (!handle_fragmented_packet(address, data, &buffer))
		//{
		//	return;
		//}

		const auto user_id = buffer.read<std::uint64_t>();
		const profile_info info(buffer);
		add_profile_info(user_id, info);
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			//scheduler::loop(clean_cached_profile_infos, scheduler::main, 5s);

			command::add("pcache_refresh", []()
			{
				game::g_DWPlayercardCacheDownloadTaskStage[0x0] = game::PLAYERCARD_CACHE_TASK_STAGE_WAITING;
			});

			// client -> server
			network::on("add_profile_info_server", [](const game::netadr_s& client_addr, const std::string_view& data)
			{
				static const auto sv_running = game::Dvar_FindVar("sv_running");
				if (sv_running == nullptr || !sv_running->current.enabled)
				{
					console::error("add_profile_info_server recieved when sv_running is 0?\n");
					return;
				}

				/*
				if (!handle_fragmented_packet(client_addr, data, &buffer))
				{
					return;
				}
				*/

				// TODO: remove this, but we store our server profile in there if not in
				static const auto my_xuid = steam::SteamUser()->GetSteamID().bits;
				const auto my_profile_info = get_profile_info();
				if (!server_profile_info_cache.contains(my_xuid) && my_profile_info.has_value())
				{
					server_profile_info_cache[my_xuid] = my_profile_info.value();
				}

				utils::byte_buffer buffer(data);
				const auto user_id = buffer.read<std::uint64_t>();
				const profile_info info(buffer);
				server_profile_info_cache[user_id] = info; // store into cache for get requests

				const auto clients = *game::svs_clients;
				for (int index = 0; index < get_max_client_count(); ++index)
				{
					const auto client = clients[index];
					if (client.remoteAddress.type != game::NA_BOT && client.remoteAddress.type != game::NA_LOOPBACK)
					{
						network::send(client.remoteAddress, "add_profile_info", buffer.get_buffer());
					}
				}
			});

			// server -> client
			network::on("add_profile_info", [](const game::netadr_s& server_addr, const std::string_view& data)
			{
				if (party::get_server_connection_state().host != server_addr) // only allow host address to add new profiles
				{
					return;
				}

				console::debug("adding new client profile info...\n");
				handle_profile_info_data(server_addr, data);
				
				// trigger refresh of PlayercardCache
				console::debug("setting task stage PLAYERCARD_CACHE_TASK_STAGE_WAITING\n");
				game::g_DWPlayercardCacheDownloadTaskStage[0x0] = game::PLAYERCARD_CACHE_TASK_STAGE_WAITING;
			});

			// client -> server
			network::on("get_profile_infos_server", [](const game::netadr_s& client_addr, const std::string_view&)
			{
				utils::byte_buffer buffer{};
				buffer.write(server_profile_info_cache.size());

				for (const auto& [key, value] : server_profile_info_cache)
				{
					buffer.write(key);
					value.serialize(buffer);
				}

				const std::string data = buffer.move_buffer();

				network::send(client_addr, "get_profile_infos_response", buffer.get_buffer());
				/*
				game::fragment_handler::fragment_data(data.data(), data.size(), [&client_addr](const utils::byte_buffer& buffer)
				{
					network::send(client_addr, "get_profile_infos_response", buffer.get_buffer());
				});
				*/
			});

			// server -> client
			network::on("get_profile_infos_response", [](const game::netadr_s& server_addr, const std::string_view& data)
			{
				utils::byte_buffer buffer(data);
				/*
				if (!handle_fragmented_packet(server_addr, data, &buffer))
				{
					return;
				}
				*/

				const auto profile_infos_count = buffer.read<size_t>();
				for (auto index = 0; index < profile_infos_count; ++index)
				{
					const auto user_id = buffer.read<std::uint64_t>();
					profile_info info{};
					info.m_memberplayer_card = buffer.read<std::string>();
					add_profile_info(user_id, info);
				}
			});
		}
	};
}

REGISTER_COMPONENT(profile_infos::component)
