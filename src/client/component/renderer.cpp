#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"
#include "directx.hpp"
#include "console/console.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>

#include <wrl/client.h>

namespace renderer
{
	namespace
	{
		utils::hook::detour r_init_draw_method_hook;

		int get_fullbright_technique()
		{
			switch (dvars::r_fullbright ? dvars::r_fullbright->current.integer : 0)
			{
			case 2:
				return game::TECHNIQUE_DEBUG_BUMPMAP;
			default:
				return game::TECHNIQUE_UNLIT;
			}
		}

		void gfxdrawmethod()
		{
			const auto fullbright = dvars::r_fullbright ? dvars::r_fullbright->current.integer : 0;

			game::gfxDrawMethod->drawScene = game::GFX_DRAW_SCENE_STANDARD;
			game::gfxDrawMethod->baseTechType = fullbright ? get_fullbright_technique() : game::TECHNIQUE_LIT;
			game::gfxDrawMethod->emissiveTechType = fullbright ? get_fullbright_technique() : game::TECHNIQUE_EMISSIVE;
			game::gfxDrawMethod->forceTechType = fullbright ? get_fullbright_technique() : game::TECHNIQUE_LIT;
		}

		void r_init_draw_method_stub()
		{
			gfxdrawmethod();
		}

		bool is_amd_native_d3d11()
		{
			if (!dx::device || dx::d3d12Device)
			{
				return false;
			}

			Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;
			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
			DXGI_ADAPTER_DESC desc{};
			return SUCCEEDED(dx::device->QueryInterface(IID_PPV_ARGS(&dxgi_device)))
				&& SUCCEEDED(dxgi_device->GetAdapter(&adapter))
				&& SUCCEEDED(adapter->GetDesc(&desc))
				&& desc.VendorId == 0x1002;
		}

		bool force_full_res_emissive = false;
		void update_full_res_emissive()
		{
			static std::optional<bool> needed;
			assert(dx::device);
			if (!needed.has_value() && dx::device)
			{
				needed = is_amd_native_d3d11();
				force_full_res_emissive = needed.value();
				if (force_full_res_emissive)
				{
					console::info("Half resolution effects disabled: they flicker on AMD GPUs with D3D11 (use -d3d12 to keep them)\n");
				}
			}
		}

		void half_res_emissive_compare_stub(utils::hook::assembler& a)
		{
			const auto keep = a.newLabel();

			a.mov(rax, reinterpret_cast<std::uint64_t>(&force_full_res_emissive));
			a.cmp(byte_ptr(rax), 0);
			a.jz(keep);
			a.xor_(ebx, ebx);

			a.bind(keep);
			a.mov(rax, 0x148B9D728);
			a.cmp(dword_ptr(rax), ebx);
			a.jmp(0x140E2975D);
		}

		bool r_update_front_end_dvar_options_stub()
		{
			update_full_res_emissive();

			if (dvars::r_fullbright && dvars::r_fullbright->modified)
			{
				game::Dvar_ClearModified(dvars::r_fullbright);
				game::R_SyncRenderThread();

				gfxdrawmethod();
			}

			return utils::hook::invoke<bool>(0x140E28B60);
		}
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

			dvars::r_fullbright = game::Dvar_RegisterInt("r_fullbright", 0, 0, 2, game::DVAR_FLAG_SAVED, "Toggles rendering without lighting");

			r_init_draw_method_hook.create(0x140DE9260, r_init_draw_method_stub);
			utils::hook::call(0x140E264B3, r_update_front_end_dvar_options_stub);

			// Fix particle effect flickering on AMD GPUs.
			utils::hook::jump(0x140E29757, utils::hook::assemble(half_res_emissive_compare_stub));
		}
	};
}

REGISTER_COMPONENT(renderer::component)