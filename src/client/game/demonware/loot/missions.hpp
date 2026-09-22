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
				std::map<std::uint32_t, std::uint32_t> currencies; // currency id -> amount earned
			};

			// keys (in raw units) earned for finishing a match
			std::uint32_t get_match_keys(std::int32_t time_played, std::int32_t mission_result);

			// salvage earned for finishing a match
			std::uint32_t get_match_salvage(std::int32_t time_played, std::int32_t mission_result);

			// next unowned level reward from mp/loot/iw7_mission_loot_master.csv
			Reward give_mission_team_reward(std::uint32_t mission_team);

			// contract reward from mp/loot/iw7_loot_mission_set_master.csv
			Reward give_contract_reward(std::uint32_t mission_set_id);
		}
	}
}
