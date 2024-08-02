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

	extern game::dvar_t* lui_debug;

	extern game::dvar_t* cg_draw2D;

	extern game::dvar_t* cg_gun_x;
	extern game::dvar_t* cg_gun_y;
	extern game::dvar_t* cg_gun_z;

	extern game::dvar_t* cg_thirdPerson;
	extern game::dvar_t* cg_thirdPersonRange;
	extern game::dvar_t* cg_thirdPersonAngle;

	extern game::dvar_t* cg_unlockall_items;
	extern game::dvar_t* cg_unlockall_loot;

	extern game::dvar_t* r_fullbright;

	extern game::dvar_t* bg_bounces;
	extern game::dvar_t* bg_playerEjection;

	extern game::dvar_t* logfile;
	extern game::dvar_t* g_log;

	std::string dvar_get_vector_domain(const int components, const game::DvarLimits& domain);
	std::string dvar_get_domain(const game::DvarType type, const game::DvarLimits& domain);
	std::string dvar_get_name(const game::dvar_t* dvar);
	std::string dvar_get_description(const game::dvar_t* dvar);
	void dvar_set_name(const game::dvar_t* dvar, const std::string& name);
	void dvar_set_description(const game::dvar_t* dvar, const std::string& description);
}
