#pragma once
#include "game/game.hpp"

namespace party
{
	struct connection_state
	{
		game::netadr_s host;
		std::string challenge;
		bool hostDefined;
		std::string motd;
		int max_clients;
		std::string base_url;
	};

	void info_response_error(const std::string& error);

	void connect(const game::netadr_s& target);
	void start_map(const std::string& mapname, bool dev = false);

	connection_state get_server_connection_state();

	int get_client_num_by_name(const std::string& name);

	int get_client_count();
	int get_bot_count();
}