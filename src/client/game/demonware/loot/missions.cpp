#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

#include "component/console/console.hpp"

#include "loot.hpp"
#include "missions.hpp"

#define TRUE_KEY_AMOUNT(num) num * 100

namespace demonware
{
	namespace loot
	{
		namespace missions
		{
			namespace
			{
				constexpr std::uint32_t match_base_keys 		= 8;
				constexpr std::uint32_t match_victory_keys 		= 4;
				constexpr std::uint32_t match_base_salvage 		= 200;
				constexpr std::uint32_t match_victory_salvage 	= 200;
				constexpr std::int32_t match_min_time_played 	= 60; // seconds

				const loot_table weapons{ "mp/loot/iw7_weapon_loot_master.csv", 6, 7, 3, 4 };
				const loot_table rigs{ "mp/loot/iw7_cosmetic_rigs_loot_master.csv", 2, 7, 4, 5 };
				const loot_table emotes{ "mp/loot/iw7_cosmetic_emotes_loot_master.csv", 2, 3, 4, 5 };
				const loot_table camos{ "mp/loot/iw7_cosmetic_camos_loot_master.csv", 2, 7, 4, 5 };
				const loot_table calling_cards{ "mp/loot/iw7_cosmetic_calling_cards_loot_master.csv", 2, 7, 4, 5 };
				const loot_table accessories{ "mp/loot/iw7_cosmetic_attachments_loot_master.csv", 2, 7, 4, 5 };
				const loot_table emblems{ "mp/loot/iw7_cosmetic_emblems_loot_master.csv", 2, 7, 4, 5 };
				const loot_table reticles{ "mp/loot/iw7_cosmetic_reticles_loot_master.csv", 2, 7, 4, 5 };

				constexpr int weapon_mk2_column 		= 8; // Y/N
				constexpr int weapon_collection_column 	= 10; // RnG = QR-M5TR collection

				// MP_CONTRACTS_CRATE_0NN, contents from MP_CONTRACTS_CRATE_0NN_DESC
				const std::unordered_map<std::uint32_t, std::vector<Pick>> contract_crates =
				{
					{ 1, { { pick_type::weapon, 1, rare, epic, { "Pistol" } } } },
					{ 2, { { pick_type::weapon, 1, legendary, legendary, { "Sniper" } } } },
					{ 3, { { pick_type::weapon, 1, legendary, legendary }, { pick_type::accessory, 1, epic, epic } } },
					{ 4, { { pick_type::weapon, 1, legendary, legendary, { "LMG" }, true } } },
					{ 5, { { pick_type::weapon, 1, legendary, legendary, { "SMG" }, true } } },
					{ 6, { { pick_type::weapon, 2, legendary, legendary } } },
					{ 7, { { pick_type::weapon, 1, rare, legendary, {}, false, true } } },
					{ 8, { { pick_type::weapon, 1, epic, epic } } },
					{ 9, { { pick_type::rig, 1, common, epic, { "Warfighter" } } } },
					{ 10, { { pick_type::rig, 1, common, epic, { "Merc" } } } },
					{ 11, { { pick_type::rig, 1, common, epic, { "Synaptic" } } } },
					{ 12, { { pick_type::rig, 1, common, epic, { "FTL" } } } },
					{ 13, { { pick_type::rig, 1, common, epic, { "Stryker" } } } },
					{ 14, { { pick_type::rig, 1, common, epic, { "Phantom" } } } },
					{ 15, { { pick_type::accessory, 1, legendary, epic } } },
					{ 16, { { pick_type::accessory, 1, epic, epic } } },
					{ 17, { { pick_type::emote, 1, common, epic, { "gesture", "taunt" } } } },
					{ 18, { { pick_type::emote, 1, epic, epic, { "gesture", "taunt" } } } },
					{ 19, { { pick_type::emote, 1, rare, epic, { "gesture" } } } },
					{ 20, { { pick_type::emote, 1, rare, epic, { "taunt" } } } },
					{ 21, { { pick_type::camo, 2 } } },
					{ 22, { { pick_type::camo, 1, legendary, legendary } } },
					{ 23, { { pick_type::camo, 1, legendary, legendary, { "SMG" } } } },
					{ 24, { { pick_type::camo, 1, legendary, legendary, { "AssaultRifle" } } } },
					{ 25, { { pick_type::camo, 2, legendary, legendary } } },
					{ 26, { { pick_type::camo, 3, rare, rare } } },
					{ 27, { { pick_type::camo, 1, epic, epic, { "LMG" } } } },
					{ 28, { { pick_type::calling_card, 1, epic, epic } } },
					{ 29, { { pick_type::calling_card, 1, legendary, epic } } },
					{ 30, { { pick_type::cosmetic, 3 } } },
					{ 31, { { pick_type::item, 2, common, epic, {}, false, false, LOOT_RARE_CRATE } } },
					{ 32, { { pick_type::item, 3, common, epic, {}, false, false, LOOT_RARE_CRATE } } },
					{ 33, { { pick_type::item, 2, common, epic, {}, false, false, LOOT_RARE_CRATE } } },
					{ 34, { { pick_type::item, 3, common, epic, {}, false, false, LOOT_RARE_CRATE } } },
					{ 35, { { pick_type::item, 1, common, epic, {}, false, false, 80431 } } }, // Keys_Contract_01
					{ 36, { { pick_type::item, 1, common, epic, {}, false, false, 80432 } } }, // Keys_Contract_02
					{ 37, { { pick_type::item, 1, common, epic, {}, false, false, 80433 } } }, // Keys_Contract_03
					{ 38, { { pick_type::item, 1, common, epic, {}, false, false, 80427 } } }, // Salv_Contract_01
					{ 39, { { pick_type::item, 1, common, epic, {}, false, false, 80428 } } }, // Salv_Contract_02
					{ 40, { { pick_type::item, 1, common, epic, {}, false, false, 80429 } } }, // Salv_Contract_03
					{ 41, { { pick_type::item, 1, common, epic, {}, false, false, 80430 } } }, // Salv_Contract_04
				};

				// zombies contracts 160-164 and 260-264 (cp/loot/iw7_zombie_loot_contract_set.csv) -> ZContracts_* currency packs
				// 30 keys, 60 keys, 90 keys, 250 salvage, 500 salvage
				std::optional<std::uint32_t> get_zombie_contract_pack(const std::uint32_t mission_set_id)
				{
					const auto index = mission_set_id % 100;
					if ((mission_set_id / 100 != 1 && mission_set_id / 100 != 2) || index < 60 || index > 64)
					{
						return {};
					}

					return 80438 + (index - 60);
				}

				const game::StringTable* get_table(const char* file)
				{
					const auto table = game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, file, 0).stringTable;
					if (!table)
					{
						console::error("[DW]: missing stringtable %s\n", file);
					}

					return table;
				}

				std::string get_value(const game::StringTable* table, const int row, const int column)
				{
					const auto value = game::StringTable_GetColumnValueForRow(table, row, column);
					return value ? value : "";
				}

				std::uint32_t get_uint(const game::StringTable* table, const int row, const int column)
				{
					return static_cast<std::uint32_t>(std::strtoul(get_value(table, row, column).data(), nullptr, 10));
				}

				std::optional<std::pair<std::uint32_t, std::uint32_t>> get_currency_pack(const std::uint32_t id)
				{
					if (id < 80000 || id > 89999)
					{
						return {};
					}

					const auto table = get_table("mp/loot/iw7_currency_pack_loot_master.csv");
					if (!table)
					{
						return {};
					}

					for (auto row = 0; row < table->rowCount; row++)
					{
						if (get_uint(table, row, 0) == id)
						{
							// currencyId, amount (keys are x100)
							return std::make_pair(get_uint(table, row, 4), get_uint(table, row, 5));
						}
					}

					return {};
				}

				void grant_item(Reward& reward, const std::uint32_t id)
				{
					const auto balance = get_item_balance(id) + 1;
					set_item_balance(id, balance);

					if (const auto pack = get_currency_pack(id))
					{
						const auto [currency_id, amount] = *pack;
						set_currency_balance(currency_id, get_currency_balance(currency_id) + amount);
						reward.currencies[currency_id] += amount;
					}

					reward.packs.push_back(id);
					if (std::find(reward.items.begin(), reward.items.end(), id) == reward.items.end())
					{
						reward.items.push_back(id);
					}

					console::demonware("[DW]: + giving mission loot %d\n", id);
				}

				bool matches_category(const std::string& value, const std::vector<std::string>& categories)
				{
					if (categories.empty())
					{
						return true;
					}

					return std::any_of(categories.begin(), categories.end(), [&](const std::string& category)
					{
						return value.starts_with(category);
					});
				}

				void collect_candidates(const loot_table& loot_table, const Pick& pick, std::vector<std::uint32_t>& candidates)
				{
					const auto table = get_table(loot_table.file);
					if (!table)
					{
						return;
					}

					for (auto row = 0; row < table->rowCount; row++)
					{
						const auto quality = get_uint(table, row, loot_table.quality);
						if (quality < pick.min_quality || quality > pick.max_quality)
						{
							continue;
						}

						// reward items, not from hacks
						if (get_uint(table, row, loot_table.salvageReturned) == 0 && get_uint(table, row, loot_table.cost) == 0)
						{
							continue;
						}

						if (!matches_category(get_value(table, row, loot_table.category), pick.categories))
						{
							continue;
						}

						if (&loot_table == &weapons)
						{
							if (pick.mk2 != (get_value(table, row, weapon_mk2_column) == "Y"))
							{
								continue;
							}

							if (pick.qm_collection && get_value(table, row, weapon_collection_column) != "RnG")
							{
								continue;
							}
						}

						candidates.push_back(get_uint(table, row, 0));
					}
				}

				std::vector<std::uint32_t> get_candidates(const Pick& pick)
				{
					std::vector<std::uint32_t> candidates;

					switch (pick.type)
					{
					case pick_type::weapon:
						collect_candidates(weapons, pick, candidates);
						break;
					case pick_type::rig:
						collect_candidates(rigs, pick, candidates);
						break;
					case pick_type::emote:
						collect_candidates(emotes, pick, candidates);
						break;
					case pick_type::camo:
						collect_candidates(camos, pick, candidates);
						break;
					case pick_type::calling_card:
						collect_candidates(calling_cards, pick, candidates);
						break;
					case pick_type::accessory:
						collect_candidates(accessories, pick, candidates);
						break;
					case pick_type::cosmetic:
						for (const auto* loot_table : { &rigs, &emotes, &camos, &calling_cards, &accessories, &emblems, &reticles })
						{
							collect_candidates(*loot_table, pick, candidates);
						}
						break;
					default:
						break;
					}

					return candidates;
				}

				void give_pick(Reward& reward, const Pick& pick)
				{
					if (pick.type == pick_type::item)
					{
						for (auto i = 0u; i < pick.count; i++)
						{
							grant_item(reward, pick.id);
						}
						return;
					}

					auto candidates = get_candidates(pick);
					if (candidates.empty())
					{
						console::error("[DW]: no mission loot candidates for pick type %d\n", static_cast<int>(pick.type));
						return;
					}

					// prefer items the player doesn't own yet
					std::vector<std::uint32_t> unowned;
					std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(unowned), [](const std::uint32_t id)
					{
						return get_item_balance(id) == 0;
					});

					auto& pool = unowned.size() >= pick.count ? unowned : candidates;

					std::random_device rd;
					std::mt19937 gen(rd());
					std::shuffle(pool.begin(), pool.end(), gen);

					for (auto i = 0u; i < pick.count && i < pool.size(); i++)
					{
						grant_item(reward, pool[i]);
					}
				}

				std::optional<std::uint32_t> get_contract_crate(const std::string& name)
				{
					constexpr std::string_view prefix = "MP_CONTRACTS_CRATE_";
					if (!name.starts_with(prefix))
					{
						return {};
					}

					return static_cast<std::uint32_t>(std::strtoul(name.data() + prefix.size(), nullptr, 10));
				}
			}

			std::uint32_t get_match_keys(const std::int32_t time_played, const std::int32_t mission_result)
			{
				if (time_played < match_min_time_played)
				{
					return 0;
				}

				auto keys = match_base_keys;
				if (mission_result == 1)
				{
					keys += match_victory_keys;
				}

				return TRUE_KEY_AMOUNT(keys);
			}

			std::uint32_t get_match_salvage(const std::int32_t time_played, const std::int32_t mission_result)
			{
				if (time_played < match_min_time_played)
				{
					return 0;
				}

				auto salvage = match_base_salvage;
				if (mission_result == 1)
				{
					salvage += match_victory_salvage;
				}

				return salvage;
			}

			Reward give_mission_team_reward(const std::uint32_t mission_team)
			{
				Reward reward{};

				const auto table = get_table("mp/loot/iw7_mission_loot_master.csv");
				if (!table || mission_team >= static_cast<std::uint32_t>(table->rowCount))
				{
					return reward;
				}

				// column c is the reward for team level c + 2, the client only asks when one below its level is unowned
				for (auto column = 0; column < table->columnCount; column++)
				{
					const auto id = get_uint(table, static_cast<int>(mission_team), column);
					if (!id)
					{
						break;
					}

					if (get_item_balance(id) == 0)
					{
						grant_item(reward, id);
						break;
					}
				}

				save();
				return reward;
			}

			Reward give_contract_reward(const std::uint32_t mission_set_id)
			{
				Reward reward{};

				if (const auto zombie_contract_pack = get_zombie_contract_pack(mission_set_id))
				{
					grant_item(reward, *zombie_contract_pack);
					save();
					return reward;
				}

				const auto table = get_table("mp/loot/iw7_loot_mission_set_master.csv");
				if (!table)
				{
					return reward;
				}

				std::string name;
				for (auto row = 0; row < table->rowCount; row++)
				{
					if (get_uint(table, row, 0) == mission_set_id)
					{
						name = get_value(table, row, 1);
						break;
					}
				}

				if (name.empty())
				{
					console::error("[DW]: unknown mission set %d\n", mission_set_id);
					return reward;
				}

				if (const auto crate = get_contract_crate(name))
				{
					const auto recipe = contract_crates.find(*crate);
					if (recipe == contract_crates.end())
					{
						console::error("[DW]: missing contract crate logic for %s\n", name.data());
						return reward;
					}

					for (const auto& pick : recipe->second)
					{
						give_pick(reward, pick);
					}
				}
				else
				{
					console::demonware("[DW]: mission set %d (%s) has no loot\n", mission_set_id, name.data());
				}

				save();
				return reward;
			}
		}
	}
}
