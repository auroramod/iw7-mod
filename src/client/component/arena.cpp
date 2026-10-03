#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "fastfiles.hpp"
#include "scheduler.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

#define MAX_ARENAS 64
#define MAP_INFO_SIZE 3032
#define MAP_INFO_LONGNAME 0
#define MAP_INFO_LOADNAME 32
#define MAP_INFO_DESCRIPTION 48
#define MAP_INFO_MAPIMAGE 80
#define MAP_INFO_MAPVOTEIMAGE 112
#define MAP_INFO_TEXT_SIZE 32
#define MAP_INFO_ALIENS 1232

namespace arena
{
	namespace
	{
		std::recursive_mutex arena_mutex;

		utils::hook::detour load_arenas_hook;
		utils::hook::detour build_map_infos_hook;

		bool parse_arena(const std::string& path)
		{
			std::lock_guard<std::recursive_mutex> _0(arena_mutex);

			std::string buffer{};
			if (!utils::io::read_file(path, &buffer) || buffer.empty())
			{
				return false;
			}

			*game::ui_num_arenas += game::GameInfo_ParseArenas(buffer.data(), MAX_ARENAS - *game::ui_num_arenas,
				&game::ui_arena_infos[*game::ui_num_arenas]);
			return true;
		}

		std::string get_art_zone(const std::string& mapname)
		{
			return mapname + "_art";
		}

		bool has_art_zone(const std::string& mapname)
		{
			return utils::io::file_exists(utils::string::va("usermaps\\%s\\%s.ff", mapname.data(), get_art_zone(mapname).data()));
		}

		void set_default_image(char* field, const std::string& image)
		{
			if (*field)
			{
				return;
			}

			strncpy_s(field, MAP_INFO_TEXT_SIZE, image.data(), _TRUNCATE);
		}

		void load_arenas_stub(const char* mode)
		{
			load_arenas_hook.invoke<void>(mode);

			if (!std::filesystem::exists("usermaps"))
			{
				return;
			}

			for (const auto& entry : std::filesystem::directory_iterator("usermaps"))
			{
				if (!entry.is_directory())
				{
					continue;
				}

				const auto mapname = entry.path().filename().string();
				const auto arena_path = entry.path().string() + "/" + mapname + ".arena";

				parse_arena(arena_path);
			}
		}

		void make_literal(char* field, const size_t size)
		{
			if (!*field || *field == 0x1F || game::DB_XAssetExists(game::ASSET_TYPE_LOCALIZE_ENTRY, field))
			{
				return;
			}

			const std::string text = field;
			field[0] = 0x1F;
			strncpy_s(field + 1, size - 1, text.data(), _TRUNCATE);
		}

		void build_map_infos_stub()
		{
			build_map_infos_hook.invoke<void>();

			for (auto i = 0; i < *game::ui_num_maps; i++)
			{
				auto* map_info = &game::ui_map_infos[MAP_INFO_SIZE * i];
				const std::string loadname = map_info + MAP_INFO_LOADNAME;

				if (loadname.starts_with("cp_"))
				{
					*reinterpret_cast<int*>(map_info + MAP_INFO_ALIENS) = 1;
				}

				if (fastfiles::usermap_exists(loadname))
				{
					make_literal(map_info + MAP_INFO_LONGNAME, MAP_INFO_TEXT_SIZE);
					make_literal(map_info + MAP_INFO_DESCRIPTION, MAP_INFO_TEXT_SIZE);

					if (has_art_zone(loadname))
					{
						set_default_image(map_info + MAP_INFO_MAPIMAGE, "image_" + loadname);
						set_default_image(map_info + MAP_INFO_MAPVOTEIMAGE, "image_" + loadname + "_vote");
					}
				}
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			load_arenas_hook.create(0x1405AF240, load_arenas_stub);
			build_map_infos_hook.create(0x1405AF570, build_map_infos_stub);

			scheduler::once([]()
			{
				std::string posters{};
				if (std::filesystem::exists("usermaps"))
				{
					for (const auto& entry : std::filesystem::directory_iterator("usermaps"))
					{
						const auto mapname = entry.path().filename().string();
						if (entry.is_directory() && mapname.starts_with("cp_") && has_art_zone(mapname))
						{
							posters.append(posters.empty() ? "" : " ").append(mapname);
						}
					}
				}

				game::Dvar_RegisterString("ui_usermap_posters", posters.data(), game::DVAR_FLAG_NONE, "");
			}, scheduler::pipeline::main);
		}
	};
}

REGISTER_COMPONENT(arena::component)
