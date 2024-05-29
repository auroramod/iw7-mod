#pragma once

namespace gsc
{
	void scr_error(const char* error, const bool force_print = false);

	std::optional<std::pair<std::string, std::string>> find_function(const char* pos);
}