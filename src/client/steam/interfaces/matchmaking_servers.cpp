#include <std_include.hpp>
#include "../steam.hpp"

namespace steam
{
	void* matchmaking_servers::RequestInternetServerList(unsigned int iApp, void** ppchFilters, unsigned int nFilters,
	                                                     matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(1);
	}

	void* matchmaking_servers::RequestLANServerList(unsigned int iApp,
	                                                matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(2);
	}

	void* matchmaking_servers::RequestFriendsServerList(unsigned int iApp, void** ppchFilters, unsigned int nFilters,
	                                                    matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(3);
	}

	void* matchmaking_servers::RequestFavoritesServerList(unsigned int iApp, void** ppchFilters, unsigned int nFilters,
	                                                      matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(4);
	}

	void* matchmaking_servers::RequestHistoryServerList(unsigned int iApp, void** ppchFilters, unsigned int nFilters,
	                                                    matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(5);
	}

	void* matchmaking_servers::RequestSpectatorServerList(unsigned int iApp, void** ppchFilters, unsigned int nFilters,
	                                                      matchmaking_server_list_response* pRequestServersResponse)
	{
		return reinterpret_cast<void*>(6);
	}

	void matchmaking_servers::ReleaseRequest(void* hServerListRequest)
	{
	}

	gameserveritem_t* matchmaking_servers::GetServerDetails(void* hRequest, int iServer)
	{
		return nullptr;
	}

	void matchmaking_servers::CancelQuery(void* hRequest)
	{
	}

	void matchmaking_servers::RefreshQuery(void* hRequest)
	{
	}

	bool matchmaking_servers::IsRefreshing(void* hRequest)
	{
		return false;
	}

	int matchmaking_servers::GetServerCount(void* hRequest)
	{
		return 0;
	}

	void matchmaking_servers::RefreshServer(void* hRequest, int iServer)
	{
	}

	void* matchmaking_servers::PingServer(unsigned int unIP, unsigned short usPort,
	                                    matchmaking_ping_response* pRequestServersResponse)
	{
		return nullptr;
	}

	int matchmaking_servers::PlayerDetails(unsigned int unIP, unsigned short usPort, void* pRequestServersResponse)
	{
		return 0;
	}

	int matchmaking_servers::ServerRules(unsigned int unIP, unsigned short usPort, void* pRequestServersResponse)
	{
		return 0;
	}

	void matchmaking_servers::CancelServerQuery(int hServerQuery)
	{
	}
}
