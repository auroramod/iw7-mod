#include <std_include.hpp>

#ifdef _DEBUG
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <component/console/console.hpp>
#include <component/scheduler.hpp>

#include "gui.hpp"
#include "kiero.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

#include <asmjit/asmjit.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gui
{
	std::unordered_map<std::string, bool> enabled_menus;

	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ID3D11RenderTargetView* render_target_view;

	namespace
	{
		struct frame_callback
		{
			std::function<void()> callback;
			bool always;
		};

		struct event
		{
			HWND hWnd;
			UINT msg;
			WPARAM wParam;
			LPARAM lParam;
		};

		struct menu_t
		{
			std::string name;
			std::string title;
			std::function<void()> render;
		};

		utils::concurrency::container<std::vector<frame_callback>> on_frame_callbacks;
		utils::concurrency::container<std::deque<notification_t>> notifications;
		utils::concurrency::container<std::vector<event>> event_queue;
		std::vector<menu_t> menus;

		bool initialized = false;
		bool toggled = false;

		static bool attached = false;

		static HWND window = NULL;

		void initialize_gui_context()
		{
			ImGui::CreateContext();

			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(device, device_context);

			initialized = true;
		}

		void run_event_queue()
		{
			event_queue.access([](std::vector<event>& queue)
			{
				for (const auto& event : queue)
				{
					ImGui_ImplWin32_WndProcHandler(event.hWnd, event.msg, event.wParam, event.lParam);
				}

				queue.clear();
			});
		}

		std::vector<int> imgui_colors =
		{
			ImGuiCol_FrameBg,
			ImGuiCol_FrameBgHovered,
			ImGuiCol_FrameBgActive,
			ImGuiCol_TitleBgActive,
			ImGuiCol_ScrollbarGrabActive,
			ImGuiCol_CheckMark,
			ImGuiCol_SliderGrab,
			ImGuiCol_SliderGrabActive,
			ImGuiCol_Button,
			ImGuiCol_ButtonHovered,
			ImGuiCol_ButtonActive,
			ImGuiCol_Header,
			ImGuiCol_HeaderHovered,
			ImGuiCol_HeaderActive,
			ImGuiCol_SeparatorHovered,
			ImGuiCol_SeparatorActive,
			ImGuiCol_ResizeGrip,
			ImGuiCol_ResizeGripHovered,
			ImGuiCol_ResizeGripActive,
			ImGuiCol_TextSelectedBg,
			ImGuiCol_NavHighlight,
		};

		void update_colors()
		{
			auto& style = ImGui::GetStyle();
			const auto colors = style.Colors;

			const auto now = std::chrono::system_clock::now();
			const auto days = std::chrono::floor<std::chrono::days>(now);
			std::chrono::year_month_day y_m_d{ days };

			if (y_m_d.month() != std::chrono::month(6))
			{
				return;
			}

			for (const auto& id : imgui_colors)
			{
				const auto color = colors[id];

				ImVec4 hsv_color =
				{
					static_cast<float>((game::Sys_Milliseconds() / 100) % 256) / 255.f,
					1.f, 1.f, 1.f,
				};

				ImVec4 rgba_color{};
				ImGui::ColorConvertHSVtoRGB(hsv_color.x, hsv_color.y, hsv_color.z, rgba_color.x, rgba_color.y, rgba_color.z);

				rgba_color.w = color.w;
				colors[id] = rgba_color;
			}
		}

		void new_gui_frame()
		{
			ImGui::GetIO().MouseDrawCursor = toggled;

			update_colors();

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			run_event_queue();

			ImGui::NewFrame();
		}

		void end_gui_frame()
		{
			ImGui::EndFrame();
			ImGui::Render();
			device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		void toggle_menu(const std::string& name)
		{
			enabled_menus[name] = !enabled_menus[name];
		}

		std::string truncate(const std::string& text, const size_t length, const std::string& end)
		{
			return text.size() <= length
				? text
				: text.substr(0, length - end.size()) + end;
		}

		void show_notifications()
		{
			static const auto window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
				ImGuiWindowFlags_NoMove;

			notifications.access([](std::deque<notification_t>& notifications_)
			{
				auto index = 0;
				for (auto i = notifications_.begin(); i != notifications_.end();)
				{
					const auto now = std::chrono::high_resolution_clock::now();
					if (now - i->creation_time >= i->duration)
					{
						i = notifications_.erase(i);
						continue;
					}

					const auto title = truncate(i->title, 34, "...");
					const auto text = truncate(i->text, 34, "...");

					ImGui::SetNextWindowSizeConstraints(ImVec2(250, 50), ImVec2(250, 50));
					ImGui::SetNextWindowBgAlpha(0.6f);
					ImGui::Begin(utils::string::va("Notification #%i", index), nullptr, window_flags);

					ImGui::SetWindowPos(ImVec2(10, 30.f + static_cast<float>(index) * 60.f));
					ImGui::SetWindowSize(ImVec2(250, 0));
					ImGui::Text(title.data());
					ImGui::Text(text.data());

					ImGui::End();

					++i;
					++index;
				}
			});
		}

		void menu_checkbox(const std::string& name, const std::string& menu)
		{
			ImGui::Checkbox(name.data(), &enabled_menus[menu]);
		}

		void run_frame_callbacks()
		{
			on_frame_callbacks.access([](std::vector<frame_callback>& callbacks)
			{
				for (const auto& callback : callbacks)
				{
					if (callback.always || toggled)
					{
						callback.callback();
					}
				}
			});
		}

		void draw_main_menu_bar()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Windows"))
				{
					for (const auto& menu : menus)
					{
						menu_checkbox(menu.title, menu.name);
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		void gui_on_frame()
		{
			if (!utils::hook::invoke<bool>(0x140BB5E70)) // is database ready to start showing
			{
				return;
			}

			static auto logged = false;
			if (!logged)
			{
				console::debug("[ImGui] Rendering frames\n");
				logged = true;
			}

			new_gui_frame();
			run_frame_callbacks();
			end_gui_frame();
		}
		
		/*
		void gui_on_frame_wrapper(utils::hook::assembler& a)
		{
			a.pushad64();
			a.call_aligned(gui_on_frame);
			a.popad64();
			
			a.inc(eax);
			a.mov(rax, asmjit::imm(0x148B13898));
			a.mov(dword_ptr(rax), eax);
			a.mov(dword_ptr(rsp, 0x20), esi);
			a.jmp(0x140E59B9D);
		}

		utils::hook::detour wnd_proc_hook;
		LRESULT wnd_proc_stub(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (wParam != VK_ESCAPE && toggled)
			{
				event_queue.access([hWnd, msg, wParam, lParam](std::vector<event>& queue)
				{
					queue.emplace_back(hWnd, msg, wParam, lParam);
				});
			}

			return wnd_proc_hook.invoke<LRESULT>(hWnd, msg, wParam, lParam);
		}
		*/

		typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		Present d3d11_present_original;

		typedef HRESULT(__stdcall* ResizeBuffers)(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
		ResizeBuffers oResizeBuffers;

		WNDPROC oWndProc;

		LRESULT __stdcall WndProc_stub(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

			if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
				return true;

			return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
		}

		HRESULT __stdcall d3d11_present_stub(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags)
		{
			if (!initialized)
			{
				console::debug("[ImGui] Doing D3D stuff\n");

				auto hr = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
				if (SUCCEEDED(hr))
				{
					device->GetImmediateContext(&device_context);

					DXGI_SWAP_CHAIN_DESC desc;
					pSwapChain->GetDesc(&desc);
					window = desc.OutputWindow;

					initialize_gui_context();

					ID3D11Texture2D* pBackBuffer;
					pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
					device->CreateRenderTargetView(pBackBuffer, NULL, &render_target_view);
					pBackBuffer->Release();
					oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc_stub);

					console::debug("[ImGui] Initializing\n");
				}

				// fallback/when done
				return d3d11_present_original(pSwapChain, SyncInterval, Flags);
			}

			gui_on_frame();

			return d3d11_present_original(pSwapChain, SyncInterval, Flags);
		}

		HRESULT resize_buffers_stub(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
			if (!initialized || !device)
			{
				return oResizeBuffers(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);
			}

			if (render_target_view) {
				device_context->OMSetRenderTargets(0, 0, 0);
				render_target_view->Release();
				render_target_view = nullptr;
			}

			HRESULT hr = oResizeBuffers(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);

			ID3D11Texture2D* pBuffer;
			pThis->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);

			device->CreateRenderTargetView(pBuffer, NULL,
				&render_target_view);

			pBuffer->Release();

			device_context->OMSetRenderTargets(1, &render_target_view, NULL);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = static_cast<FLOAT>(Width);
			vp.Height = static_cast<FLOAT>(Height);
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			device_context->RSSetViewports(1, &vp);

			return hr;
		}
	}

	void toggle()
	{
		if (!toggled)
		{
			*reinterpret_cast<int*>(0x14779C73D) = 0;
			*game::keyCatchers |= 0x10;
		}
		else
		{
			*reinterpret_cast<int*>(0x14779C73D) = 1;
			*game::keyCatchers &= ~0x10;
		}
		toggled = !toggled;
	}

	bool gui_key_event(const int local_client_num, const int key, const int down)
	{
		if (key == game::K_F11 && down)
		{
			toggle();
			return false;
		}

		if (key == game::K_ESCAPE && down && toggled)
		{
			toggle();
			return false;
		}

		return !toggled;
	}

	bool gui_char_event(const int local_client_num, const int key)
	{
		return !toggled;
	}

	bool gui_mouse_event(const int local_client_num, int x, int y)
	{
		return !toggled;
	}

	void on_frame(const std::function<void()>& callback, bool always)
	{
		on_frame_callbacks.access([always, callback](std::vector<frame_callback>& callbacks)
		{
			callbacks.emplace_back(callback, always);
		});
	}

	bool is_menu_open(const std::string& name)
	{
		return enabled_menus[name];
	}

	void notification(const std::string& title, const std::string& text, const std::chrono::milliseconds duration)
	{
		notification_t notification{};
		notification.title = title;
		notification.text = text;
		notification.duration = duration;
		notification.creation_time = std::chrono::high_resolution_clock::now();

		notifications.access([notification](std::deque<notification_t>& notifications_)
		{
			notifications_.push_front(notification);
		});
	}

	void copy_to_clipboard(const std::string& text)
	{
		utils::string::set_clipboard_data(text);
		notification("Text copied to clipboard", utils::string::va("\"%s\"", text.data()));
	}

	void register_menu(const std::string& name, const std::string& title,
		const std::function<void()>& callback, bool always)
	{
		menus.emplace_back(name, title, callback);
		enabled_menus[name] = false;

		on_frame([=]
		{
			if (enabled_menus.at(name))
			{
				callback();
			}
		}, always);
	}

	void register_callback(const std::function<void()>& callback, bool always)
	{
		on_frame([=]
		{
			callback();
		}, always);
	}

	bool InputU8(const char* label, unsigned char* v, int step, int step_fast, ImGuiInputTextFlags flags)
	{
		// Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
		const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
		return ImGui::InputScalar(label, ImGuiDataType_U8, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
	}

	bool InputUInt6(const char* label, unsigned int v[6], ImGuiInputTextFlags flags)
	{
		return ImGui::InputScalarN(label, ImGuiDataType_U32, v, 6, NULL, NULL, "%d", flags);
	}

	void shutdown_gui()
	{
		if (initialized)
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		initialized = false;
	}
	
	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_dedi())
			{
				return;
			}

			scheduler::loop([&]
			{
				if (!attached)
				{
					auto result = kiero::init(kiero::RenderType::D3D11);

					console::debug("[GUI] kiero result is %s\n", kiero::status_to_str(result).c_str());

					if (result == kiero::Status::Success)
					{
						attached = true;

						kiero::bind(8, (void**)&d3d11_present_original, d3d11_present_stub);
						kiero::bind(13, (void**)&oResizeBuffers, resize_buffers_stub);
					}
				}
			}, scheduler::renderer);

			on_frame([]
			{
				show_notifications();
				draw_main_menu_bar();
			});
		}

		void pre_destroy() override
		{
			if (game::environment::is_dedi())
			{
				return;
			}

			shutdown_gui();
		}
	};
}

REGISTER_COMPONENT(gui::component)
#endif
