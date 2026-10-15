#pragma once

namespace motd
{
	using messages_t = std::map<std::string, std::vector<std::string>>;

	messages_t get_messages();
}