#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include <utils/hook.hpp>
#include <utils/thread.hpp>

#include "dvars.hpp"

#include "game/game.hpp"
#include "game/demonware/dw_include.hpp"
#include "game/demonware/servers/lobby_server.hpp"
#include "game/demonware/servers/auth3_server.hpp"
#include "game/demonware/servers/stun_server.hpp"
#include "game/demonware/servers/umbrella_server.hpp"
#include "game/demonware/server_registry.hpp"

#define TCP_BLOCKING true
#define UDP_BLOCKING false

namespace demonware
{
	namespace
	{
		std::atomic_bool exit_server;
		std::thread server_thread;
		utils::concurrency::container<std::unordered_map<SOCKET, bool>> blocking_sockets;
		utils::concurrency::container<std::unordered_map<SOCKET, tcp_server*>> socket_map;
		server_registry<tcp_server> tcp_servers;
		server_registry<udp_server> udp_servers;

		tcp_server* find_server(const SOCKET socket)
		{
			return socket_map.access<tcp_server*>([&](const std::unordered_map<SOCKET, tcp_server*>& map) -> tcp_server*
			{
				const auto entry = map.find(socket);
				if (entry == map.end())
				{
					return nullptr;
				}

				return entry->second;
			});
		}

		bool socket_link(const SOCKET socket, const uint32_t address)
		{
			auto* server = tcp_servers.find(address);
			if (!server)
			{
				return false;
			}

			socket_map.access([&](std::unordered_map<SOCKET, tcp_server*>& map)
			{
				map[socket] = server;
			});

			return true;
		}

		void socket_unlink(const SOCKET socket)
		{
			socket_map.access([&](std::unordered_map<SOCKET, tcp_server*>& map)
			{
				const auto entry = map.find(socket);
				if (entry != map.end())
				{
					map.erase(entry);
				}
			});
		}

		bool is_socket_blocking(const SOCKET socket, const bool def)
		{
			return blocking_sockets.access<bool>([&](std::unordered_map<SOCKET, bool>& map)
			{
				const auto entry = map.find(socket);
				if (entry == map.end())
				{
					return def;
				}

				return entry->second;
			});
		}

		void remove_blocking_socket(const SOCKET socket)
		{
			blocking_sockets.access([&](std::unordered_map<SOCKET, bool>& map)
			{
				const auto entry = map.find(socket);
				if (entry != map.end())
				{
					map.erase(entry);
				}
			});
		}

		void add_blocking_socket(const SOCKET socket, const bool block)
		{
			blocking_sockets.access([&](std::unordered_map<SOCKET, bool>& map)
			{
				map[socket] = block;
			});
		}

		void server_main()
		{
			exit_server = false;

			while (!exit_server)
			{
				tcp_servers.frame();
				udp_servers.frame();
				std::this_thread::sleep_for(50ms);
			}
		}

		namespace io
		{
			int getaddrinfo_stub(const char* name, const char* service,
				const addrinfo* hints, addrinfo** res)
			{
#ifdef DW_DEBUG
				printf("[ network ]: [getaddrinfo]: \"%s\" \"%s\"\n", name, service);
#endif

				base_server* server = tcp_servers.find(name);
				if (!server)
				{
					server = udp_servers.find(name);
				}

				if (!server)
				{
					return getaddrinfo(name, service, hints, res);
				}

				const auto address = utils::memory::get_allocator()->allocate<sockaddr>();
				const auto ai = utils::memory::get_allocator()->allocate<addrinfo>();

				auto in_addr = reinterpret_cast<sockaddr_in*>(address);
				in_addr->sin_addr.s_addr = server->get_address();
				in_addr->sin_family = AF_INET;

				ai->ai_family = AF_INET;
				ai->ai_socktype = SOCK_STREAM;
				ai->ai_addr = address;
				ai->ai_addrlen = sizeof(sockaddr);
				ai->ai_next = nullptr;
				ai->ai_flags = 0;
				ai->ai_protocol = 0;
				ai->ai_canonname = const_cast<char*>(name);

				*res = ai;

				return 0;
			}

			void freeaddrinfo_stub(addrinfo* ai)
			{
				if (!utils::memory::get_allocator()->find(ai))
				{
					return freeaddrinfo(ai);
				}

				utils::memory::get_allocator()->free(ai->ai_addr);
				utils::memory::get_allocator()->free(ai);
			}

			int getpeername_stub(const SOCKET s, sockaddr* addr, socklen_t* addrlen)
			{
				auto* server = find_server(s);

				if (server)
				{
					auto in_addr = reinterpret_cast<sockaddr_in*>(addr);
					in_addr->sin_addr.s_addr = server->get_address();
					in_addr->sin_family = AF_INET;
					*addrlen = sizeof(sockaddr);

					return 0;
				}

				return getpeername(s, addr, addrlen);
			}

			int getsockname_stub(const SOCKET s, sockaddr* addr, socklen_t* addrlen)
			{
				auto* server = find_server(s);

				if (server)
				{
					auto in_addr = reinterpret_cast<sockaddr_in*>(addr);
					in_addr->sin_addr.s_addr = server->get_address();
					in_addr->sin_family = AF_INET;
					*addrlen = sizeof(sockaddr);

					return 0;
				}

				return getsockname(s, addr, addrlen);
			}

			hostent* gethostbyname_stub(const char* name)
			{
#ifdef DW_DEBUG
				printf("[ network ]: [gethostbyname]: \"%s\"\n", name);
#endif

				base_server* server = tcp_servers.find(name);
				if (!server)
				{
					server = udp_servers.find(name);
				}

				if (!server)
				{
#pragma warning(push)
#pragma warning(disable: 4996)
					return gethostbyname(name);
#pragma warning(pop)
				}

				static thread_local in_addr address{};
				address.s_addr = server->get_address();

				static thread_local in_addr* addr_list[2]{};
				addr_list[0] = &address;
				addr_list[1] = nullptr;

				static thread_local hostent host{};
				host.h_name = const_cast<char*>(name);
				host.h_aliases = nullptr;
				host.h_addrtype = AF_INET;
				host.h_length = sizeof(in_addr);
				host.h_addr_list = reinterpret_cast<char**>(addr_list);

				return &host;
			}

			int connect_stub(const SOCKET s, const struct sockaddr* addr, const int len)
			{
				if (len == sizeof(sockaddr_in))
				{
					const auto* in_addr = reinterpret_cast<const sockaddr_in*>(addr);
					if (socket_link(s, in_addr->sin_addr.s_addr)) return 0;
				}

				return connect(s, addr, len);
			}

			int closesocket_stub(const SOCKET s)
			{
				remove_blocking_socket(s);
				socket_unlink(s);

				return closesocket(s);
			}

			int send_stub(const SOCKET s, const char* buf, const int len, const int flags)
			{
				auto* server = find_server(s);

				if (server)
				{
					server->handle_input(buf, len);
					return len;
				}

				return send(s, buf, len, flags);
			}

			int recv_stub(const SOCKET s, char* buf, const int len, const int flags)
			{
				auto* server = find_server(s);

				if (server)
				{
					if (server->pending_data())
					{
						return static_cast<int>(server->handle_output(buf, len));
					}
					else
					{
						WSASetLastError(WSAEWOULDBLOCK);
						return -1;
					}
				}

				return recv(s, buf, len, flags);
			}

			int sendto_stub(const SOCKET s, const char* buf, const int len, const int flags, const sockaddr* to,
				const int tolen)
			{
				const auto* in_addr = reinterpret_cast<const sockaddr_in*>(to);
				auto* server = udp_servers.find(in_addr->sin_addr.s_addr);

				if (server)
				{
					server->handle_input(buf, len, { s, to, tolen });
					return len;
				}

				return sendto(s, buf, len, flags, to, tolen);
			}

			int recvfrom_stub(const SOCKET s, char* buf, const int len, const int flags, struct sockaddr* from,
				int* fromlen)
			{
				// Not supported yet
				if (is_socket_blocking(s, UDP_BLOCKING))
				{
					return recvfrom(s, buf, len, flags, from, fromlen);
				}

				size_t result = 0;
				udp_servers.for_each([&](udp_server& server)
				{
					if (server.pending_data(s))
					{
						result = server.handle_output(
							s, buf, static_cast<size_t>(len), from, fromlen);
					}
				});

				if (result)
				{
					return static_cast<int>(result);
				}

				return recvfrom(s, buf, len, flags, from, fromlen);
			}

			int select_stub(const int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
				struct timeval* timeout)
			{
				if (exit_server)
				{
					return select(nfds, readfds, writefds, exceptfds, timeout);
				}

				auto result = 0;
				std::vector<SOCKET> read_sockets;
				std::vector<SOCKET> write_sockets;

				socket_map.access([&](std::unordered_map<SOCKET, tcp_server*>& sockets)
				{
					for (auto& s : sockets)
					{
						if (readfds)
						{
							if (FD_ISSET(s.first, readfds))
							{
								if (s.second->pending_data())
								{
									read_sockets.push_back(s.first);
									FD_CLR(s.first, readfds);
								}
							}
						}

						if (writefds)
						{
							if (FD_ISSET(s.first, writefds))
							{
								write_sockets.push_back(s.first);
								FD_CLR(s.first, writefds);
							}
						}

						if (exceptfds)
						{
							if (FD_ISSET(s.first, exceptfds))
							{
								FD_CLR(s.first, exceptfds);
							}
						}
					}
				});

				if ((!readfds || readfds->fd_count == 0) && (!writefds || writefds->fd_count == 0))
				{
					timeout->tv_sec = 0;
					timeout->tv_usec = 0;
				}

				result = select(nfds, readfds, writefds, exceptfds, timeout);
				if (result < 0) result = 0;

				for (const auto& socket : read_sockets)
				{
					if (readfds)
					{
						FD_SET(socket, readfds);
						result++;
					}
				}

				for (const auto& socket : write_sockets)
				{
					if (writefds)
					{
						FD_SET(socket, writefds);
						result++;
					}
				}

				return result;
			}

			int ioctlsocket_stub(const SOCKET s, const long cmd, u_long* argp)
			{
				if (static_cast<unsigned long>(cmd) == (FIONBIO))
				{
					add_blocking_socket(s, *argp == 0);
				}

				return ioctlsocket(s, cmd, argp);
			}

			BOOL internet_get_connected_state_stub(LPDWORD, DWORD)
			{
				// Allow offline play
				return TRUE;
			}
		}

#ifdef DW_DEBUG
		void bd_logger_stub(int /*type*/, const char* const /*channelName*/, const char* /*fileLoc*/, const char* const /*file*/,
			const char* const function, const unsigned int /*line*/, const char* const msg, ...)
		{
			char buffer[2048];

			va_list ap;
			va_start(ap, msg);

			vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

			printf("%s: %s\n", function, buffer);

			va_end(ap);
		}
#endif
	}

	class component final : public component_interface
	{
	public:
		component()
		{
			udp_servers.create<stun_server>("genesis.stun.us.demonware.net");
			udp_servers.create<stun_server>("genesis.stun.eu.demonware.net");
			udp_servers.create<stun_server>("genesis.stun.jp.demonware.net");
			udp_servers.create<stun_server>("genesis.stun.au.demonware.net");

			udp_servers.create<stun_server>("stun.us.demonware.net");
			udp_servers.create<stun_server>("stun.eu.demonware.net");
			udp_servers.create<stun_server>("stun.jp.demonware.net");
			udp_servers.create<stun_server>("stun.au.demonware.net");

			tcp_servers.create<auth3_server>("iw-pc-steam-auth3.prod.demonware.net");
			tcp_servers.create<lobby_server>("iw-pc-steam-lobby.prod.demonware.net");
			//tcp_servers.create<qbl_server>("iw-pc-steam-qbl.prod.demonware.net");
			tcp_servers.create<umbrella_server>("prod.umbrella.demonware.net");
		}

		void post_load() override
		{
			server_thread = utils::thread::create_named_thread("Demonware", server_main);
		}

		void* load_import(const std::string& library, const std::string& function) override
		{
			if (library == "WS2_32.dll")
			{
				if (function == "#3") return io::closesocket_stub;
				if (function == "#4") return io::connect_stub;
				if (function == "#5") return io::getpeername_stub;
				if (function == "#6") return io::getsockname_stub;
				if (function == "#10") return io::ioctlsocket_stub;
				if (function == "#16") return io::recv_stub;
				if (function == "#17") return io::recvfrom_stub;
				if (function == "#18") return io::select_stub;
				if (function == "#19") return io::send_stub;
				if (function == "#20") return io::sendto_stub;
				if (function == "#52") return io::gethostbyname_stub;
				if (function == "getaddrinfo") return io::getaddrinfo_stub;
				if (function == "freeaddrinfo") return io::freeaddrinfo_stub;
			}

			if (function == "InternetGetConnectedState")
			{
				return io::internet_get_connected_state_stub;
			}

			return nullptr;
		}

		void post_unpack() override
		{
#ifdef DW_DEBUG
			//utils::hook::jump(0x141285040, bd_logger_stub, true);
#endif

			utils::hook::set<uint8_t>(0x14B5BB96F, 0x0);  // CURLOPT_SSL_VERIFYPEER
			utils::hook::set<uint8_t>(0x14B7C6CB1, 0xAF); // CURLOPT_SSL_VERIFYHOST
			utils::hook::set<uint8_t>(0x1415E4650, 0x0);  // HTTPS -> HTTP

			utils::hook::copy_string(0x1415E8010, "http://dev.umbrella.demonware.net");
			utils::hook::copy_string(0x1415E8038, "http://cert.umbrella.demonware.net");
			utils::hook::copy_string(0x1415E8060, "http://prod.umbrella.demonware.net");
			utils::hook::copy_string(0x1415E8418, "http://dev.uno.demonware.net/v1.0");
			utils::hook::copy_string(0x1415E8440, "http://cert.uno.demonware.net/v1.0");
			utils::hook::copy_string(0x1415E8468, "http://prod.uno.demonware.net/v1.0");
			utils::hook::copy_string(0x1415E3600, "http://%s:%d/auth/");

			// Skip bdAuth::validateResponseSignature
			utils::hook::set(0x14129D200, 0xC301B0); // bdRSAKey::importKey
			utils::hook::set(0x14129D360, 0xC300000001B8); // bdRSAKey::verifySignatureSHA256

			// Remove Online_PatchStreamer checks
			utils::hook::set<uint8_t>(0x14052A6D0, 0xC3);
			utils::hook::set(0x14052AB60, 0xC300000001B8);
			utils::hook::set(0x14052B800, 0xC300000001B8);

			// Remove Online_Dailylogin check
			utils::hook::set(0x140533390, 0xC300000001B8);

			// Increase Demonware connection timeouts
			dvars::override::register_int("demonwareConsideredConnectedTime", 300000, 0, 0x7FFFFFFF, 0x0); // 5s -> 5min
			dvars::override::register_int("dw_addrHandleTimeout", 300000, 0, 0x7FFFFFFF, 0x0); // 5s -> 5min
		}

		void pre_destroy() override
		{
			exit_server = true;
			if (server_thread.joinable())
			{
				server_thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(demonware::component)