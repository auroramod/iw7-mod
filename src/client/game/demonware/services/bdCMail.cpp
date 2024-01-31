#include <std_include.hpp>
#include "../dw_include.hpp"

namespace demonware
{
	bdCMail::bdCMail() : service(29, "bdCMail") // bdMail
	{
		this->register_task(10, &bdCMail::getMailInfo);
	}

	void bdCMail::getMailInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
