#pragma once
#include "../../game/structs.hpp"
namespace gsc
{
	void load_main_handles();
	void load_init_handles();
	game::assets::ScriptFile* find_script(game::assets::XAssetType type, const char* name, int allow_create_default);
}
