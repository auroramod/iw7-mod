#pragma once
#include "game/game.hpp"
#include "variable_value.hpp"
#include "vector.hpp"

#include <utils/string.hpp>

namespace scripting
{
	class entity;
	class array;
	class function;
	class value_wrap;

	namespace
	{
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

		std::string get_typename(const game::VariableValue& value)
		{
			if (value.type == game::VAR_POINTER)
			{
				const auto type = game::scr_VarGlob->objectVariableValue[value.u.uintValue].w.type;
				return var_typename[type];
			}
			else
			{
				return var_typename[value.type];
			}
		}

		template <typename T, typename A = array>
		std::string get_c_typename()
		{
			auto& info = typeid(T);

			if (info == typeid(std::string))
			{
				return "string";
			}

			if (info == typeid(const char*))
			{
				return "string";
			}

			if (info == typeid(entity))
			{
				return "entity";
			}

			if (info == typeid(array))
			{
				return "array";
			}

			if (info == typeid(function))
			{
				return "function";
			}

			if (info == typeid(vector))
			{
				return "vector";
			}

			return info.name();
		}
	}

	class script_value
	{
	public:
		script_value() = default;
		script_value(const game::VariableValue& value);
		script_value(const value_wrap& value);

		script_value(int value);
		script_value(unsigned int value);
		script_value(bool value);

		script_value(float value);
		script_value(double value);

		script_value(const char* value);
		script_value(const std::string& value);

		script_value(const entity& value);
		script_value(const array& value);

		script_value(const function& value);

		script_value(const vector& value);

		std::string type_name() const
		{
			return get_typename(this->get_raw());
		}

		std::string to_string() const;

		template <typename T>
		bool is() const;

		template <typename T>
		T as() const
		{
			if (!this->is<T>())
			{
				const auto type = get_typename(this->get_raw());
				const auto c_type = get_c_typename<T>();
				throw std::runtime_error(std::format("has type '{}' but should be '{}'", type, c_type));
			}

			return get<T>();
		}

		template <typename T, typename I = int>
		T* as_ptr()
		{
			const auto value = this->as<I>();

			if (!value)
			{
				throw std::runtime_error("is null");
			}

			return reinterpret_cast<T*>(value);
		}

		const game::VariableValue& get_raw() const;

		variable_value value_{};
	private:
		template <typename T>
		T get() const;
	};

	class value_wrap
	{
	public:
		value_wrap(const scripting::script_value& value, int argument_index);

		std::string to_string() const
		{
			return this->value_.to_string();
		}

		std::string type_name() const
		{
			return this->value_.type_name();
		}

		template <typename T>
		T as() const
		{
			try
			{
				return this->value_.as<T>();
			}
			catch (const std::exception& e)
			{
				throw std::runtime_error(utils::string::va("parameter %d %s", this->argument_index_, e.what()));
			}
		}

		template <typename T, typename I = int>
		T* as_ptr()
		{
			try
			{
				return this->value_.as_ptr<T>();
			}
			catch (const std::exception& e)
			{
				throw std::runtime_error(utils::string::va("parameter %d %s", this->argument_index_, e.what()));
			}
		}

		template <typename T>
		T is() const
		{
			return this->value_.is<T>();
		}

		const game::VariableValue& get_raw() const
		{
			return this->value_.get_raw();
		}

		int argument_index_{};
		scripting::script_value value_;
	};
}
