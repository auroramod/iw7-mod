#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/console.hpp"
#include "component/fastfiles.hpp"
#include "component/filesystem.hpp"
#include "component/logfile.hpp"
#include "component/scripting.hpp"
#include "component/gsc/script_loading.hpp"

#include "game/dvars.hpp"

#include "game/scripting/array.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/function.hpp"

#include <xsk/gsc/types.hpp>
#include <xsk/gsc/interfaces/compiler.hpp>
#include <xsk/gsc/interfaces/decompiler.hpp>
#include <xsk/gsc/interfaces/assembler.hpp>
#include <xsk/gsc/interfaces/disassembler.hpp>
#include <xsk/utils/compression.hpp>
#include <xsk/resolver.hpp>
#include <interface.hpp>

#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

namespace gsc
{
	namespace
	{
		auto compiler = ::gsc::compiler();
		auto decompiler = ::gsc::decompiler();
		auto assembler = ::gsc::assembler();
		auto disassembler = ::gsc::disassembler();

		std::unordered_map<std::string, std::uint32_t> main_handles;
		std::unordered_map<std::string, std::uint32_t> init_handles;

		utils::memory::allocator scriptfile_allocator;
		std::unordered_map<std::string, game::ScriptFile*> loaded_scripts;

		struct
		{
			char* buf = nullptr;
			char* pos = nullptr;
			unsigned int size = 0x1000000;
		} script_memory;

		char* allocate_buffer(size_t size)
		{
			if (script_memory.buf == nullptr)
			{
				script_memory.buf = game::PMem_AllocFromSource_NoDebug(script_memory.size, 4, game::PMEM_SOURCE_SCRIPT);
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
			//game::PMem_PopFromSource_NoDebug(script_memory.buf, script_memory.size, 4, game::PMEM_SOURCE_SCRIPT);
			script_memory.buf = nullptr;
			script_memory.pos = nullptr;
		}

		void clear()
		{
			main_handles.clear();
			init_handles.clear();
			loaded_scripts.clear();
			scriptfile_allocator.clear();
			free_script_memory();
		}

		bool read_scriptfile(const std::string& name, std::string* data)
		{
			if (filesystem::read_file(name, data))
			{
				return true;
			}

			const auto name_str = name.data();
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

		bool force_load = false;

		game::ScriptFile* load_custom_script(const char* file_name, const std::string& real_name)
		{
			if (loaded_scripts.contains(real_name))
			{
				return loaded_scripts[real_name];
			}

			if (!force_load)
			{
				return nullptr;
			}

			std::string source_buffer;
			if (!read_scriptfile(real_name + ".gsc", &source_buffer) || source_buffer.empty())
			{
				return nullptr;
			}

			if (game::DB_XAssetExists(game::ASSET_TYPE_SCRIPTFILE, file_name) && 
				!game::DB_IsXAssetDefault(game::ASSET_TYPE_SCRIPTFILE, file_name))
			{
				// filter out gsc rawfiles that contain developer code (has ScriptFile counterparts for ship, won't compile either)
				if ((real_name.starts_with("maps/createfx") || real_name.starts_with("maps/createart") || real_name.starts_with("maps/mp")) 
					&& (real_name.ends_with("_fx") || real_name.ends_with("_fog") || real_name.ends_with("_hdr")))
				{
					return game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, file_name, false).scriptfile;
				}
			}

			auto data = std::vector<std::uint8_t>{source_buffer.begin(), source_buffer.end()};

			try
			{
				compiler->compile(real_name, data);
			}
			catch (const std::exception& e)
			{
				console::error("*********** script compile error *************\n");
				console::error("failed to compile '%s':\n%s", real_name.data(), e.what());
				console::error("**********************************************\n");
				return nullptr;
			}

			auto assembly = compiler->output();

			try
			{
				assembler->assemble(real_name, assembly);
			}
			catch (const std::exception& e)
			{
				console::error("*********** script compile error *************\n");
				console::error("failed to assemble '%s':\n%s", real_name.data(), e.what());
				console::error("**********************************************\n");
				return nullptr;
			}

			const auto script_file_ptr = scriptfile_allocator.allocate<game::ScriptFile>();
			script_file_ptr->name = file_name;

			const auto stack = assembler->output_stack();
			script_file_ptr->len = static_cast<int>(stack.size());

			const auto script = assembler->output_script();
			script_file_ptr->bytecodeLen = static_cast<int>(script.size());

			script_file_ptr->buffer = game::Hunk_AllocateTempMemoryHigh(stack.size() + 1);
			std::memcpy(script_file_ptr->buffer, stack.data(), stack.size());

			script_file_ptr->bytecode = allocate_buffer(script.size() + 1);
			std::memcpy(script_file_ptr->bytecode, script.data(), script.size());

			script_file_ptr->compressedLen = 0;

			loaded_scripts[real_name] = script_file_ptr;

			return script_file_ptr;
		}

		std::string get_script_file_name(const std::string& name)
		{
			const auto id = xsk::gsc::iw7::resolver::token_id(name);
			if (!id)
			{
				return name;
			}

			return std::to_string(id);
		}

		std::vector<std::uint8_t> decompile_scriptfile(const std::string& name, const std::string& real_name)
		{
			const auto* script_file = game::DB_FindXAssetHeader(game::ASSET_TYPE_SCRIPTFILE, name.data(), false).scriptfile;
			if (script_file == nullptr)
			{
				throw std::runtime_error(std::format("Could not load scriptfile '{}'", real_name));
			}

			console::info("Decompiling scriptfile '%s'\n", real_name.data());

			std::vector<std::uint8_t> stack{script_file->buffer, script_file->buffer + script_file->len};
			std::vector<std::uint8_t> bytecode{script_file->bytecode, script_file->bytecode + script_file->bytecodeLen};

			auto decompressed_stack = xsk::utils::zlib::decompress(stack, static_cast<std::uint32_t>(stack.size()));

			disassembler->disassemble(name, bytecode, decompressed_stack);
			auto output = disassembler->output();

			decompiler->decompile(name, output);

			return decompiler->output();
		}

		void load_script(const std::string& name)
		{
			if (!game::Scr_LoadScript(name.data()))
			{
				return;
			}

			const auto main_handle = game::Scr_GetFunctionHandle(name.data(), xsk::gsc::iw7::resolver::token_id("main"));
			const auto init_handle = game::Scr_GetFunctionHandle(name.data(), xsk::gsc::iw7::resolver::token_id("init"));

			if (main_handle)
			{
				console::info("Loaded '%s::main'\n", name.data());
				main_handles[name] = main_handle;
			}

			if (init_handle)
			{
				console::info("Loaded '%s::init'\n", name.data());
				init_handles[name] = init_handle;
			}
		}

		void load_scripts(const std::filesystem::path& root_dir, const std::filesystem::path& script_dir)
		{
			std::filesystem::path script_dir_path = root_dir / script_dir;
			if (!utils::io::directory_exists(script_dir_path.generic_string()))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir_path.generic_string());
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

		int db_is_x_asset_default(game::XAssetType type, const char* name)
		{
			if (loaded_scripts.contains(name))
			{
				return 0;
			}

			return game::DB_IsXAssetDefault(type, name);
		}

		void gscr_load_gametype_script_stub(void* a1, void* a2)
		{
			utils::hook::invoke<void>(0x5D390_b, a1, a2);

			for (const auto& path : filesystem::get_search_paths())
			{
				load_scripts(path, "scripts/mp/");
				load_scripts(path, "scripts/");
				force_load = true;
				const auto _0 = gsl::finally([&]
				{
					force_load = false;
				});
				load_scripts(path, "scripts/mp_patches/");
				
			}
		}

		void db_get_raw_buffer_stub(const game::RawFile* rawfile, char* buf, const int size)
		{
			if (rawfile->len > 0 && rawfile->compressedLen == 0)
			{
				std::memset(buf, 0, size);
				std::memcpy(buf, rawfile->buffer, std::min(rawfile->len, size));
				return;
			}

			utils::hook::invoke<void>(0xA77AB0_b, rawfile, buf, size);
		}

		//void pmem_init_stub()
		//{
		//	utils::hook::invoke<void>(SELECT_VALUE(0x420260_b, 0x5A5590_b));

		//	const auto type_0 = &game::g_scriptmem[0];
		//	const auto type_1 = &game::g_scriptmem[1];

		//	const auto size_0 = 0x100000; // default size
		//	const auto size_1 = 0x100000 + script_memory.size;

		//	const auto block = reinterpret_cast<char*>(VirtualAlloc(NULL, size_0 + size_1, MEM_RESERVE, PAGE_READWRITE));

		//	type_0->buf = block;
		//	type_0->size = size_0;

		//	type_1->buf = block + size_0;
		//	type_1->size = size_1;

		//	utils::hook::set<uint32_t>(SELECT_VALUE(0x420252_b, 0x5A5582_b), size_0 + size_1);
		//}
	}

	void load_main_handles()
	{
		for (auto& function_handle : main_handles)
		{
			console::info("Executing '%s::main'\n", function_handle.first.data());
			game::RemoveRefToObject(game::Scr_ExecThread(function_handle.second, 0));
		}
	}

	void load_init_handles()
	{
		for (auto& function_handle : init_handles)
		{
			console::info("Executing '%s::init'\n", function_handle.first.data());
			game::RemoveRefToObject(game::Scr_ExecThread(function_handle.second, 0));
		}
	}

	game::assets::ScriptFile* find_script(game::assets::XAssetType type, const char* name, int allow_create_default)
	{
		std::string real_name = name;
		const auto id = static_cast<std::uint16_t>(std::atoi(name));
		if (id)
		{
			real_name = xsk::gsc::iw7::resolver::token_name(id);
		}

		auto* script = load_custom_script(name, real_name);
		if (script)
		{
			return script;
		}

		return game::DB_FindXAssetHeader(type, name, allow_create_default).scriptfile;
	}

	class loading final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// allow custom scripts to include other custom scripts
			xsk::gsc::iw7::resolver::init([](const auto& include_name)
			{
				const auto real_name = include_name + ".gsc";

				std::string file_buffer;
				if (!read_scriptfile(real_name, &file_buffer) || file_buffer.empty())
				{
					const auto name = get_script_file_name(include_name);
					if (game::DB_XAssetExists(game::ASSET_TYPE_SCRIPTFILE, name.data()))
					{
						return decompile_scriptfile(name, real_name);
					}
					else
					{
						throw std::runtime_error(std::format("Could not load gsc file '{}'", real_name));
					}
				}

				std::vector<std::uint8_t> result;
				result.assign(file_buffer.begin(), file_buffer.end());

				return result;
			});

			// hook xasset functions to return our own custom scripts
			utils::hook::call(0xC09D37_b, find_script);
			utils::hook::call(0xC09D47_b, db_is_x_asset_default);

			// GScr_LoadScripts
			utils::hook::call(0xB5C98A_b, gscr_load_gametype_script_stub);

			// loads scripts with an uncompressed stack
			utils::hook::call(0xC09DA7_b, db_get_raw_buffer_stub);

			// Increase script memory
			//utils::hook::call(SELECT_VALUE(0x38639C_b, 0x15C4D6_b), pmem_init_stub);

			scripting::on_shutdown([](bool free_scripts, bool post_shutdown)
			{
				if (free_scripts && post_shutdown)
				{
					xsk::gsc::iw7::resolver::cleanup();
					clear();
				}
			});
		}
	};
}

//REGISTER_COMPONENT(gsc::loading)