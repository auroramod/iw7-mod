#include <std_include.hpp>
#include "../dw_include.hpp"

namespace demonware
{
	bdContentStreaming::bdContentStreaming() : service(50, "bdContentStreaming")
	{
		this->register_task(2, &bdContentStreaming::listFilesByOwner);
		this->register_task(3, &bdContentStreaming::listAllPublisherFiles);
	}

	void bdContentStreaming::listFilesByOwner(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdContentStreaming::listAllPublisherFiles(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
