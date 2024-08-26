#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

#include "utils/io.hpp"
#include "utils/json.hpp"

namespace demonware
{
	namespace loot
	{
		const std::string& json_data_path = "iw7-mod/players2/user/loot/loot.json";
		nlohmann::json json_buffer;

		namespace csv
		{
			struct LootCsv
			{
				std::string file;
				std::uint32_t index;
				std::uint32_t quality;
				std::uint32_t salvageReturned;
				std::uint32_t cost;
			};

			struct LootCrateCsv
			{
				std::string file;
				std::uint32_t index;
				std::uint32_t cost;
				std::uint32_t premiumCost;
				std::uint32_t CODPointsSKU;
			};

			LootCsv weapon
			{
				.file = "mp/loot/iw7_weapon_loot_master.csv",
				.index = 0,
				.quality = 6,
				.salvageReturned = 3,
				.cost = 4,
			};

			LootCsv killstreak
			{
				.file = "mp/loot/iw7_killstreak_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 3,
				.cost = 4,
			};

			LootCsv consumable
			{
				.file = "mp/loot/iw7_consumable_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_attachments
			{
				.file = "mp/loot/iw7_cosmetic_attachments_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_calling_cards
			{
				.file = "mp/loot/iw7_cosmetic_calling_cards_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_camos
			{
				.file = "mp/loot/iw7_cosmetic_camos_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_emblems
			{
				.file = "mp/loot/iw7_cosmetic_emblems_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_emotes
			{
				.file = "mp/loot/iw7_cosmetic_emotes_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_heroes
			{
				.file = "mp/loot/iw7_cosmetic_heroes_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_reticles
			{
				.file = "mp/loot/iw7_cosmetic_reticles_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv cosmetic_rigs
			{
				.file = "mp/loot/iw7_cosmetic_rigs_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 4,
				.cost = 5,
			};

			LootCsv zombiefatefortune
			{
				.file = "cp/loot/iw7_zombiefatefortune_loot_master.csv",
				.index = 0,
				.quality = 2,
				.salvageReturned = 0xFF,
				.cost = 0xFF,
			};

			LootCrateCsv loot_crate
			{
				.file = "mp/loot/iw7_loot_crate_loot_master.csv",
				.index = 0,
				.cost = 3,
				.premiumCost = 4,
				.CODPointsSKU = 5,
			};
		}

		std::vector<std::uint32_t> lootmap_weapon;
		std::vector<std::uint32_t> lootmap_killstreak;
		std::vector<std::uint32_t> lootmap_consumable;
		std::vector<std::uint32_t> lootmap_cosmetic_attachments;
		std::vector<std::uint32_t> lootmap_cosmetic_calling_cards;
		std::vector<std::uint32_t> lootmap_cosmetic_camos;
		std::vector<std::uint32_t> lootmap_cosmetic_emblems;
		std::vector<std::uint32_t> lootmap_cosmetic_emotes;
		std::vector<std::uint32_t> lootmap_cosmetic_heroes;
		std::vector<std::uint32_t> lootmap_cosmetic_reticles;
		std::vector<std::uint32_t> lootmap_cosmetic_rigs;
		std::vector<std::uint32_t> lootmap_zombiefatefortune;

		std::unordered_map<std::uint32_t, Item> m_lootmap;

		struct LootCrate
		{
			std::uint32_t cost;
			std::uint32_t premiumCost;
			std::uint32_t salvageCost;
		};

		std::unordered_map<std::uint32_t, LootCrate> lootcrates;

		void read_loot_csv(csv::LootCsv& csv, std::vector<std::uint32_t>& lootmap)
		{
			lootmap.clear();

			const auto asset = game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, csv.file.data(), 0).stringTable;
			if (!asset) return;

			for (auto row = 0; row < asset->rowCount; row++)
			{
				const std::uint32_t id = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv.index));
				const std::uint32_t quality = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv.quality));

				std::uint32_t salvageReturned = 0;
				std::uint32_t cost = 0;
				if (csv.salvageReturned != 0xFF)
				{
					salvageReturned = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv.salvageReturned));
				}
				if (csv.cost != 0xFF)
				{
					cost = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv.cost));
				}

				const Item loot = { id, quality, salvageReturned, cost };
				m_lootmap[id] = loot;
				lootmap.push_back(id);
			}
		}

		void read_weapon_csv()
		{
			read_loot_csv(csv::weapon, lootmap_weapon);
		}

		void read_killstreak_csv()
		{
			read_loot_csv(csv::killstreak, lootmap_killstreak);
		}

		void read_consumable_csv()
		{
			read_loot_csv(csv::consumable, lootmap_consumable);
		}

		void read_cosmetic_attachments_csv()
		{
			read_loot_csv(csv::cosmetic_attachments, lootmap_cosmetic_attachments);
		}

		void read_cosmetic_calling_cards_csv()
		{
			read_loot_csv(csv::cosmetic_calling_cards, lootmap_cosmetic_calling_cards);
		}

		void read_cosmetic_camos_csv()
		{
			read_loot_csv(csv::cosmetic_camos, lootmap_cosmetic_camos);
		}

		void read_cosmetic_emblems_csv()
		{
			read_loot_csv(csv::cosmetic_emblems, lootmap_cosmetic_emblems);
		}

		void read_cosmetic_emotes_csv()
		{
			read_loot_csv(csv::cosmetic_emotes, lootmap_cosmetic_emotes);
		}

		void read_cosmetic_heroes_csv()
		{
			read_loot_csv(csv::cosmetic_heroes, lootmap_cosmetic_heroes);
		}

		void read_cosmetic_reticles_csv()
		{
			read_loot_csv(csv::cosmetic_reticles, lootmap_cosmetic_reticles);
		}

		void read_cosmetic_rigs_csv()
		{
			read_loot_csv(csv::cosmetic_rigs, lootmap_cosmetic_rigs);
		}

		void read_zombiefatefortune_csv()
		{
			read_loot_csv(csv::zombiefatefortune, lootmap_zombiefatefortune);
		}

		void read_loot_crate_csv()
		{
			lootcrates.clear();

			const auto asset = game::DB_FindXAssetHeader(game::ASSET_TYPE_STRINGTABLE, csv::loot_crate.file.data(), 0).stringTable;
			if (!asset) return;

			for (auto row = 0; row < asset->rowCount; row++)
			{
				const std::uint32_t id = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv::loot_crate.index));
				const std::uint32_t cost = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv::loot_crate.cost));
				const std::uint32_t premiumCost = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv::loot_crate.premiumCost));
				const std::uint32_t CODPointsSKU = std::atoi(game::StringTable_GetColumnValueForRow(asset, row, csv::loot_crate.CODPointsSKU));
				lootcrates[id] = { cost, premiumCost, CODPointsSKU };
			}
		}

		void cache_loot()
		{
			static bool once = false;
			if (once) return;
			once = true;

			m_lootmap.clear();

			// mp
			read_weapon_csv();
			read_killstreak_csv();
			read_consumable_csv();
			read_cosmetic_attachments_csv();
			read_cosmetic_calling_cards_csv();
			read_cosmetic_camos_csv();
			read_cosmetic_emblems_csv();
			read_cosmetic_emotes_csv();
			read_cosmetic_heroes_csv();
			read_cosmetic_reticles_csv();
			read_cosmetic_rigs_csv();

			// cp
			read_zombiefatefortune_csv();

			// crates
			read_loot_crate_csv();
		}

		// Brought to you by ChatGPT
		std::vector<Item> get_random_loot_from_map(std::vector<std::uint32_t>& lootmap, const size_t itemAmount, const float luckFactor, std::uint32_t quaranteedQuality = 0)
		{
			// Edge case: if the vector is empty
			if (lootmap.empty()) {
				throw std::runtime_error("Lootmap is empty");
			}

			// Edge case: if itemAmount is more than the number of available items
			if (itemAmount > lootmap.size()) {
				throw std::runtime_error("Item amount exceeds the number of available items");
			}

			// Validate the luckFactor (should be positive)
			if (luckFactor <= 0) {
				throw std::invalid_argument("Luck factor must be greater than zero");
			}

			// Calculate the total weight based on adjusted rarity values
			double totalWeight = 0;
			std::vector<double> adjustedWeights(lootmap.size());

			for (size_t i = 0; i < lootmap.size(); ++i) {
				if (!get_loot(lootmap[i]).quality) continue;

				// Calculate weight as inverse of rarity, adjusted by luckFactor
				adjustedWeights[i] = 1.0 / std::pow(get_loot(lootmap[i]).quality, luckFactor);
				totalWeight += adjustedWeights[i];
			}

			assert(adjustedWeights.size() >= itemAmount);

			// Set up the random number generator
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, totalWeight);

			std::vector<Item> selectedItems;

			// Select items
			while (selectedItems.size() < itemAmount) {
				double randomValue = dis(gen);
				double cumulativeWeight = 0;

				// Find the item corresponding to the random value
				for (size_t i = 0; i < lootmap.size(); ++i) {
					cumulativeWeight += adjustedWeights[i];
					if (randomValue < cumulativeWeight) {
						if (quaranteedQuality && get_loot(lootmap[i]).quality < quaranteedQuality)
							continue;
						else
							quaranteedQuality = 0;

						// Add item to the result if it's not already selected
						if (std::find(selectedItems.begin(), selectedItems.end(), get_loot(lootmap[i])) == selectedItems.end()) {
							selectedItems.push_back(get_loot(lootmap[i]));
							break;
						}
					}
				}
			}

			return selectedItems;
		}

		std::vector<std::uint32_t> combined_mp_lootmaps;
		std::vector<std::uint32_t> combine_mp_lootmaps()
		{
			std::vector<std::uint32_t>& lootmap = combined_mp_lootmaps;
			if (!combined_mp_lootmaps.empty())
			{
				return combined_mp_lootmaps;
			}

			lootmap.insert(lootmap.end(), lootmap_weapon.begin(), lootmap_weapon.end());
			lootmap.insert(lootmap.end(), lootmap_killstreak.begin(), lootmap_killstreak.end());
			lootmap.insert(lootmap.end(), lootmap_consumable.begin(), lootmap_consumable.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_attachments.begin(), lootmap_cosmetic_attachments.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_calling_cards.begin(), lootmap_cosmetic_calling_cards.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_camos.begin(), lootmap_cosmetic_camos.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_emblems.begin(), lootmap_cosmetic_emblems.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_emotes.begin(), lootmap_cosmetic_emotes.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_heroes.begin(), lootmap_cosmetic_heroes.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_reticles.begin(), lootmap_cosmetic_reticles.end());
			lootmap.insert(lootmap.end(), lootmap_cosmetic_rigs.begin(), lootmap_cosmetic_rigs.end());
			return lootmap;
		}

		std::vector<std::uint32_t> get_all_lootmaps()
		{
			cache_loot();

			std::vector<std::uint32_t> lootmap = combine_mp_lootmaps();
			lootmap.insert(lootmap.end(), lootmap_zombiefatefortune.begin(), lootmap_zombiefatefortune.end());
			return lootmap;
		}

		std::vector<Item> all_loot;
		std::vector<Item> get_all_loot() 
		{
			if (!all_loot.empty())
			{
				return all_loot;
			}

			std::vector<Item>& items = all_loot;
			auto lootmap = get_all_lootmaps();
			for (size_t i = 0; i < lootmap.size(); i++)
			{
				items.push_back(get_loot(lootmap[i]));
			}

			return items;
		};

		std::vector<Item> get_all_loot_owned()
		{
			auto lootmap = get_all_lootmaps();
			std::vector<Item> items{};
			for (size_t i = 0; i < lootmap.size(); i++)
			{
				if (get_item_balance(lootmap[i]))
				{
					items.push_back(get_loot(lootmap[i]));
				}
			}

			return items;
		}

		Item get_loot(const std::uint32_t item_id)
		{
			if (m_lootmap.contains(item_id))
			{
				return m_lootmap[item_id];
			}

			return {};
		}

		std::vector<Item> get_random_loot_CommonCrate(const float scale = 1.0f, std::uint32_t quality = 0)
		{
			std::vector<std::uint32_t> lootmap = combine_mp_lootmaps();

			return get_random_loot_from_map(lootmap, 3, scale, quality);
		}

		std::vector<Item> get_random_loot_RareCrate()
		{
			return get_random_loot_CommonCrate(0.34f, 2);
		}

		std::vector<Item> get_random_loot_ZombieCrate(const float scale = 1.0f, std::uint32_t quality = 0)
		{
			std::vector<std::uint32_t> lootmap = combine_mp_lootmaps();

			auto mp = get_random_loot_from_map(lootmap, 2, scale);
			auto cp = get_random_loot_from_map(lootmap_zombiefatefortune, 1, scale);

			std::vector<Item> items;
			items.push_back(mp[0]);
			items.push_back(mp[1]);
			items.push_back(cp[0]);

			return items;
		}

		std::vector<Item> get_random_loot_ZombieRareCrate()
		{
			return get_random_loot_ZombieCrate(0.34f, 2);
		}

		std::vector<Item> get_random_loot_ZombieCardPack(const float scale = 1.0f, std::uint32_t quality = 0)
		{
			return get_random_loot_from_map(lootmap_zombiefatefortune, 3, scale, quality);
		}

		std::vector<Item> get_random_loot_ZombieRareCardPack()
		{
			return get_random_loot_ZombieCardPack(0.34f, 2);
		}

		std::vector<Item> get_random_loot(const std::uint32_t lootbox_id)
		{
			cache_loot();

			switch (lootbox_id)
			{
			case 70000: // CommonCrate
				return get_random_loot_CommonCrate();
			case 70001: // RareCrate
				return get_random_loot_RareCrate();
			case 70002: // ZombieCrate
				return get_random_loot_ZombieCrate();
			case 70003: // ZombieRareCrate
				return get_random_loot_ZombieRareCrate();
			case 70004: // ZombieCardPack
				return get_random_loot_ZombieCardPack();
			case 70005: // ZombieRareCardPack
				return get_random_loot_ZombieRareCardPack();
			default:
				printf("[DW]: Missing LootCrate logic for %d\n", lootbox_id);
				return {};
			}
		}

		std::uint32_t get_lootcrate_cost(const std::uint32_t id, const std::uint32_t type)
		{
			if (lootcrates.contains(id))
			{
				switch(type)
				{
				case CurrencyType::keys:
					return lootcrates[id].cost;
					break;
				case CurrencyType::codpoints:
					return lootcrates[id].premiumCost;
					break;
				case CurrencyType::salvage:
					return lootcrates[id].salvageCost;
					break;
				}
			}

			return 0;
		}

		template <typename T> void json_put(nlohmann::json& field, T value)
		{
			field = value;
		}

		template <typename T> T json_read(nlohmann::json& field)
		{
			if (!field.is_null())
			{
				return field.get<T>();
			}
			else
			{
				return T{};
			}
		}

		void save_json_data()
		{
			auto dump = json_buffer.dump(4);
			utils::io::write_file(json_data_path, dump);
		}

		void read_json_data()
		{
			if (!json_buffer.empty())
			{
				return;
			}

			if (utils::io::file_exists(json_data_path))
			{
				auto data = utils::io::read_file(json_data_path);
				json_buffer = nlohmann::json::parse(data);
				return;
			}

			json_buffer = {};

			set_currency_balance(CurrencyType::keys, 9999999);
			set_currency_balance(CurrencyType::salvage, 9999);
			set_currency_balance(CurrencyType::codpoints, 99999);
			save_json_data();
		}

		void set_item_balance(const std::uint32_t item_id, const std::uint32_t amount)
		{
			json_put(json_buffer["Loot"][std::to_string(item_id)]["Balance"], amount);
		}

		std::uint32_t get_item_balance(const std::uint32_t item_id)
		{
			read_json_data();
			return json_read<std::uint32_t>(json_buffer["Loot"][std::to_string(item_id)]["Balance"]);
		}

		void set_currency_balance(const std::uint32_t currency_id, const std::uint32_t amount)
		{
			json_put(json_buffer["Currency"][std::to_string(currency_id)]["Balance"], amount);
		}

		std::uint32_t get_currency_balance(const std::uint32_t currency_id)
		{
			read_json_data();
			return json_read<std::uint32_t>(json_buffer["Currency"][std::to_string(currency_id)]["Balance"]);
		}

		void save()
		{
			save_json_data();
		}
	}
}