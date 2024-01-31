#pragma once

namespace demonware
{
	class bdTitleUtilities final : public service
	{
	public:
		bdTitleUtilities();

	private:
		void isProfanity(service_server* server, byte_buffer* buffer) const;
		void getServerTime(service_server* server, byte_buffer* buffer) const;
	};
}
