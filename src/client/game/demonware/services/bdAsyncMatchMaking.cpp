#include <std_include.hpp>
#include "../services.hpp"

namespace demonware
{
	bdAsyncMatchMaking::bdAsyncMatchMaking() : service(145, "bdAsyncMatchMaking")
	{
		this->register_task(2, &bdAsyncMatchMaking::unk2);
		this->register_task(3, &bdAsyncMatchMaking::unk3);
		this->register_task(6, &bdAsyncMatchMaking::unk6);
		this->register_task(7, &bdAsyncMatchMaking::unk7);
	}

	void bdAsyncMatchMaking::unk2(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdAsyncMatchMaking::unk3(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdAsyncMatchMaking::unk6(service_server* server, byte_buffer* /*buffer*/) const
	{
		class task6Result final : public bdTaskResult
		{
		public:
			std::string jsondata;

			void serialize(byte_buffer* data) override
			{
				data->write_string(jsondata);
			}

			void deserialize(byte_buffer* data) override
			{
				data->read_string(&jsondata);
			}
		};

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

		auto response = new task6Result;
		response->jsondata = std::string(s_buffer.GetString(), s_buffer.GetSize());
		reply->add(response);

		reply->send();
	}

	void bdAsyncMatchMaking::unk7(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}
}
