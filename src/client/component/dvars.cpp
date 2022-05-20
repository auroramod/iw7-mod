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
	game::dvar_t* dvar_register_new_internal(const char* name, int hash, game::dvar_type type, unsigned int flags,
		game::dvar_value* value, game::dvar_limits* domain, bool level, const char* description)
	{
		auto* dvar = dvar_register_new_hook.invoke<game::dvar_t*>(name, hash, type, flags, value, domain, level, description);

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

		if (dvar && name && dvar_on_register_function_map.find(name) != dvar_on_register_function_map.end())
		{
			dvar_on_register_function_map[name]();
			dvar_on_register_function_map.erase(name);
		}

		return dvar;
	}

	utils::hook::detour dvar_reregister_hook;
	void dvar_reregister(game::dvar_t* dvar, const char* name, game::dvar_type type, unsigned int flags,
		game::dvar_value* value, game::dvar_limits* domain, bool level, const char* description)
	{
		dvar_reregister_hook.invoke<game::dvar_t*>(dvar, name, type, flags, value, domain, level, description);

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
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// the post_unpack happens too late for some dvars...
			dvar_register_new_hook.create(0xCEBA60_b, dvar_register_new_internal);
			dvar_reregister_hook.create(0xCEC210_b, dvar_reregister);
		}
	};
}

REGISTER_COMPONENT(dvars::component)