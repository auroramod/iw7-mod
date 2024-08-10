#pragma once

namespace demonware
{
	class bdMarketingComms final : public service
	{
	public:
		bdMarketingComms();

	private:
		void reportFullMessagesViewed(service_server* server, byte_buffer* buffer) const;
		void getMessages(service_server* server, byte_buffer* buffer) const;
	};
}
