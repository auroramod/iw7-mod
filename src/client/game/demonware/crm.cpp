#include <std_include.hpp>
#include "crm.hpp"

namespace demonware::crm
{
	namespace
	{
		// protobuf writer
		class encoder
		{
		public:
			void uint(const uint32_t field, const uint64_t value)
			{
				this->varint(static_cast<uint64_t>(field) << 3 | 0);
				this->varint(value);
			}

			void bytes(const uint32_t field, std::string_view data, const size_t max_length = std::string_view::npos)
			{
				data = data.substr(0, max_length);

				this->varint(static_cast<uint64_t>(field) << 3 | 2);
				this->varint(data.size());
				this->out_.append(data);
			}

			std::string take()
			{
				return std::move(this->out_);
			}

		private:
			void varint(uint64_t value)
			{
				for (; value >= 0x80; value >>= 7)
				{
					this->out_.push_back(static_cast<char>(value | 0x80));
				}
				this->out_.push_back(static_cast<char>(value));
			}

			std::string out_;
		};
	}

	std::string marketing_message::serialize() const
	{
		rapidjson::Document doc;
		doc.SetObject();

		doc.AddMember("title", rapidjson::StringRef(this->title.data(), this->title.size()), doc.GetAllocator());
		doc.AddMember("image", rapidjson::StringRef(this->image.data(), this->image.size()), doc.GetAllocator());
		doc.AddMember("popup_image", rapidjson::StringRef(this->popup_image.data(), this->popup_image.size()), doc.GetAllocator());
		doc.AddMember("content_long", rapidjson::StringRef(this->content_long.data(), this->content_long.size()), doc.GetAllocator());
		doc.AddMember("content_short", rapidjson::StringRef(this->content_short.data(), this->content_short.size()), doc.GetAllocator());
		doc.AddMember("action", rapidjson::StringRef(this->action.data(), this->action.size()), doc.GetAllocator());
		doc.AddMember("action_title", rapidjson::StringRef(this->action_title.data(), this->action_title.size()), doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		return { buffer.GetString(), buffer.GetSize() };
	}

	std::string commercial_message::serialize() const
	{
		encoder e;
		e.uint(1, this->id);                       // m_messageID
		e.bytes(2, this->language, 16);            // m_languageCode
		e.bytes(3, this->content, 4096);           // m_content
		e.bytes(4, this->metadata, 2048);          // m_metadata
		e.bytes(5, this->payload, 1024);           // m_payload
		e.bytes(6, this->payload_signature, 65);   // m_payloadSignature
		e.uint(7, this->assignment_id);            // m_assignmentID
		e.uint(8, this->location);                 // m_locationID
		return e.take();
	}

	std::string serialize(const std::vector<commercial_message>& messages)
	{
		encoder e;
		for (const auto& message : messages)
		{
			e.bytes(1, message.serialize());
		}
		return e.take();
	}
}