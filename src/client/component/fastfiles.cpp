#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "fastfiles.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console/console.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>

//#define XFILE_DEBUG

namespace fastfiles
{
	static utils::concurrency::container<std::string> current_fastfile;

	std::string get_current_fastfile()
	{
		std::string fastfile_copy;
		current_fastfile.access([&](std::string& fastfile)
		{
			fastfile_copy = fastfile;
		});
		return fastfile_copy;
	}

	namespace
	{
		utils::hook::detour db_try_load_x_file_internal_hook;
		utils::hook::detour db_init_load_x_file_hook;
		utils::hook::detour db_load_x_zone_hook;
		utils::hook::detour db_find_xasset_header_hook;

		void db_try_load_x_file_internal_stub(const char* zone_name, const unsigned int zone_flags,
			const bool is_base_map, const bool was_paused, const int failure_mode)
		{
			current_fastfile.access([&](std::string& fastfile)
			{
				fastfile = zone_name;
			});
			return db_try_load_x_file_internal_hook.invoke<void>(zone_name, zone_flags, is_base_map, was_paused, failure_mode);
		}

		void db_init_load_x_file_stub(const char* name, std::uint64_t offset)
		{
			console::info("Loading xfile %s\n", name);
			return db_init_load_x_file_hook.invoke<void>(name, offset);
		}

		void db_load_x_zone_stub(const char* parent_name, const unsigned int zone_flags, const bool is_base_map, const int failure_mode)
		{
			console::info("Loading zone %s\n", parent_name);
			return db_load_x_zone_hook.invoke<void>(parent_name, zone_flags, is_base_map, failure_mode);
		}

		game::XAssetHeader db_find_xasset_header_stub(game::XAssetType type, const char* name, const int allow_create_default)
		{
			auto result = db_find_xasset_header_hook.invoke<game::XAssetHeader>(type, name, allow_create_default);
			if (!result.data && type != game::ASSET_TYPE_LOCALIZE_ENTRY)
			{
				console::error("Error: Could not find %s \"%s\"\n",
					game::g_assetNames[static_cast<unsigned int>(type)],
					name);
			}
			return result;
		}
	}

	bool exists(const std::string& zone)
	{
		const auto is_localized = game::DB_IsLocalized(zone.data());
		const auto handle = game::Sys_CreateFile((is_localized ? game::SF_ZONE_LOC : game::SF_ZONE),
			utils::string::va("%s.ff", zone.data()));

		if (handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}

		return false;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			db_try_load_x_file_internal_hook.create(0x3BBC40_b, db_try_load_x_file_internal_stub);
#if defined(DEBUG) and defined(XFILE_DEBUG)
			db_init_load_x_file_hook.create(0x9E8D10_b, db_init_load_x_file_stub);
#else
			db_load_x_zone_hook.create(0x3BA920_b, db_load_x_zone_stub);
#endif

			db_find_xasset_header_hook.create(game::DB_FindXAssetHeader, db_find_xasset_header_stub);

			command::add("listassetpool", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("listassetpool <poolnumber> [filter]: list all the assets in the specified pool\n");

					for (auto i = 0; i < game::XAssetType::ASSET_TYPE_COUNT; i++)
					{
						console::info("%d %s\n", i, game::g_assetNames[i]);
					}
					return;
				}

				const auto type = static_cast<game::XAssetType>(atoi(params.get(1)));

				if (type < 0 || type >= game::XAssetType::ASSET_TYPE_COUNT)
				{
					console::error("Invalid pool passed must be between [%d, %d]\n", 0, game::XAssetType::ASSET_TYPE_COUNT - 1);
					return;
				}

				console::info("Listing assets in pool %s\n", game::g_assetNames[type]);

				const std::string filter = params.get(2);
				game::DB_EnumXAssets(type, [type, filter](const game::XAssetHeader header)
				{
					const game::XAsset asset{ type, header };
					const auto* const asset_name = game::DB_GetXAssetName(&asset);

					if (!filter.empty() && !utils::string::match_compare(filter, asset_name, false))
					{
						return;
					}

					console::info("%s\n", asset_name);
				});
			});
		}
	};
}

REGISTER_COMPONENT(fastfiles::component)