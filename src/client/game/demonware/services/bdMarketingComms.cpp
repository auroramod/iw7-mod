#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

namespace demonware
{
	bdMarketingComms::bdMarketingComms() : service(104, "bdMarketingComms")
	{
		this->register_task(4, &bdMarketingComms::reportFullMessagesViewed);
		this->register_task(6, &bdMarketingComms::getMessages);
	}

	void bdMarketingComms::reportFullMessagesViewed(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}

#pragma pack(push, 1)
	struct bdCommsGetMessagesRequest
	{
		char __pad0[23];
	}; static_assert(sizeof(bdCommsGetMessagesRequest) == 23);

	struct unk_s
	{
		char __pad0[17];
		unsigned char unk;
	};

	struct bdCommsGetMessagesResponse
	{
		unk_s unk[1];
	}; //static_assert(sizeof(bdCommsGetMessagesResponse) == 180);
#pragma pack(pop)

	void bdMarketingComms::getMessages(service_server* server, byte_buffer* buffer) const
	{
		bdCommsGetMessagesRequest request{};

		class bdCommsGetMessagesResult final : public bdTaskResult
		{
		public:
			bdCommsGetMessagesResponse response;

			void serialize(byte_buffer* data) override
			{
				data->write_struct(&response, sizeof(bdCommsGetMessagesResponse));
			}
		};

		//buffer->read_struct(&request, sizeof(bdCommsGetMessagesRequest));

		auto reply = server->create_reply(this->task_id());

		auto info = std::make_unique<bdCommsGetMessagesResult>();

		unsigned char unk[18]{ 0x0A, 0x10, 0x08, 0x00, 0x12, 0x00, 0x1A, 0x00, 0x22, 0x00, 0x2A, 0x00, 0x32, 0x00, 0x38, 0x00, 0x40, 0x01 };
		for (auto i = 0; i < 1; i++)
		{
			memcpy(info->response.unk[i].__pad0, unk, 17);
		}
		info->response.unk[0].unk = 0x01;

		reply.add(info);

		reply.send_struct();
	}
}
