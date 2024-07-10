#pragma once
#include <xsk/gsc/engine/iw7.hpp>

namespace gsc
{
	struct col_line_t
	{
		std::uint16_t line;
		std::uint16_t column;
	};

	struct loaded_script_t
	{
		game::ScriptFile* ptr;
		std::map<std::uint32_t, col_line_t> devmap;
	};

	extern std::unique_ptr<xsk::gsc::iw7::context> gsc_ctx;

	game::ScriptFile* find_script(game::XAssetType type, const char* name, int allow_create_default);
	loaded_script_t* get_loaded_script(const std::string& name);
}
