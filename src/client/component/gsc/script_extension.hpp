#pragma once

#include "game/scripting/array.hpp"
#include "game/scripting/execution.hpp"
#include "game/scripting/function.hpp"

namespace gsc
{
	class function_args
	{
	public:
		function_args(std::vector<scripting::script_value>);

		unsigned int size() const;
		std::vector<scripting::script_value> get_raw() const;
		scripting::value_wrap get(const int index) const;

		scripting::value_wrap operator[](const int index) const
		{
			return this->get(index);
		}
	private:
		std::vector<scripting::script_value> values_;
	};

	using builtin_function = void(*)();
	using builtin_method = void(*)(game::scr_entref_t);

	using script_function = std::function<scripting::script_value(const function_args&)>;
	using script_method = std::function<scripting::script_value(const game::scr_entref_t, const function_args&)>;

	constexpr size_t func_table_count = 0x1000;
	constexpr size_t meth_table_count = 0x1000;

	extern builtin_function func_table[func_table_count];
	extern builtin_method meth_table[meth_table_count];

	extern const game::dvar_t* developer_script;

	std::uint16_t get_function_id();

	namespace function
	{
		void add(const std::string& name, script_function function);
	}

	namespace method
	{
		void add(const std::string& name, script_method function);
	}
}