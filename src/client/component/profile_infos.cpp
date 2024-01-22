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

		game::fragment_handler::fragment_data(data.data(), data.size(), [&address](const utils::byte_buffer& buffer)
		{
			network::send(address, "add_profile_info_server", buffer.get_buffer());
		});
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

	void handle_profile_info_data(const game::netadr_s& address, const std::string_view& data)
	{
		utils::byte_buffer buffer(data);

		std::string final_packet{};
		if (!game::fragment_handler::handle(address, buffer, final_packet))
		{
			console::error("failed to handle fragmented packet\n");
			return;
		}

		buffer = utils::byte_buffer(final_packet);
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

			network::on("add_profile_info_server", [](const game::netadr_s& address, const std::string_view& data)
			{
				console::debug("add_profile_info_server\n");

				static const auto sv_running = game::Dvar_FindVar("sv_running");
				if (sv_running == nullptr || !sv_running->current.enabled)
				{
					console::error("add_profile_info_server recieved when sv_running is 0?\n");
				}

				if (!game::environment::is_dedi())
				{
					handle_profile_info_data(address, data); // for host to have since loopback might not be supported
				}

				const auto clients = *game::svs_clients;
				for (int index = 0; index < get_max_client_count(); ++index)
				{
					const auto client = clients[index];
					if (client.remoteAddress.type != game::NA_BOT && client.remoteAddress.type != game::NA_LOOPBACK)
					{
						network::send(client.remoteAddress, "add_profile_info", data.data());
					}
				}
			});

			network::on("add_profile_info", [](const game::netadr_s& address, const std::string_view& data)
			{
				console::debug("add_profile_info\n");

				if (party::get_server_connection_state().host != address) // only allow host address to add new profiles
				{
					return;
				}

				handle_profile_info_data(address, data);
			});
		}
	};
}

REGISTER_COMPONENT(profile_infos::component)
