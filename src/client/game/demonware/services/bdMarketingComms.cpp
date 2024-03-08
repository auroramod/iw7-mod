#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

namespace demonware
{
	bdMarketingComms::bdMarketingComms() : service(104, "bdMarketingComms")
	{
		this->register_task(1, &bdMarketingComms::getMessages);
		this->register_task(4, &bdMarketingComms::reportFullMessagesViewed);
		this->register_task(6, &bdMarketingComms::unk6);
	}

	void bdMarketingComms::getMessages(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketingComms::reportFullMessagesViewed(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdMarketingComms::unk6(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		server->create_reply(this->task_id(), BD_NO_FILE).send();
	}
}
