#pragma once

#include "game.hpp"
#include "structs.hpp"

namespace dvars
{
	extern game::dvar_t* con_inputBoxColor;
	extern game::dvar_t* con_inputHintBoxColor;
	extern game::dvar_t* con_outputBarColor;
	extern game::dvar_t* con_outputSliderColor;
	extern game::dvar_t* con_outputWindowColor;
	extern game::dvar_t* con_inputDvarMatchColor;
	extern game::dvar_t* con_inputDvarValueColor;
	extern game::dvar_t* con_inputDvarInactiveValueColor;
	extern game::dvar_t* con_inputCmdMatchColor;

	std::string dvar_get_vector_domain(const int components, const game::dvar_limits& domain);
	std::string dvar_get_domain(const game::dvar_type type, const game::dvar_limits& domain);
	std::string dvar_get_name(const game::dvar_t* dvar);
	std::string dvar_get_description(const game::dvar_t* dvar);
	void dvar_set_name(const game::dvar_t* dvar, const std::string& name);
	void dvar_set_description(const game::dvar_t* dvar, const std::string& description);
}
