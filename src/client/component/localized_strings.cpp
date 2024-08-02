#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "localized_strings.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>

namespace localized_strings
{
	namespace
	{
		utils::hook::detour seh_string_ed_get_string_hook;

		using localized_map = std::unordered_map<std::string, std::string>;
		utils::concurrency::container<localized_map> localized_overrides;

		const char* seh_string_ed_get_string(const char* reference)
		{
			const auto* str = localized_overrides.access<const char*>([&](const localized_map& map)
			{
				const auto entry = map.find(reference);
				if (entry != map.end())
				{
					return entry->second.data();
				}
				return static_cast<const char*>(nullptr);
			});
			if (str != nullptr)
			{
				return str;
			}
			return seh_string_ed_get_string_hook.invoke<const char*>(reference);
		}
	}

	void override(const std::string& key, const std::string& value)
	{
		localized_overrides.access([&](localized_map& map)
		{
			map[key] = value;
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Change some localized strings
			seh_string_ed_get_string_hook.create(0x140CBBB10, &seh_string_ed_get_string);
		}
	};
}

REGISTER_COMPONENT(localized_strings::component)