#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "dvars.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#include <intrin.h> // Include this header for intrinsic functions
#define __ROR4__(value, shift) _rotr(value, shift) // Define __ROR4__ using _rotr intrinsic
#define __ROL4__(value, shift) _rotl(value, shift) // Define __ROL4__ using _rotl intrinsic

namespace dvars
{
	namespace crypt
	{
		void hash_boolean(game::DvarValue* value) // Unused
		{
			int hashed = value->boolean_.enabled; // this is always 0 in the actual game
			int temp = 0;
			unsigned int key = 0x89E1A665;
			int xorKey = 0x87C6B47F;

			while (key != 0xE9CC2CC0)
			{
				switch (key)
				{
				case 0x89E1A665:
					temp = static_cast<unsigned short>(hashed);
					xorKey = 0x008B237C;
					break;
				case 0x1C9C36AA:
					temp = static_cast<unsigned short>(hashed) ^ 0x7434;
					xorKey = 0xAB74F2A4;
					hashed = __ROR4__(hashed, 16);
					break;
				case 0x6006EB8D:
					temp = static_cast<unsigned short>(hashed) ^ 0x9133;
					xorKey = 0xA6F5E979;
					hashed = temp ^ __ROL4__(hashed, 16);
					break;
				case 0x896A8519:
					temp ^= 0x52D0;
					xorKey = 0x6B8E2B15;
					hashed = __ROL4__(hashed, 16);
					break;
				case 0xC6F302F4:
					xorKey = 0x2F3F2E34;
					hashed = __ROR4__(hashed, 16);
					break;
				case 0xE2E4AE0C:
					hashed ^= temp;
					xorKey = 0x82E24581;
					break;
				case 0xF6B9AD6A:
					temp ^= 0xCA71;
					xorKey = 0x52DB7D55;
					break;
				}
				key ^= xorKey;
			}

			value->boolean_.hashedValue = hashed;
		}

		void hash_integer(game::DvarValue* value)
		{
			int hashed = value->integer_.integer;
			int temp = 0;
			int xorKey = 0x87C6B47F;
			unsigned int key = 0x89E1A665;

			while (key != 0xE9CC2CC0)
			{
				switch (key)
				{
				case 0x89E1A665:
					temp = static_cast<unsigned short>(hashed);
					xorKey = 0x008B237C;
					break;
				case 0x1C9C36AA:
					temp = static_cast<unsigned short>(hashed) ^ 0x7434;
					xorKey = 0xAB74F2A4;
					hashed = __ROR4__(hashed, 16);
					break;
				case 0x6006EB8D:
					temp = static_cast<unsigned short>(hashed) ^ 0x9133;
					xorKey = 0xA6F5E979;
					hashed = temp ^ __ROL4__(hashed, 16);
					break;
				case 0x896A8519:
					temp ^= 0x52D0;
					xorKey = 0x6B8E2B15;
					hashed = __ROL4__(hashed, 16);
					break;
				case 0xC6F302F4:
					xorKey = 0x2F3F2E34;
					hashed = __ROR4__(hashed, 16);
					break;
				case 0xE2E4AE0C:
					hashed ^= temp;
					xorKey = 0x82E24581;
					break;
				case 0xF6B9AD6A:
					temp ^= 0xCA71;
					xorKey = 0x52DB7D55;
					break;
				}
				key ^= xorKey;
			}

			value->integer_.hashedValue = hashed;
		}

		void hash_enum(game::DvarValue* value)
		{
			hash_integer(value);
		}

		void hash(game::DvarType type, game::DvarValue* value)
		{
			switch (type) 
			{
			case game::DvarType::boolean:
				hash_boolean(value);
				break;
			case game::DvarType::integer:
				hash_integer(value);
				break;
			case game::DvarType::enumeration:
				hash_enum(value);
				break;
			default:
				break;
			}
		}

		int unhash_integer(game::dvar_t* dvar)
		{
			int original = dvar->current.integer_.integer;
			if (original == 0)
				return 0;

			int result = dvar->current.integer_.hashedValue;
			int temp = 0;
			int xorKey = 0x040ABA5A;
			unsigned int key = 0x19B62CC6;

			while (key != 0x9E5E5A15)
			{
				switch (key) {
				case 0x19B62CC6:
					temp = static_cast<unsigned short>(result);
					xorKey = 0xC1BCD395;
					break;
				case 0x4AD465E7:
					temp ^= 0xCA71;
					xorKey = 0x05CA2DF8;
					break;
				case 0x8F9CFC5:
					xorKey = 0x0A2F246E;
					result = __ROL4__(temp ^ result, 16);
					break;
				case 0x6B2A7033:
					temp ^= 0x52D0;
					xorKey = 0xAF9030E3;
					break;
				case 0xC4BA40D0:
					result = __ROR4__(result, 16);
					xorKey = 0xCC438F15;
					break;
				case 0xD80AFF53:
					result = temp ^ __ROR4__(result, 16) ^ 0x9133;
					temp = static_cast<unsigned short>(result);
					xorKey = 0xB3208F60;
					break;
				case 0xEF93D298:
					temp = static_cast<unsigned short>(result) ^ 0x7434;
					xorKey = 0x09C60C3F;
					break;
				}
				key ^= xorKey;
			}

			// if (result != original) ExitProcess(0);
			return result;
		}
	}

	struct dvar_combined
	{
		std::string string{};
		game::DvarValue value{};
		game::DvarLimits limits{};
		unsigned int flags{};
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
		static std::unordered_map<uint32_t, std::unordered_map<std::string, dvar_combined>> register_overrides;

		void register_bool(const std::string& name, const bool value, const unsigned int flags)
		{
			dvar_combined values;
			values.value.enabled = value;
			values.flags = flags;
			register_overrides[game::DvarType::boolean][name] = std::move(values);
		}

		void register_float(const std::string& name, const float value, const float min, const float max,
			const unsigned int flags)
		{
			dvar_combined values;
			values.value.value = value;
			values.limits.value.min = min;
			values.limits.value.max = max;
			values.flags = flags;
			register_overrides[game::DvarType::value][name] = std::move(values);
		}

		void register_int(const std::string& name, const int value, const int min, const int max,
			const unsigned int flags)
		{
			dvar_combined values;
			values.value.integer = value;
			values.limits.integer.min = min;
			values.limits.integer.max = max;
			values.flags = flags;
			register_overrides[game::DvarType::integer][name] = std::move(values);
		}

		void register_string(const std::string& name, const std::string& value,
			const unsigned int flags)
		{
			dvar_combined values;
			values.string = value;
			values.flags = flags;
			register_overrides[game::DvarType::string][name] = std::move(values);
		}

		void register_vec2(const std::string& name, float x, float y, float min, float max,
			const unsigned int flags)
		{
			dvar_combined values;
			values.value.vector[0] = x;
			values.value.vector[1] = y;
			values.limits.vector.min = min;
			values.limits.vector.max = max;
			values.flags = flags;
			register_overrides[game::DvarType::vec2][name] = std::move(values);
		}

		void register_vec3(const std::string& name, float x, float y, float z, float min,
			float max, const unsigned int flags)
		{
			dvar_combined values;
			values.value.vector[0] = x;
			values.value.vector[1] = y;
			values.value.vector[2] = z;
			values.limits.vector.min = min;
			values.limits.vector.max = max;
			values.flags = flags;
			register_overrides[game::DvarType::vec3][name] = std::move(values);
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

	void inline dvar_register_bool(const char* name, bool& value, unsigned int& flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::boolean], name);
		if (var)
		{
			value = var->value.enabled;
			flags = var->flags;
		}
	}

	void inline dvar_register_float(const char* name, float& value, float& min, float& max, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::value], name);
		if (var)
		{
			value = var->value.value;
			min = var->limits.value.min;
			max = var->limits.value.max;
			flags = var->flags;
		}
	}

	void inline dvar_register_int(const char* name, int& value, int& min, int& max, unsigned int& flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::integer], name);
		if (var)
		{
			value = var->value.integer;
			min = var->limits.integer.min;
			max = var->limits.integer.max;
			flags = var->flags;
		}
	}

	void inline dvar_register_string(const char* name, const char* value, unsigned int& flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::string], name);
		if (var)
		{
			value = var->string.data();
			flags = var->flags;
		}
	}

	void inline dvar_register_vector2(const char* name, float& x, float& y, float& min, float& max,
		unsigned int& flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::vec2], name);
		if (var)
		{
			x = var->value.vector[0];
			y = var->value.vector[1];
			min = var->limits.vector.min;
			max = var->limits.vector.max;
			flags = var->flags;
		}
	}

	void inline dvar_register_vector3(const char* name, float& x, float& y, float& z, float& min,
		float& max, unsigned int& flags, const char* description)
	{
		auto* var = find_dvar(override::register_overrides[game::DvarType::vec3], name);
		if (var)
		{
			x = var->value.vector[0];
			y = var->value.vector[1];
			z = var->value.vector[2];
			min = var->limits.vector.min;
			max = var->limits.vector.max;
			flags = var->flags;
		}
	}

	void inline check_dvar_overrides(const char* name, game::DvarType& type, unsigned int& flags, game::DvarValue* value, game::DvarLimits* domain, const char* description)
	{
		switch (type)
		{
		case game::DvarType::boolean:
			dvar_register_bool(name, value->enabled, flags, description);
			break;
		case game::DvarType::value:
			dvar_register_float(name, value->value, domain->value.min, domain->value.max, flags, description);
			break;
		case game::DvarType::integer:
			dvar_register_int(name, value->integer, domain->integer.min, domain->integer.max, flags, description);
			break;
		case game::DvarType::string:
			dvar_register_string(name, value->string, flags, description);
			break;
		case game::DvarType::vec2:
			dvar_register_vector2(name, value->vector[0], value->vector[1], domain->vector.min, domain->vector.max, flags, description);
			break;
		case game::DvarType::vec3:
			dvar_register_vector3(name, value->vector[0], value->vector[1], value->vector[2], domain->vector.min, domain->vector.max, flags, description);
			break;
		default:
			break;
		}

		dvars::crypt::hash(type, value);
	}

	utils::hook::detour dvar_register_new_hook;
	utils::hook::detour dvar_re_register_hook;
	utils::hook::detour dvar_de_register_hook;
	utils::hook::detour dvar_register_variant_hook;

	game::dvar_t* dvar_register_new(const char* name, unsigned int checksum, game::DvarType type, unsigned int flags,
		game::DvarValue* value, game::DvarLimits* domain, char level, const char* description)
	{
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

	inline void dvar_re_register(game::dvar_t* dvar, const char* name, game::DvarType type, unsigned int flags,
		game::DvarValue* resetValue, game::DvarLimits* domain, char level, const char* description)
	{
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
		check_dvar_overrides(name, type, flags, value, domain, description);

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

	game::dvar_t* fps_labels_register_stub(const char*, bool, unsigned int, const char*)
	{
		return game::Dvar_RegisterBool("cg_drawFPS", false, game::DVAR_FLAG_READ, "cg_drawFPS is deprecated, use cg_fpsCounter instead");
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvar_register_new_hook.create(0x140CEBA60, dvar_register_new);
			dvar_re_register_hook.create(0x140CEC210, dvar_re_register);
			dvar_de_register_hook.create(0x140CE9F30, dvar_de_register);
			dvar_register_variant_hook.create(0x140CEBDD0, dvar_register_variant);

			// We need to apply these straight away
			MH_ApplyQueued();

			// de-register cg_drawFPSLabels and register a placeholder cg_drawFps
			utils::hook::call(0x140BAEAE9, fps_labels_register_stub);
		}

		component_priority priority() override
		{
			return component_priority::dvars;
		}
	};
}

REGISTER_COMPONENT(dvars::component)