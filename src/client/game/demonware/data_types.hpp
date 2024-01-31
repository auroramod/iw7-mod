#pragma once

namespace demonware
{
	class bdTaskResult
	{
	public:
		virtual ~bdTaskResult() = default;

		virtual void serialize(byte_buffer*)
		{
		}

		virtual void deserialize(byte_buffer*)
		{
		}
	};

	class bdFileData final : public bdTaskResult
	{
	public:
		std::string file_data;

		explicit bdFileData(std::string buffer) : file_data(std::move(buffer))
		{
		}

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_blob(this->file_data);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_blob(&this->file_data);
		}
	};

	class bdStringResult final : public bdTaskResult
	{
	public:
		std::string buffer;

		explicit bdStringResult(std::string data) : buffer(std::move(data))
		{
		}

		void serialize(byte_buffer* data) override
		{
			data->write_string(buffer);
		}

		void deserialize(byte_buffer* data) override
		{
			data->read_string(&buffer);
		}
	};

	class bdProfanityResult final : public bdTaskResult
	{
	public:
		uint32_t isProfanity;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint32(this->isProfanity);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint32(&this->isProfanity);
		}
	};

	struct bdFileInfo final : public bdTaskResult
	{
		uint64_t file_id;
		uint32_t create_time;
		uint32_t modified_time;
		bool visibility;
		uint64_t owner_id;
		std::string filename;
		uint32_t file_size;
		std::string checksum;

		void serialize(byte_buffer* data) override
		{
			data->write_uint32(file_size);
			data->write_uint64(file_id);
			data->write_uint32(create_time);
			data->write_uint32(modified_time);
			data->write_bool(visibility);
			data->write_uint64(owner_id);
			data->write_string(filename);
			data->write_string(checksum);
		}

		void deserialize(byte_buffer* data) override
		{
			data->read_uint32(&file_size);
			data->read_uint64(&file_id);
			data->read_uint32(&create_time);
			data->read_uint32(&modified_time);
			data->read_bool(&visibility);
			data->read_uint64(&owner_id);
			data->read_string(&filename);
			data->read_string(&checksum);
		}
	};

	class bdTimeStamp final : public bdTaskResult
	{
	public:
		uint32_t unix_time;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint32(this->unix_time);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint32(&this->unix_time);
		}
	};

	class bdDMLInfo : public bdTaskResult
	{
	public:
		std::string country_code; // Char [3]
		std::string country; // Char [65]
		std::string region; // Char [65]
		std::string city; // Char [129]
		float latitude;
		float longitude;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_string(this->country_code);
			buffer->write_string(this->country);
			buffer->write_string(this->region);
			buffer->write_string(this->city);
			buffer->write_float(this->latitude);
			buffer->write_float(this->longitude);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_string(&this->country_code);
			buffer->read_string(&this->country);
			buffer->read_string(&this->region);
			buffer->read_string(&this->city);
			buffer->read_float(&this->latitude);
			buffer->read_float(&this->longitude);
		}
	};

	class bdDMLRawData final : public bdDMLInfo
	{
	public:
		uint32_t asn; // Autonomous System Number.
		std::string timezone;

		void serialize(byte_buffer* buffer) override
		{
			bdDMLInfo::serialize(buffer);

			buffer->write_uint32(this->asn);
			buffer->write_string(this->timezone);
		}

		void deserialize(byte_buffer* buffer) override
		{
			bdDMLInfo::deserialize(buffer);

			buffer->read_uint32(&this->asn);
			buffer->read_string(&this->timezone);
		}
	};

	class bdFileQueryResult final : public bdTaskResult
	{
	public:
		uint64_t user_id;
		std::string platform;
		std::string filename;
		uint32_t errorcode;
		std::string filedata;

		void serialize(byte_buffer* data) override
		{
			data->write_uint64(user_id);
			data->write_string(platform);
			data->write_string(filename);
			data->write_uint32(errorcode);
			data->write_blob(filedata);
		}

		void deserialize(byte_buffer* data) override
		{
			data->read_uint64(&user_id);
			data->read_string(&platform);
			data->read_string(&filename);
			data->read_uint32(&errorcode);
			data->read_blob(&filedata);
		}
	};

	class bdFile2 final : public bdTaskResult
	{
	public:
		uint32_t unk1;
		uint32_t unk2;
		uint32_t unk3;
		bool priv;
		uint64_t owner_id;
		std::string platform;
		std::string filename;
		std::string data;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint32(this->unk1);
			buffer->write_uint32(this->unk2);
			buffer->write_uint32(this->unk3);
			buffer->write_bool(this->priv);
			buffer->write_uint64(this->owner_id);
			buffer->write_string(this->platform);
			buffer->write_string(this->filename);
			buffer->write_blob(this->data);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint32(&this->unk1);
			buffer->read_uint32(&this->unk2);
			buffer->read_uint32(&this->unk3);
			buffer->read_bool(&this->priv);
			buffer->read_uint64(&this->owner_id);
			buffer->read_string(&this->platform);
			buffer->read_string(&this->filename);
			buffer->read_blob(&this->data);
		}
	};

	class bdContextUserStorageFileInfo final : public bdTaskResult
	{
	public:
		std::uint32_t create_time;
		std::uint32_t modifed_time;
		bool priv;
		std::uint64_t owner_id;
		std::string account_type;
		std::string filename;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint32(this->create_time);
			buffer->write_uint32(this->modifed_time);
			buffer->write_bool(this->priv);
			buffer->write_uint64(this->owner_id);
			buffer->write_string(this->account_type);
			buffer->write_string(this->filename);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint32(&this->create_time);
			buffer->read_uint32(&this->modifed_time);
			buffer->read_bool(&this->priv);
			buffer->read_uint64(&this->owner_id);
			buffer->read_string(&this->account_type);
			buffer->read_string(&this->filename);
		}
	};

	class bdPublicProfileInfo final : public bdTaskResult
	{
	public:
		std::uint64_t m_entityID;
		std::string m_memberplayer_card;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint64(this->m_entityID);
			buffer->write_blob(this->m_memberplayer_card);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint64(&this->m_entityID);
			buffer->read_blob(&this->m_memberplayer_card);
		}
	};

	class bdSessionID final : public bdTaskResult
	{
	public:
		uint64_t session_id;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_blob(LPSTR(&this->session_id), sizeof this->session_id);
		}

		void deserialize(byte_buffer* buffer) override
		{
			int size{};
			char* data{};
			buffer->read_blob(&data, &size);

			if (data && uint32_t(size) >= sizeof(this->session_id))
			{
				this->session_id = *reinterpret_cast<uint64_t*>(data);
			}
		}
	};

	class bdMatchMakingInfo final : bdTaskResult
	{
		bdSessionID m_sessionID;
		std::string m_hostAddr;
		uint32_t m_hostAddrSize;
		uint32_t m_gameType;
		uint32_t m_maxPlayers;
		uint32_t m_numPlayers;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_blob(this->m_hostAddr);
			this->m_sessionID.serialize(buffer);
			buffer->write_uint32(this->m_gameType);
			buffer->write_uint32(this->m_maxPlayers);
			buffer->write_uint32(this->m_numPlayers);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_blob(&this->m_hostAddr);
			buffer->read_uint32(&this->m_gameType);
			buffer->read_uint32(&this->m_maxPlayers);
		}
	};

	class bdPerformanceValue final : public bdTaskResult
	{
	public:
		uint64_t user_id;
		int64_t performance;

		void serialize(byte_buffer* buffer) override
		{
			buffer->write_uint64(this->user_id);
			buffer->write_int64(this->performance);
		}

		void deserialize(byte_buffer* buffer) override
		{
			buffer->read_uint64(&this->user_id);
			buffer->read_int64(&this->performance);
		}
	};
}
