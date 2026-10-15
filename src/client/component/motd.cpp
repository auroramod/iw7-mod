#include <std_include.hpp>

#include "game/game.hpp"
#include "console/console.hpp"
#include "loader/component_loader.hpp"

#include "motd.hpp"
#include "scheduler.hpp"

#include <utils/http.hpp>
#include <utils/json.hpp>

#define CRM_DATA_URL "https://raw.githubusercontent.com/auroramod/website/publish/data/iw7/crm.json"

namespace motd
{
	namespace
	{
		std::mutex mutex;
		messages_t messages;

		messages_t parse(const std::string& data)
		{
			messages_t result;

			const auto json = nlohmann::json::parse(data, nullptr, false);
			if (!json.is_object())
			{
				console::error("[CRM] invalid crm.json\n");
				return result;
			}

			for (const auto& [location, value] : json.items())
			{
				auto& list = result[location];

				if (value.is_object())
				{
					list.emplace_back(value.dump());
				}
				else if (value.is_array())
				{
					for (const auto& entry : value)
					{
						if (entry.is_object())
						{
							list.emplace_back(entry.dump());
						}
					}
				}
			}

			return result;
		}

		void fetch()
		{
			const auto result = utils::http::get_data(CRM_DATA_URL, {}, {}, {});
			if (!result.has_value())
			{
				console::error("[CRM] failed to download crm.json\n");
				return;
			}

			auto parsed = parse(result->buffer);

			std::lock_guard<std::mutex> _(mutex);
			messages = std::move(parsed);
		}
	}

	messages_t get_messages()
	{
		std::lock_guard<std::mutex> _(mutex);
		return messages;
	}

	class component final : public component_interface
	{
	public:
		void post_load() override
		{
			if (game::environment::is_dedi())
			{
				return;
			}

			scheduler::once(fetch, scheduler::async);
		}
	};
}

REGISTER_COMPONENT(motd::component)