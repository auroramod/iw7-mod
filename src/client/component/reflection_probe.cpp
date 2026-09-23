#include <std_include.hpp>
#ifdef DEBUG
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "scheduler.hpp"
#include "directx.hpp"
#include "console/console.hpp"

#include <utils/hook.hpp>

#pragma warning(push)
#pragma warning(disable: 4459)
#include <DirectXTex.h>
#pragma warning(pop)

#include <wrl/client.h>

namespace reflection_probes
{
	namespace
	{
		game::dvar_t* r_reflectionProbeGenerate = nullptr;
		game::dvar_t* r_reflectionProbeGenerateExit = nullptr;
		game::dvar_t* r_reflectionProbeGenerateTarget = nullptr;
		game::dvar_t* r_reflectionProbeGenerateFrames = nullptr;
		game::dvar_t* r_reflectionProbeGenerateFullView = nullptr;
		game::dvar_t* r_reflectionProbeGenerateGuardBand = nullptr;
		game::dvar_t* r_reflectionProbeGenerateSize = nullptr;

		utils::hook::detour r_calc_cubemap_view_values_hook;
		utils::hook::detour scr_update_frame_hook;
		utils::hook::detour cg_draw_active_frame_hook;

		constexpr auto render_target_count = 2048;
		constexpr auto face_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		constexpr auto face_pixel_size = 8;

		bool generating = false;
		float probe_origin[3];
		int server_time;
		int do_render;

		std::atomic_bool capture_requested = false;
		HRESULT capture_result;
		DirectX::Image* capture_target;
		bool logged_capture_source = false;

		game::GfxViewport full_viewport{};
		unsigned int full_face_side = 0;
		float full_face_tan = 1.0f;

		std::string clean_name(const std::string& name)
		{
			auto new_name = name;

			for (auto& c : new_name)
			{
				if (!std::isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_')
				{
					c = '_';
				}
			}

			return new_name;
		}

		void flip_diagonally(uint8_t* image, int width, int height, int channels)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = i + 1; j < width; ++j)
				{
					for (int k = 0; k < channels; ++k)
					{
						std::swap(image[(i * width + j) * channels + k], image[(j * width + i) * channels + k]);
					}
				}
			}
		}

		void flip_horizontally(uint8_t* image, int width, int height, int channels)
		{
			for (int i = 0; i < height; ++i)
			{
				for (int j = 0; j < width / 2; ++j)
				{
					for (int k = 0; k < channels; ++k)
					{
						std::swap(image[(i * width + j) * channels + k], image[(i * width + (width - 1 - j)) * channels + k]);
					}
				}
			}
		}

		void flip_vertically(uint8_t* image, int width, int height, int channels)
		{
			for (int i = 0; i < height / 2; ++i)
			{
				for (int j = 0; j < width; ++j)
				{
					for (int k = 0; k < channels; ++k)
					{
						std::swap(image[(i * width + j) * channels + k], image[((height - 1 - i) * width + j) * channels + k]);
					}
				}
			}
		}

		template <typename T>
		void write_block(std::ofstream& file, const std::uint8_t type, const T* data, const std::uint32_t count)
		{
			const std::uint8_t exists = 1;
			file.write(reinterpret_cast<const char*>(&type), 1);
			file.write(reinterpret_cast<const char*>(&exists), 1);
			file.write(reinterpret_cast<const char*>(&count), 4);
			file.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(sizeof(T)) * count);
		}

		bool write_iw7_image(const std::filesystem::path& path, const DirectX::ScratchImage& image)
		{
			constexpr std::uint8_t dump_type_string = 6;
			constexpr std::uint8_t dump_type_array = 8;

			const auto& metadata = image.GetMetadata();
			const auto name = "*reflection_probe_array";

			game::GfxImage asset{};
			asset.imageFormat = metadata.format;
			asset.flags = 0x28301;
			asset.mapType = game::MAPTYPE_CUBE_ARRAY;
			asset.semantic = game::TS_FUNCTION;
			asset.category = game::IMG_CATEGORY_AUTO_GENERATED;
			asset.dataLen1 = static_cast<unsigned int>(image.GetPixelsSize());
			asset.dataLen2 = asset.dataLen1;
			asset.width = static_cast<unsigned short>(metadata.width);
			asset.height = static_cast<unsigned short>(metadata.height);
			asset.depth = 1;
			asset.numElements = static_cast<unsigned short>(metadata.arraySize / 6);
			asset.levelCount = static_cast<unsigned char>(metadata.mipLevels);
			asset.pixelData = image.GetPixels();
			asset.name = name;

			std::ofstream file(path, std::ios::binary | std::ios::trunc);
			if (!file)
			{
				return false;
			}

			write_block(file, dump_type_array, &asset, 1);

			const std::uint8_t exists = 1;
			file.write(reinterpret_cast<const char*>(&dump_type_string), 1);
			file.write(reinterpret_cast<const char*>(&exists), 1);
			file.write(name, std::strlen(name) + 1);

			write_block(file, dump_type_array, image.GetPixels(), asset.dataLen1);

			return file.good();
		}

		bool dump_probe_array(DirectX::ScratchImage& faces, const unsigned int output_size)
		{
			const auto* mapname = game::Dvar_FindVar("mapname");
			const auto parent_path = std::filesystem::path("dump") / clean_name(mapname ? mapname->current.string : "unknown") / "images";
			const auto path = parent_path / "_reflection_probe_array.dds";

			std::error_code ec;
			std::filesystem::create_directories(parent_path, ec);

			DirectX::ScratchImage resized;
			const auto* source = &faces;
			auto hr = S_OK;
			if (faces.GetMetadata().width != output_size)
			{
				hr = DirectX::Resize(faces.GetImages(), faces.GetImageCount(), faces.GetMetadata(),
					output_size, output_size, DirectX::TEX_FILTER_DEFAULT, resized);
				if (FAILED(hr))
				{
					console::error("Failed to resize reflection probes (0x%08X)\n", static_cast<unsigned int>(hr));
					return false;
				}
				source = &resized;
			}

			auto levels = 1u;
			while ((output_size >> (levels - 1)) > 1)
			{
				levels++;
			}

			DirectX::ScratchImage mip_chain;
			hr = DirectX::GenerateMipMaps(source->GetImages(), source->GetImageCount(), source->GetMetadata(),
				DirectX::TEX_FILTER_DEFAULT, levels, mip_chain);
			if (FAILED(hr))
			{
				console::error("Failed to generate reflection probe mips (0x%08X)\n", static_cast<unsigned int>(hr));
				return false;
			}

			DirectX::ScratchImage compressed_image;
			hr = DirectX::Compress(dx::device, mip_chain.GetImages(), mip_chain.GetImageCount(), mip_chain.GetMetadata(),
				DXGI_FORMAT_BC6H_UF16, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, compressed_image);
			if (FAILED(hr))
			{
				console::error("Failed to compress reflection probes (0x%08X)\n", static_cast<unsigned int>(hr));
				return false;
			}

			hr = DirectX::SaveToDDSFile(compressed_image.GetImages(), compressed_image.GetImageCount(),
				compressed_image.GetMetadata(), DirectX::DDS_FLAGS_NONE, path.wstring().data());
			if (FAILED(hr))
			{
				console::error("Failed to dump image \"%s\" (0x%08X)\n", path.string().data(), static_cast<unsigned int>(hr));
				return false;
			}

			console::info("Saved reflection probes to \"%s\" (%ux%u, %u mips)\n", path.string().data(),
				output_size, output_size, levels);

			const auto iw7_image_path = parent_path / "_reflection_probe_array.iw7Image";
			auto backup_path = iw7_image_path;
			backup_path += ".bak";
			if (std::filesystem::exists(iw7_image_path, ec) && !std::filesystem::exists(backup_path, ec))
			{
				std::filesystem::copy_file(iw7_image_path, backup_path, ec);
			}

			if (!write_iw7_image(iw7_image_path, compressed_image))
			{
				console::error("Failed to write \"%s\"\n", iw7_image_path.string().data());
				return false;
			}

			console::info("Saved reflection probes to \"%s\"\n", iw7_image_path.string().data());
			return true;
		}

		bool render_full_view()
		{
			return !r_reflectionProbeGenerateFullView || r_reflectionProbeGenerateFullView->current.enabled;
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

		bool is_display_encoded(const DXGI_FORMAT format)
		{
			switch (format)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
			case DXGI_FORMAT_B8G8R8X8_UNORM:
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				return true;
			default:
				return false;
			}
		}

		game::GfxImage* get_render_target(const int index)
		{
			// render target pool: 288 byte entries, GfxImage at +16, freed entries are zeroed
			auto* image = reinterpret_cast<game::GfxImage*>(0x149B3F248 + 288 * index);
			return image->name && image->texture.map ? image : nullptr;
		}

		game::GfxImage* find_render_target(const char* name, const unsigned int width, const unsigned int height,
			const bool allow_larger)
		{
			// targets are keyed by name and size, every view size has its own copy
			game::GfxImage* different_size = nullptr;
			game::GfxImage* larger = nullptr;
			for (auto i = 0; i < render_target_count; i++)
			{
				auto* image = get_render_target(i);
				if (!image || std::strcmp(image->name, name))
				{
					continue;
				}

				if (image->width == width && image->height == height)
				{
					return image;
				}

				if (allow_larger && image->width >= width && image->height >= height &&
					(!larger || static_cast<unsigned int>(image->width) * image->height <
						static_cast<unsigned int>(larger->width) * larger->height))
				{
					larger = image;
				}

				if (!different_size)
				{
					different_size = image;
				}
			}

			if (larger)
			{
				return larger;
			}

			if (different_size)
			{
				console::warn("Reflection target \"%s\" has a %ux%u copy but no %ux%u copy\n",
					name, different_size->width, different_size->height, width, height);
			}

			return nullptr;
		}

		game::GfxImage* find_largest_render_target(const char* name)
		{
			game::GfxImage* largest = nullptr;
			for (auto i = 0; i < render_target_count; i++)
			{
				auto* image = get_render_target(i);
				if (!image || std::strcmp(image->name, name))
				{
					continue;
				}

				if (!largest || static_cast<unsigned int>(image->width) * image->height >
					static_cast<unsigned int>(largest->width) * largest->height)
				{
					largest = image;
				}
			}

			return largest;
		}

		HRESULT crop_face(const DirectX::Image& source, DirectX::Image& target)
		{
			auto side = std::min(source.width, source.height);
			if (full_face_side && full_viewport.height)
			{
				side = std::min(side, static_cast<size_t>(std::lround(static_cast<double>(full_face_side) *
					source.height / full_viewport.height)));
			}

			const auto x = (source.width - side) / 2;
			const auto y = (source.height - side) / 2;

			DirectX::ScratchImage square;
			auto hr = square.Initialize2D(face_format, side, side, 1, 1);
			if (FAILED(hr))
			{
				return hr;
			}

			hr = DirectX::CopyRectangle(source, DirectX::Rect(x, y, side, side), *square.GetImage(0, 0, 0),
				DirectX::TEX_FILTER_DEFAULT, 0, 0);
			if (FAILED(hr))
			{
				return hr;
			}

			const auto* result = square.GetImage(0, 0, 0);
			DirectX::ScratchImage resized;
			if (side != target.width || side != target.height)
			{
				// box filter only handles exact halving
				hr = DirectX::Resize(*result, target.width, target.height, DirectX::TEX_FILTER_TRIANGLE, resized);
				if (FAILED(hr))
				{
					return hr;
				}
				result = resized.GetImage(0, 0, 0);
			}

			for (auto row = 0u; row < target.height; row++)
			{
				std::memcpy(target.pixels + row * target.rowPitch, result->pixels + row * result->rowPitch,
					target.width * face_pixel_size);
			}

			return S_OK;
		}

		HRESULT capture_render_target(DirectX::Image& target)
		{
			if (!dx::device || !dx::deviceContext)
			{
				return E_POINTER;
			}

			const auto width = static_cast<unsigned int>(target.width);
			const auto height = static_cast<unsigned int>(target.height);

			const auto* target_name = r_reflectionProbeGenerateTarget
				? r_reflectionProbeGenerateTarget->current.string
				: "Frontend Color";
			const auto backbuffer = !_stricmp(target_name, "backbuffer");
			const auto saved_screen = !_stricmp(target_name, "R_RENDERTARGET_SAVED_SCREEN");

			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
			auto typed_format = DXGI_FORMAT_UNKNOWN;
			if (backbuffer)
			{
				auto* swap_chain = get_swap_chain();
				if (!swap_chain)
				{
					return E_POINTER;
				}

				const auto hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&texture));
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else
			{
				auto* image = render_full_view()
					? find_largest_render_target(target_name)
					: find_render_target(target_name, width, height, saved_screen);
				if (!image)
				{
					return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}

				texture = image->texture.map;
				typed_format = image->imageFormat;
			}

			D3D11_TEXTURE2D_DESC texture_desc{};
			texture->GetDesc(&texture_desc);
			if (!logged_capture_source)
			{
				logged_capture_source = true;
				console::info("Reflection capture: \"%s\" %ux%u (format %u), cube face %ux%u\n",
					target_name, texture_desc.Width, texture_desc.Height,
					static_cast<unsigned int>(texture_desc.Format), width, height);

				// the game expects linear HDR probes, tonemapped sources come out dark with clipped lights
				const auto format = typed_format != DXGI_FORMAT_UNKNOWN ? typed_format : texture_desc.Format;
				if (is_display_encoded(format))
				{
					console::warn("Reflection capture source is display referred (tonemapped, clipped at 1.0), "
						"probes will be too dark and lights will clip. Use \"Frontend Color\" for linear HDR\n");
				}
			}

			if (!render_full_view() && !backbuffer && !saved_screen &&
				(texture_desc.Width != width || texture_desc.Height != height))
			{
				return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
			}

			DirectX::ScratchImage captured;
			auto hr = DirectX::CaptureTexture(dx::device, dx::deviceContext, texture.Get(), captured);
			if (FAILED(hr))
			{
				return hr;
			}

			if (DirectX::IsTypeless(captured.GetMetadata().format) && typed_format != DXGI_FORMAT_UNKNOWN)
			{
				captured.OverrideFormat(typed_format);
			}

			const auto* source = captured.GetImage(0, 0, 0);
			if (!source || (!render_full_view() && (source->width < target.width || source->height < target.height)))
			{
				return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
			}

			auto filter = DirectX::TEX_FILTER_DEFAULT;
			if (is_display_encoded(source->format))
			{
				filter |= DirectX::TEX_FILTER_SRGB_IN;
			}

			DirectX::ScratchImage converted;
			hr = DirectX::Convert(*source, face_format, filter, DirectX::TEX_THRESHOLD_DEFAULT, converted);
			if (FAILED(hr))
			{
				return hr;
			}

			if (render_full_view())
			{
				hr = crop_face(*converted.GetImage(0, 0, 0), target);
			}
			else
			{
				hr = DirectX::CopyRectangle(*converted.GetImage(0, 0, 0), DirectX::Rect(0, 0, target.width, target.height),
					target, DirectX::TEX_FILTER_DEFAULT, 0, 0);
			}

			if (FAILED(hr))
			{
				return hr;
			}

			// opaque alpha, 1.0 in half float
			auto* pixels = reinterpret_cast<std::uint16_t*>(target.pixels);
			for (auto i = 0u; i < target.width * target.height; i++)
			{
				pixels[i * 4 + 3] = 0x3C00;
			}

			return S_OK;
		}

		void r_present_frame_stub(const char a1)
		{
			if (capture_requested)
			{
				capture_result = capture_render_target(*capture_target);
				capture_requested = false;
			}

			utils::hook::invoke<void>(0x140E59A40, a1);
		}

		void r_calc_cubemap_view_values_stub(game::refdef_t* refdef, game::CubemapShot cubemapShot, unsigned int cubemapSize)
		{
			r_calc_cubemap_view_values_hook.invoke<void>(refdef, cubemapShot, cubemapSize);

			refdef->view.org[0] = probe_origin[0];
			refdef->view.org[1] = probe_origin[1];
			refdef->view.org[2] = probe_origin[2];

			// a face sized viewport breaks lighting (light tile and volumetric buffers are sized for the display),
			// so render the full view and crop the 90 degree face out of the middle. the guard band around it keeps
			// screen space effects from seaming the faces
			if (generating && render_full_view() && full_viewport.width && full_viewport.height)
			{
				const auto aspect = static_cast<float>(full_viewport.width) / static_cast<float>(full_viewport.height);

				refdef->displayViewport = full_viewport;
				refdef->view.tanHalfFovX = aspect * full_face_tan;
				refdef->view.tanHalfFovY = full_face_tan;
				refdef->viewOffset[0] = refdef->view.tanHalfFovX;
				refdef->viewOffset[1] = refdef->view.tanHalfFovY;
			}
		}

		bool render_frame(int shot, unsigned int size)
		{
			game::R_BeginFrame();

			const auto result = cg_draw_active_frame_hook.invoke<int>(0, server_time, 0, shot, size, 1, 3);
			if (!result)
			{
				game::R_ToggleSmpFrame(0);
				game::R_IssueRenderCommandsBegin(3);
			}

			game::R_EndFrame();
			game::R_IssueRenderCommandsEnd();
			game::R_SyncRenderThread();

			return result != 0;
		}

		unsigned int output_face_size()
		{
			const auto requested = r_reflectionProbeGenerateSize
				? std::clamp(r_reflectionProbeGenerateSize->current.integer, 16, 2048)
				: 128;

			auto size = 16u;
			while (size * 2 <= static_cast<unsigned int>(requested))
			{
				size *= 2;
			}

			return size;
		}

		void R_GenerateReflections(game::GfxReflectionProbe* probes, unsigned int count)
		{
			if (!count)
			{
				return;
			}

			full_viewport = {};
			if (auto* swap_chain = get_swap_chain())
			{
				DXGI_SWAP_CHAIN_DESC desc{};
				if (SUCCEEDED(swap_chain->GetDesc(&desc)))
				{
					full_viewport.width = desc.BufferDesc.Width;
					full_viewport.height = desc.BufferDesc.Height;
				}
			}

			const auto output_size = output_face_size();
			const auto size = render_full_view() ? output_size : std::max(512u, output_size);

			full_face_side = 0;
			full_face_tan = 1.0f;
			if (render_full_view() && full_viewport.height)
			{
				const auto guard_band = r_reflectionProbeGenerateGuardBand
					? std::clamp(r_reflectionProbeGenerateGuardBand->current.value, 0.0f, 0.5f)
					: 0.15f;

				// whole pixels for the face, then the fov that makes exactly those pixels 90 degrees
				full_face_side = std::max(1u, static_cast<unsigned int>(std::lround(full_viewport.height / (1.0f + guard_band))));
				full_face_tan = static_cast<float>(full_viewport.height) / static_cast<float>(full_face_side);

				if (full_face_side < output_size)
				{
					console::warn("Reflection probe faces are %ux%u but only %u pixels of the view cover a face, "
						"raise the resolution or lower r_reflectionProbeGenerateGuardBand\n", output_size, output_size, full_face_side);
				}
			}

			DirectX::ScratchImage faces;
			const auto hr = faces.InitializeCube(face_format, size, size, count, 1);
			if (FAILED(hr))
			{
				console::error("Failed to allocate reflection probes (0x%08X)\n", static_cast<unsigned int>(hr));
				return;
			}

			if (render_full_view())
			{
				console::info("Reflection probe view: full %ux%u, face %u pixels, output %ux%u\n",
					full_viewport.width, full_viewport.height, full_face_side, output_size, output_size);
			}
			else
			{
				console::info("Reflection probe view: cube face %ux%u, output %ux%u\n", size, size, output_size, output_size);
			}

			generating = true;
			logged_capture_source = false;

			auto success = true;
			const auto frames = r_reflectionProbeGenerateFrames ? std::max(1, r_reflectionProbeGenerateFrames->current.integer) : 128;

			for (auto probe_index = 0u; probe_index < count && success; ++probe_index)
			{
				const auto* probe = &probes[probe_index];

				probe_origin[0] = probe->origin[0];
				probe_origin[1] = probe->origin[1];
				probe_origin[2] = probe->origin[2];

				for (int shot = game::CUBEMAPSHOT_RIGHT; shot < game::CUBEMAPSHOT_COUNT && success; ++shot)
				{
					auto* face = const_cast<DirectX::Image*>(faces.GetImage(0, probe_index * 6 + (shot - 1), 0));

					// the first frames after the camera jumps still use the previous view's temporal state
					for (auto frame = 0; frame < frames; ++frame)
					{
						if (frame == frames - 1)
						{
							capture_target = face;
							capture_result = E_FAIL;
							capture_requested = true;
						}

						if (!render_frame(shot, size))
						{
							console::error("Reflection probe %u face %d: no active frame\n", probe_index, shot);
							success = false;
							break;
						}
					}

					capture_requested = false;

					if (success && FAILED(capture_result))
					{
						console::error("Reflection probe %u face %d: render target capture failed (0x%08X)\n",
							probe_index, shot, static_cast<unsigned int>(capture_result));
						success = false;
					}
				}

				if (success)
				{
					console::info("Generated reflection probe %u/%u\n", probe_index + 1, count);
				}
			}

			capture_requested = false;
			capture_target = nullptr;
			generating = false;

			if (!success)
			{
				game::Dvar_SetBool(r_reflectionProbeGenerate, false);
				return;
			}

			const auto side_size = static_cast<int>(size);
			for (auto probe_index = 0u; probe_index < count; ++probe_index)
			{
				const auto side = [&](const int shot)
				{
					return faces.GetImage(0, probe_index * 6 + (shot - 1), 0)->pixels;
				};

				flip_diagonally(side(game::CUBEMAPSHOT_RIGHT), side_size, side_size, face_pixel_size);

				flip_diagonally(side(game::CUBEMAPSHOT_LEFT), side_size, side_size, face_pixel_size);
				flip_horizontally(side(game::CUBEMAPSHOT_LEFT), side_size, side_size, face_pixel_size);
				flip_vertically(side(game::CUBEMAPSHOT_LEFT), side_size, side_size, face_pixel_size);

				flip_vertically(side(game::CUBEMAPSHOT_BACK), side_size, side_size, face_pixel_size);

				flip_horizontally(side(game::CUBEMAPSHOT_FRONT), side_size, side_size, face_pixel_size);

				flip_diagonally(side(game::CUBEMAPSHOT_UP), side_size, side_size, face_pixel_size);

				flip_diagonally(side(game::CUBEMAPSHOT_DOWN), side_size, side_size, face_pixel_size);
			}

			dump_probe_array(faces, output_size);

			if (r_reflectionProbeGenerateExit && r_reflectionProbeGenerateExit->current.enabled)
			{
				game::Com_Quit_f();
			}

			game::Dvar_SetBool(r_reflectionProbeGenerate, false);
		}

		char scr_update_frame_stub()
		{
			do_render = 0;
			server_time = 0;

			const auto result = scr_update_frame_hook.invoke<char>();

			if (r_reflectionProbeGenerate && r_reflectionProbeGenerate->current.enabled && do_render &&
				game::CL_IsGameClientActive(0) && server_time > 1000)
			{
				game::R_SyncRenderThread();

				const auto* gfxworld = *game::g_world;
				R_GenerateReflections(gfxworld->draw.reflectionProbeData.reflectionProbes,
					gfxworld->draw.reflectionProbeData.reflectionProbeCount);
			}

			return result;
		}

		int cg_draw_active_frame_stub(int localClientNum, int serverTime, int demoType, int cubemapShot,
			int cubemapSize, int renderScreen, unsigned int drawType)
		{
			const auto result = cg_draw_active_frame_hook.invoke<int>(localClientNum, serverTime, demoType,
				cubemapShot, cubemapSize, renderScreen, drawType);

			server_time = serverTime;
			do_render = result;
			return result;
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

			scheduler::once([]()
			{
				r_reflectionProbeGenerate = game::Dvar_RegisterBool("r_reflectionProbeGenerate", false, game::DVAR_FLAG_NONE,
					"Generate cube maps for reflection probes");
				r_reflectionProbeGenerateExit = game::Dvar_RegisterBool("r_reflectionProbeGenerateExit", false, game::DVAR_FLAG_NONE,
					"Exit when done generating reflection probes");
				r_reflectionProbeGenerateTarget = game::Dvar_RegisterString("r_reflectionProbeGenerateTarget", "Frontend Color", game::DVAR_FLAG_NONE,
					"Render target the reflection probe faces are read from, \"Frontend Color\" is linear HDR. Also accepts \"backbuffer\"");
				r_reflectionProbeGenerateFrames = game::Dvar_RegisterInt("r_reflectionProbeGenerateFrames", 128, 1, 1024, game::DVAR_FLAG_NONE,
					"Frames rendered per reflection probe face before it is captured");
				r_reflectionProbeGenerateFullView = game::Dvar_RegisterBool("r_reflectionProbeGenerateFullView", true, game::DVAR_FLAG_NONE,
					"Render reflection probe faces at the full view size and crop the face out of the middle");
				r_reflectionProbeGenerateGuardBand = game::Dvar_RegisterFloat("r_reflectionProbeGenerateGuardBand", 0.15f, 0.0f, 0.5f, game::DVAR_FLAG_NONE,
					"Extra view rendered around each reflection probe face and cropped away");
				r_reflectionProbeGenerateSize = game::Dvar_RegisterInt("r_reflectionProbeGenerateSize", 128, 16, 2048, game::DVAR_FLAG_NONE,
					"Reflection probe face resolution, rounded down to a power of two");
			}, scheduler::main);

			cg_draw_active_frame_hook.create(game::CG_DrawActiveFrame.get(), cg_draw_active_frame_stub);
			scr_update_frame_hook.create(0x1403471E0, scr_update_frame_stub);
			r_calc_cubemap_view_values_hook.create(0x140E21C10, r_calc_cubemap_view_values_stub);
			utils::hook::call(0x140E56382, r_present_frame_stub);
		}
	};
}

REGISTER_COMPONENT(reflection_probes::component)
#endif
