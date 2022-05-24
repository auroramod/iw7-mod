#include <std_include.hpp>
#include "console.hpp"
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include <utils/flags.hpp>
#include <utils/nt.hpp>
#include <utils/concurrency.hpp>
#include <utils/thread.hpp>
#include <utils/string.hpp>

namespace game_console
{
	void print(int type, const std::string& data);
}

namespace console
{
	bool is_enabled()
	{
		return game::environment::is_dedi() || !utils::flags::has_flag("noconsole");
	}

	namespace native
	{
		bool is_enabled()
		{
			return utils::flags::has_flag("nativeconsole");
		}

		static bool ingame = false;
		static bool exit = false;

		DWORD WINAPI console(LPVOID)
		{
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			SetConsoleTitle("IW7-Mod");

			std::string cmd;
			exit = false;

			while (!exit)
			{
				std::getline(std::cin, cmd);
				if (ingame)
				{
					game::Cbuf_AddText(0, cmd.data());
				}
			}

			return 0;
		}
	}

	namespace sys
	{
#define COMMAND_HISTORY 64

		struct WinConData
		{
			HWND hWnd;
			HWND hwndBuffer;
			HWND codLogo;
			HFONT hfBufferFont;
			HWND hwndInputLine;
			//char errorString[512];
			char consoleText[512];
			//char returnedText[512];
			char consoleHistory[COMMAND_HISTORY][512];
			int consoleHistoryPos;
			int consoleHistoryCount;
			int windowWidth;
			int windowHeight;
			WNDPROC	SysInputLineWndProc;
			char cleanBuffer[65536];
			//char buffer[32768];
			//unsigned int bufferLen;
			_RTL_CRITICAL_SECTION critSect;
			//bool disabled;
		} s_wcd;

		HICON icon;
		HANDLE logo;

		const char* ClassName = "IW7-Mod WinConsole";
		const char* WindowName = "IW7-Mod Console";

		LRESULT ConWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			case WM_SIZE:
				// todo: window size?
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			case WM_ACTIVATE:
				if (LOWORD(wParam) != WA_INACTIVE)
					SetFocus(s_wcd.hwndInputLine);
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			case WM_CLOSE:
				DestroyWindow(hWnd);
				return 0;
			default:
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			}
		}

		void __fastcall Conbuf_AppendText(const char* pMsg)
		{
			char* b = s_wcd.cleanBuffer;
			const char* msg;
			unsigned int bufLen;
			int i = 0;
			static unsigned int s_totalChars;

			if (console::is_enabled() && s_wcd.hwndBuffer)
			{
				// if the message is REALLY long, use just the last portion of it
				if (strlen(pMsg) > sizeof(s_wcd.cleanBuffer) - 1) 
				{
					msg = pMsg + strlen(pMsg) - sizeof(s_wcd.cleanBuffer) + 1;
				}
				else 
				{
					msg = pMsg;
				}

				// copy into an intermediate buffer
				while (msg[i] && ((b - s_wcd.cleanBuffer) < sizeof(s_wcd.cleanBuffer) - 1)) 
				{
					if (msg[i] == '\n' && msg[i + 1] == '\r') 
					{
						b[0] = '\r';
						b[1] = '\n';
						b += 2;
						i++;
					}
					else if (msg[i] == '\r') 
					{
						b[0] = '\r';
						b[1] = '\n';
						b += 2;
					}
					else if (msg[i] == '\n') 
					{
						b[0] = '\r';
						b[1] = '\n';
						b += 2;
					}
					// else if (idStr::IsColor(&msg[i])) 
					//{
					//	i++;
					//}
					else 
					{
						*b = msg[i];
						b++;
					}
					i++;
				}

				*b = '\0';
				bufLen = static_cast<unsigned int>(b - s_wcd.cleanBuffer);
				s_totalChars += bufLen;

				if (s_totalChars  <= 0x8000)
				{
					SendMessageA(s_wcd.hwndBuffer, EM_SETSEL, 0xFFFF, 0xFFFF);
				}
				else
				{
					SendMessageA(s_wcd.hwndBuffer, EM_SETSEL, 0, -1);
					s_totalChars = bufLen;
				}
				SendMessageA(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xFFFF);
				SendMessageA(s_wcd.hwndBuffer, EM_SCROLLCARET, 0, 0);
				SendMessageA(s_wcd.hwndBuffer, EM_REPLACESEL, 0, (LPARAM)s_wcd.cleanBuffer);
			}
		}

		void Sys_Print(const char* msg)
		{
			if (console::is_enabled())
			{
				Conbuf_AppendText(msg);
			}
		}

		LRESULT InputLineWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			char dest[sizeof(s_wcd.consoleText) + 8];

			switch (uMsg)
			{
			case WM_KILLFOCUS:
				if ((HWND)wParam != s_wcd.hWnd)
				{
					return CallWindowProcA(s_wcd.SysInputLineWndProc, hWnd, uMsg, wParam, lParam);
				}
				SetFocus(hWnd);
				return 0;
			case WM_KEYDOWN:
				// todo: add history
				break;
			case WM_CHAR:
				auto key = wParam;

				// enter the line
				if (key == VK_RETURN)
				{
					memset(dest, 0, sizeof(dest));
					memset(s_wcd.consoleText, 0, sizeof(s_wcd.consoleText));

					auto length = GetWindowTextA(s_wcd.hwndInputLine, s_wcd.consoleText, sizeof(s_wcd.consoleText));
					if (length)
					{
						sprintf_s(dest, sizeof(s_wcd.consoleText), "]%s\n", s_wcd.consoleText);
						SetWindowTextA(s_wcd.hwndInputLine, "");

						Sys_Print(dest);
						game::Cbuf_AddText(0, s_wcd.consoleText);

						// todo: add history
					}

					return 0;
				}
				break;
			}

			return CallWindowProcA(s_wcd.SysInputLineWndProc, hWnd, uMsg, wParam, lParam);
		}

		void Sys_CreateConsole(HINSTANCE hInstance)
		{
			tagRECT Rect;
			WNDCLASSA WndClass;

			memset(&Rect, 0, sizeof(Rect));
			memset(&WndClass, 0, sizeof(WndClass));

			memset(&s_wcd, 0, sizeof(s_wcd));

			WndClass.style = 0;
			WndClass.cbClsExtra = 0;
			WndClass.lpfnWndProc = ConWndProc;
			WndClass.hInstance = hInstance;
			WndClass.hIcon = icon;
			WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			WndClass.hCursor = LoadCursorA(0, (LPCSTR)0x7F00);
			WndClass.lpszMenuName = NULL;
			WndClass.lpszClassName = ClassName;
			if (RegisterClassA(&WndClass))
			{
				Rect.top = 0;
				Rect.left = 0;
				Rect.right = 620;
				Rect.bottom = 450;
				AdjustWindowRect(&Rect, 0x80CA0000, 0);
				auto v4 = GetDesktopWindow();
				auto v5 = GetDC(v4);
				auto v6 = GetDeviceCaps(v5, 8);
				auto v7 = GetDeviceCaps(v5, 10);
				auto v8 = GetDesktopWindow();
				ReleaseDC(v8, v5);
				s_wcd.windowHeight = Rect.bottom - Rect.top + 1;
				s_wcd.windowWidth = Rect.right - Rect.left + 1;
				s_wcd.consoleHistoryPos = 0;
				auto v9 = CreateWindowExA(
					0,
					ClassName,
					WindowName,
					0x80CA0000,
					(v6 - 600) / 2,
					(v7 - 450) / 2,
					Rect.right - Rect.left + 1,
					Rect.bottom - Rect.top + 1,
					0,
					0,
					hInstance,
					0);
				s_wcd.hWnd = v9;
				if (v9)
				{
					auto v10 = GetDC(v9);
					auto v11 = GetDeviceCaps(v10, 90);
					auto v12 = MulDiv(8, v11, 72);
					s_wcd.hfBufferFont = CreateFontA(-v12, 0, 0, 0, 300, 0, 0, 0, 1u, 0, 0, 0, 0x31u, "Courier New");
					ReleaseDC(s_wcd.hWnd, v10);
					auto v13 = logo;
					if (v13)
					{
						s_wcd.codLogo = CreateWindowExA(
							0,
							"Static",
							0,
							0x5000000Eu,
							5,
							5,
							0,
							0,
							s_wcd.hWnd,
							(HMENU)1,
							hInstance,
							0);
						SendMessageA(s_wcd.codLogo, 0x172u, 0, (LPARAM)v13);
					}
					s_wcd.hwndInputLine = CreateWindowExA(
						0,
						"edit",
						0,
						0x50800080u,
						6,
						400,
						608,
						20,
						s_wcd.hWnd,
						(HMENU)0x65,
						hInstance,
						0);
					s_wcd.hwndBuffer = CreateWindowExA(
						0,
						"edit",
						0,
						0x50A00844u,
						6,
						70,
						606,
						324,
						s_wcd.hWnd,
						(HMENU)0x64,
						hInstance,
						0);
					SendMessageA(s_wcd.hwndBuffer, 0x30u, (WPARAM)s_wcd.hfBufferFont, 0);
					SendMessageA(s_wcd.hwndBuffer, 0xC5u, 0x8000u, 0);
					s_wcd.SysInputLineWndProc = (WNDPROC)SetWindowLongPtrA(s_wcd.hwndInputLine, -4, (LONG_PTR)InputLineWndProc);
					SendMessageA(s_wcd.hwndInputLine, 0x30u, (WPARAM)s_wcd.hfBufferFont, 0);
					SetFocus(s_wcd.hwndInputLine);
					//s_wcd.buffer[0] = 0;
					//s_wcd.cleanBuffer[0] = 0;
					SetWindowTextA(s_wcd.hwndBuffer, s_wcd.cleanBuffer);
					//s_wcd.bufferLen = 0;
					InitializeCriticalSection(&s_wcd.critSect);
				}
			}
		}

		void Sys_ShowConsole()
		{
			if (!s_wcd.hWnd)
			{
				Sys_CreateConsole(GetModuleHandleA((LPCSTR)s_wcd.hWnd));
			}
			if (s_wcd.hWnd)
			{
				ShowWindow(s_wcd.hWnd, TRUE);
				SendMessageA(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xFFFF);
			}
		}
	}

	std::string format(va_list* ap, const char* message)
	{
		static thread_local char buffer[0x1000];

		const auto count = _vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer), message, *ap);

		if (count < 0) return {};
		return { buffer, static_cast<size_t>(count) };
	}

	void dispatch_message(const int type, const std::string& message)
	{
		if (console::is_enabled())
		{
			if (native::is_enabled())
			{
				printf("%s\n", message.data());
			}
			else
			{
				sys::Sys_Print(utils::string::va("%s\n", message.data()));
			}
		}
		
		game_console::print(type, message);
	}

	void print(const int type, const char* fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);
		const auto result = format(&ap, fmt);
		va_end(ap);

		dispatch_message(type, result);
	}

	class component final : public component_interface
	{
	public:
		component()
		{
			if (!console::is_enabled() || native::is_enabled()) return;

			//hide_console();

			(void)_pipe(this->handles_, 1024, _O_TEXT);
			(void)_dup2(this->handles_[1], 1);
			(void)_dup2(this->handles_[1], 2);

			//setvbuf(stdout, nullptr, _IONBF, 0);
			//setvbuf(stderr, nullptr, _IONBF, 0);
		}

		void post_start() override
		{
			if (console::is_enabled())
			{
				if (native::is_enabled())
				{
					CreateThread(0, 0, native::console, 0, 0, 0);
				}
				else
				{
					const utils::nt::library self;
					sys::icon = LoadIconA(self.get_handle(), MAKEINTRESOURCEA(ID_ICON));
					sys::logo = LoadImageA(self.get_handle(), MAKEINTRESOURCEA(IMAGE_LOGO), 0, 0, 0, LR_COPYFROMRESOURCE);

					this->terminate_runner_ = false;
					this->console_runner_ = utils::thread::create_named_thread("Console IO", [this]
					{
						this->runner();
					});
				}
			}
		}

		void post_unpack() override
		{
			if (console::is_enabled())
			{
				if (native::is_enabled())
				{
					native::ingame = true;
				}
				else
				{
					this->initialize();
				}
			}
		}

		void pre_destroy() override
		{
			if (console::is_enabled())
			{
				if (native::is_enabled())
				{
					native::ingame = false;
					native::exit = true;
				}
				else
				{
					this->terminate_runner_ = true;

					printf("\r\n");
					_flushall();

					if (this->console_runner_.joinable())
					{
						this->console_runner_.join();
					}

					if (this->console_thread_.joinable())
					{
						this->console_thread_.join();
					}

					_close(this->handles_[0]);
					_close(this->handles_[1]);

					messages.access([&](message_queue& msgs)
					{
						msgs = {};
					});
				}
			}
		}

		// only needed for sys_console
	private:
		volatile bool console_initialized_ = false;
		volatile bool terminate_runner_ = false;

		std::thread console_runner_;
		std::thread console_thread_;

		int handles_[2]{};

		using message_queue = std::queue<std::string>;
		utils::concurrency::container<message_queue> messages;

		void initialize()
		{
			this->console_thread_ = utils::thread::create_named_thread("Console", [this]()
			{
				sys::Sys_ShowConsole();

				if (!game::environment::is_dedi())
				{
					// Hide that shit
					ShowWindow(console::get_window(), SW_MINIMIZE);
				}

				{
					messages.access([&](message_queue&)
					{
						this->console_initialized_ = true;
					});
				}

				MSG msg;
				while (!this->terminate_runner_)
				{
					if (PeekMessageA(&msg, nullptr, NULL, NULL, PM_REMOVE))
					{
						//if (msg.message == WM_QUIT)
						//{
						//	game::Cbuf_AddText(0, "quit");
						//	break;
						//}

						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					else
					{
						this->log_messages();
						std::this_thread::sleep_for(1ms);
					}
				}
			});
		}

		void log_messages()
		{
			/*while*/
			if (this->console_initialized_ && !messages.get_raw().empty())
			{
				std::queue<std::string> message_queue_copy;

				{
					messages.access([&](message_queue& msgs)
					{
						message_queue_copy = std::move(msgs);
						msgs = {};
					});
				}

				while (!message_queue_copy.empty())
				{
					log_message(message_queue_copy.front());
					message_queue_copy.pop();
				}
			}

			fflush(stdout);
			fflush(stderr);
		}

		static void log_message(const std::string& message)
		{
			OutputDebugStringA(message.data());
			sys::Conbuf_AppendText(message.data());
		}

		void runner()
		{
			char buffer[1024];

			while (!this->terminate_runner_ && this->handles_[0])
			{
				const auto len = _read(this->handles_[0], buffer, sizeof(buffer));
				if (len > 0)
				{
					dispatch_message(con_type_info, std::string(buffer, len));
				}
				else
				{
					std::this_thread::sleep_for(1ms);
				}
			}

			std::this_thread::yield();
		}
	};

	HWND get_window()
	{
		return sys::s_wcd.hWnd;
	}

	void set_title(std::string title)
	{
		SetWindowTextA(get_window(), title.data());
	}

	void set_size(const int width, const int height)
	{
		RECT rect;
		GetWindowRect(get_window(), &rect);

		SetWindowPos(get_window(), nullptr, rect.left, rect.top, width, height, 0);

		auto* const logo_window = sys::s_wcd.codLogo;
		SetWindowPos(logo_window, nullptr, 5, 5, width - 25, 60, 0);
	}
}

REGISTER_COMPONENT(console::component)
