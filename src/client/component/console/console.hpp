#pragma once

namespace console
{
	void set_title(const std::string& title);

	enum print_type
	{
		print_type_error = 1,
		print_type_debug = 2,
		print_type_warning = 3,
		print_type_info = 7
	};

	void dispatch_message(int type, const std::string& message);

	void print(int type, const char* fmt, ...);

	template <typename... Args>
	void error(const char* fmt, Args&&... args)
	{
		print(print_type_error, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void debug(const char* fmt, Args&&... args)
	{
#ifdef DEBUG
		print(print_type_debug, fmt, std::forward<Args>(args)...);
#endif
	}

	template <typename... Args>
	void warn(const char* fmt, Args&&... args)
	{
		print(print_type_warning, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void info(const char* fmt, Args&&... args)
	{
		print(print_type_info, fmt, std::forward<Args>(args)...);
	}

	void init();
}