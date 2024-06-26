#include <std_include.hpp>

#include "loader/component_loader.hpp"
#include "game/game.hpp"

#include "script_error.hpp"
#include "script_extension.hpp"
#include "script_loading.hpp"

#include "component/scripting.hpp"

#include "component/console/console.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

#include "error_info_map.hpp"

using namespace utils::string;

namespace gsc
{
	namespace
	{
		utils::hook::detour scr_emit_function_hook;

		std::uint32_t current_filename = 0;

		std::string unknown_function_error;

		bool force_error_print = false;
		std::optional<std::string> gsc_error_msg;

		std::array<const char*, 27> var_typename =
		{
			"undefined",
			"object",
			"string",
			"localized string",
			"vector",
			"float",
			"int",
			"codepos",
			"precodepos",
			"function",
			"builtin function",
			"builtin method",
			"stack",
			"animation",
			"pre animation",
			"thread",
			"thread",
			"thread",
			"thread",
			"struct",
			"removed entity",
			"entity",
			"array",
			"removed thread",
			"<free>",
			"thread list",
			"endon list",
		};

		void scr_emit_function_stub(std::uint32_t filename, std::uint32_t thread_name, char* code_pos)
		{
			current_filename = filename;
			scr_emit_function_hook.invoke<void>(filename, thread_name, code_pos);
		}

		std::string get_filename_name()
		{
			const auto filename_str = game::SL_ConvertToString(static_cast<game::scr_string_t>(current_filename));
			const auto id = std::atoi(filename_str);
			if (!id)
			{
				return filename_str;
			}

			return scripting::get_token(id);
		}

		void get_unknown_function_error(const char* code_pos)
		{
			const auto function = find_function(code_pos);
			if (function.has_value())
			{
				const auto& pos = function.value();
				unknown_function_error = std::format(
					"while processing function '{}' in script '{}':\nunknown script '{}'", pos.first, pos.second, scripting::current_file
				);
			}
			else
			{
				unknown_function_error = std::format("unknown script '{}'", scripting::current_file);
			}
		}

		void get_unknown_function_error(std::uint32_t thread_name)
		{
			const auto filename = get_filename_name();
			const auto name = scripting::get_token(thread_name);

			unknown_function_error = std::format(
				"while processing script '{}':\nunknown function '{}::{}'", scripting::current_file, filename, name
			);
		}

		void compile_error_stub(const char* code_pos, [[maybe_unused]] const char* msg)
		{
			get_unknown_function_error(code_pos);
			game::Com_Error(game::ERR_SCRIPT_DROP, "script link error\n%s", unknown_function_error.data());
		}
		
		std::uint32_t find_variable_stub(std::uint32_t parent_id, std::uint32_t thread_name)
		{
			const auto res = game::FindVariable(parent_id, thread_name);
			if (!res)
			{
				get_unknown_function_error(thread_name);
				game::Com_Error(game::ERR_SCRIPT_DROP, "script link error\n%s", unknown_function_error.data());
			}
			return res;
		}

		unsigned int scr_get_object(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_POINTER)
				{
					return value->u.pointerValue;
				}

				scr_error(va("Type %s is not an object", var_typename[value->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		unsigned int scr_get_const_string(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (game::Scr_CastString(value))
				{
					assert(value->type == game::VAR_STRING);
					return value->u.stringValue;
				}

				game::Scr_ErrorInternal();
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		unsigned int scr_get_const_istring(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_ISTRING)
				{
					return value->u.stringValue;
				}

				scr_error(va("Type %s is not a localized string", var_typename[value->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		void scr_validate_localized_string_ref(int parm_index, const char* token, int token_len)
		{
			assert(token);
			assert(token_len >= 0);

			if (token_len < 2)
			{
				return;
			}

			for (auto char_iter = 0; char_iter < token_len; ++char_iter)
			{
				if (!std::isalnum(static_cast<unsigned char>(token[char_iter])) && token[char_iter] != '_')
				{
					scr_error(va("Illegal localized string reference: %s must contain only alpha-numeric characters and underscores", token));
				}
			}
		}

		void scr_get_vector(unsigned int index, float* vector_value)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_VECTOR)
				{
					std::memcpy(vector_value, value->u.vectorValue, sizeof(std::float_t[3]));
					return;
				}

				scr_error(va("Type %s is not a vector", var_typename[value->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
		}

		int scr_get_int(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_INTEGER)
				{
					return value->u.intValue;
				}

				scr_error(va("Type %s is not an int", var_typename[value->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		float scr_get_float(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				auto* value = game::scr_VmPub->top - index;
				if (value->type == game::VAR_FLOAT)
				{
					return value->u.floatValue;
				}

				if (value->type == game::VAR_INTEGER)
				{
					return static_cast<float>(value->u.intValue);
				}

				scr_error(va("Type %s is not a float", var_typename[value->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0.0f;
		}

		int scr_get_pointer_type(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				if ((game::scr_VmPub->top - index)->type == game::VAR_POINTER)
				{
					return static_cast<int>(game::GetObjectType((game::scr_VmPub->top - index)->u.uintValue));
				}

				scr_error(va("Type %s is not an object", var_typename[(game::scr_VmPub->top - index)->type]));
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		int scr_get_type(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				return (game::scr_VmPub->top - index)->type;
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return 0;
		}

		const char* scr_get_type_name(unsigned int index)
		{
			if (index < game::scr_VmPub->outparamcount)
			{
				return var_typename[(game::scr_VmPub->top - index)->type];
			}

			scr_error(va("Parameter %u does not exist", index + 1));
			return nullptr;
		}
	}

	namespace
	{
		utils::hook::detour scr_error_internal_hook;

		std::optional<std::string> get_opcode_name(const std::uint8_t opcode)
		{
			try
			{
				const auto index = gsc_ctx->opcode_enum(opcode);
				return { gsc_ctx->opcode_name(index) };
			}
			catch (...)
			{
				return {};
			}
		}

		std::uint32_t get_opcode_size(const std::uint8_t opcode)
		{
			try
			{
				const auto index = gsc_ctx->opcode_enum(opcode);
				return gsc_ctx->opcode_size(index);
			}
			catch (...)
			{
				return 0;
			}
		}

		void builtin_call_error(const std::string& error)
		{
			const auto function_id = get_function_id();

			if (function_id > func_table_count)
			{
				console::warn("in call to builtin method \"%s\"%s", gsc_ctx->meth_name(function_id).data(), error.data());
			}
			else
			{
				console::warn("in call to builtin function \"%s\"%s", gsc_ctx->func_name(function_id).data(), error.data());
			}
		}

		void print_callstack(uint8_t opcode_id)
		{
			bool first = true;

			for (auto frame = game::scr_VmPub->function_frame; frame != game::scr_VmPub->function_frame_start; --frame)
			{
				const auto function_stack = game::scr_function_stack;
				const auto pos = frame == game::scr_VmPub->function_frame ? function_stack->pos : frame->fs.pos;
				const auto function = find_function(frame->fs.pos);

				if (function.has_value())
				{
					auto& file_name = function.value().second;

					if (gsc::source_pos_map.contains(file_name))
					{
						const auto script = gsc::loaded_scripts[file_name];
						assert(script);

						const auto& pos_map = gsc::source_pos_map[file_name];
						std::uint32_t opcode_size = 0;
						if (first)
						{
							opcode_size = get_opcode_size(opcode_id);
						}
						if (!opcode_size)
						{
							opcode_size = 3; // function call
						}
						auto position = static_cast<std::uint32_t>(pos - script->bytecode);
						position = position + opcode_size;

						if (pos_map.contains(position))
						{
							const auto& info = pos_map.at(position);

							console::warn("\tat function \"%s\" in file \"%s.gsc\" (line %d, col %d)\n",
								function.value().first.data(), file_name.data(), info.line, info.column);
						}
						else
						{
							goto NO_DEVMAP;
						}
					}
					else
					{
					NO_DEVMAP:
						console::warn("\tat function \"%s\" in file \"%s.gsc\"\n", function.value().first.data(), file_name.data());
					}
				}
				else
				{
					console::warn("\tat unknown location %p\n", pos);
				}

				first = false;
			}
		}

		void vm_error_internal()
		{
			const bool dev_script = developer_script ? developer_script->current.enabled : false;

			if (!dev_script && !force_error_print)
			{
				return;
			}

			console::warn("*********** script runtime error *************\n");

			const auto opcode_id = *reinterpret_cast<std::uint8_t*>(0x6B22940_b);
			const std::string error_str = gsc_error_msg.has_value()
				? utils::string::va(": %s", gsc_error_msg.value().data())
				: "";

			if ((opcode_id >= gsc_ctx->opcode_id(xsk::gsc::opcode::OP_CallBuiltin0) && opcode_id <= gsc_ctx->opcode_id(xsk::gsc::opcode::OP_CallBuiltin))
				|| (opcode_id >= gsc_ctx->opcode_id(xsk::gsc::opcode::OP_CallBuiltinMethod0) && opcode_id <= gsc_ctx->opcode_id(xsk::gsc::opcode::OP_CallBuiltinMethod)))
			{
				builtin_call_error(error_str);
			}
			else
			{
				const auto opcode = get_opcode_name(opcode_id);
				if (opcode.has_value())
				{
					console::warn("while processing instruction %s%s\n", opcode.value().data(), error_str.data());
				}
				else
				{
					console::warn("while processing instruction 0x%X%s\n", opcode_id, error_str.data());
				}
			}

			force_error_print = false;
			gsc_error_msg = {};

			print_callstack(opcode_id);
			console::warn("**********************************************\n");
		}

		void vm_error_stub(__int64 mark_pos)
		{
			try
			{
				vm_error_internal();
			}
			catch (xsk::gsc::error& err)
			{
				console::error("vm_error: %s\n", err.what());
			}

			utils::hook::invoke<void>(0x510C80_b, mark_pos);
		}

		void scr_error_internal_stub()
		{
			const auto ret_addr = reinterpret_cast<std::uint64_t>(_ReturnAddress());
			if (!gsc_error_msg.has_value())
			{
				const auto it = scr_error_info_map.find(ret_addr - 5);

				if (it != scr_error_info_map.end() && !it->second.empty())
				{
					gsc_error_msg = it->second;
				}
			}

			scr_error_internal_hook.invoke<void>();
		}
	}

	void scr_error(const char* error, const bool force_print)
	{
		force_error_print = force_print;
		gsc_error_msg = error;

		game::Scr_ErrorInternal();
	}

	std::optional<std::pair<std::string, std::string>> find_function(const char* pos)
	{
		for (const auto& file : scripting::script_function_table_sort)
		{
			for (auto i = file.second.begin(); i != file.second.end() && std::next(i) != file.second.end(); ++i)
			{
				const auto next = std::next(i);
				if (pos >= i->second && pos < next->second)
				{
					return {std::make_pair(i->first, file.first)};
				}
			}
		}

		return {};
	}

	class error final : public component_interface
	{
	public:
		void post_unpack() override
		{
			scr_emit_function_hook.create(0xBFCF90_b, &scr_emit_function_stub);

			scr_error_internal_hook.create(game::Scr_ErrorInternal, scr_error_internal_stub);

			utils::hook::call(0xC0F8C1_b, vm_error_stub); // LargeLocalResetToMark

			utils::hook::call(0xBFCF3A_b, compile_error_stub); // CompileError (LinkFile)
			utils::hook::call(0xBFCF86_b, compile_error_stub); // ^
			utils::hook::call(0xBFD06F_b, find_variable_stub); // Scr_EmitFunction

			// Restore basic error messages for commonly used scr functions
			utils::hook::jump(0xC0BA10_b, scr_get_object);
			utils::hook::jump(0xC0B4C0_b, scr_get_const_string);
			utils::hook::jump(0xC0B270_b, scr_get_const_istring);
			utils::hook::jump(0xB52210_b, scr_validate_localized_string_ref);
			utils::hook::jump(0xC0BF40_b, scr_get_vector);
			utils::hook::jump(0xC0B950_b, scr_get_int);
			utils::hook::jump(0xC0B7E0_b, scr_get_float);

			utils::hook::jump(0xC0BC00_b, scr_get_pointer_type);
			utils::hook::jump(0xC0BDE0_b, scr_get_type);
			utils::hook::jump(0xC0BE50_b, scr_get_type_name);
		}
	};
}

REGISTER_COMPONENT(gsc::error)
