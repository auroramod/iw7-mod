#include <std_include.hpp>
#include "../dw_include.hpp"

#include "steam/steam.hpp"

#include "utils/json.hpp"
#include "../loot/loot.hpp"

//#define VERIFY_CRATE_AMOUNT

namespace demonware
{
	bdReward::bdReward() : service(139, "bdReward")
	{
		this->register_task(1, &bdReward::incrementTime);
		this->register_task(2, &bdReward::claimRewardRoll);
		this->register_task(3, &bdReward::claimClientAchievements);
		this->register_task(4, &bdReward::reportRewardEvents);
	}

	void bdReward::incrementTime(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdReward::claimRewardRoll(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdReward::claimClientAchievements(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdReward::reportRewardEvents(service_server* server, byte_buffer* buffer) const
	{
		std::string platform;
		unsigned short numEvents;
		std::int32_t rewardEventType;

		std::string json_buffer;

		buffer->read_string(&platform);
		buffer->read_uint16(&numEvents);
		buffer->read_int32(&rewardEventType); // none 0, json 1

		assert(rewardEventType == 1);
		assert(numEvents == 1);

		buffer->read_string(&json_buffer);

		const auto send = [&](nlohmann::json& json_reply)
		{
			auto result = std::make_unique<bdRewardEvent>();

			result->push_type = BD_REWARD_EVENT_MESSAGE;
			result->r2 = 1;
			result->user_id = steam::SteamUser()->GetSteamID().bits;
			result->platform1 = "steam";
			result->platform2 = platform;
			result->rewardEventType = rewardEventType;
			result->r7 = 1;
			result->r8 = 1;
			result->json_buffer = json_reply.dump(0);

			byte_buffer reply_buffer;
			result->serialize(&reply_buffer);

			auto reply = server->create_message(BD_LOBBY_SERVICE_PUSH_MESSAGE);
			reply.send(&reply_buffer, true);
		};

		nlohmann::json json;
		json = json.parse(json_buffer);

		const auto action = json["Action"].get<std::string>();
		if (action == "DailyLogin")
		{
#ifdef DW_DEBUG
			printf("[DW]: daily login requested...\n");
#endif

			nlohmann::json json_reply;
			json_reply["Action"] = "DailyLoginResponse";

			json_reply["LoginDayCountSP"] = -1;
			json_reply["FirstTimeTodaySP"] = false;
			json_reply["LoginDayCount"] = 1;
			json_reply["FirstTimeToday"] = false;

			// Packs
			json_reply["BasicPacks"] = nlohmann::json::value_type::array();
			json_reply["BasicPacks"][0]["Currencies"] = nlohmann::json::value_type::object();
			json_reply["BasicPacks"][0]["Currencies"]["11"] = 0;
			json_reply["BasicPacks"][0]["Currencies"]["12"] = 0;
			json_reply["BasicPacks"][0]["Currencies"]["20"] = 0;
			json_reply["BasicPacks"][0]["Items"] = nlohmann::json::value_type::array();
			json_reply["BasicPacks"][0]["Id"] = 200018;
			json_reply["SeasonPassPacks"] = nlohmann::json::value_type::array();

			// Extra Items
			json_reply["ExtraItems"] = nlohmann::json::value_type::array();

			// Items
			json_reply["Items"] = nlohmann::json::value_type::array();

			// mp/loot/iw7_loot_crate_loot_master.csv
			json_reply["Items"][0]["ItemId"] = 70005; // ZombieRareCardPack
			json_reply["Items"][0]["Collision"] = 0; // not sure what this does
			json_reply["Items"][0]["Balance"] = 999;
			loot::set_item_balance(70005, 999);

			// Currencies
			json_reply["Currencies"] = nlohmann::json::value_type::array();

			// Keys
			json_reply["Currencies"][0]["CurrencyId"] = loot::CurrencyType::keys;
			json_reply["Currencies"][0]["Balance"] = loot::get_currency_balance(loot::CurrencyType::keys);

			// Salvage
			json_reply["Currencies"][1]["CurrencyId"] = loot::CurrencyType::salvage;
			json_reply["Currencies"][1]["Balance"] = loot::get_currency_balance(loot::CurrencyType::salvage);

			// CodPoints
			json_reply["Currencies"][2]["CurrencyId"] = loot::CurrencyType::codpoints;
			json_reply["Currencies"][2]["Balance"] = loot::get_currency_balance(loot::CurrencyType::codpoints);

			json_reply["ClientTx"] = json["ClientTx"];

			send(json_reply);
		}
		else if (action == "ClaimLootCrates")
		{
#ifdef DW_DEBUG
			printf("[DW]: supply drop open requested...\n");
#endif

			nlohmann::json json_reply;
			json_reply["Action"] = "ClaimLootCratesResponse";

			json_reply["Packs"] = nlohmann::json::value_type::array();
			json_reply["Items"] = nlohmann::json::value_type::array();
			json_reply["Currencies"] = nlohmann::json::value_type::array();

			// Items, Packs

			const auto rule_id = json["RuleId"].is_number_integer() ? json["RuleId"].get<int>() : 0;
			const auto crate_id = 70000 + rule_id;

			int itemidx = 0;

			auto crate_balance = loot::get_item_balance(crate_id);
#ifdef VERIFY_CRATE_AMOUNT
			if (!crate_balance)
			{
				server->create_reply(this->task_id(), BD_MARKETPLACE_ERROR).send();
				return;
			}
#else
			if (!crate_balance)
			{
				crate_balance = 1;
			}
#endif

			const auto new_crate_balance = crate_balance - 1;

			json_reply["Items"][itemidx]["ItemId"] = crate_id;
			json_reply["Items"][itemidx]["Collision"] = 0; // not sure what this does
			json_reply["Items"][itemidx]["Balance"] = new_crate_balance;
			itemidx++;

			loot::set_item_balance(crate_id, new_crate_balance);

			auto loot = loot::get_random_loot(crate_id);

			for (auto i = 0; i < loot.size(); i++)
			{
				const auto item_id = loot[i].id;

				json_reply["Packs"][i] = item_id;

				const auto balance = loot::get_item_balance(item_id) + 1;
				loot::set_item_balance(item_id, balance);

				json_reply["Items"][i + itemidx]["ItemId"] = item_id;
				json_reply["Items"][i + itemidx]["Collision"] = 0;
				json_reply["Items"][i + itemidx]["Balance"] = balance;

#ifdef DW_DEBUG
				printf("[DW]: loot %d\n", item_id);
#endif
			}

			loot::save();

			json_reply["Error"] = "";
			json_reply["ClientTx"] = json["ClientTx"];

			send(json_reply);
		}
		/*else if (action == "StartMission")
		{
			printf("%s\n", json_buffer.data());
			//const auto match_id = json["MatchId"].get<int>();
			//const auto mission_set_instance_id = json["MissionSetInstanceId"].get<unsigned int>();
		}
		else if (action == "StartMissionSet")
		{
			printf("%s\n", json_buffer.data());
			//const auto mission_set_id = json["MissionSetId"].get<unsigned int>();
		}
		else if (action == "EndMission")
		{
			printf("%s\n", json_buffer.data());
		}
		else if (action == "EndMissionSet")
		{
			printf("%s\n", json_buffer.data());
		}
		else if (action == "ResetMissions")
		{
			printf("%s\n", json_buffer.data());
		}*/
		else
		{
			//printf("[DW]: unhandled reward action \"%s\"...\n", action.data());
		}

		server->create_reply(this->task_id(), BD_NO_ERROR).send();
	}
}
