#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"


#include "command.hpp"
#include "console/console.hpp"
#include "fastfiles.hpp"
#include "filesystem.hpp"
#include "game_module.hpp"
#include "localized_strings.hpp"
#include "party.hpp"
#include "scheduler.hpp"
#include "scripting.hpp"
#include "server_list.hpp"

#include "game/ui_scripting/execution.hpp"
//#include "game/scripting/execution.hpp"

#include "ui_scripting.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/io.hpp>
#include <utils/binary_resource.hpp>

#include "steam/steam.hpp"

namespace ui_scripting
{
	namespace
	{
		std::unordered_map<game::hks::cclosure*, std::function<arguments(const function_arguments& args)>> converted_functions;

		utils::hook::detour hks_start_hook;
		utils::hook::detour hks_shutdown_hook;
		utils::hook::detour hks_package_require_hook;

		utils::hook::detour hks_load_hook;

		/*
		const auto lui_common = utils::nt::load_resource(LUI_COMMON);
		const auto lui_updater = utils::nt::load_resource(LUI_UPDATER);
		const auto lua_json = utils::nt::load_resource(LUA_JSON);
		*/

		struct globals_t
		{
			std::string in_require_script;
			std::unordered_map<std::string, std::string> loaded_scripts;
			bool load_raw_script{};
			std::string raw_script_name{};
		};

		globals_t globals{};

		bool is_loaded_script(const std::string& name)
		{
			return globals.loaded_scripts.contains(name);
		}

		std::string get_root_script(const std::string& name)
		{
			const auto itr = globals.loaded_scripts.find(name);
			return itr == globals.loaded_scripts.end() ? std::string() : itr->second;
		}

		void print_error(const std::string& error)
		{
			console::error("************** LUI script execution error **************\n");
			console::error("%s\n", error.data());
			console::error("********************************************************\n");
		}

		void print_loading_script(const std::string& name)
		{
			console::info("Loading LUI script '%s'\n", name.data());
		}

		std::string get_current_script()
		{
			const auto state = *game::hks::lua_state;
			game::hks::lua_Debug info{};
			game::hks::hksi_lua_getstack(state, 1, &info);
			game::hks::hksi_lua_getinfo(state, "nSl", &info);
			return info.short_src;
		}

		int load_buffer(const std::string& name, const std::string& data)
		{
			const auto state = *game::hks::lua_state;
			const auto sharing_mode = state->m_global->m_bytecodeSharingMode;
			state->m_global->m_bytecodeSharingMode = game::hks::HKS_BYTECODE_SHARING_ON;
			const auto _0 = gsl::finally([&]()
			{
				state->m_global->m_bytecodeSharingMode = sharing_mode;
			});

			game::hks::HksCompilerSettings compiler_settings{};
			return game::hks::hksi_hksL_loadbuffer(state, &compiler_settings, data.data(), data.size(), name.data());
		}

		void load_script(const std::string& name, const std::string& data)
		{
			globals.loaded_scripts[name] = name;

			const auto lua = get_globals();
			const auto load_results = lua["loadstring"](data, name);

			if (load_results[0].is<function>())
			{
				const auto results = lua["pcall"](load_results);
				if (!results[0].as<bool>())
				{
					print_error(results[1].as<std::string>());
				}
			}
			else if (load_results[1].is<std::string>())
			{
				print_error(load_results[1].as<std::string>());
			}
		}

		void load_scripts(const std::string& script_dir)
		{
			if (!utils::io::directory_exists(script_dir))
			{
				return;
			}

			const auto scripts = utils::io::list_files(script_dir);

			for (const auto& script : scripts)
			{
				std::string data{};
				if (std::filesystem::is_directory(script) && utils::io::read_file(script + "/__init__.lua", &data))
				{
					print_loading_script(script);
					load_script(script + "/__init__.lua", data);
				}
			}
		}

		void setup_functions()
		{
			const auto lua = get_globals();

			lua["io"]["fileexists"] = utils::io::file_exists;
			lua["io"]["writefile"] = utils::io::write_file;
			lua["io"]["movefile"] = utils::io::move_file;
			lua["io"]["filesize"] = utils::io::file_size;
			lua["io"]["createdirectory"] = utils::io::create_directory;
			lua["io"]["directoryexists"] = utils::io::directory_exists;
			lua["io"]["directoryisempty"] = utils::io::directory_is_empty;
			lua["io"]["listfiles"] = utils::io::list_files;
			lua["io"]["removefile"] = utils::io::remove_file;
			lua["io"]["readfile"] = static_cast<std::string(*)(const std::string&)>(utils::io::read_file);

			using game = table;
			auto game_type = game();
			lua["game"] = game_type;

			/*
			game_type["addlocalizedstring"] = [](const game&, const std::string& string,
				const std::string& value)
			{
				localized_strings::override(string, value);
			};
			*/

			game_type["sharedset"] = [](const game&, const std::string& key, const std::string& value)
			{
				scripting::shared_table.access([key, value](scripting::shared_table_t& table)
				{
					table[key] = value;
				});
			};

			game_type["sharedget"] = [](const game&, const std::string& key)
			{
				std::string result;
				scripting::shared_table.access([key, &result](scripting::shared_table_t& table)
				{
					result = table[key];
				});
				return result;
			};

			game_type["sharedclear"] = [](const game&)
			{
				scripting::shared_table.access([](scripting::shared_table_t& table)
				{
					table.clear();
				});
			};

			/*
			game_type["assetlist"] = [](const game&, const std::string& type_string)
			{
				auto table_ = table();
				auto index = 1;
				auto type_index = -1;
				for (auto i = 0; i < ::game::XAssetType::ASSET_TYPE_COUNT; i++)
				{
					if (type_string == ::game::g_assetNames[i])
					{
						type_index = i;
					}
				}
				if (type_index == -1)
				{
					throw std::runtime_error("Asset type does not exist");
				}
				const auto type = static_cast<::game::XAssetType>(type_index);
				fastfiles::enum_assets(type, [type, &table_, &index](const ::game::XAssetHeader header)
				{
					const auto asset = ::game::XAsset{type, header};
					const std::string asset_name = ::game::DB_GetXAssetName(&asset);
					table_[index++] = asset_name;
				}, true);
				return table_;
			};
			*/

			game_type["getcurrentgamelanguage"] = [](const game&)
			{
				return steam::SteamApps()->GetCurrentGameLanguage();
			};

			game_type["isdefaultmaterial"] = [](const game&, const std::string& material)
			{
				return static_cast<bool>(::game::DB_IsXAssetDefault(::game::ASSET_TYPE_MATERIAL,
					material.data()));
			};

			auto server_list_table = table();
			lua["serverlist"] = server_list_table;

			server_list_table["getplayercount"] = server_list::get_player_count;
			server_list_table["getservercount"] = server_list::get_server_count;
		}

		void enable_globals()
		{
			const auto lua = get_globals();
			const std::string code =
				"local g = getmetatable(_G)\n"
				"if not g then\n"
				"g = {}\n"
				"setmetatable(_G, g)\n"
				"end\n"
				"g.__newindex = nil\n";

			lua["loadstring"](code)[0]();
		}

		void start()
		{
			globals = {};
			const auto lua = get_globals();
			enable_globals(); // EnableGlobals() isn't a thing?

			setup_functions();

			lua["print"] = [](const variadic_args& va)
			{
				std::string buffer{};
				const auto to_string = get_globals()["tostring"];

				for (auto i = 0; i < va.size(); i++)
				{
					const auto& arg = va[i];
					const auto str = to_string(arg)[0].as<std::string>();
					buffer.append(str);

					if (i < va.size() - 1)
					{
						buffer.append("\t");
					}
				}

				console::info("%s\n", buffer.data());
			};

			lua["table"]["unpack"] = lua["unpack"];
			lua["luiglobals"] = lua;

			/*
			load_script("lui_common", lui_common);
			load_script("lui_updater", lui_updater);
			load_script("lua_json", lua_json);
			*/

			for (const auto& path : filesystem::get_search_paths_rev())
			{
				load_scripts(path + "/ui_scripts/");		
			}
		}

		void try_start()
		{
			try
			{
				start();
			}
			catch (const std::exception& e)
			{
				console::error("Failed to load LUI scripts: %s\n", e.what());
			}
		}

		void* hks_start_stub(char a1, char a2)
		{
			const auto _0 = gsl::finally(&try_start);
			return hks_start_hook.invoke<void*>(a1, a2);
		}

		void hks_shutdown_stub()
		{
			converted_functions.clear();
			globals = {};
			return hks_shutdown_hook.invoke<void>();
		}

		void* hks_package_require_stub(game::hks::lua_State* state)
		{
			const auto script = get_current_script();
			const auto root = get_root_script(script);
			globals.in_require_script = root;
			return hks_package_require_hook.invoke<void*>(state);
		}

		game::XAssetHeader db_find_x_asset_header_stub(game::XAssetType type, const char* name, int allow_create_default)
		{
			game::XAssetHeader header{.luaFile = nullptr};

			if (!is_loaded_script(globals.in_require_script))
			{
				return game::DB_FindXAssetHeader(type, name, allow_create_default);
			}

			const auto folder = globals.in_require_script.substr(0, globals.in_require_script.find_last_of("/\\"));
			const std::string name_ = name;
			const std::string target_script = folder + "/" + name_ + ".lua";

			if (utils::io::file_exists(target_script))
			{
				globals.load_raw_script = true;
				globals.raw_script_name = target_script;
				header.luaFile = reinterpret_cast<game::LuaFile*>(1);
			}
			else if (name_.starts_with("ui/"))
			{
				return game::DB_FindXAssetHeader(type, name, allow_create_default);
			}

			return header;
		}

		int hks_load_stub(game::hks::lua_State* state, void* compiler_options, 
			void* reader, void* reader_data, const char* chunk_name)
		{
			if (globals.load_raw_script)
			{
				globals.load_raw_script = false;
				globals.loaded_scripts[globals.raw_script_name] = globals.in_require_script;
				return load_buffer(globals.raw_script_name, utils::io::read_file(globals.raw_script_name));
			}

			return hks_load_hook.invoke<int>(state, compiler_options, reader,
				reader_data, chunk_name);
		}

		std::string current_error;
		int main_handler(game::hks::lua_State* state)
		{
			bool error = false;

			try
			{
				const auto value = state->m_apistack.base[-1];
				if (value.t != game::hks::TCFUNCTION)
				{
					return 0;
				}

				const auto closure = value.v.cClosure;
				if (!converted_functions.contains(closure))
				{
					return 0;
				}

				const auto& function = converted_functions[closure];

				const auto args = get_return_values();
				const auto results = function(args);

				for (const auto& result : results)
				{
					push_value(result);
				}

				return static_cast<int>(results.size());
			}
			catch (const std::exception& e)
			{
				current_error = e.what();
				error = true;
			}

			if (error)
			{
				game::hks::hksi_luaL_error(state, current_error.data());
			}

			return 0;
		}
	}

	table get_globals()
	{
		const auto state = *game::hks::lua_state;
		return state->globals.v.table;
	}

	template <typename F>
	game::hks::cclosure* convert_function(F f)
	{
		const auto state = *game::hks::lua_state;
		const auto closure = game::hks::cclosure_Create(state, main_handler, 0, 0, 0);
		converted_functions[closure] = wrap_function(f);
		return closure;
	}

	bool lui_running()
	{
		return *game::hks::lua_state != nullptr;
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

			utils::hook::call(0x5FC2F7_b, db_find_x_asset_header_stub);
			utils::hook::call(0x5FC0AB_b, db_find_x_asset_header_stub);

			hks_load_hook.create(0x11E0B00_b, hks_load_stub);

			hks_package_require_hook.create(0x11C7F00_b, hks_package_require_stub);
			hks_start_hook.create(0x615090_b, hks_start_stub);
			hks_shutdown_hook.create(0x6124B0_b, hks_shutdown_stub);
		}
	};
}

REGISTER_COMPONENT(ui_scripting::component)
