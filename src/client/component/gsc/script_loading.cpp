#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/console/console.hpp"
#include "component/fastfiles.hpp"
#include "component/filesystem.hpp"
#include "component/scripting.hpp"

#include "script_extension.hpp"

#include "game/game.hpp"

#include "script_loading.hpp"

#include <utils/compression.hpp>
#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

namespace gsc
{
	std::unique_ptr<xsk::gsc::iw7::context> gsc_ctx = std::make_unique<xsk::gsc::iw7::context>(xsk::gsc::instance::server);
	std::unordered_map<std::string, loaded_script_t> loaded_scripts;

	namespace
	{
		utils::hook::detour scr_begin_load_scripts_hook;
		utils::hook::detour scr_end_load_scripts_hook;

		std::unordered_map<std::string, std::uint32_t> main_handles;
		std::unordered_map<std::string, std::uint32_t> init_handles;

		utils::memory::allocator scriptfile_allocator;

		char* script_mem_buf = nullptr;
		std::uint64_t script_mem_buf_size = 0;

		struct
		{
			char* buf = nullptr;
			char* pos = nullptr;
			const std::uint64_t size = 0x100000i64;
		} script_memory;

		char* allocate_buffer(size_t size)
		{
			if (script_memory.buf == nullptr)
			{
				script_memory.buf = script_mem_buf;
				script_memory.pos = script_memory.buf;
			}

			if (script_memory.pos + size > script_memory.buf + script_memory.size)
			{
				game::Com_Error(game::ERR_FATAL, "Out of custom script memory");
			}

			const auto pos = script_memory.pos;
			script_memory.pos += size;
			return pos;
		}

		void free_script_memory()
		{
			if (script_memory.buf != nullptr)
			{
				memset(script_memory.buf, 0, reinterpret_cast<size_t>(script_memory.pos) - reinterpret_cast<size_t>(script_memory.buf));
				script_memory.buf = nullptr;
				script_memory.pos = nullptr;
			}
		}

		void clear()
		{
			main_handles.clear();
			init_handles.clear();
			loaded_scripts.clear();
			scriptfile_allocator.clear();
			free_script_memory();
		}

		bool read_raw_script_file(const std::string& name, std::string* data)
		{
			if (filesystem::read_file(name, data))
			{
				return true;
			}

			const auto* name_str = name.data();
			if (game::DB_XAssetExists(game::ASSET_TYPE_RAWFILE, name_str) &&
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_RAWFILE, name_str))
			{
				const auto asset = game::DB_FindXAssetHeader(game::ASSET_TYPE_RAWFILE, name_str, false);
				const auto len = game::DB_GetRawFileLen(asset.rawfile);
				data->resize(len);
				game::DB_GetRawBuffer(asset.rawfile, data->data(), len);
				if (len > 0)
				{
					data->pop_back();
				}

				return true;
			}

			return false;
		}

		std::map<std::uint32_t, col_line_t> parse_devmap(const xsk::gsc::buffer& devmap)
		{
			auto data = devmap.data;

			const auto read_32 = [&]()
			{
				const auto val = *reinterpret_cast<const std::uint32_t*>(data);
				data += sizeof(std::uint32_t);
				return val;
			};

			const auto read_16 = [&]()
			{
				const auto val = *reinterpret_cast<const std::uint16_t*>(data);
				data += sizeof(std::uint16_t);
				return val;
			};

			std::map<std::uint32_t, col_line_t> pos_map;

			const auto devmap_count = read_32();
			for (auto i = 0u; i < devmap_count; i++)
			{
				const auto script_pos = read_32();
				const auto line = read_16();
				const auto col = read_16();

				pos_map[script_pos] = { line, col };
			}

			return pos_map;
		}

		game::ScriptFile* load_custom_script(const char* file_name, const std::string& real_name)
		{
			if (const auto itr = loaded_scripts.find(file_name); itr != loaded_scripts.end())
			{
				return itr->second.ptr;
			}

			if (game::Com_FrontEnd_IsInFrontEnd())
			{
				return nullptr;
			}

			std::string source_buffer{};
			if (!read_raw_script_file(real_name + ".gsc", &source_buffer) || source_buffer.empty())
			{
				return nullptr;
			}

			// filter out "GSC rawfiles" that were used for development usage and are not meant for us.
			// each "GSC rawfile" has a ScriptFile counterpart to be used instead
			if (game::DB_XAssetExists(game::ASSET_TYPE_SCRIPTFILE, file_name) &&
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_SCRIPTFILE, file_name))
			{
				if (real_name.starts_with(utils::string::va("scripts/%s/maps/", game::Com_GameMode_GetActiveGameModeStr()))
					&& (real_name.ends_with("_fx") || real_name.ends_with("_sound")))
				{
					console::debug("Refusing to compile rawfile '%s'\n", real_name.data());
					return game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, file_name, false).scriptfile;
				}
			}

			console::debug("Loading custom gsc '%s.gsc'", real_name.data());

			try
			{
				auto& compiler = gsc_ctx->compiler();
				auto& assembler = gsc_ctx->assembler();

				std::vector<std::uint8_t> data;
				data.assign(source_buffer.begin(), source_buffer.end());

				const auto assembly_ptr = compiler.compile(real_name, data);
				const auto& [bytecode, stack, devmap] = assembler.assemble(*assembly_ptr);

				const auto script_file_ptr = static_cast<game::ScriptFile*>(scriptfile_allocator.allocate(sizeof(game::ScriptFile)));
				script_file_ptr->name = file_name;

				script_file_ptr->len = static_cast<int>(stack.size);
				script_file_ptr->bytecodeLen = static_cast<int>(bytecode.size);

				const auto stack_size = static_cast<std::uint32_t>(stack.size + 1);
				const auto byte_code_size = static_cast<std::uint32_t>(bytecode.size + 1);

				script_file_ptr->buffer = static_cast<char*>(scriptfile_allocator.allocate(stack_size));
				std::memcpy(const_cast<char*>(script_file_ptr->buffer), stack.data, stack.size);

				script_file_ptr->bytecode = allocate_buffer(byte_code_size);
				std::memcpy(script_file_ptr->bytecode, bytecode.data, bytecode.size);

				script_file_ptr->compressedLen = 0;

				loaded_script_t loaded_script{};
				loaded_script.ptr = script_file_ptr;
				loaded_script.devmap = parse_devmap(devmap);
				loaded_scripts.insert(std::make_pair(file_name, loaded_script));

				console::debug("Loaded custom gsc '%s.gsc'", real_name.data());

				return script_file_ptr;
			}
			catch (const std::exception& e)
			{
				console::error("*********** script compile error *************\n");
				console::error("failed to compile '%s':\n%s", real_name.data(), e.what());
				console::error("**********************************************\n");
				return nullptr;
			}
		}

		std::string get_raw_script_file_name(const std::string& name)
		{
			if (name.ends_with(".gsh"))
			{
				return name;
			}

			return name + ".gsc";
		}

		std::string get_script_file_name(const std::string& name)
		{
			const auto id = gsc_ctx->token_id(name);
			if (!id)
			{
				return name;
			}

			return std::to_string(id);
		}

		std::pair<xsk::gsc::buffer, std::vector<std::uint8_t>> read_compiled_script_file(const std::string& name, const std::string& real_name)
		{
			const auto* script_file = game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, name.data(), false).scriptfile;
			if (script_file == nullptr)
			{
				throw std::runtime_error(std::format("Could not load scriptfile '{}'", real_name));
			}

			console::debug("Decompiling scriptfile '%s'\n", real_name.data());

			const auto len = script_file->compressedLen;
			const std::string stack{script_file->buffer, static_cast<std::uint32_t>(len)};

			const auto decompressed_stack = utils::compression::zlib::decompress(stack);

			std::vector<std::uint8_t> stack_data;
			stack_data.assign(decompressed_stack.begin(), decompressed_stack.end());

			return {{reinterpret_cast<std::uint8_t*>(script_file->bytecode), static_cast<std::uint32_t>(script_file->bytecodeLen)}, stack_data};
		}

		void load_script(const std::string& name)
		{
			if (!game::Scr_LoadScript(name.data()))
			{
				return;
			}

			const auto main_handle = game::Scr_GetFunctionHandle(name.data(), gsc_ctx->token_id("main"));
			const auto init_handle = game::Scr_GetFunctionHandle(name.data(), gsc_ctx->token_id("init"));

			if (main_handle)
			{
				console::debug("Loaded '%s::main'\n", name.data());
				main_handles[name] = main_handle;
			}

			if (init_handle)
			{
				console::debug("Loaded '%s::init'\n", name.data());
				init_handles[name] = init_handle;
			}
		}

		void load_scripts(const std::filesystem::path& root_dir, const std::filesystem::path& subfolder)
		{
			std::filesystem::path script_dir = root_dir / subfolder;
			if (!utils::io::directory_exists(script_dir.generic_string()))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir.generic_string());
			for (const auto& script : scripts)
			{
				if (!script.ends_with(".gsc"))
				{
					continue;
				}

				std::filesystem::path path(script);
				const auto relative = path.lexically_relative(root_dir).generic_string();
				const auto base_name = relative.substr(0, relative.size() - 4);

				load_script(base_name);
			}
		}

		void load_scripts()
		{
			if (!game::Com_FrontEnd_IsInFrontEnd())
			{
				for (const auto& path : filesystem::get_search_paths())
				{
					load_scripts(path, "custom_scripts/");
					load_scripts(path, "custom_scripts/"s + game::Com_GameMode_GetActiveGameModeStr() + "/");

					if (game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_CP || game::Com_GameMode_GetActiveGameMode() == game::GAME_MODE_MP)
					{
						load_scripts(path, "custom_scripts/cp_mp/");
					}
				}
			}
		}

		void init_compiler()
		{
			const bool dev_script = developer_script ? developer_script->current.enabled : false;
			const auto comp_mode = dev_script ?
				xsk::gsc::build::dev :
				xsk::gsc::build::prod;

			gsc_ctx->init(comp_mode, []([[maybe_unused]] auto const* ctx, const auto& included_path) -> std::pair<xsk::gsc::buffer, std::vector<std::uint8_t>>
			{
				const auto script_name = std::filesystem::path(included_path).replace_extension().string();

				std::string file_buffer;
				if (!read_raw_script_file(included_path, &file_buffer) || file_buffer.empty())
				{
					const auto name = get_script_file_name(script_name);
					if (game::DB_XAssetExists(game::ASSET_TYPE_SCRIPTFILE, name.data()))
					{
						return read_compiled_script_file(name, script_name);
					}

					throw std::runtime_error(std::format("Could not load gsc file '{}'", script_name));
				}

				std::vector<std::uint8_t> script_data;
				script_data.assign(file_buffer.begin(), file_buffer.end());

				return {{}, script_data};
			});
		}

		void scr_begin_load_scripts_stub(bool a1)
		{
			// start the compiler
			init_compiler();

			scr_begin_load_scripts_hook.invoke<void>(a1);

			 // load scripts
			load_scripts();
		}

		void scr_end_load_scripts_stub(const char* a1)
		{
			// cleanup the compiler
			gsc_ctx->cleanup();

			scr_end_load_scripts_hook.invoke<void>(a1);
		}

		utils::hook::detour g_load_structs_hook;
		void g_load_structs_stub(float a1)
		{
			for (auto& function_handle : main_handles)
			{
				console::debug("Executing '%s::main'\n", function_handle.first.data());
				game::RemoveRefToObject(game::Scr_ExecThread(function_handle.second, 0));
			}

			g_load_structs_hook.invoke<void>(a1);
		}

		utils::hook::detour scr_load_level_hook;
		void scr_load_level_stub()
		{
			for (auto& function_handle : init_handles)
			{
				console::debug("Executing '%s::init'\n", function_handle.first.data());
				game::RemoveRefToObject(game::Scr_ExecThread(function_handle.second, 0));
			}

			scr_load_level_hook.invoke<void>();
		}

		int db_is_x_asset_default(game::XAssetType type, const char* name)
		{
			if (loaded_scripts.contains(name))
			{
				return 0;
			}

			return game::DB_IsXAssetDefault(type, name);
		}

		void db_get_raw_buffer_stub(const game::RawFile* rawfile, char* buf, const int size)
		{
			if (rawfile->len > 0 && rawfile->compressedLen == 0)
			{
				std::memset(buf, 0, size);
				std::memcpy(buf, rawfile->buffer, std::min(rawfile->len, size));
				return;
			}

			game::DB_GetRawBuffer(rawfile, buf, size);
		}

		// donetsk developers will paste this in days
		utils::hook::detour db_alloc_x_zone_memory_internal_hook;
		void db_alloc_x_zone_memory_internal_stub(unsigned __int64* blockSize, const char* filename, game::XZoneMemory* zoneMem, unsigned int type)
		{
			bool patch = false; // ugly fix for script memory allocation
			if (!_stricmp(filename, "code_post_gfx") && type == 2)
			{
				patch = true;
				console::debug("patching memory for '%s'\n", filename);
			}

			if (patch)
			{
				blockSize[game::XFILE_BLOCK_SCRIPT] += script_memory.size;
			}

			db_alloc_x_zone_memory_internal_hook.invoke<void>(blockSize, filename, zoneMem, type);

			if (patch)
			{
				blockSize[game::XFILE_BLOCK_SCRIPT] -= script_memory.size;
				script_mem_buf = zoneMem->blocks[game::XFILE_BLOCK_SCRIPT].alloc + blockSize[game::XFILE_BLOCK_SCRIPT];
				script_mem_buf_size = script_memory.size;
			}
		}
	}

	game::ScriptFile* find_script(game::XAssetType type, const char* name, int allow_create_default)
	{
		std::string real_name = name;
		const auto id = static_cast<std::uint16_t>(std::atoi(name));
		if (id)
		{
			real_name = gsc_ctx->token_name(id);
		}

		auto* script = load_custom_script(name, real_name);
		if (script)
		{
			return script;
		}

		return game::DB_FindXAssetHeader(type, name, allow_create_default).scriptfile;
	}

	loaded_script_t* get_loaded_script(const std::string& name)
	{
		if (loaded_scripts.contains(name))
		{
			return &loaded_scripts[name];
		}
		return nullptr;
	}

	class loading final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Allocate script memory (PMem doesn't work)
			db_alloc_x_zone_memory_internal_hook.create(0x140A75450, db_alloc_x_zone_memory_internal_stub);

			// Increase allocated script memory
			utils::hook::set<uint32_t>(0x140A75B5C + 1, 0x480000 + static_cast<std::uint32_t>(script_memory.size));
			utils::hook::set<uint32_t>(0x140A75BAA + 4, 0x480 + (static_cast<std::uint32_t>(script_memory.size) >> 12));
			utils::hook::set<uint32_t>(0x140A75BBE + 6, 0x480 + (static_cast<std::uint32_t>(script_memory.size) >> 12));

			// Load our scripts with an uncompressed stack
			utils::hook::call(0x140C09DA7, db_get_raw_buffer_stub);

			// Compiler start and cleanup, also loads scripts
			scr_begin_load_scripts_hook.create(0x140BFD500, scr_begin_load_scripts_stub);
			scr_end_load_scripts_hook.create(0x140BFD630, scr_end_load_scripts_stub);

			// ProcessScript: hook xasset functions to return our own custom scripts
			utils::hook::call(0x140C09D37, find_script);
			utils::hook::call(0x140C09D47, db_is_x_asset_default);

			// execute main handle
			g_load_structs_hook.create(0x140409FB0, g_load_structs_stub);

			// execute init handle
			scr_load_level_hook.create(0x140B51B40, scr_load_level_stub);

			scripting::on_shutdown([](bool free_scripts, bool post_shutdown)
			{
				if (free_scripts && post_shutdown)
				{
					clear();
				}
			});
		}
	};
}

REGISTER_COMPONENT(gsc::loading)
