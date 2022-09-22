#pragma once

namespace demonware
{
	class bdAsyncMatchMaking final : public service
	{
	public:
		bdAsyncMatchMaking();

	private:
		void unk2(service_server* server, byte_buffer* buffer) const;
		void unk3(service_server* server, byte_buffer* buffer) const;
		void unk6(service_server* server, byte_buffer* buffer) const;
		void unk7(service_server* server, byte_buffer* buffer) const;
	};
}
