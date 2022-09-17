#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

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

	}

	namespace override
	{

	}

	std::unordered_map<std::string, std::function<void()>> dvar_on_register_function_map;
	void on_register(const std::string& name, const std::function<void()>& callback)
	{
		dvar_on_register_function_map[name] = callback;
	}

	utils::hook::detour dvar_register_new_hook;
	game::dvar_t* dvar_register_new(const char* name, unsigned int checksum, game::DvarType type, unsigned int flags,
		game::DvarValue* value, game::DvarLimits* domain, char level, const char* description)
	{
		auto* dvar = dvar_register_new_hook.invoke<game::dvar_t*>(name, checksum, type, flags, value, domain, level, description);

		if (dvar && name && dvar_on_register_function_map.find(name) != dvar_on_register_function_map.end())
		{
			dvar_on_register_function_map[name]();
			dvar_on_register_function_map.erase(name);
		}

		return dvar;
	}

	std::recursive_mutex register_var_lock;
	utils::hook::detour dvar_register_variant_hook;
	game::dvar_t* dvar_register_variant(const char* name, unsigned int checksum, game::DvarType type, unsigned int flags,
		game::DvarValue* value, game::DvarLimits* domain, const char* description)
	{
		std::lock_guard<std::recursive_mutex> $(register_var_lock);
		auto* dvar = dvar_register_variant_hook.invoke<game::dvar_t*>(name, checksum, type, flags, value, domain, description);

		if (dvar)
		{
			if (name)
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
			dvar_register_new_hook.create(0xCEBA60_b, dvar_register_new);
			dvar_register_variant_hook.create(0xCEBDD0_b, dvar_register_variant);
		}

		int priority() override
		{
			return COMPONENT_MAX_PRIORITY - 1;
		}
	};
}

REGISTER_COMPONENT(dvars::component)