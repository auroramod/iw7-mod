#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "dvars.hpp"
#include "directx.hpp"
#include "console/console.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#include <wrl/client.h>

namespace renderer
{
	namespace
	{
		utils::hook::detour r_init_draw_method_hook;
		utils::hook::detour r_warning_hook;
		utils::hook::detour r_init_smodel_lists_hook;
		utils::hook::detour r_begin_frame_data_hook;

		constexpr std::size_t smodel_list_scale = 32;
		static_assert(256 * smodel_list_scale <= 12 * (1 << 10), "delayed lists past 0x140DD09D0's sort stack");
		constexpr std::size_t smodel_regions = 11;
		constexpr std::size_t smodel_lists = 4;
		constexpr std::size_t smodel_stock_arena_size = 0x43000;
		constexpr std::size_t smodel_pool_size = smodel_stock_arena_size * smodel_list_scale;
		constexpr std::size_t smodel_delayed_slot_size = 16;
		constexpr std::size_t smodel_list_stride = 0x48;
		constexpr std::uint32_t smodel_view_record_bytes = 0x4000;
		constexpr std::size_t frame_data_records_used = 0x1898;
		constexpr std::size_t frame_data_records = 0x43C400;

		const auto* smodel_list_sizes = reinterpret_cast<const std::uint16_t*>(0x14153C768);
		std::uint8_t* smodel_pool[2]{};
		std::atomic<std::size_t> smodel_pool_used[2]{};

		std::uint8_t* frame_data()
		{
			return *reinterpret_cast<std::uint8_t**>(0x148F73088);
		}

		std::size_t frame_data_index()
		{
			return static_cast<std::size_t>(*reinterpret_cast<std::uint32_t*>(0x148F73098) & 1);
		}

		void r_init_smodel_lists_stub(std::uint8_t* lists, std::uint8_t* delayed_stack)
		{
			std::size_t list_bytes = 0, delayed_slots = 0;
			for (auto region = 0u; region < smodel_regions; region++)
			{
				list_bytes += smodel_lists * smodel_list_sizes[region * 2] * smodel_list_scale;
				delayed_slots += smodel_lists * smodel_list_sizes[region * 2 + 1] * smodel_list_scale;
			}

			const auto index = frame_data_index();
			const auto at = smodel_pool_used[index].fetch_add(list_bytes);
			if (!smodel_pool[index] || at + list_bytes > smodel_pool_size)
			{
				static std::atomic_bool warned = false;
				if (!warned.exchange(true))
				{
					console::warn("[renderer] static model list pool full\n");
				}

				r_init_smodel_lists_hook.invoke<void>(lists, delayed_stack);
				return;
			}

			thread_local std::vector<std::uint8_t> delayed;
			delayed.resize(delayed_slots * smodel_delayed_slot_size);

			auto* data = frame_data();
			*reinterpret_cast<std::uint32_t*>(lists) = 0;
			const auto records = static_cast<std::uint32_t>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(data + frame_data_records_used), smodel_view_record_bytes));
			*reinterpret_cast<std::uint8_t**>(lists + 0x08) = data + frame_data_records + records;
			*reinterpret_cast<std::uint32_t*>(lists + 0x10) = 0;
			*reinterpret_cast<std::uint32_t*>(lists + 0x14) = smodel_view_record_bytes;

			auto* memory = smodel_pool[index] + at;
			auto* slots = delayed.data();
			auto* list = lists + 0x28;
			for (auto region = 0u; region < smodel_regions; region++)
			{
				const auto bytes = static_cast<std::size_t>(smodel_list_sizes[region * 2]) * smodel_list_scale;
				const auto count = static_cast<std::uint32_t>(smodel_list_sizes[region * 2 + 1] * smodel_list_scale);
				for (auto l = 0u; l < smodel_lists; l++)
				{
					*reinterpret_cast<std::uint8_t**>(list + 0x08) = memory;
					*reinterpret_cast<std::uint8_t**>(list + 0x10) = memory;
					memory += bytes;
					*reinterpret_cast<std::uint8_t**>(list + 0x18) = memory;
					*reinterpret_cast<std::uint8_t**>(list + 0x28) = slots;
					slots += static_cast<std::size_t>(count) * smodel_delayed_slot_size;
					*reinterpret_cast<std::int64_t*>(list - 0x10) = -1;
					*reinterpret_cast<std::uint64_t*>(list - 0x08) = 0;
					*reinterpret_cast<std::uint32_t*>(list) = 0;
					*reinterpret_cast<std::int64_t*>(list + 0x20) = -1;
					*reinterpret_cast<std::uint32_t*>(list + 0x30) = 0;
					*reinterpret_cast<std::uint32_t*>(list + 0x34) = count;
					list += smodel_list_stride;
				}
			}
		}

		std::uint64_t r_begin_frame_data_stub(const unsigned int a1)
		{
			const auto result = r_begin_frame_data_hook.invoke<std::uint64_t>(a1);
			smodel_pool_used[frame_data_index()] = 0;
			return result;
		}

		constexpr std::size_t umbra_query_arena_size = 64ull * 1024ull * 1024ull;

		std::uint64_t umbra_query_init_stub(const std::uint64_t query, const std::uint64_t tome)
		{
			thread_local std::uint8_t* arena = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, umbra_query_arena_size,
				MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
			if (!arena)
			{
				return utils::hook::invoke<std::uint64_t>(0x140E904B0, query, tome);
			}
			const auto object = (query + 7) & ~7ull;
			const auto allocator = utils::hook::invoke<std::uint64_t>(0x140E96C20, object, arena, umbra_query_arena_size);
			return utils::hook::invoke<std::uint64_t>(0x140E981F0, allocator, tome, 0ull, 0ull);
		}

		void r_warning_stub(int id, std::uint64_t a, std::uint64_t b, std::uint64_t c)
		{
			if (id >= 115 && id <= 117)
			{
				static std::uint32_t last_umbra[3]{};
				const auto now = GetTickCount();
				if (now - last_umbra[id - 115] > 1000)
				{
					last_umbra[id - 115] = now;
					const char* what[] = { "camera outside umbra view volume", "umbra query out of memory", "umbra query failed" };
					console::warn("[renderer] %s\n", what[id - 115]);
				}
			}

			if (id >= 23 && id <= 25)
			{
				static std::uint32_t last[3]{};
				const auto now = GetTickCount();
				if (now - last[id - 23] > 1000)
				{
					last[id - 23] = now;
					const char* what[] = { "too many visible static models (record bytes %llu, limit 16384)",
						"static model surface list full", "static model delayed list full" };
					console::warn("[renderer] %s\n", utils::string::va(what[id - 23], a));
				}
			}

			r_warning_hook.invoke<void>(id, a, b, c);
		}

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

			for (auto& pool : smodel_pool)
			{
				pool = static_cast<std::uint8_t*>(VirtualAlloc(nullptr, smodel_pool_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
			}
			r_init_smodel_lists_hook.create(0x140DCFF30, r_init_smodel_lists_stub);
			r_begin_frame_data_hook.create(0x140E28290, r_begin_frame_data_stub);
			r_warning_hook.create(0x140E4B0B0, r_warning_stub);
			utils::hook::call(0x1405FB7B4, umbra_query_init_stub);
			utils::hook::call(0x140E264B3, r_update_front_end_dvar_options_stub);

			// fix particle effect flickering on AMD GPUs
			utils::hook::jump(0x140E29757, utils::hook::assemble(half_res_emissive_compare_stub));
		}
	};
}

REGISTER_COMPONENT(renderer::component)