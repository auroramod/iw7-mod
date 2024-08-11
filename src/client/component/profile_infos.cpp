#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "../steam/steam.hpp"

#include "dvars.hpp"
#include "console/console.hpp"
#include "network.hpp"
#include "party.hpp"
#include "profile_infos.hpp"

#include <utils/concurrency.hpp>
#include <utils/io.hpp>
#include <utils/hook.hpp>

#include "game/utils/fragment_handler.hpp"

namespace profile_infos
{
	namespace
	{
		using profile_map = std::unordered_map<std::uint64_t, profile_info>;
		utils::concurrency::container<profile_map, std::mutex> profile_mapping{};

		std::optional<profile_info> load_profile_info()
		{
			std::string data{};
			if (!utils::io::read_file("iw7-mod/players2/user/profile_info", &data))
			{
				//console::error("[load_profile_info] failed to load profile_info for self!\n");
				return {};
			}

			profile_info info{};
			info.m_memberplayer_card = std::move(data);

			return info;
		}

		void set_playercardcache_to_download(const std::uint64_t user_id)
		{
			game::XUID xuid{ user_id };
			game::PlayercardCache_AddToDownload(0, xuid);
			*game::g_DWPlayercardCacheDownloadTaskStage = game::PLAYERCARD_CACHE_TASK_STAGE_WAITING;
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

	void clear_profile_infos()
	{
		profile_mapping.access([](profile_map& profiles)
		{
			profiles.clear();
		});
	}

	void remove_profile_info(const std::uint64_t user_id)
	{
		profile_mapping.access([user_id](profile_map& profiles)
		{
			for (auto i = profiles.begin(); i != profiles.end();)
			{
				if (i->first == user_id)
				{
					i = profiles.erase(i);
				}
				else
				{
					i++;
				}
			}
		});
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

	std::optional<profile_info> get_profile_info(const std::uint64_t user_id)
	{
		const auto my_xuid = steam::SteamUser()->GetSteamID().bits;
		if (user_id == my_xuid)
		{
			return get_profile_info();
		}

		return profile_mapping.access<std::optional<profile_info>>([user_id](profile_map& profiles)
		{
			std::optional<profile_info> result{};

			const auto profile_entry = profiles.find(user_id);
			if (profile_entry != profiles.end())
			{
				result = profile_entry->second;
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
		utils::io::write_file("iw7-mod/players2/user/profile_info", data);
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
			if (svs_clients[i].header.state >= 1 && !game::SV_ClientIsBot(i) && !game::Session_IsHost(game::SV_MainMP_GetServerLobby(), i))
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
			if (svs_clients[i].header.state >= 1 && !game::SV_ClientIsBot(i) && game::Session_IsHost(game::SV_MainMP_GetServerLobby(), i))
			{
				auto self = load_profile_info();
				if (self.has_value())
				{
					send_profile_info(addr, steam::SteamUser()->GetSteamID().bits, self.value());
					break;
				}
			}
		}
	}

	void add_profile_info(const game::netadr_s& sender_addr, const std::uint64_t user_id, const profile_info& info)
	{
		if (user_id == steam::SteamUser()->GetSteamID().bits)
		{
			return;
		}

		if (game::Com_IsAnyLocalServerRunning()) // is_host()
		{
			send_all_profile_infos(sender_addr); // send all stored profile infos to the new player

			if (!game::environment::is_dedi())
			{
				send_self_profile(sender_addr); // send self profile info to the new player too
			}

			// send new player info to all clients
			send_profile_info_to_all_clients(user_id, info);
		}

		profile_mapping.access([&](profile_map& profiles)
		{
			profiles[user_id] = info;
		});
	}

	namespace
	{
		utils::hook::detour session_unregister_remote_player_hook;
		void session_unregister_remote_player_stub(game::SessionData* session, const int slot)
		{
			session_unregister_remote_player_hook.invoke<void>(session, slot);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			session_unregister_remote_player_hook.create(0x140C73970, session_unregister_remote_player_stub);

			//dvars::override::register_int("playercard_cache_validity_life", 5000, 0, 3600000, 0x0); // 5sec

			network::on("profileInfo", [](const game::netadr_s& client_addr, const std::string_view& data)
			{
				utils::byte_buffer buffer(data);
				std::string final_packet{};
				if (game::fragment_handler::handle(client_addr, buffer, final_packet))
				{
					buffer = utils::byte_buffer(final_packet);

					const auto user_id = buffer.read<std::uint64_t>();

					const profile_info info(buffer);
					if (info.m_memberplayer_card.empty())
					{
						// player has no card set
						return;
					}

					add_profile_info(client_addr, user_id, info);
					set_playercardcache_to_download(user_id);
				}
			});
		}
	};
}

REGISTER_COMPONENT(profile_infos::component)
