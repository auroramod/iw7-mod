#pragma once

namespace demonware
{
	class bdAsyncMatchMaking final : public service
	{
	public:
		bdAsyncMatchMaking();

	private:
		void setPlayerInfo(service_server* server, byte_buffer* buffer) const;
		void getMatchMakingPlayerToken(service_server* server, byte_buffer* buffer) const;
		void initMatchMaking(service_server* server, byte_buffer* buffer) const;
		void startMatchMaking(service_server* server, byte_buffer* buffer) const;
	};
}
