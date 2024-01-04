#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/scheduler.hpp"
#include "component/scripting.hpp"

#include "component/console/console.hpp"
#include "game/game.hpp"

#include "game/scripting/event.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/functions.hpp"
#include "game/scripting/lua/engine.hpp"

#include <utils/hook.hpp>
#include "gsc/script_loading.hpp"

namespace scripting
{
	std::unordered_map<int, std::unordered_map<std::string, int>> fields_table;

	std::unordered_map<std::string, std::unordered_map<std::string, const char*>> script_function_table;
	std::unordered_map<std::string, std::vector<std::pair<std::string, const char*>>> script_function_table_sort;
	std::unordered_map<const char*, std::pair<std::string, std::string>> script_function_table_rev;

	utils::concurrency::container<shared_table_t> shared_table;

	std::string current_file;

	namespace
	{
		utils::hook::detour vm_notify_hook;
		utils::hook::detour vm_execute_hook;
		utils::hook::detour g_load_structs_hook;
		utils::hook::detour scr_load_level_hook;
		utils::hook::detour g_shutdown_game_hook;

		utils::hook::detour scr_add_class_field_hook;

		utils::hook::detour scr_set_thread_position_hook;
		utils::hook::detour process_script_hook;

		utils::hook::detour sl_get_canonical_string_hook;

		utils::hook::detour db_find_xasset_header_hook;

		std::string current_script_file;
		unsigned int current_file_id{};

		game::dvar_t* g_dump_scripts;

		std::vector<std::function<void(bool, bool)>> shutdown_callbacks;

		std::unordered_map<unsigned int, std::string> canonical_string_table;

		void vm_notify_stub(const unsigned int notify_list_owner_id, const game::scr_string_t string_value,
			game::VariableValue* top)
		{
			const auto* string = game::SL_ConvertToString(string_value);
			if (string)
			{
				event e;
				e.name = string;
				e.entity = notify_list_owner_id;

				for (auto* value = top; value->type != game::VAR_PRECODEPOS; --value)
				{
					e.arguments.emplace_back(*value);
				}

				lua::engine::notify(e);
			}

			vm_notify_hook.invoke<void>(notify_list_owner_id, string_value, top);
		}

		void g_shutdown_game_stub(const int free_scripts)
		{
			if (free_scripts)
			{
				script_function_table_sort.clear();
				script_function_table.clear();
				script_function_table_rev.clear();
				canonical_string_table.clear();
			}

			for (const auto& callback : shutdown_callbacks)
			{
				callback(free_scripts, false);
			}

			scripting::notify(*game::levelEntityId, "shutdownGame_called", { 1 });
			lua::engine::stop();

			g_shutdown_game_hook.invoke<void>(free_scripts);

			for (const auto& callback : shutdown_callbacks)
			{
				callback(free_scripts, true);
			}
		}

		void scr_add_class_field_stub(unsigned int classnum, game::scr_string_t name, unsigned int canonical_string, unsigned int offset)
		{
			const auto name_str = game::SL_ConvertToString(name);

			if (fields_table[classnum].find(name_str) == fields_table[classnum].end())
			{
				fields_table[classnum][name_str] = offset;
			}

			scr_add_class_field_hook.invoke<void>(classnum, name, canonical_string, offset);
		}

		void process_script_stub(const char* filename)
		{
			current_script_file = filename;

			const auto file_id = atoi(filename);
			if (file_id)
			{
				current_file_id = static_cast<std::uint16_t>(file_id);
			}
			else
			{
				current_file_id = 0;
				current_file = filename;
			}

			process_script_hook.invoke<void>(filename);
		}

		void add_function_sort(unsigned int id, const char* pos)
		{
			std::string filename = current_file;
			if (current_file_id)
			{
				filename = scripting::get_token(current_file_id);
			}

			if (!script_function_table_sort.contains(filename))
			{
				const auto script = gsc::find_script(game::ASSET_TYPE_SCRIPTFILE, current_script_file.data(), false);
				if (script)
				{
					const auto end = &script->bytecode[script->bytecodeLen];
					script_function_table_sort[filename].emplace_back("__end__", end);
				}
			}

			const auto name = scripting::get_token(id);
			auto& itr = script_function_table_sort[filename];
			itr.insert(itr.end() - 1, { name, pos });
		}

		void add_function(const std::string& file, unsigned int id, const char* pos)
		{
			const auto name = get_token(id);
			script_function_table[file][name] = pos;
			script_function_table_rev[pos] = { file, name };
		}

		void scr_set_thread_position_stub(unsigned int thread_name, const char* code_pos)
		{
			add_function_sort(thread_name, code_pos);

			if (current_file_id)
			{
				const auto name = get_token(current_file_id);
				add_function(name, thread_name, code_pos);
			}
			else
			{
				add_function(current_file, thread_name, code_pos);
			}

			scr_set_thread_position_hook.invoke<void>(thread_name, code_pos);
		}

		unsigned int sl_get_canonical_string_stub(const char* str)
		{
			const auto result = sl_get_canonical_string_hook.invoke<unsigned int>(str);
			canonical_string_table[result] = str;
			return result;
		}
	}

	std::string get_token(unsigned int id)
	{
		if (canonical_string_table.find(id) != canonical_string_table.end())
		{
			return canonical_string_table[id];
		}

		return scripting::find_token(id);
	}

	void on_shutdown(const std::function<void(bool, bool)>& callback)
	{
		shutdown_callbacks.push_back(callback);
	}

	std::optional<std::string> get_canonical_string(const unsigned int id)
	{
		if (canonical_string_table.find(id) == canonical_string_table.end())
		{
			return {};
		}

		return { canonical_string_table[id] };
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			vm_notify_hook.create(0xC10460_b, vm_notify_stub);

			scr_add_class_field_hook.create(0xC061F0_b, scr_add_class_field_stub);

			scr_set_thread_position_hook.create(0xBFD190_b, scr_set_thread_position_stub);
			process_script_hook.create(0xC09D20_b, process_script_stub);
			sl_get_canonical_string_hook.create(game::SL_GetCanonicalString, sl_get_canonical_string_stub);

			g_shutdown_game_hook.create(0xB21CC0_b, g_shutdown_game_stub);

			scheduler::loop([]
			{
				lua::engine::run_frame();
			}, scheduler::pipeline::server);
		}
	};
}

REGISTER_COMPONENT(scripting::component)
