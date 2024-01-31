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
					return !memcmp(a->ip, b->ip, 4);
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
			return net_compare_base_address(a, b) && a->port == b->port;
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
			utils::hook::invoke<void>(0xD57A00_b);
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
			utils::hook::jump(0xD942C0_b, dw_send_to_stub);

			// redirect dw packet receives to our stub
			utils::hook::jump(0xD93D70_b, dw_recv_from_stub);

			// intercept command handling
			cl_dispatch_connectionless_packet_hook.create(0x9B2250_b, cl_dispatch_connectionless_packet_stub);

			// handle xuid without secure connection
			utils::hook::nop(0xC53315_b, 2);
			utils::hook::nop(0xC55EC7_b, 6);

			utils::hook::jump(game::NET_CompareAdr, net_compare_address);
			utils::hook::jump(game::NET_CompareBaseAdr, net_compare_base_address);

			// don't establish secure conenction
			utils::hook::set<uint8_t>(0x9DBFDD_b, 0xEB);
			utils::hook::set<uint8_t>(0x9DC47D_b, 0xEB);
			utils::hook::set<uint8_t>(0x9DDC79_b, 0xEB);
			utils::hook::set<uint8_t>(0x9AA9F9_b, 0xEB);
			utils::hook::set<uint8_t>(0xC56030_b, 0xEB);
			utils::hook::set<uint8_t>(0xC5341A_b, 0xEB);
			utils::hook::set<uint8_t>(0xC4FFC6_b, 0xEB);
			utils::hook::set<uint8_t>(0xC533B4_b, 0xEB);

			// ignore unregistered connection
			utils::hook::jump(0xC4F200_b, 0xC4F1AB_b);
			utils::hook::jump(0xC4F2F6_b, 0xC4F399_b);

			// ignore configstring mismatch
			utils::hook::set<uint8_t>(0x9B6F91_b, 0xEB);

			// ignore dw handle in SvClientMP::FindClientAtAddress
			utils::hook::set<uint8_t>(0xC58B2B_b, 0xEB);

			// ignore dw handle in SV_DirectConnect
			utils::hook::nop(0xC4EE1A_b, 2);
			utils::hook::nop(0xC4F0FB_b, 6);

			// ignore impure client
			utils::hook::jump(0xC500C8_b, 0xC500DE_b); // maybe add sv_pure dvar?

			// don't send checksum
			utils::hook::set<uint8_t>(0xCE6C7C_b, 0x0);

			// don't read checksum
			utils::hook::set(0xCE6E60_b, 0xC301B0);

			// don't try to reconnect client
			utils::hook::call(0xC4F05F_b, reconnect_migratated_client);
			utils::hook::nop(0xC4F03C_b, 4); // this crashes when reconnecting for some reason

			// increase allowed packet size
			const auto max_packet_size = 0x20000;
			utils::hook::set<int>(0xBB4F01_b, max_packet_size);
			utils::hook::set<int>(0xBB4F31_b, max_packet_size);
			utils::hook::set<int>(0xBB4E22_b, max_packet_size);
			utils::hook::set<int>(0xBB4F31_b, max_packet_size);

			// ignore built in "print" oob command and add in our own
			utils::hook::set<uint8_t>(0x9B0326_b, 0xEB);
			network::on("print", [](const game::netadr_s&, const std::string_view& data)
			{
				const std::string message{ data };
				console::info(message.data());
			});

			// initialize query_socket
			utils::hook::jump(0xD57C7E_b, net_init_stub);

			// use our own protocol version
			utils::hook::jump(0xCE8290_b, get_protocol_version_stub);
		}
	};
}

REGISTER_COMPONENT(network::component)