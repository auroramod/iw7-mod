#include <std_include.hpp>
#include "syscon.hpp"
#include "loader/component_loader.hpp"

#include "console.hpp"

#include "game/game.hpp"

#include <utils/thread.hpp>
#include <utils/concurrency.hpp>

namespace
{
	bool is_dark_mode_windows()
	{
		HKEY reg_key;
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_QUERY_VALUE,
			&reg_key) ==
			ERROR_SUCCESS)
		{
			DWORD light_theme_value = 0x1;
			DWORD length = sizeof(light_theme_value);
			RegQueryValueExA(reg_key, "AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&light_theme_value), &length);
			RegCloseKey(reg_key);
			return light_theme_value == 0x0;
		}

		return false;
	}

	static bool darkmode = is_dark_mode_windows();
}

namespace syscon
{
#define CONSOLE_BACKGROUND_COLOR darkmode ? RGB(50, 50, 50) : RGB(255, 255, 255)
#define CONSOLE_TEXT_COLOR darkmode ? RGB(232, 230, 227) : RGB(0, 0, 0)

	// todo:
	// - history
	// - resize

	struct WinConData
	{
		HWND hWnd;
		HWND hwndBuffer;
		HWND codLogo;
		HFONT hfBufferFont;
		HWND hwndInputLine;
		char consoleText[512];
		int windowWidth;
		int windowHeight;
		WNDPROC	SysInputLineWndProc;
		char cleanBuffer[65536];
		_RTL_CRITICAL_SECTION critSect;
	} s_wcd;

	HBRUSH bg_brush;

	HICON icon;
	HANDLE logo;

	LRESULT ConWndProc(const HWND hwnd, const UINT umsg, const WPARAM wparam, const LPARAM lparam)
	{
		switch (umsg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wparam) != WA_INACTIVE)
			{
				SetFocus(s_wcd.hwndInputLine);
			}
			break;
		case WM_CLOSE:
			game::Cbuf_AddCall(0, game::Com_Quit_f);
			DestroyWindow(hwnd);
			return 0;
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORSTATIC:
			SetBkColor(reinterpret_cast<HDC>(wparam), CONSOLE_BACKGROUND_COLOR);
			SetTextColor(reinterpret_cast<HDC>(wparam), CONSOLE_TEXT_COLOR);
			return reinterpret_cast<LONG_PTR>(bg_brush);
		}

		return DefWindowProcA(hwnd, umsg, wparam, lparam);
	}

	unsigned int Conbuf_CleanText(const char* source, char* target, size_t size)
	{
		char* b = target;
		int i = 0;
		while (source && source[i] && (reinterpret_cast<size_t>(b) - reinterpret_cast<size_t>(target)) < (size - 1))
		{
			if (source[i] == '\n' && source[i + 1] == '\r')
			{
				b[0] = '\r';
				b[1] = '\n';
				b += 2;
				i++;
			}
			else if (source[i] == '\r' || source[i] == '\n')
			{
				b[0] = '\r';
				b[1] = '\n';
				b += 2;
			}
			else if (source[0] == '^' && source[1] && source[1] != '^' && source[1] >= 48 && source[1] <= 64) // Q_IsColorString
			{
				i++;
			}
			else
			{
				*b = source[i];
				b++;
			}
			i++;
		}

		*b = '\0';
		return static_cast<unsigned int>(b - target);
	}

	void Conbuf_AppendText(const char* pmsg)
	{
		const char* msg;
		unsigned int buf_len;
		static unsigned int s_total_chars = 0;

		if (s_wcd.hwndBuffer)
		{
			// if the message is REALLY long, use just the last portion of it
			if (strlen(pmsg) > ((sizeof(s_wcd.cleanBuffer) / 2) - 1))
			{
				msg = pmsg + strlen(pmsg) - ((sizeof(s_wcd.cleanBuffer) / 2) + 1);
			}
			else
			{
				msg = pmsg;
			}

			// copy into an intermediate buffer
			buf_len = Conbuf_CleanText(msg, s_wcd.cleanBuffer, sizeof(s_wcd.cleanBuffer));

			s_total_chars += buf_len;

			if (s_total_chars <= sizeof(s_wcd.cleanBuffer))
			{
				SendMessageA(s_wcd.hwndBuffer, EM_SETSEL, 0xFFFF, 0xFFFF);
			}
			else
			{
				SendMessageA(s_wcd.hwndBuffer, EM_SETSEL, 0, -1);
				s_total_chars = buf_len;
			}
			SendMessageA(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xFFFF);
			SendMessageA(s_wcd.hwndBuffer, EM_SCROLLCARET, 0, 0);
			SendMessageA(s_wcd.hwndBuffer, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(s_wcd.cleanBuffer));
		}
	}

	void Sys_Print(const char* msg)
	{
		Conbuf_AppendText(msg);
	}

	LRESULT InputLineWndProc(const HWND hwnd, const UINT umsg, const WPARAM wparam, const LPARAM lparam)
	{
		char dest[sizeof(s_wcd.consoleText) + 8];

		switch (umsg)
		{
		case WM_KILLFOCUS:
			if (reinterpret_cast<HWND>(wparam) == s_wcd.hWnd)
			{
				SetFocus(hwnd);
				return 0;
			}
			break;
		case WM_CHAR:
			const auto key = wparam;

			// enter the line
			if (key == VK_RETURN)
			{
				memset(dest, 0, sizeof(dest));
				memset(s_wcd.consoleText, 0, sizeof(s_wcd.consoleText));

				const auto length = GetWindowTextA(s_wcd.hwndInputLine, s_wcd.consoleText, sizeof(s_wcd.consoleText));
				if (length)
				{
					sprintf_s(dest, sizeof(dest), "]%s\n", s_wcd.consoleText);
					SetWindowTextA(s_wcd.hwndInputLine, "");

					Sys_Print(dest);
					game::Cbuf_AddText(0, s_wcd.consoleText);
				}

				return 0;
			}
			break;
		}

		return CallWindowProcA(s_wcd.SysInputLineWndProc, hwnd, umsg, wparam, lparam);
	}

	void Sys_CreateConsole(const HINSTANCE hinstance)
	{
		RECT rect;
		WNDCLASSA wndclass;
		HDC hdc;
		int nheight;
		int swidth, sheight;
		DWORD DEDSTYLE = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;

		const char* class_name = "IW7-Mod WinConsole";
		const char* window_name = "IW7-Mod Console";

		memset(&rect, 0, sizeof(rect));
		memset(&wndclass, 0, sizeof(rect));
		memset(&hdc, 0, sizeof(hdc));

		memset(&s_wcd, 0, sizeof(s_wcd));

		wndclass.style = 0;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.lpfnWndProc = ConWndProc;
		wndclass.hInstance = hinstance;
		wndclass.hIcon = icon;
		wndclass.hbrBackground = bg_brush;
		wndclass.hCursor = LoadCursorA(0, IDC_ARROW);
		wndclass.lpszMenuName = nullptr;
		wndclass.lpszClassName = class_name;

		if (!RegisterClassA(&wndclass))
		{
			return;
		}

		rect.top = 0;
		rect.left = 0;
		rect.right = 620;
		rect.bottom = 450;
		AdjustWindowRect(&rect, DEDSTYLE, 0);

		hdc = GetDC(GetDesktopWindow());
		swidth = GetDeviceCaps(hdc, HORZRES);
		sheight = GetDeviceCaps(hdc, VERTRES);
		ReleaseDC(GetDesktopWindow(), hdc);

		s_wcd.windowHeight = rect.bottom - rect.top + 1;
		s_wcd.windowWidth = rect.right - rect.left + 1;

		// create main window
		s_wcd.hWnd = CreateWindowExA(
			0,
			class_name,
			window_name,
			DEDSTYLE,
			(swidth - 600) / 2,
			(sheight - 450) / 2,
			rect.right - rect.left + 1,
			rect.bottom - rect.top + 1,
			0,
			0,
			hinstance,
			nullptr);

		if (!s_wcd.hWnd)
		{
			return;
		}

		// create fonts
		hdc = GetDC(s_wcd.hWnd);
		nheight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		s_wcd.hfBufferFont = CreateFontA(
			nheight,
			0,
			0,
			0,
			FW_LIGHT,
			0,
			0,
			0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FF_MODERN | FIXED_PITCH,
			"Courier New");
		ReleaseDC(s_wcd.hWnd, hdc);

		// create logo
		if (logo)
		{
			s_wcd.codLogo = CreateWindowExA(
				0,
				"Static",
				0,
				WS_CHILDWINDOW | WS_VISIBLE | 0xE,
				5,
				5,
				0,
				0,
				s_wcd.hWnd,
				reinterpret_cast<HMENU>(1),
				hinstance,
				nullptr);
			SendMessageA(s_wcd.codLogo, 0x172u, 0, reinterpret_cast<LPARAM>(logo));
		}

		// create the input line
		s_wcd.hwndInputLine = CreateWindowExA(
			0,
			"edit",
			0,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
			6,
			426,
			608,
			20,
			s_wcd.hWnd,
			reinterpret_cast<HMENU>(0x65),
			hinstance,
			0);

		// create the scrollbuffer
		s_wcd.hwndBuffer = CreateWindowExA(
			0,
			"edit",
			0,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
			6,
			70,
			608,
			348,
			s_wcd.hWnd,
			reinterpret_cast<HMENU>(0x64),
			hinstance,
			0);

		SendMessageA(s_wcd.hwndBuffer, WM_SETFONT, reinterpret_cast<WPARAM>(s_wcd.hfBufferFont), 0);
		SendMessageA(s_wcd.hwndBuffer, EM_LIMITTEXT, sizeof(s_wcd.cleanBuffer), 0);
		s_wcd.SysInputLineWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(s_wcd.hwndInputLine, -4,
			reinterpret_cast<LONG_PTR>(InputLineWndProc)));
		SendMessageA(s_wcd.hwndInputLine, WM_SETFONT, reinterpret_cast<WPARAM>(s_wcd.hfBufferFont), 0);
		SetFocus(s_wcd.hwndInputLine);
		SetWindowTextA(s_wcd.hwndBuffer, s_wcd.cleanBuffer);
		InitializeCriticalSection(&s_wcd.critSect);
	}

	void Sys_ShowConsole()
	{
		if (!s_wcd.hWnd)
		{
			Sys_CreateConsole(GetModuleHandleA(reinterpret_cast<LPCSTR>(s_wcd.hWnd)));
		}
		if (s_wcd.hWnd)
		{
			ShowWindow(s_wcd.hWnd, TRUE);
			SendMessageA(s_wcd.hwndBuffer, EM_LINESCROLL, 0, 0xFFFF);
		}
	}

	void Sys_DestroyConsole()
	{
		if (s_wcd.hWnd)
		{
			ShowWindow(s_wcd.hWnd, SW_HIDE);
			CloseWindow(s_wcd.hWnd);
			DestroyWindow(s_wcd.hWnd);
			s_wcd.hWnd = nullptr;
			DeleteCriticalSection(&s_wcd.critSect);
		}
	}

	class component final : public component_interface
	{
	public:
		component()
		{
			syscon::bg_brush = CreateSolidBrush(CONSOLE_BACKGROUND_COLOR);

			const utils::nt::library self;
			syscon::icon = LoadIconA(self.get_handle(), MAKEINTRESOURCEA(ID_ICON));
			syscon::logo = LoadImageA(self.get_handle(), MAKEINTRESOURCEA(IMAGE_LOGO), 0, 0, 0, LR_COPYFROMRESOURCE);

			(void)_pipe(this->handles_, 1024, _O_TEXT);
			(void)_dup2(this->handles_[1], 1);
			(void)_dup2(this->handles_[1], 2);

			//setvbuf(stdout, nullptr, _IONBF, 0);
			//setvbuf(stderr, nullptr, _IONBF, 0);
		}

		~component()
		{
			if (syscon::bg_brush) DeleteObject(syscon::bg_brush);

			if (syscon::icon) DestroyIcon(syscon::icon);
			if (syscon::logo) DeleteObject(syscon::logo);
		}

		void post_start() override
		{
			this->terminate_runner_ = false;
			this->console_runner_ = utils::thread::create_named_thread("Console IO", [this]()
			{
				this->runner();
			});
		}

		void post_unpack() override
		{
			this->initialize();
		}

		void pre_destroy() override
		{
			this->destroy();
		}

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
				syscon::Sys_ShowConsole();

			if (!game::environment::is_dedi())
			{
				// Hide that shit
				ShowWindow(syscon::s_wcd.hWnd, SW_MINIMIZE);
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

		void destroy()
		{
			syscon::Sys_DestroyConsole();

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
			//OutputDebugStringA(message.data());
			syscon::Conbuf_AppendText(message.data());
		}

		void runner()
		{
			char buffer[1024];

			while (!this->terminate_runner_ && this->handles_[0])
			{
				const auto len = _read(this->handles_[0], buffer, sizeof(buffer));
				if (len > 0)
				{
					console::dispatch_message(console::print_type_info, std::string(buffer, len));
				}
				else
				{
					std::this_thread::sleep_for(1ms);
				}
			}

			std::this_thread::yield();
		}
	};

	void set_title(const std::string& title)
	{
		if (syscon::s_wcd.hWnd)
		{
			SetWindowTextA(syscon::s_wcd.hWnd, title.data());
		}
	}
}

namespace syscon
{
	void init()
	{
		// register component
		static component_loader::installer<syscon::component> __component = component_loader::installer<syscon::component>("syscon::component");
	}
}