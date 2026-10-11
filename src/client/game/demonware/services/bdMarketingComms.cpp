#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"

namespace demonware
{
	bdMarketingComms::bdMarketingComms() : service(104, "bdMarketingComms")
	{
		this->register_task(6, &bdMarketingComms::getMessages);
		this->register_task(7, &bdMarketingComms::reportFullMessagesViewed);
	}

	std::vector<crm::commercial_message> build_messages()
	{
		crm::marketing_message motd;
		motd.title = "Welcome to IW7-Mod!";
		motd.image = "image_motd_mp_halloween_halloween";
		motd.popup_image = "image_motd_mp_halloween_halloween_small";
		motd.content_long = "auroramod.dev";
		motd.content_short = "Message of The Day";

		crm::commercial_message message;
		message.id = 1;
		message.location = 1;
		message.assignment_id = 1;
		message.content = motd.serialize();

		return { std::move(message) };
	}

	void bdMarketingComms::getMessages(service_server* server, byte_buffer* buffer) const
	{
		class crm_payload final : public bdTaskResult
		{
		public:
			std::string data;

			explicit crm_payload(std::string buffer) : data(std::move(buffer)) {}

			void serialize(byte_buffer* buffer) override
			{
				buffer->write_struct(this->data.data(), static_cast<int>(this->data.size()));
			}
		};

		auto result = std::make_unique<crm_payload>(crm::serialize(build_messages()));

		auto reply = server->create_reply(this->task_id());
		reply.add(result);
		reply.send_struct();
	}


	void bdMarketingComms::reportFullMessagesViewed(service_server* server, byte_buffer* buffer) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply.send();
	}
}
