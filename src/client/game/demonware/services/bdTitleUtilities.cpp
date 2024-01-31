#include <std_include.hpp>
#include "../dw_include.hpp"

namespace demonware
{
	bdTitleUtilities::bdTitleUtilities() : service(12, "bdTitleUtilities")
	{
		this->register_task(1, &bdTitleUtilities::isProfanity);
		this->register_task(6, &bdTitleUtilities::getServerTime);
	}

	void bdTitleUtilities::isProfanity(service_server* server, byte_buffer* /*buffer*/) const
	{
		//std::string text;
		//buffer->read_string(&text)

		auto result = std::make_unique<bdProfanityResult>();
		result->isProfanity = 0;

		auto reply = server->create_reply(this->task_id());
		reply.add(result);
		reply.send();
	}

	void bdTitleUtilities::getServerTime(service_server* server, byte_buffer* /*buffer*/) const
	{
		auto time_result = std::make_unique<bdTimeStamp>();
		time_result->unix_time = uint32_t(time(nullptr));

		auto reply = server->create_reply(this->task_id());
		reply.add(time_result);
		reply.send();
	}
}
