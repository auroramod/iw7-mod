#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace dvars
{
	struct dvar_base
	{
		unsigned int flags{};
	};

	struct dvar_bool : dvar_base
	{
		bool value{};
	};

	struct dvar_float : dvar_base
	{
		float value{};
		float min{};
		float max{};
	};

	struct dvar_vector2 : dvar_base
	{
		float x{};
		float y{};
		float min{};
		float max{};
	};

	struct dvar_vector3 : dvar_base
	{
		float x{};
		float y{};
		float z{};
		float min{};
		float max{};
	};

	struct dvar_int : dvar_base
	{
		int value{};
		int min{};
		int max{};
	};

	struct dvar_string : dvar_base
	{
		std::string value{};
	};

	namespace
	{
		template <typename T>
		T* find_dvar(std::unordered_map<std::string, T>& map, const std::string& name)
		{
			auto i = map.find(name);
			if (i != map.end())
			{
				return &i->second;
			}

			return nullptr;
		}

		bool find_dvar(std::unordered_set<std::string>& set, const std::string& name)
		{
			return set.find(name) != set.end();
		}
	}

	namespace disable
	{
		static std::unordered_set<std::string> re_register_disables;
		static std::unordered_set<std::string> de_register_disables;

		void re_register(const std::string& name)
		{
			re_register_disables.emplace(name);
		}

		void de_register(const std::string& name)
		{
			de_register_disables.emplace(name);
		}
	}

	namespace override
	{
		static std::unordered_map<std::string, dvar_bool> register_bool_overrides;
		static std::unordered_map<std::string, dvar_float> register_float_overrides;
		static std::unordered_map<std::string, dvar_int> register_int_overrides;
		static std::unordered_map<std::string, dvar_string> register_string_overrides;
		static std::unordered_map<std::string, dvar_vector2> register_vector2_overrides;
		static std::unordered_map<std::string, dvar_vector3> register_vector3_overrides;

		void register_bool(const std::string& name, const bool value, const unsigned int flags)
		{
			dvar_bool values;
			values.value = value;
			values.flags = flags;
			register_bool_overrides[name] = std::move(values);
		}

		void register_float(const std::string& name, const float value, const float min, const float max,
			const unsigned int flags)
		{
			dvar_float values;
			values.value = value;
			values.min = min;
			values.max = max;
			values.flags = flags;
			register_float_overrides[name] = std::move(values);
		}

		void register_int(const std::string& name, const int value, const int min, const int max,
			const unsigned int flags)
		{
			dvar_int values;
			values.value = value;
			values.min = min;
			values.max = max;
			values.flags = flags;
			register_int_overrides[name] = std::move(values);
		}

		void register_string(const std::string& name, const std::string& value,
			const unsigned int flags)
		{
			dvar_string values;
			values.value = value;
			values.flags = flags;
			register_string_overrides[name] = std::move(values);
		}

		void register_vec2(const std::string& name, float x, float y, float min, float max,
			const unsigned int flags)
		{
			dvar_vector2 values;
			values.x = x;
			values.y = y;
			values.min = min;
			values.max = max;
			values.flags = flags;
			register_vector2_overrides[name] = std::move(values);
		}

		void register_vec3(const std::string& name, float x, float y, float z, float min,
			float max, const unsigned int flags)
		{
			dvar_vector3 values;
			values.x = x;
			values.y = y;
			values.z = z;
			values.min = min;
			values.max = max;
			values.flags = flags;
			register_vector3_overrides[name] = std::move(values);
		}
	}

	namespace callback
	{
		std::unordered_map<std::string, std::function<void()>> dvar_on_register_function_map;
		std::unordered_map<std::string, std::function<void()>> dvar_on_re_register_function_map;
		std::unordered_map<std::string, std::function<void()>> dvar_on_de_register_function_map;

		void on_register(const std::string& name, const std::function<void()>& callback)
		{
			dvar_on_register_function_map[name] = callback;
		}

		void on_re_register(const std::string& name, const std::function<void()>& callback)
		{
			dvar_on_re_register_function_map[name] = callback;
		}

		void on_de_register(const std::string& name, const std::function<void()>& callback)
		{
			dvar_on_de_register_function_map[name] = callback;
		}
	}

	utils::hook::detour dvar_register_bool_hook;
	utils::hook::detour dvar_register_float_hook;
	utils::hook::detour dvar_register_int_hook;
	utils::hook::detour dvar_register_string_hook;
	utils::hook::detour dvar_register_vector2_hook;
	utils::hook::detour dvar_register_vector3_hook;

	game::dvar_t* dvar_register_bool(const char* name, bool value, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_bool_overrides, name);
		if (var)
		{
			value = var->value;
			flags = var->flags;
		}

		return dvar_register_bool_hook.invoke<game::dvar_t*>(name, value, flags, description);
	}

	game::dvar_t* dvar_register_float(const char* name, float value, float min, float max, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_float_overrides, name);
		if (var)
		{
			value = var->value;
			min = var->min;
			max = var->max;
			flags = var->flags;
		}

		return dvar_register_float_hook.invoke<game::dvar_t*>(name, value, min, max, flags, description);
	}

	game::dvar_t* dvar_register_int(const char* name, int value, int min, int max, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_int_overrides, name);
		if (var)
		{
			value = var->value;
			min = var->min;
			max = var->max;
			flags = var->flags;
		}

		return dvar_register_int_hook.invoke<game::dvar_t*>(name, value, min, max, flags, description);
	}

	game::dvar_t* dvar_register_string(const char* name, const char* value, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_string_overrides, name);
		if (var)
		{
			value = var->value.data();
			flags = var->flags;
		}

		return dvar_register_string_hook.invoke<game::dvar_t*>(name, value, flags, description);
	}

	game::dvar_t* dvar_register_vector2(const char* name, float x, float y, float min, float max,
		unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_vector2_overrides, name);
		if (var)
		{
			x = var->x;
			y = var->y;
			min = var->min;
			max = var->max;
			flags = var->flags;
		}

		return dvar_register_vector2_hook.invoke<game::dvar_t*>(name, x, y, min, max, flags, description);
	}

	game::dvar_t* dvar_register_vector3(const char* name, float x, float y, float z, float min,
		float max, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_vector3_overrides, name);
		if (var)
		{
			x = var->x;
			y = var->y;
			z = var->z;
			min = var->min;
			max = var->max;
			flags = var->flags;
		}

		return dvar_register_vector3_hook.invoke<game::dvar_t*>(name, x, y, z, min, max, flags, description);
	}

	std::recursive_mutex _mutex;

	utils::hook::detour dvar_register_new_hook;
	utils::hook::detour dvar_re_register_hook;
	utils::hook::detour dvar_de_register_hook;
	utils::hook::detour dvar_register_variant_hook;

	game::dvar_t* dvar_register_new(const char* name, unsigned int checksum, game::DvarType type, unsigned int flags,
		game::DvarValue* value, game::DvarLimits* domain, char level, const char* description)
	{
		std::lock_guard<std::recursive_mutex> $(_mutex);
		auto* dvar = dvar_register_new_hook.invoke<game::dvar_t*>(name, checksum, type, flags, value, domain, level, description);

		if (dvar && name)
		{
			auto* callback = find_dvar(callback::dvar_on_register_function_map, name);
			if (callback)
			{
				(*callback)();
			}
		}

		return dvar;
	}

	void dvar_re_register(game::dvar_t* dvar, const char* name, game::DvarType type, unsigned int flags,
		game::DvarValue* resetValue, game::DvarLimits* domain, char level, const char* description)
	{
		std::lock_guard<std::recursive_mutex> $(_mutex);

		if (dvar && name)
		{
			const auto disabled = find_dvar(disable::re_register_disables, name);
			if (disabled)
			{
				return;
			}

			auto* callback = find_dvar(callback::dvar_on_re_register_function_map, name);
			if (callback)
			{
				(*callback)();
			}
		}

		return dvar_re_register_hook.invoke<void>(dvar, name, type, flags, resetValue, domain, level, description);
	}

	game::dvar_t* dvar_de_register(game::dvar_t* dvar)
	{
		std::lock_guard<std::recursive_mutex> $(_mutex);

		auto name = dvars::dvar_get_name(dvar);
		if (dvar && !name.empty())
		{
			const auto disabled = find_dvar(disable::de_register_disables, name);
			if (disabled)
			{
				return dvar;
			}

			auto* callback = find_dvar(callback::dvar_on_de_register_function_map, name);
			if (callback)
			{
				(*callback)();
			}
		}

		return dvar_de_register_hook.invoke<game::dvar_t*>(dvar);
	}

	game::dvar_t* dvar_register_variant(const char* name, unsigned int checksum, game::DvarType type, unsigned int flags,
		game::DvarValue* value, game::DvarLimits* domain, const char* description)
	{
		std::lock_guard<std::recursive_mutex> $(_mutex);
		auto* dvar = dvar_register_variant_hook.invoke<game::dvar_t*>(name, checksum, type, flags, value, domain, description);

		if (dvar)
		{
			if (name && !utils::string::is_numeric(name))
			{
				dvars::dvar_set_name(dvar, name);
			}
			if (description)
			{
				dvars::dvar_set_description(dvar, description);
			}
		}

		return dvar;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvar_register_bool_hook.create(0x140CEB380, &dvar_register_bool);
			dvar_register_float_hook.create(0x140CEB890, &dvar_register_float);
			dvar_register_int_hook.create(0x140CEB920, &dvar_register_int);
			dvar_register_string_hook.create(0x140CEBD50, &dvar_register_string);
			dvar_register_vector2_hook.create(0x140CEBF50, &dvar_register_vector2);
			dvar_register_vector3_hook.create(0x140CEBFE0, &dvar_register_vector3);

			dvar_register_new_hook.create(0x140CEBA60, dvar_register_new);
			dvar_re_register_hook.create(0x140CEC210, dvar_re_register);
			dvar_de_register_hook.create(0x140CE9F30, dvar_de_register);
			dvar_register_variant_hook.create(0x140CEBDD0, dvar_register_variant);
		}

		component_priority priority() override
		{
			return component_priority::dvars;
		}
	};
}

REGISTER_COMPONENT(dvars::component)