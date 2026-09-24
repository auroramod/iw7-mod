#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "fx.hpp"

#include <utils/hook.hpp>

#define PATCH_FX

// iw7 still loads legacy effects (FxEffectDef) but stripped the code that updates and draws them
namespace fx
{
	namespace
	{
		utils::hook::detour fx_load_effect_hook;
		utils::hook::detour fx_init_spawn_cmd_hook;
		utils::hook::detour fxw_update_pass4_internal_hook;
		utils::hook::detour fxw_update_pass5_internal_hook;

		void FX_LoadEffect_Stub(const char* effectName, FXRegisteredDef* outDef)
		{
			fx_load_effect_hook.invoke<void>(effectName, outDef);

			std::string name = effectName;
			if (const auto extension = name.find_last_of('.'); extension != std::string::npos)
			{
				name.resize(extension);
			}

			if (!game::DB_IsXAssetDefault(game::ASSET_TYPE_VFX, name.data()))
			{
				return;
			}

			const auto* def = game::DB_FindXAssetHeader(game::ASSET_TYPE_FX, name.data(), 0).fx;
			if (def && !game::DB_IsXAssetDefault(game::ASSET_TYPE_FX, name.data()))
			{
				outDef->type = game::FX_COMBINED_FX;
				outDef->fx = const_cast<game::FxEffectDef*>(def);
			}
		}

		void* FX_InitSpawnCmd_Stub(FxSystem* system, __int64 localClientBits, unsigned int elem, unsigned int elemParent, unsigned int elemClass,
			int fxEntNum, int timeValue, int removeRef, FXRegisteredDef* def, int msecBegin, float* origin, const float(*axis)[3],
			int dobjHandle, int boneIndex, void* unk, unsigned int owner, unsigned int markEntnum, bool markGivenModelsOnly, bool markViewmodel,
			int markViewmodelClientIndex, unsigned short initSpawnFlags)
		{
			if (def->type != game::FX_COMBINED_FX)
			{
				return fx_init_spawn_cmd_hook.invoke<void*>(system, localClientBits, elem, elemParent, elemClass, fxEntNum, timeValue, removeRef,
					def, msecBegin, origin, axis, dobjHandle, boneIndex, unk, owner, markEntnum, markGivenModelsOnly, markViewmodel,
					markViewmodelClientIndex, initSpawnFlags);
			}

			if (def->fx)
			{
				FX_InitLegacySpawnCmd(system, elem, elemParent, elemClass, fxEntNum, timeValue, removeRef, static_cast<const FxEffectDef*>(def->fx),
					msecBegin, origin, axis, dobjHandle, boneIndex, owner, markEntnum, markGivenModelsOnly, markViewmodel,
					markViewmodelClientIndex, initSpawnFlags);
			}

			return nullptr;
		}

		void FXW_UpdatePass4Internal_Stub(FxPass4Cmd* passCmd)
		{
			auto* system = passCmd->system;
			if ((system->systemFlags & FX_SYSTEM_FLAG_PAUSED) == 0)
			{
				FX_UpdatePass0InitUpdateLists(system);
				FX_UpdatePass0(system, system->command->updatePass0, (system->systemFlags & FX_SYSTEM_FLAG_DEFERRED_UPDATE) != 0, 1);
				FX_UpdateBoltedEffects(system);
			}

			FX_UpdatePass4(system, passCmd);

			fxw_update_pass4_internal_hook.invoke<void>(passCmd);
		}

		void FXW_SystemFlush_Stub(FxSystem* system, const int localClientNum)
		{
			auto* command = system->command;
			command->numUpdatePass0 = 0;
			command->numUpdatePass4 = 0;
			command->updatePass0[0] = ~0u;
			command->updatePass4[0] = ~0u;

			FxPass4Cmd passCmd{};
			engine::FXI_InitPass4Cmd(localClientNum, &passCmd);

			command = engine::FX_GetSystem(localClientNum)->command;
			command->requestIndex ^= 1;
			command->numStopBoltedEffectsRequest = 0;
			command->numThroughWithEffectIndex = 0;
			command->numKillEffectDefIndex = 0;
			command->numRetriggerEffectRequest = 0;
			command->numUpdatePass0 = 0;
			command->numUpdatePass4 = 0;

			engine::FXW_CreateNewEffects(system, 0, nullptr, 0, engine::cg_fxEntitiesArray.get());
			FX_ProcessPass4Commands(system, &passCmd);
			engine::FXW_UpdatePass4ProcessCommands(system);
			engine::FXW_SortEffects(system);
		}

		void FXW_UpdatePass5Internal_Stub(FxPass5Cmd* passCmd)
		{
			auto* system = passCmd->system;
			if ((system->systemFlags & (FX_SYSTEM_FLAG_PAUSED | FX_SYSTEM_FLAG_SKIP_DRAW)) == 0 && engine::R_ShouldGenerateFxVerts())
			{
				FX_DrawNonSpriteElems(system, passCmd);
			}

			fxw_update_pass5_internal_hook.invoke<void>(passCmd);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// skip "fx/" and "vfx/" name prefix checks
			utils::hook::set<uint8_t>(0x140B34889, 0xEB); // Scr_LoadFx
			utils::hook::nop(0x140D0FBFD, 2); // ParticleSystem_Register

#ifdef PATCH_FX
			fx_load_effect_hook.create(0x1403D8120, FX_LoadEffect_Stub);
			fx_init_spawn_cmd_hook.create(0x140ABB770, FX_InitSpawnCmd_Stub);

			fxw_update_pass4_internal_hook.create(0x140AC17B0, FXW_UpdatePass4Internal_Stub);
			utils::hook::jump(0x140AC1EE0, FXW_SystemFlush_Stub, true);
			fxw_update_pass5_internal_hook.create(0x140AC1870, FXW_UpdatePass5Internal_Stub);

			utils::hook::jump(0x140A8C630, FX_DrawSpriteElems, true); // FXIR_DrawSpriteElems
			utils::hook::jump(0x140A8E8B0, FX_GenSpriteQuad, true); // FXV_GenSpriteVerts
			utils::hook::jump(0x140A947D0, FX_GenParticleSimAnimationQuads, true); // FX_GenParticleSimAnimationVerts

			// FXW_ElemEmitEffect was replaced with an empty function
			utils::hook::call(0x140ABDE7E, FX_ElemEmitEffect); // FXW_SpawnElem
#endif
		}
	};
}

REGISTER_COMPONENT(fx::component)
