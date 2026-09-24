#pragma once

namespace autocomplete
{
	enum class match_type
	{
		command,
		dvar,
		argument,
	};

	struct match
	{
		std::string name;
		std::string description;
		match_type type{};
	};

	struct result
	{
		std::string head;
		std::string token;
		std::string command;
		std::size_t arg_index{};
		std::vector<match> matches;
		int selected = -1;
		bool show_list = true;
	};

	result query(const std::string& input);

	std::string get_ghost_text(const result& result);

	std::string complete(const std::string& input, bool reverse = false);

	std::string prepare_command(const std::string& input);

	std::uint32_t get_generation();

	void add_history(const std::string& input);
	const std::deque<std::string>& get_history();
	void clear_history();
}
