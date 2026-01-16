#pragma once

#include <game/game.hpp>
#include <utils/byte_buffer.hpp>

namespace profile_infos
{
	struct profile_info
	{
		std::string m_memberplayer_card{};

		profile_info() = default;
		profile_info(utils::byte_buffer& buffer);
		void serialize(utils::byte_buffer& buffer) const;
	};

	void clear_profile_infos();

	void send_profile_info(const game::netadr_s& address, const std::uint64_t user_id, const profile_info& info);

	std::optional<profile_info> get_profile_info();
	std::optional<profile_info> get_profile_info(const uint64_t user_id);

	void update_profile_info(const profile_info& info);
}
