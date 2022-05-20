#include <std_include.hpp>
#include "game.hpp"

#include <utils/flags.hpp>

namespace game
{
	uint64_t base_address;

	void load_base_address()
	{
		const auto module = GetModuleHandle(NULL);
		base_address = uint64_t(module);
	}

	int Cmd_Argc()
	{
		return cmd_args->argc[cmd_args->nesting];
	}

	const char* Cmd_Argv(const int index)
	{
		return cmd_args->argv[cmd_args->nesting][index];
	}

	int SV_Cmd_Argc()
	{
		return sv_cmd_args->argc[sv_cmd_args->nesting];
	}

	const char* SV_Cmd_Argv(const int index)
	{
		return sv_cmd_args->argv[sv_cmd_args->nesting][index];
	}

	bool VirtualLobby_Loaded()
	{
		return 0; //return *mp::virtualLobby_loaded == 1;
	}

	bool Sys_IsDatabaseReady2()
	{
		return game::databaseCompletedEvent2;
	}

	namespace environment
	{
		bool is_dedi()
		{
			return utils::flags::has_flag("dedicated");
		}
	}
}

uintptr_t operator"" _b(const uintptr_t ptr)
{
	return game::base_address + ptr;
}