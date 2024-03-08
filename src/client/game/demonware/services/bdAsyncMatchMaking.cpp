#include <std_include.hpp>
#include "../dw_include.hpp"

namespace demonware
{
	bdAsyncMatchMaking::bdAsyncMatchMaking() : service(145, "bdAsyncMatchMaking")
	{
		this->register_task(2, &bdAsyncMatchMaking::setPlayerInfo);
		this->register_task(3, &bdAsyncMatchMaking::getMatchMakingPlayerToken);
		this->register_task(6, &bdAsyncMatchMaking::initMatchMaking);
		this->register_task(7, &bdAsyncMatchMaking::startMatchMaking);
	}

	void bdAsyncMatchMaking::setPlayerInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdAsyncMatchMaking::getMatchMakingPlayerToken(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

	void bdAsyncMatchMaking::initMatchMaking(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());

		rapidjson::Document response_json;
		response_json.SetObject();

		response_json.AddMember("matchmaking_id", 0, response_json.GetAllocator());

		rapidjson::Document backoff_timing_info;
		backoff_timing_info.SetObject();
		response_json.AddMember("backoff_timing_info", backoff_timing_info, response_json.GetAllocator());

		rapidjson::StringBuffer s_buffer{};
		rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::ASCII<>>
			writer(s_buffer);
		response_json.Accept(writer);

		auto response = std::make_unique<bdStringResult>(std::string(s_buffer.GetString(), s_buffer.GetSize()));
		reply.add(response);

		reply.send();
	}

	void bdAsyncMatchMaking::startMatchMaking(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
