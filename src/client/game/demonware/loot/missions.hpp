#pragma once

namespace demonware
{
	namespace loot
	{
		namespace missions
		{
			struct Reward
			{
				std::vector<std::uint32_t> packs; // ids shown on the AAR screen
				std::vector<std::uint32_t> items; // ids whose inventory balance changed
				std::map<std::uint32_t, std::uint32_t> currencies; // currency id, amount earned
			};

			enum loot_quality : std::uint32_t
			{
				common = 1,
				rare = 2,
				legendary = 3,
				epic = 4,
			};

			struct loot_table
			{
				const char* file;
				int quality;
				int category;
				int salvageReturned;
				int cost;
			};

			enum class pick_type
			{
				weapon,
				rig,
				emote,
				camo,
				calling_card,
				accessory,
				cosmetic,
				item, // fixed id (supply drops, currency packs)
			};

			struct Pick
			{
				pick_type type;
				std::uint32_t count;
				std::uint32_t min_quality = common;
				std::uint32_t max_quality = epic;
				std::vector<std::string> categories{}; // empty is any
				bool mk2 = false;
				bool qm_collection = false;
				std::uint32_t id = 0; // pick_type::item only
			};

			std::uint32_t get_match_keys(std::int32_t time_played, std::int32_t mission_result);
			std::uint32_t get_match_salvage(std::int32_t time_played, std::int32_t mission_result);

			Reward give_mission_team_reward(std::uint32_t mission_team);
			Reward give_contract_reward(std::uint32_t mission_set_id);
		}
	}
}
