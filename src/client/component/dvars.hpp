#pragma once

namespace dvars
{
	namespace disable
	{
		void re_register(const std::string& name);
		void de_register(const std::string& name);
	}

	namespace override
	{
		void register_bool(const std::string& name, bool value, const unsigned int flags);
		void register_float(const std::string& name, float value, float min, float max, const unsigned int flags);
		void register_int(const std::string& name, int value, int min, int max, const unsigned int flags);
		void register_string(const std::string& name, const std::string& value, const unsigned int flags);
		void register_vec2(const std::string& name, float x, float y, float min, float max, const unsigned int flags);
		void register_vec3(const std::string& name, float x, float y, float z, float min, float max, const unsigned int flags);
	}

	namespace callback
	{
		void on_register(const std::string& name, const std::function<void()>& callback);
		void on_re_register(const std::string& name, const std::function<void()>& callback);
		void on_de_register(const std::string& name, const std::function<void()>& callback);
	}
}