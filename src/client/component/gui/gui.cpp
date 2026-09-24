#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <component/console/console.hpp>
#include <component/scheduler.hpp>

#include "gui.hpp"
#include "component/directx.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/nt.hpp>
#include <utils/concurrency.hpp>

#include <asmjit/asmjit.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace gui
{
	std::unordered_map<std::string, bool> enabled_menus;

	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	namespace
	{
		struct frame_callback
		{
			std::function<void()> callback;
			bool always;
			bool needs_db;
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

#ifdef _DEBUG
		std::atomic_bool enabled = true;
#else
		std::atomic_bool enabled = false;
#endif

		ImFont* console_font = nullptr;

		std::mutex capture_mutex;
		std::unordered_set<std::string> capture_owners;
		std::optional<std::uint8_t> saved_mouse_state;

		auto& mouse_active = *reinterpret_cast<std::uint8_t*>(0x14779C73D);

		static HWND window = NULL;

		void initialize_gui_context()
		{
			ImGui::CreateContext();

			ImGui::StyleColorsDark();

			auto& io = ImGui::GetIO();
			io.FontAllowUserScaling = true;
			io.Fonts->AddFontDefault();

			static auto console_font_data = utils::nt::load_resource(FONT_JETBRAINS_MONO);
			if (!console_font_data.empty())
			{
				ImFontConfig config{};
				config.FontDataOwnedByAtlas = false;
				console_font = io.Fonts->AddFontFromMemoryTTF(console_font_data.data(),
					static_cast<int>(console_font_data.size()), 17.0f, &config);
			}

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
			ImGui::GetIO().MouseDrawCursor = is_capturing_input();

			update_colors();

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			run_event_queue();

			ImGui::NewFrame();
		}

		void end_gui_frame(IDXGISwapChain* swap_chain)
		{
			ImGui::EndFrame();
			ImGui::Render();

			ID3D11Texture2D* back_buffer = nullptr;
			if (FAILED(swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))))
			{
				return;
			}

			ID3D11RenderTargetView* render_target_view = nullptr;
			const auto hr = device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
			back_buffer->Release();

			if (FAILED(hr))
			{
				return;
			}

			device_context->OMSetRenderTargets(1, &render_target_view, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			device_context->OMSetRenderTargets(0, nullptr, nullptr);

			render_target_view->Release();
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
					ImGui::TextUnformatted(title.data());
					ImGui::TextUnformatted(text.data());

					ImGui::End();

					++i;
					++index;
				}
			});
		}

#ifdef _DEBUG
		void menu_checkbox(const std::string& name, const std::string& menu)
		{
			ImGui::Checkbox(name.data(), &enabled_menus[menu]);
		}

#endif

		void run_frame_callbacks(const bool db_ready)
		{
			on_frame_callbacks.access([db_ready](std::vector<frame_callback>& callbacks)
			{
				for (const auto& callback : callbacks)
				{
					if (callback.needs_db && !db_ready)
					{
						continue;
					}

					if (callback.always || toggled)
					{
						callback.callback();
					}
				}
			});
		}

#ifdef _DEBUG
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
#endif

		void gui_on_frame(IDXGISwapChain* swap_chain)
		{
			// false while fastfiles load, keep drawing but skip whatever reads assets
			const auto db_ready = game::Sys_IsDatabaseReady();

			static auto logged = false;
			if (!logged)
			{
				console::debug("[ImGui] Rendering frames\n");
				logged = true;
			}

			new_gui_frame();
			run_frame_callbacks(db_ready);
			end_gui_frame(swap_chain);
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

		WNDPROC oWndProc;

		LRESULT __stdcall WndProc_stub(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

			if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
				return true;

			return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
		}

		IDXGISwapChain* get_swap_chain()
		{
			const auto active_window = *reinterpret_cast<int*>(0x148B1BE34);
			if (active_window < 0)
			{
				return nullptr;
			}

			return reinterpret_cast<IDXGISwapChain**>(0x148B1BE48)[12 * active_window];
		}

		void draw_gui(IDXGISwapChain* swap_chain)
		{
			static ID3D11Device* game_device = nullptr;

			if (initialized && game_device != dx::device)
			{
				shutdown_gui();
			}

			if (!initialized)
			{
				if (!enabled)
				{
					return;
				}

				if (FAILED(swap_chain->GetDevice(IID_PPV_ARGS(&device))))
				{
					return;
				}

				game_device = dx::device;
				device->GetImmediateContext(&device_context);

				DXGI_SWAP_CHAIN_DESC desc{};
				swap_chain->GetDesc(&desc);
				window = desc.OutputWindow;

				initialize_gui_context();

				if (!oWndProc)
				{
					oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc_stub);
				}

				console::debug("[ImGui] Initializing\n");
				return;
			}

			gui_on_frame(swap_chain);
		}

		utils::hook::detour r_present_frame_hook;

		void r_present_frame_stub(const char a1)
		{
			auto* const swap_chain = get_swap_chain();
			if (swap_chain && dx::device)
			{
				const auto mutex = *reinterpret_cast<HANDLE*>(0x148B1BC98);
				WaitForSingleObject(mutex, INFINITE);
				draw_gui(swap_chain);
				ReleaseMutex(mutex);
			}

			r_present_frame_hook.invoke<void>(a1);
		}
	}

	void toggle()
	{
		toggled = !toggled;

		if (toggled)
		{
			*game::keyCatchers |= 0x10;
		}
		else
		{
			*game::keyCatchers &= ~0x10;
		}

		set_input_capture("menu", toggled);
	}

	void enable()
	{
		enabled = true;
	}

	void set_input_capture(const std::string& owner, const bool capture)
	{
		std::lock_guard _(capture_mutex);

		const auto was_capturing = !capture_owners.empty();
		if (capture)
		{
			capture_owners.insert(owner);
		}
		else
		{
			capture_owners.erase(owner);
		}

		const auto is_capturing = !capture_owners.empty();
		if (is_capturing == was_capturing)
		{
			return;
		}

		if (is_capturing)
		{
			saved_mouse_state = mouse_active;
			mouse_active = 0;
		}
		else
		{
			mouse_active = saved_mouse_state.value_or(1);
			saved_mouse_state.reset();
		}
	}

	bool is_capturing_input()
	{
		std::lock_guard _(capture_mutex);
		return !capture_owners.empty();
	}

	ImFont* get_console_font()
	{
		return console_font;
	}

	bool gui_key_event(const int local_client_num, const int key, const int down)
	{
#ifdef _DEBUG
		if (key == game::K_F11)
		{
			static auto held = false;
			if (down && !held)
			{
				toggle();
			}

			held = down != 0;
			return false;
		}

		if (key == game::K_ESCAPE && down && toggled)
		{
			toggle();
			return false;
		}
#endif

		return !toggled;
	}

	bool gui_char_event(const int local_client_num, const int key)
	{
		return !toggled;
	}

	bool gui_mouse_event(const int local_client_num, int x, int y)
	{
		return !is_capturing_input();
	}

	void on_frame(const std::function<void()>& callback, bool always, bool needs_db)
	{
		on_frame_callbacks.access([always, needs_db, callback](std::vector<frame_callback>& callbacks)
		{
			callbacks.emplace_back(callback, always, needs_db);
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
		console_font = nullptr;
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

			r_present_frame_hook.create(0x140E59A40, r_present_frame_stub);

			on_frame([]
			{
				show_notifications();
#ifdef _DEBUG
				draw_main_menu_bar();
#endif
			}, false, false);
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
