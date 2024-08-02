#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "console/console.hpp"
#include "scheduler.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace dvar_cheats
{
	void apply_sv_cheats(const game::dvar_t* dvar, const game::DvarSetSource source, game::DvarValue* value)
	{
		static const auto sv_cheats_checksum = game::Dvar_GenerateChecksum("sv_cheats");

		if (dvar && dvar->checksum == sv_cheats_checksum)
		{
			// if dedi, do not allow internal to change value so servers can allow cheats if they want to
			if (game::environment::is_dedi() && source == game::DvarSetSource::DVAR_SOURCE_INTERNAL)
			{
				value->enabled = dvar->current.enabled;
			}

			// if sv_cheats was enabled and it changes to disabled, we need to reset all cheat dvars
			/*else if (dvar->current.enabled && !value->enabled)
			{
				for (auto i = 0; i < *game::dvarCount; ++i)
				{
					const auto var = game::dvarPool[i];
					if (var && (var->flags & game::DvarFlags::DVAR_FLAG_CHEAT))
					{
						game::Dvar_Reset(var, game::DvarSetSource::DVAR_SOURCE_INTERNAL);
					}
				}
			}*/ // resets the r_hudOutline dvars, which are cheat dvars for some reason.
		}
	}

	bool dvar_flag_checks(const game::dvar_t* dvar, const game::DvarSetSource source)
	{
		if ((dvar->flags & game::DvarFlags::DVAR_FLAG_WRITE))
		{
#ifdef DEBUG
			console::error("%s is write protected\n", dvars::dvar_get_name(dvar).data());
#endif
			return false;
		}

		if ((dvar->flags & game::DvarFlags::DVAR_FLAG_READ))
		{
#ifdef DEBUG
			console::error("%s is read only\n", dvars::dvar_get_name(dvar).data());
#endif
			return false;
		}

		// only check cheat/replicated values when the source is external
		if (source == game::DvarSetSource::DVAR_SOURCE_EXTERNAL)
		{
			const auto cl_ingame = game::CL_IsGameClientActive(0);
			const auto sv_running = game::SV_Loaded();

			if ((dvar->flags & game::DvarFlags::DVAR_FLAG_REPLICATED) && cl_ingame && !sv_running)
			{
				console::error("%s can only be changed by the server\n", dvars::dvar_get_name(dvar).data());
				return false;
			}

			if ((dvar->flags & game::DvarFlags::DVAR_FLAG_CHEAT) && (!game::shared::cheats_ok() && *game::isCheatOverride))
			{
//#ifdef DEBUG
				console::error("%s is cheat protected\n", dvars::dvar_get_name(dvar).data());
//#endif
				return false;
			}
		}

		// pass all the flag checks, allow dvar to be changed
		return true;
	}

	void* get_dvar_flag_checks_stub()
	{
		return utils::hook::assemble([](utils::hook::assembler& a)
		{
			const auto can_set_value = a.newLabel();
			const auto zero_source = a.newLabel();

			a.pushad64();
			a.mov(r8, rdi);
			a.mov(edx, esi);
			a.mov(rcx, rbx);
			a.call_aligned(apply_sv_cheats); // check if we are setting sv_cheats
			a.popad64();
			a.cmp(esi, 0);
			a.jz(zero_source); // if the SetSource is 0 (INTERNAL) ignore flag checks

			a.pushad64();
			a.mov(edx, esi); // source
			a.mov(rcx, rbx); // dvar
			a.call_aligned(dvar_flag_checks); // protect read/write/cheat/replicated dvars
			a.cmp(al, 1);
			a.jz(can_set_value);

			// if we get here, we are non-zero source and CANNOT set values
			a.popad64(); // if I do this before the jz it won't work. for some reason the popad64 is affecting the ZR flag
			a.jmp(0x140CEDBDF);

			// if we get here, we are non-zero source and CAN set values
			a.bind(can_set_value);
			a.popad64(); // if I do this before the jz it won't work. for some reason the popad64 is affecting the ZR flag
			a.cmp(esi, 1);
			a.jmp(0x140CED8EE);

			// if we get here, we are zero source and ignore flags
			a.bind(zero_source);
			a.jmp(0x140CED97A);
		});
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::nop(0x140CED8D4, 8); // let our stub handle zero-source sets
			utils::hook::jump(0x140CED8DF, get_dvar_flag_checks_stub(), true); // check extra dvar flags when setting values

			scheduler::once([]
			{
				game::Dvar_RegisterBool("sv_cheats", false, game::DvarFlags::DVAR_FLAG_REPLICATED, "Allow cheat commands and dvars on this server");
			}, scheduler::pipeline::main);
		}
	};
}

REGISTER_COMPONENT(dvar_cheats::component)