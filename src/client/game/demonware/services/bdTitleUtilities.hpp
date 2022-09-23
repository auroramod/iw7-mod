#pragma once

namespace demonware
{
	class bdTitleUtilities final : public service
	{
	public:
		bdTitleUtilities();

	private:
		void is_profanity(service_server* server, byte_buffer* buffer) const;
		void get_server_time(service_server* server, byte_buffer* buffer) const;
	};
}
