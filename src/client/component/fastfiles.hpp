#pragma once

namespace fastfiles
{
	std::string get_current_fastfile();
	bool exists(const std::string& zone, bool ignore_usermap = false);

	void set_usermap(const std::string& usermap);
	void clear_usermap();
	std::optional<std::string> get_current_usermap();
	bool usermap_exists(const std::string& name);
	bool is_stock_map(const std::string& name);
}
