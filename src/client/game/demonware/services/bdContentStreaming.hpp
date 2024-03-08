#pragma once

namespace demonware
{
	class bdContentStreaming final : public service
	{
	public:
		bdContentStreaming();

	private:
		void listFilesByOwner(service_server* server, byte_buffer* buffer) const;
		void listAllPublisherFiles(service_server* server, byte_buffer* buffer) const;
	};
}
