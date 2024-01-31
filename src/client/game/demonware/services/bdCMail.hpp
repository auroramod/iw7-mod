#pragma once

namespace demonware
{
	class bdCMail final : public service
	{
	public:
		bdCMail();

	private:
		void getMailInfo(service_server* server, byte_buffer* buffer) const;
	};
}
