#include <std_include.hpp>
#include "../services.hpp"

#include "game/game.hpp"

namespace demonware
{
	bdUNK123::bdUNK123() : service(123, "bdUNK123")
	{
		this->register_task(1, &bdUNK123::unk1);
	}

	void bdUNK123::unk1(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		server->create_reply(this->task_id(), game::BD_NO_FILE)->send();
	}
}
