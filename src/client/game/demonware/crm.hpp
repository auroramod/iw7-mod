#pragma once

namespace demonware::crm
{
	enum location : uint32_t
	{
		unusable = 0,
		mp_motd = 1,
		mp_msgs = 2,
		enforcement_notifications = 9,
		cs_notifications = 10,
		cp_motd = 11,
		cp_msgs = 12,
	};

	struct marketing_message
	{
		std::string title;
		std::string image;
		std::string popup_image;
		std::string content_long;
		std::string content_short;
		std::string action;
		std::string action_title;

		std::string serialize() const;
	};

	struct commercial_message
	{
		uint64_t id{};
		uint32_t location{};      // m_locationID
		uint32_t assignment_id{}; // m_assignmentID
		std::string language{ "en" };
		std::string content{ "{}" };
		std::string metadata{ "{}" };
		std::string payload{ "{}" };
		std::string payload_signature{ "{}" };

		std::string serialize() const;
	};

	std::string serialize(const std::vector<commercial_message>& messages);
}