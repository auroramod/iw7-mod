#pragma once
#include <utils/concurrency.hpp>

namespace scripting
{
	extern std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;
	extern std::unordered_map<std::string, std::unordered_map<std::string, const char*>> script_function_table;
	extern std::unordered_map<std::string, std::vector<std::pair<std::string, const char*>>> script_function_table_sort;
	extern std::unordered_map<const char*, std::pair<std::string, std::string>> script_function_table_rev;

	extern std::string current_file;

	void on_shutdown(const std::function<void(bool, bool)>& callback);
	std::optional<std::string> get_canonical_string(const unsigned int id);
	std::string get_token(unsigned int id);
}