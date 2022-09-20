#pragma once

namespace demonware
{
	class bdCMail final : public service
	{
	public:
		bdCMail();

	private:
		void unk10(service_server* server, byte_buffer* buffer) const;
	};
}
