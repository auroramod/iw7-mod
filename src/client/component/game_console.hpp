#pragma once

namespace game_console
{
	using output_queue = std::deque<std::string>;

	bool console_char_event(int local_client_num, int key);
	bool console_key_event(int local_client_num, int key, int down);

	bool is_new_console_enabled();
	bool is_console_open();
	void toggle_console();

	void execute(const std::string& input);

	void access_output(const std::function<void(const output_queue&)>& callback);
	void clear_output();
}
