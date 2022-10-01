#include <std_include.hpp>
#include "../services.hpp"

#include <utils/io.hpp>

namespace demonware
{
	bdMarketplace::bdMarketplace() : service(80, "bdMarketplace")
	{
		this->register_task(42, &bdMarketplace::startExchangeTransaction);
		this->register_task(43, &bdMarketplace::purchaseOnSteamInitialize);
		this->register_task(49, &bdMarketplace::getExpiredInventoryItems);
		this->register_task(60, &bdMarketplace::steamProcessDurable);
		this->register_task(85, &bdMarketplace::unk85);
		this->register_task(122, &bdMarketplace::purchaseSkus);
		this->register_task(130, &bdMarketplace::getBalance);
		this->register_task(132, &bdMarketplace::getBalanceV2);
		this->register_task(165, &bdMarketplace::getInventoryPaginated);
		this->register_task(193, &bdMarketplace::putPlayersInventoryItems);
		this->register_task(232, &bdMarketplace::getEntitlements);
	}

	void bdMarketplace::startExchangeTransaction(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::purchaseOnSteamInitialize(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::getExpiredInventoryItems(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::steamProcessDurable(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::unk85(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::purchaseSkus(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::getBalance(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::getBalanceV2(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::getInventoryPaginated(service_server* server, byte_buffer* /*buffer*/) const
	{
		auto reply = server->create_reply(this->task_id());
		
		std::map<uint32_t, uint32_t> LootID_ranges;
		LootID_ranges.insert({ 230000, 230011 }); // mp\loot\iw7_cosmetic_heroes_loot_master.csv
		LootID_ranges.insert({ 110005, 110162 }); // mp\loot\iw7_cosmetic_emotes_loot_master.csv
		
		for (auto const& pair : LootID_ranges)
		{
			for (size_t loot_id = pair.first; loot_id <= pair.second; loot_id++)
			{
				auto loot_item = new bdMarketplaceInventory{};
				loot_item->m_userID = 0;
				loot_item->m_accountType = "iw_pc_steam";
				loot_item->m_itemId = loot_id;
				loot_item->m_itemQuantity = 1;
				loot_item->m_itemXp = 0;
				loot_item->m_itemData = "";
				loot_item->m_expireDateTime = 0xFFFFFFFF;
				loot_item->m_expiryDuration = 0x7FFFFFFFFFFFFFFF;
				loot_item->m_collisionField = 2;
				loot_item->m_modDateTime = static_cast<uint32_t>(time(nullptr));

				reply->add(loot_item);
			}
		}

		reply->send();
	}

	void bdMarketplace::putPlayersInventoryItems(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdMarketplace::getEntitlements(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}
}
