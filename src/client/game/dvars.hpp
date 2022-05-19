#pragma once

#include "game.hpp"
#include "structs.hpp"

namespace dvars
{
	std::string dvar_get_vector_domain(const int components, const game::dvar_limits& domain);
	std::string dvar_get_domain(const game::dvar_type type, const game::dvar_limits& domain);
}
