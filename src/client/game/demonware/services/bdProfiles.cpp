#include <std_include.hpp>
#include "../services.hpp"
#include "bdStorage.hpp"

#include "../../../component/profile_infos.hpp"

namespace demonware
{
	bdProfiles::bdProfiles() : service(8, "bdProfiles")
	{
		this->register_task(1, &bdProfiles::getPublicInfos);
		this->register_task(2, &bdProfiles::getPrivateInfo);
		this->register_task(3, &bdProfiles::setPublicInfo);
		this->register_task(4, &bdProfiles::setPrivateInfo);
		this->register_task(5, &bdProfiles::deleteProfile);
		this->register_task(6, &bdProfiles::setPrivateInfoByUserID);
		this->register_task(7, &bdProfiles::getPrivateInfoByUserID);
		this->register_task(8, &bdProfiles::setPublicInfoByUserID);
	}

	void bdProfiles::getPublicInfos(service_server* server, byte_buffer* buffer) const
	{
		std::vector<std::pair<uint64_t, profile_infos::profile_info>> profile_infos{};

		std::uint64_t entity_id;
		buffer->read_uint64(&entity_id);

		const auto result = new bdPublicProfileInfo;
		result->m_entityID = entity_id;
		result->m_version = 4; // not sure if this is right

		const auto folder = bdStorage::get_user_file_path(std::format("profileInfo/profileInfo_{}", entity_id));
		if (utils::io::read_file(folder, &result->m_ddl))
		{
			auto reply = server->create_reply(this->task_id());
			reply->add(result);
			reply->send();
		}
		else
		{
			auto reply = server->create_reply(this->task_id(), game::BD_NO_PROFILE_INFO_EXISTS);
			reply->send();
		}
	}

	void bdProfiles::setPublicInfo(service_server* server, byte_buffer* buffer) const
	{
		std::int32_t version;
		buffer->read_int32(&version);

		std::string ddl;
		buffer->read_blob(&ddl);

		const auto folder = bdStorage::get_user_file_path(std::format("profileInfo/profileInfo_{}", steam::SteamUser()->GetSteamID().bits));
		utils::io::write_file(folder, ddl);

		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::getPrivateInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPrivateInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::deleteProfile(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPrivateInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::getPrivateInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPublicInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}
}
