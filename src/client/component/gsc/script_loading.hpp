#pragma once
#include <xsk/gsc/engine/iw7.hpp>

namespace gsc
{
	extern std::unique_ptr<xsk::gsc::iw7::context> gsc_ctx;
    extern std::unordered_map<std::string, game::ScriptFile*> loaded_scripts;

    struct source_pos_info
    {
        xsk::u16 line;
        xsk::u16 column;
    };
    using code_pos_map = std::map<uint32_t, source_pos_info>;
    extern std::unordered_map<std::string, code_pos_map> source_pos_map;

	game::ScriptFile* find_script(game::XAssetType type, const char* name, int allow_create_default);
}
