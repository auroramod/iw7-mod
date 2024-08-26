#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

#include "steam/steam.hpp"

#include "utils/json.hpp"

namespace demonware
{
	bdMarketplace::bdMarketplace() : service(80, "bdMarketplace")
	{
		this->register_task(42, &bdMarketplace::startExchangeTransaction);
		this->register_task(43, &bdMarketplace::purchaseOnSteamInitialize);
		this->register_task(44, &bdMarketplace::purchaseOnSteamFinalize);
		this->register_task(49, &bdMarketplace::getExpiredInventoryItems);
		this->register_task(58, &bdMarketplace::validateInventoryItemsToken);
		this->register_task(60, &bdMarketplace::steamProcessDurable);
		this->register_task(85, &bdMarketplace::steamProcessDurableV2);
		this->register_task(106, &bdMarketplace::purchaseSkus);
		this->register_task(130, &bdMarketplace::getBalance);
		this->register_task(132, &bdMarketplace::getBalanceV2);
		this->register_task(165, &bdMarketplace::getInventoryPaginated);
		this->register_task(193, &bdMarketplace::putPlayersInventoryItems);
		this->register_task(199, &bdMarketplace::pawnItems);
		this->register_task(232, &bdMarketplace::getEntitlements);
	}

	void bdMarketplace::startExchangeTransaction(service_server* server, byte_buffer* buffer) const
	{
		class bdStartExchangeTransactionResult final : public bdTaskResult
		{
		public:
			std::string m_exchangeTransactionId;

			void serialize(byte_buffer* data) override
			{
				data->write_string(m_exchangeTransactionId);
			}
		};

		std::string platform;
		buffer->read_string(&platform);

		auto reply = server->create_reply(this->task_id());
		auto info = std::make_unique<bdStartExchangeTransactionResult>();
		info->m_exchangeTransactionId = std::to_string(service_reply::transaction_id);
		reply.add(info);
		reply.send();
	}

	void bdMarketplace::purchaseOnSteamInitialize(service_server* server, byte_buffer* buffer) const
	{
		/*class bdPurchaseInitializeResult final : public bdTaskResult
		{
		public:
			std::uint64_t m_orderId;

			void serialize(byte_buffer* data) override
			{
				data->write_uint64(m_orderId);
			}
		};

		std::string platform;
		std::string m_exchangeTransactionId;
		std::uint32_t skuId;
		std::string language;
		std::uint64_t userID;
		std::string contextUser;

		buffer->read_string(&platform);
		buffer->read_string(&m_exchangeTransactionId);
		buffer->read_uint32(&skuId);
		buffer->read_string(&language);
		buffer->read_uint64(&userID);
		buffer->read_string(&contextUser);

		auto reply = server->create_reply(this->task_id());
		auto info = std::make_unique<bdPurchaseInitializeResult>();
		info->m_orderId = 0; // would need to emulate steam microtransaction stuff...
		reply.add(info);
		reply.send();*/

		server->create_reply(this->task_id(), BD_MARKETPLACE_STEAM_NOT_APPROVED).send();
	}

	void bdMarketplace::purchaseOnSteamFinalize(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::getExpiredInventoryItems(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::validateInventoryItemsToken(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::steamProcessDurable(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::steamProcessDurableV2(service_server* server, byte_buffer* buffer) const
	{
		std::string platform;
		buffer->read_string(&platform);

		std::string unk1;
		buffer->read_string(&unk1);

		std::uint32_t unk2;
		buffer->read_uint32(&unk2);

		std::uint64_t unk3;
		buffer->read_uint64(&unk3);

		std::string unk4;
		buffer->read_string(&unk4);

		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::purchaseSkus(service_server* server, byte_buffer* buffer) const
	{
		std::string platform, clientTransactionId;
		uint32_t numSkuIds;
		uint32_t skuIds[4]{};
		uint32_t numQuanities;
		uint32_t skuQuanities[4]{};
		uint32_t numMaxResults;
		uint32_t numDiscounts;
		uint32_t skuDiscounts[4]{};
		uint32_t numCouponRecipients;
		// coupon data here
		uint32_t numCouponMetadata;
		// coupon metadata here
		uint8_t customSourceType;
		bool ignoreEntitlements;

		buffer->read_string(&platform);
		buffer->read_string(&clientTransactionId);

		buffer->read_uint32(&numSkuIds);
		assert(numSkuIds < 4);

		for (auto i = 0u; i < numSkuIds; i++)
		{
			buffer->read_uint32(&skuIds[i]);
		}

		buffer->read_uint32(&numQuanities);
		assert(numQuanities < 4);

		for (auto i = 0u; i < numQuanities; i++)
		{
			buffer->read_uint32(&skuQuanities[i]);
		}

		buffer->read_uint32(&numMaxResults);

		buffer->read_uint32(&numDiscounts);
		assert(numDiscounts < 4);

		for (auto i = 0u; i < numDiscounts; i++)
		{
			buffer->read_uint32(&skuDiscounts[i]);
		}
		
		buffer->read_uint32(&numCouponRecipients);
		assert(numCouponRecipients == 0);

		buffer->read_uint32(&numCouponMetadata);
		assert(numCouponMetadata == 0);

		buffer->read_ubyte(&customSourceType);
		buffer->read_bool(&ignoreEntitlements);

		assert(numSkuIds == 1);

		auto id = skuIds[0];
		auto num = skuQuanities[0];

		// schematic buy comes through here
		if (id < 70000 || id > 75223)
		{
			const auto item = loot::get_loot(id);
			const auto cost = item.cost;
			const auto item_balance = loot::get_item_balance(id);
			const auto currency_balance = loot::get_currency_balance(loot::CurrencyType::salvage);

			if (cost > currency_balance)
			{
				server->create_reply(this->task_id(), BD_MARKETPLACE_ERROR).send();
				return;
			}

			loot::set_currency_balance(loot::CurrencyType::salvage, currency_balance - cost);
			loot::set_item_balance(id, item_balance + num);

			loot::save();

			server->create_reply(this->task_id(), BD_NO_ERROR).send();
			return;
		}

		int currency_id = loot::CurrencyType::keys;
		if (id > 70000 && (id - 5000) >= 70000)
		{
			id = id - 5000;
			currency_id = loot::CurrencyType::codpoints;
		}

		const auto current_balance = loot::get_currency_balance(currency_id);
		const auto cost = loot::get_lootcrate_cost(id, currency_id);

		if (cost > current_balance)
		{
			server->create_reply(this->task_id(), BD_MARKETPLACE_ERROR).send();
			return;
		}

		loot::set_currency_balance(currency_id, current_balance - cost);
		loot::set_item_balance(id, loot::get_item_balance(id) + num);

		loot::save();

		server->create_reply(this->task_id(), BD_NO_ERROR).send();
	}

	void bdMarketplace::getBalance(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::getBalanceV2(service_server* server, byte_buffer* buffer) const
	{
		std::string platform;
		buffer->read_string(&platform);

		std::uint32_t maxNumResults;
		buffer->read_uint32(&maxNumResults);

		auto reply = server->create_reply(this->task_id());

		std::uint32_t numResults = 0;

		auto add_result = [&](std::uint8_t id, std::uint32_t value)
		{
			if (numResults >= maxNumResults) return;
			auto info = std::make_unique<bdMarketplaceCurrency>();
			info->m_currencyId = id;
			info->m_value = value;
			reply.add(info);
			numResults++;
		};

		add_result(loot::CurrencyType::keys, loot::get_currency_balance(loot::CurrencyType::keys));
		add_result(loot::CurrencyType::salvage, loot::get_currency_balance(loot::CurrencyType::salvage));
		add_result(loot::CurrencyType::codpoints, loot::get_currency_balance(loot::CurrencyType::codpoints));

		reply.send();
	}

	void bdMarketplace::getInventoryPaginated(service_server* server, byte_buffer* buffer) const
	{
		if (game::environment::is_dedi())
		{
			server->create_reply(this->task_id()).send();
			return;
		}

		std::string platform;
		std::uint32_t itemsPerPage, pageNum;

		buffer->read_string(&platform);
		buffer->read_uint32(&pageNum);
		buffer->read_uint32(&itemsPerPage);

		assert(itemsPerPage);

		static unsigned int paginated_index = 0;

		auto reply = server->create_reply(this->task_id());

		auto all_loot = loot::get_all_loot_owned();

		const unsigned int index = paginated_index;
		for (size_t i = 0; i < all_loot.size() - index; i++)
		{
			const auto loot = all_loot[paginated_index];
			const auto balance = loot::get_item_balance(loot.id);
			assert(balance);

			auto info = std::make_unique<bdMarketplaceInventory>();

			info->m_playerId = steam::SteamUser()->GetSteamID().bits;
			info->unk = "";
			info->m_itemId = loot.id;
			info->m_itemQuantity = balance;
			info->m_itemXp = 0;
			info->m_itemData = "";
			info->m_expireDateTime = 0;
			info->m_expiryDuration = 0;
			info->m_collisionField = 0;
			info->m_modDateTime = 0;
			
			reply.add(info);

			paginated_index++;
			if (i >= itemsPerPage - 1)
			{
				reply.send();
				return;
			}
		}

		paginated_index = 0;
		reply.send();
	}

	void bdMarketplace::putPlayersInventoryItems(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::pawnItems(service_server* server, byte_buffer* buffer) const
	{
		std::string platform, clientTransactionId;
		std::uint32_t numItemsToPawn;

		buffer->read_string(&platform);
		buffer->read_string(&clientTransactionId);

		auto currency = loot::get_currency_balance(loot::CurrencyType::salvage);

		buffer->read_uint32(&numItemsToPawn);
		for (auto i = 0u; i < numItemsToPawn; i++)
		{
			std::uint32_t id, nextBalance;
			std::uint16_t unk2; // collision?

			buffer->read_uint32(&id);
			buffer->read_uint32(&nextBalance);
			buffer->read_uint16(&unk2);

			const auto loot = loot::get_loot(id);
			const auto amount = loot::get_item_balance(loot.id);

			if (amount == 1) // why are you trying to pawn when we only have 1...
			{
				continue;
			}

			loot::set_item_balance(loot.id, nextBalance);
			currency += loot.salvageReturned;

#ifdef DW_DEBUG
			printf("[DW]: pawning %d for %d salvage\n", loot.id, loot.salvageReturned);
#endif
		}
		loot::set_currency_balance(loot::CurrencyType::salvage, currency);
		loot::save();

		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketplace::getEntitlements(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
