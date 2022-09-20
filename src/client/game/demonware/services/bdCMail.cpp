#include <std_include.hpp>
#include "../services.hpp"

namespace demonware
{
	bdCMail::bdCMail() : service(29, "bdCMail")
	{
		this->register_task(10, &bdCMail::unk10);
	}

	void bdCMail::unk10(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}
}
