#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "network.hpp"

#include "game/game.hpp"
#include "game/utils/fragment_handler.hpp"

#include "console/console.hpp"
#include "dvars.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace network
{
	std::unordered_map<std::string, callback>& get_callbacks()
	{
		static std::unordered_map<std::string, callback> callbacks{};
		return callbacks;
	}

	void on(const std::string& command, const callback& callback)
	{
		get_callbacks()[utils::string::to_lower(command)] = callback;
	}

	bool handle_command(game::netadr_s* address, const char* command, game::msg_t* message)
	{
		const auto cmd_string = utils::string::to_lower(command);
		auto& callbacks = get_callbacks();
		const auto handler = callbacks.find(cmd_string);
		const auto offset = cmd_string.size() + 5;
		if (message->cursize < 0 || static_cast<size_t>(message->cursize) < offset || handler == callbacks.end())
		{
			return false;
		}

		const std::basic_string_view data(message->data + offset, message->cursize - offset);

		console::debug("[network] handling command \"%s\"\n", cmd_string.data());

		try
		{
			handler->second(*address, data);
		}
		catch (const std::exception& e)
		{
			printf("Error: %s\n", e.what());
		}
		//catch (...)
		//{
		//}

		return true;
	}

	namespace
	{
		utils::hook::detour cl_dispatch_connectionless_packet_hook;
		bool cl_dispatch_connectionless_packet_stub(int client_num, game::netadr_s* from, game::msg_t* msg, int time)
		{
			if (handle_command(from, game::Cmd_Argv(0), msg))
			{
				return true;
			}

			return cl_dispatch_connectionless_packet_hook.invoke<bool>(client_num, from, msg, time);
		}

		int dw_send_to_stub(const int length, const char* data, game::netadr_s* to)
		{
			sockaddr s = {};
			game::NetadrToSockadr(to, &s);
			return sendto(*game::query_socket, data, length, 0, &s, sizeof(sockaddr));
		}

		void sockadr_to_netadr(const sockaddr* s, game::netadr_s* a)
		{
			if (s->sa_family == 2)
			{
				a->type = game::NA_IP;
				*(int*)&a->ip = *(int*)&s->sa_data[2];
				a->port = *(unsigned short*)(&s->sa_data[0]);
			}
		}

		int dw_recv_from_stub(game::netadr_s* from, char* data, int maxsize)
		{
			sockaddr s = {};
			int slen = sizeof(sockaddr);
			int ret;
			int datalen;
			ret = recvfrom(*game::query_socket, data, maxsize, 0, &s, &slen);
			if (ret == SOCKET_ERROR)
			{
				return -2;
			}
			sockadr_to_netadr(&s, from);
			datalen = ret;
			if (!datalen)
			{
				console::warn("Sys_GetPacket: Empty or discarded packet from %s\n", net_adr_to_string(*from));
				return 0;
			}
			if (datalen == maxsize)
			{
				console::warn("Sys_GetPacket: Oversize packet from %s\n", net_adr_to_string(*from));
				return 0;
			}
			return datalen;
		}

		int net_compare_base_address(const game::netadr_s* a, const game::netadr_s* b)
		{
			if (a->type == b->type)
			{
				switch (a->type)
				{
				case game::netadrtype_t::NA_BOT:
				case game::netadrtype_t::NA_LOOPBACK:
					return a->port == b->port;

				case game::netadrtype_t::NA_IP:
					return memcmp(a->ip, b->ip, 4) == 0;
				case game::netadrtype_t::NA_BROADCAST:
					return true;
				default:
					break;
				}
			}

			return false;
		}

		int net_compare_address(const game::netadr_s* a, const game::netadr_s* b)
		{
			return net_compare_base_address(a, b) != 0 && (a->port == b->port);
		}

		void string_to_sockaddr(const char* str, sockaddr_in* address)
		{
			game::netadr_s addr{};
			game::NET_StringToAdr(str, &addr);
			game::NetadrToSockadr(&addr, reinterpret_cast<sockaddr*>(address));
		}

		SOCKET create_socket(const char* net_interface, int port, int protocol)
		{
			sockaddr_in address{};

			if (net_interface && net_interface != "localhost"s)
			{
				string_to_sockaddr(net_interface, &address);
			}

			address.sin_family = AF_INET;
			address.sin_port = ntohs(static_cast<short>(port));

			const auto sock = socket(AF_INET, SOCK_DGRAM, protocol);

			if (sock == INVALID_SOCKET)
			{
				return 0;
			}

			u_long arg = 1;
			if (ioctlsocket(sock, FIONBIO, &arg) == SOCKET_ERROR)
			{
				return 0;
			}

			char optval[4] = { 1, 0, 0, 0 };
			if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, optval, sizeof(optval)) == SOCKET_ERROR)
			{
				return 0;
			}

			if (bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
			{
				closesocket(sock);
				return 0;
			}

			return sock;
		}

		void init_socket()
		{
			const auto net_ip = game::Dvar_FindVar("net_ip");
			const auto net_port = game::Dvar_FindVar("net_port");

			auto port_diff = 0;
			for (port_diff = 0; port_diff < 10; port_diff++)
			{
				*game::query_socket = create_socket(
					net_ip->current.string, net_port->current.integer + port_diff, IPPROTO_UDP);
				if (*game::query_socket)
				{
					break;
				}
			}

			if (!*game::query_socket)
			{
				console::warn("WARNING: Couldn't allocate IP/UDP port, LAN discovery will not work!\n");
				return;
			}

			game::Dvar_SetInt(net_port, net_port->current.integer + port_diff);
		}

		void net_init_stub()
		{
			init_socket();
			utils::hook::invoke<void>(0x140D57A00);
		}

		int get_protocol_version_stub()
		{
			return PROTOCOL;
		}

		void reconnect_migratated_client(void*, game::netadr_s* from, const int, const int, const char*,
			const char*, bool)
		{
			// This happens when a client tries to rejoin after being recently disconnected, OR by a duplicated guid
			// We don't want this to do anything. It decides to crash seemingly randomly
			// Rather than try and let the player in, just tell them they are a duplicate player and reject connection
			game::NET_OutOfBandPrint(game::NS_SERVER, from, "error\nYou are already connected to the server.");
		}

		void* memmove_stub(void* dst, void* src, size_t size)
		{
			return std::memmove(dst, src, std::min(size, 1262ull));
		}
	}

	void send(const game::netadr_s& address, const std::string& command, const std::string& data, const char separator)
	{
		std::string packet = "\xFF\xFF\xFF\xFF";
		packet.append(command);
		packet.push_back(separator);
		packet.append(data);

		send_data(address, packet);
	}

	void send_data(const game::netadr_s& address, const std::string& data)
	{
		auto size = static_cast<int>(data.size());
		if (size > 1280)
		{
			console::error("Packet was too long. Truncated!\n");
			size = 1280;
		}

		if (address.type == game::NA_LOOPBACK)
		{
			game::NET_SendLoopPacket(game::NS_CLIENT1, size, data.data(), &address);
		}
		else
		{
			game::Sys_SendPacket(size, data.data(), &address);
		}
	}

	bool are_addresses_equal(const game::netadr_s& a, const game::netadr_s& b)
	{
		return net_compare_address(&a, &b);
	}

	const char* net_adr_to_string(const game::netadr_s& a)
	{
		if (a.type == game::netadrtype_t::NA_LOOPBACK)
		{
			return "loopback";
		}

		if (a.type == game::netadrtype_t::NA_BOT)
		{
			return "bot";
		}

		if (a.type == game::netadrtype_t::NA_IP || a.type == game::netadrtype_t::NA_BROADCAST)
		{
			if (a.port)
			{
				return utils::string::va("%u.%u.%u.%u:%u", a.ip[0], a.ip[1], a.ip[2], a.ip[3], htons(a.port));
			}

			return utils::string::va("%u.%u.%u.%u", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
		}

		return "bad";
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scheduler::loop(game::fragment_handler::clean, scheduler::async, 5s);

			// redirect dw packet sends to our stub
			utils::hook::jump(0x140D942C0, dw_send_to_stub);

			// redirect dw packet receives to our stub
			utils::hook::jump(0x140D93D70, dw_recv_from_stub);

			// intercept command handling
			cl_dispatch_connectionless_packet_hook.create(0x1409B2250, cl_dispatch_connectionless_packet_stub);

			// handle xuid without secure connection
			utils::hook::nop(0x140C53315, 2);
			utils::hook::nop(0x140C55EC7, 6);

			utils::hook::jump(game::NET_CompareAdr, net_compare_address);
			utils::hook::jump(game::NET_CompareBaseAdr, net_compare_base_address);

			// don't establish secure conenction
			utils::hook::set<uint8_t>(0x1409DBFDD, 0xEB);
			utils::hook::set<uint8_t>(0x1409DC47D, 0xEB);
			utils::hook::set<uint8_t>(0x1409DDC79, 0xEB);
			utils::hook::set<uint8_t>(0x1409AA9F9, 0xEB);
			utils::hook::set<uint8_t>(0x140C56030, 0xEB);
			utils::hook::set<uint8_t>(0x140C5341A, 0xEB);
			utils::hook::set<uint8_t>(0x140C4FFC6, 0xEB);
			utils::hook::set<uint8_t>(0x140C533B4, 0xEB);

			// ignore unregistered connection
			utils::hook::jump(0x140C4F200, 0x140C4F1AB);
			utils::hook::jump(0x140C4F2F6, 0x140C4F399);

			// ignore configstring mismatch
			utils::hook::set<uint8_t>(0x1409B6F91, 0xEB);

			// ignore dw handle in SvClientMP::FindClientAtAddress
			utils::hook::set<uint8_t>(0x140C58B2B, 0xEB);

			// ignore dw handle in SV_DirectConnect
			utils::hook::nop(0x140C4EE1A, 2);
			utils::hook::nop(0x140C4F0FB, 6);

			// ignore impure client
			utils::hook::jump(0x140C500C8, 0x140C500DE); // maybe add sv_pure dvar?

			// don't send checksum
			utils::hook::set<uint8_t>(0x140CE6C7C, 0x0);

			// don't read checksum
			utils::hook::set(0x140CE6E60, 0xC301B0);

			// don't try to reconnect client
			utils::hook::call(0x140C4F05F, reconnect_migratated_client);
			utils::hook::nop(0x140C4F03C, 4); // this crashes when reconnecting for some reason

			// increase allowed packet size
			const auto max_packet_size = 0x20000;
			utils::hook::set<int>(0x140BB4F01, max_packet_size);
			utils::hook::set<int>(0x140BB4F31, max_packet_size);
			utils::hook::set<int>(0x140BB4E22, max_packet_size);
			utils::hook::set<int>(0x140BB4F31, max_packet_size);

			// increase cl_maxpackets
			dvars::override::register_int("cl_maxpackets", 1000, 1, 1000, game::DVAR_FLAG_NONE);

			// increase snaps
			dvars::override::register_int("sv_remote_client_snapshot_msec", 33, -1, 100, game::DVAR_FLAG_NONE);

			// disable snapshot default values overriding the actual values every time the server starts
			utils::hook::set<uint8_t>(0x140C56780, 0xC3); // SV_SnapshotMP_InitRuntime

			// ignore built in "print" oob command and add in our own
			utils::hook::set<uint8_t>(0x1409B0326, 0xEB);

			// initialize query_socket
			utils::hook::jump(0x140D57C7E, net_init_stub);

			// use our own protocol version
			utils::hook::jump(0x140CE8290, get_protocol_version_stub);

			// patch buffer overflow
			utils::hook::call(0x140BB4ABB, memmove_stub); // NET_DeferPacketToClient
		}
	};
}

REGISTER_COMPONENT(network::component)