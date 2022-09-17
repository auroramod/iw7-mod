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

	extern game::dvar_t* branding;

	extern game::dvar_t* r_fullbright;

	std::string dvar_get_vector_domain(const int components, const game::DvarLimits& domain);
	std::string dvar_get_domain(const game::DvarType type, const game::DvarLimits& domain);
	std::string dvar_get_name(const game::dvar_t* dvar);
	std::string dvar_get_description(const game::dvar_t* dvar);
	void dvar_set_name(const game::dvar_t* dvar, const std::string& name);
	void dvar_set_description(const game::dvar_t* dvar, const std::string& description);
}
