#pragma once

namespace demonware
{
	class bdUNK123 final : public service
	{
	public:
		bdUNK123();

	private:
		void unk1(service_server* server, byte_buffer* buffer) const;
	};
}
