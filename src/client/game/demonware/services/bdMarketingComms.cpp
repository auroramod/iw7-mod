#include <std_include.hpp>
#include "../dw_include.hpp"

#include "game/game.hpp"
#include "component/motd.hpp"

namespace demonware
{
	bdMarketingComms::bdMarketingComms() : service(104, "bdMarketingComms")
	{
		this->register_task(6, &bdMarketingComms::getMessages);
		this->register_task(7, &bdMarketingComms::reportFullMessagesViewed);
	}

	std::vector<crm::commercial_message> build_messages()
	{
		static const std::unordered_map<std::string, crm::location> locations
		{
			{"MP_MOTD", crm::mp_motd},
			{"MP_MSGS", crm::mp_msgs},
			{"ENFORCEMENT_NOTIFICATIONS", crm::enforcement_notifications},
			{"CS_NOTIFICATIONS", crm::cs_notifications},
			{"CP_MOTD", crm::cp_motd},
			{"CP_MSGS", crm::cp_msgs},
		};

		std::vector<crm::commercial_message> messages;

		uint32_t id = 1;
		for (auto& [name, contents] : motd::get_messages())
		{
			const auto it = locations.find(name);
			if (it == locations.end())
			{
				continue;
			}

			for (auto& content : contents)
			{
				crm::commercial_message message;
				message.id = id;
				message.assignment_id = id;
				message.location = it->second;
				message.content = std::move(content);

				messages.emplace_back(std::move(message));
				++id;
			}
		}

		return messages;
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
