#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "fastfiles.hpp"

#include "game/game.hpp"

#include "command.hpp"
#include "console/console.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/concurrency.hpp>
#include <utils/io.hpp>

#include <zlib.h>

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
		utils::hook::detour db_add_xasset_hook;

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

		game::dvar_t* g_dump_scripts;
		void dump_gsc_script(const std::string& name, game::XAssetHeader header)
		{
			if (!g_dump_scripts->current.enabled)
			{
				return;
			}

			std::string buffer;
			buffer.append(header.scriptfile->name, strlen(header.scriptfile->name) + 1);
			buffer.append(reinterpret_cast<char*>(&header.scriptfile->compressedLen), 4);
			buffer.append(reinterpret_cast<char*>(&header.scriptfile->len), 4);
			buffer.append(reinterpret_cast<char*>(&header.scriptfile->bytecodeLen), 4);
			buffer.append(header.scriptfile->buffer, header.scriptfile->compressedLen);
			buffer.append(header.scriptfile->bytecode, header.scriptfile->bytecodeLen);

			const auto out_name = utils::string::va("gsc_dump/%s.gscbin", name.data());
			utils::io::write_file(out_name, buffer);

			console::info("Dumped %s\n", out_name);
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

		game::XAssetHeader db_add_xasset_stub(game::XAssetType type, game::XAssetHeader* header_ptr)
		{
			auto header = *header_ptr;
			
			if (type == game::ASSET_TYPE_SCRIPTFILE && header.scriptfile)
			{
				dump_gsc_script(header.scriptfile->name ? header.scriptfile->name : "__unnamed__", header);
			}

			auto result = db_add_xasset_hook.invoke<game::XAssetHeader>(type, header_ptr);
			return result;
		}
	}

	namespace zone_loading
	{
		utils::hook::detour db_is_patch_hook;

		bool check_missing_content_func(const char* zone_name)
		{
			const char* lang_code = game::SEH_GetCurrentLanguageCode();
			char buffer[0x100]{ 0 };
			const auto len = sprintf_s(buffer, "%s_", lang_code);

			if (!strncmp(zone_name, buffer, len))
			{
				printf("Tried to load missing language zone: %s\n", zone_name);
				return true;
			}

			return false;
		}

		bool db_is_patch_stub(const char* name)
		{
			if (db_is_patch_hook.invoke<bool>(name)) return true;
			if (check_missing_content_func(name)) return true;
			return false;
		}

		void skip_extra_zones_stub(utils::hook::assembler& a)
		{
			const auto skip = a.newLabel();
			const auto original = a.newLabel();

			//a.pushad64();
			a.test(edi, game::DB_ZONE_CUSTOM); // allocFlags
			a.jnz(skip);

			a.bind(original);
			//a.popad64();
			a.call(0x3BC450_b); // strnicmp_ffotd
			a.mov(r12d, edi);
			a.mov(rdx, 0x1467970_b); // "patch_"
			a.jmp(0x3BA9C0_b);

			a.bind(skip);
			//a.popad64();
			a.mov(r12d, game::DB_ZONE_CUSTOM);
			a.not_(r12d);
			a.and_(edi, r12d);
			a.jmp(0x3BAC06_b);
		}
	}
	using namespace zone_loading;

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
			db_add_xasset_hook.create(0xA76520_b, db_add_xasset_stub);

			g_dump_scripts = game::Dvar_RegisterBool("g_dumpScripts", false, game::DVAR_FLAG_NONE, "Dump GSC scripts");

			// Don't fatal on certain missing zones
			db_is_patch_hook.create(0x3BC580_b, db_is_patch_stub);
			// Don't load extra zones with loadzone
			utils::hook::nop(0x3BA9B1_b, 15);
			utils::hook::jump(0x3BA9B1_b, utils::hook::assemble(skip_extra_zones_stub), true);

			// Allow loading of unsigned fastfiles
			utils::hook::set<uint8_t>(0x9E8CAE_b, 0xEB); // DB_InflateInit

			// Skip signature validation
			utils::hook::set(0x9E6390_b, 0xC301B0);

			command::add("loadzone", [](const command::params& params)
			{
				if (params.size() < 2)
				{
					console::info("usage: loadzone <zone>\n");
					return;
				}

				const char* name = params.get(1);

				if (!fastfiles::exists(name))
				{
					console::warn("loadzone: zone %s could not be found!\n", name);
					return;
				}

				game::XZoneInfo info{};
				info.name = name;
				info.allocFlags = game::DB_ZONE_COMMON;
				info.allocFlags |= game::DB_ZONE_CUSTOM; // skip extra zones with this flag

				game::DB_LoadXAssets(&info, 1, game::DBSyncMode::DB_LOAD_ASYNC);
			});

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