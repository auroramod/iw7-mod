#include <std_include.hpp>

#include "loader/component_loader.hpp"
#include "directx.hpp"

#include <utils/flags.hpp>
#include <utils/hook.hpp>

#include "d3d12.h"
#include <d3d11.h>
#include <d3d11on12.h>
#include <wrl/client.h>

ID3D11Device* dx::device = nullptr;
ID3D11DeviceContext* dx::deviceContext = nullptr;
ID3D12Device* dx::d3d12Device = nullptr;
ID3D12CommandQueue* dx::d3d12CommandQueue = nullptr;

namespace dx
{
	namespace
	{
		using Microsoft::WRL::ComPtr;

		ComPtr<ID3D12Device> g_d3d12_device;
		ComPtr<ID3D12CommandQueue> g_d3d12_queue;

		inline void store_dx12()
		{
			dx::d3d12Device = g_d3d12_device.Get();
			dx::d3d12CommandQueue = g_d3d12_queue.Get();
		}

		inline void store_dx11(ID3D11Device** out_device, ID3D11DeviceContext** out_context)
		{
			if (out_device && out_context && *out_device && *out_context)
			{
				dx::device = *out_device;
				dx::deviceContext = *out_context;
			}
		}

		void use_separate_shader_preload_cache()
		{
			utils::hook::copy_string(0x141444EF0, "players2\\upshd.d12");
			utils::hook::copy_string(0x141444EE0, "upshd.d12");
		}

		HRESULT create_d3d12_objects(IDXGIAdapter* adapter)
		{
			g_d3d12_queue.Reset();
			g_d3d12_device.Reset();

			// 11_0 minimum, 12_1 would exclude e.g. Polaris (RX 400/500)
			auto hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_d3d12_device));
			if (FAILED(hr))
			{
				return hr;
			}

			D3D12_COMMAND_QUEUE_DESC desc{};
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;

			hr = g_d3d12_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_d3d12_queue));
			if (FAILED(hr))
			{
				g_d3d12_device.Reset();
			}

			return hr;
		}

		HRESULT WINAPI d3d11_create_device_stub(
			IDXGIAdapter* adapter,
			D3D_DRIVER_TYPE driver_type,
			HMODULE software,
			UINT flags,
			const D3D_FEATURE_LEVEL* feature_levels,
			UINT feature_level_count,
			UINT sdk_version,
			ID3D11Device** out_device,
			D3D_FEATURE_LEVEL* out_feature_level,
			ID3D11DeviceContext** out_immediate_context)
		{
			if (GetModuleHandleA("renderdoc.dll"))
			{
				// the game creates its device with this flag, which crashes RenderDoc when the game creates its swap chain on the unhooked device
				flags &= ~D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY;
			}

			const auto real_device = out_device && out_immediate_context;
			if (real_device && utils::flags::has_flag("d3d12"))
			{
				if (SUCCEEDED(create_d3d12_objects(adapter)))
				{
					IUnknown* queues[] = { g_d3d12_queue.Get() };

					const auto hr = D3D11On12CreateDevice(
						g_d3d12_device.Get(),
						flags,
						feature_levels,
						feature_level_count,
						queues,
						static_cast<UINT>(_countof(queues)),
						0,
						out_device,
						out_immediate_context,
						out_feature_level);

					if (SUCCEEDED(hr))
					{
						store_dx12();
						store_dx11(out_device, out_immediate_context);
						use_separate_shader_preload_cache();
						return hr;
					}
				}

				g_d3d12_queue.Reset();
				g_d3d12_device.Reset();
				store_dx12();
			}

			const auto hr = D3D11CreateDevice(
				adapter,
				driver_type,
				software,
				flags,
				feature_levels,
				feature_level_count,
				sdk_version,
				out_device,
				out_feature_level,
				out_immediate_context);

			store_dx11(out_device, out_immediate_context);
			return hr;
		}
	}

	class component final : public component_interface
	{
	public:
		void* load_import(const std::string& library, const std::string& function) override
		{
			if (library == "d3d11.dll" && function == "D3D11CreateDevice")
			{
				return d3d11_create_device_stub;
			}

			return nullptr;
		}
	};
}

REGISTER_COMPONENT(dx::component)
