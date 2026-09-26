#pragma once

#include "structs.hpp"

namespace fx::engine
{
	WEAK game::symbol<FxSystem*(int localClientNum)> FX_GetSystem{ 0x140AC0E00 };

	WEAK game::symbol<void(FxAccessLock* lock)> FX_WaitEnterReadSystemLock{ 0x140AC14D0 };
	WEAK game::symbol<void(FxAccessLock* lock)> FX_ExitReadSystemLock{ 0x140AC1410 };

	WEAK game::symbol<unsigned int(FxSystem* system, FxEffect* effect)> FXW_GetEffectStatus{ 0x140AC13F0 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect, int status)> FXW_PutEffectStatus{ 0x140AC1400 };
	WEAK game::symbol<unsigned int(FxSystem* system, FxEffect* effect)> FXW_DelRefToEffect{ 0x140ABD0B0 };

	WEAK game::symbol<void(FxSystem* system, int isDeferred, FxEffect* effect, unsigned int effectHandle, int elemDefIndex,
		const FxSpatialFrame* effectFrameWhenPlayed, int msecWhenPlayed, int sequence)> FXW_SpawnElem{ 0x140ABD8B0 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect, unsigned int effectHandle, FxTrail* trail,
		const FxSpatialFrame* frameBegin, const FxSpatialFrame* frameEnd, int msecWhenPlayed, int msecUpdateBegin, int msecUpdateEnd,
		float distanceTravelledBegin, float distanceTravelledEnd, float arcDistanceTravelled)> FXW_SpawnTrailLoopingElems{ 0x140AC33A0 };
	WEAK game::symbol<void(FxSystem* system, int isDeferred, FxEffect* effect, unsigned int effectHandle, int restartEffect)> FXIW_BeginLooping{ 0x140ABFDE0 };
	WEAK game::symbol<void(FxSystem* system, int isDeferred, FxEffect* effect, unsigned int effectHandle, int restartEffect)> FXIW_TriggerOneShot{ 0x140ABFFC0 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect)> FX_EffectRestartLooping{ 0x140AC02A0 };

	WEAK game::symbol<void(FxSystem* system, FxEffect* effect)> FXW_StopEffect{ 0x140AC01B0 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect, unsigned int effectHandle, int freeTrailsAndFountains)> FXW_KillEffect{ 0x140ABD130 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect)> FXW_PreventRewind{ 0x140AC0190 };
	WEAK game::symbol<void(FxSystem* system, unsigned int elemHandle, FxEffect* effect, unsigned int effectHandle, unsigned int elemClass)> FXW_FreeElem{ 0x140ABEB10 };
	WEAK game::symbol<void(FxSystem* system, unsigned int elemHandle)> FXW_FreeChildEffectElem{ 0x140ABEA80 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect)> FXW_RunGarbageCollection_FreeSparkFountains{ 0x140ABF050 };

	WEAK game::symbol<unsigned int(FxSystem* system, int isDeferred, unsigned int* effectHandles, int numUpdate, void* fxEntities)> FXW_CreateNewEffects{ 0x140ABCB20 };
	WEAK game::symbol<void(FxSystem* system, FxEffect* effect)> FXW_SortNewElemsInEffect{ 0x140AB9F20 };
	WEAK game::symbol<void(FxSystem* system)> FXW_SortEffects{ 0x140AB9AD0 };
	WEAK game::symbol<FxSystem*(int localClientNum, FxPass4Cmd* passCmd)> FXI_InitPass4Cmd{ 0x140AC1CC0 };
	WEAK game::symbol<void(FxSystem* system)> FXW_UpdatePass4ProcessCommands{ 0x140AC1850 };

	WEAK game::symbol<void(int localClientNum, FxEffect* effect, const FxElemDef* elemDef, const FxSpatialFrame* frame, int randomSeed, int msecWhenPlayed)> FXIW_SpawnSound{ 0x140ABD620 };
	WEAK game::symbol<FXRegisteredDef*(const FXRegisteredDef* impactEffect, int surfaceType)> FX_GetSurfaceTypeImpactEffect{ 0x140AC3670 };
	WEAK game::symbol<void*(int localClientNum, const FXRegisteredDef* def, int msecBegin, const float* origin, const float(*axis)[3])> FX_PlayOrientedEffect{ 0x140ABC100 };

	WEAK game::symbol<void(const FxElemDef* elemDef, const FxEffect* effect, const FxSpatialFrame* frame, int randomSeed, orientation_t* orient, const FxCamera* camera)> FX_GetOrientation{ 0x140AC3910 };
	WEAK game::symbol<void(const FxElemDef* elemDef, const FxTrail* trail, const FxSpatialFrame* frame, int randomSeed, float* outOrigin, float* outRight, float* outUp)> FX_GetOriginForTrailElem{ 0x140AC3BD0 };
	WEAK game::symbol<void(const FxElemDef* elemDef, int randomSeed, float msecLifeSpan, float msecElapsed, const orientation_t* orient, const float* baseVel, float* velocity)> FX_GetVelocityAtTime{ 0x140AC3D50 };
	WEAK game::symbol<void(const FxSpatialFrame* frame, FxSpatialFrame* out)> FX_SpatialFrameInvert{ 0x140AC4560 };
	WEAK game::symbol<void(const FxSpatialFrame* frame0, const FxSpatialFrame* frame1, FxSpatialFrame* out)> FX_SpatialFrameMultiply{ 0x140AC45D0 };
	WEAK game::symbol<void(float* origin, float* lighting)> FX_CalculateLighting{ 0x140AC0290 };

	WEAK game::symbol<int(unsigned int localClientNum, unsigned int dobjHandle)> FX_GetBoltTemporalBits{ 0x140AC2D60 };
	WEAK game::symbol<bool(unsigned int localClientNum, unsigned int dobjHandle, int boneIndex, orientation_t* orient)> FX_GetBoneOrientation{ 0x140AC0370 };
	WEAK game::symbol<void*(unsigned int localClientNum, int dobjHandle)> CG_GetPose{ 0x14014F080 };

	WEAK game::symbol<bool(float* pos, float* outSample, unsigned int numInstances, const unsigned int* samplers, VectorFieldType filterType)> CG_VectorField_SamplePosAgainstInstances{ 0x1405D9780 };

	WEAK game::symbol<bool(unsigned int worldId, void* results, const float* start, const float* end, const Bounds* bounds, int passEntityNum,
		int unk1, int contentMask, int unk2, void* unk3, int unk4)> PhysicsQuery_LegacyTrace{ 0x14055C9E0 };

	WEAK game::symbol<void(const float* angles, float(*axis)[3])> AnglesToAxis{ 0x140CE6240 };

	WEAK game::symbol<bool()> R_ShouldGenerateFxVerts{ 0x140E4D9F0 };
	WEAK game::symbol<void(FxDrawState* draw, const FxCamera* camera)> FX_DrawElement_Setup_Camera{ 0x140A8E310 };
	WEAK game::symbol<void(FxDrawState* draw, int elemMsecBegin, float* outRealNormTime)> FX_DrawElement_Setup_Time{ 0x140A8E4B0 };
	WEAK game::symbol<void(FxDrawState* draw, const float* elemOrigin)> FX_DrawElement_Setup_Position{ 0x140A8E3C0 };
	WEAK game::symbol<void(FxDrawState* draw)> FX_DrawElement_Setup_VisualState{ 0x140A8E560 };
	WEAK game::symbol<void(const FxElemPreVisualState* preVisState, float msecLifeSpan, FxElemVisualState* visState)> FX_EvaluateVisualState{ 0x140AC2ED0 };
	WEAK game::symbol<void(float* color, const FxElemDef* elemDef)> FX_EvaluateVisualState_DoLighting{ 0x140AC30F0 };
	WEAK game::symbol<void(const FxElemPreVisualState* preVisState, FxElemVisualState* visState)> FX_EvaluateScale{ 0x140AC2DC0 };
	WEAK game::symbol<void(const FxElemPreVisualState* preVisState, FxElemVisualState* visState)> FX_EvaluateSize{ 0x140AC2E10 };
	WEAK game::symbol<void(const FxElemDef* elemDef, const FxEffect* effect, int randomSeed, float normTime, FxElemPreVisualState* preVisState)> FX_SetupVisualState{ 0x140AC32F0 };
	WEAK game::symbol<bool(const FxDrawState* draw)> FX_CullElementForDraw_Light{ 0x140A91090 };
	WEAK game::symbol<void(FxDrawState* draw, int elemType, GfxParticleCloud* cloud)> FX_DrawElem_Cloud_PopulateGfxParticleCloud{ 0x140A8BF10 };

	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_BillboardSprite{ 0x140A8C870 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_OrientedSprite{ 0x140A8D8C0 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_Tail{ 0x140A8DFB0 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_Flare{ 0x140A8CC20 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_ParticleSimAnimation{ 0x140A8DB30 };
	WEAK game::symbol<void(FxDrawState* draw)> FX_DrawElem_Cloud{ 0x140A8BDF0 };
	WEAK game::symbol<void(FxDrawState* draw)> FX_DrawElem_SparkCloud{ 0x140A8C0D0 };
	WEAK game::symbol<void(FxDrawState* draw)> FX_DrawElem_SparkFountain{ 0x140AB9FC0 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_Model{ 0x140A8D4B0 };
	WEAK game::symbol<void(FxDrawState* draw)> FXIR_DrawElem_SpotLight{ 0x140A8DDD0 };

	WEAK game::symbol<void(GfxFlareSurfGlob** flareSurfGlob)> R_BeginFlareSurfs{ 0x1405F3370 };
	WEAK game::symbol<void(GfxFlareSurfGlob** flareSurfGlob)> R_EndFlareSurfs{ 0x1405F3500 };
	WEAK game::symbol<void(void* particleManager, FxDrawState* draw)> FX_DrawModularParticles{ 0x140D10AE0 };
	WEAK game::symbol<void(GfxCodeSurfGlob* codeSurfGlob, const char* name, FxSpriteInfo* sprite, Material* material,
		int vertIndexBase, r_double_index_t* indices, int reactiveTurbulenceGroup, char flags, int sortOrder)> FX_SpriteDeferFlush{ 0x140A96BD0 };
	WEAK game::symbol<void(GfxCodeSurfGlob* codeSurfGlob, FxSpriteInfo* sprite, int reactiveTurbulenceGroup)> FX_SpriteDirectFlush{ 0x140A96CE0 };
	WEAK game::symbol<bool(GfxCodeSurfBuffers* outBuffers, GfxCodeSurfGlob* codeSurfGlob, unsigned int vertCount, unsigned int indexCount, int argCount)> R_ReserveCodeSurfBuffers{ 0x140DDA720 };

	WEAK game::symbol<void(float* origin, float radius, float red, float green, float blue, float intensity,
		ParticleModuleInitLightOmni* module, GfxLightDef* lightDef)> R_AddOmniLightToScene{ 0x140E2EB90 };
	WEAK game::symbol<void(unsigned short handle)> RB_FreeOcclusionQuery{ 0x140E69DF0 };
	WEAK game::symbol<void(unsigned short handle, const float* position)> RB_SetOcclusionQueryPosition{ 0x140E6A420 };
	WEAK game::symbol<float(unsigned short handle)> RB_GetOcclusionQueryVisibility{ 0x140E69E30 };

	WEAK game::symbol<bool(GfxCodeSurfBuffers* outBuffers, GfxCodeSurfGlob* codeSurfGlob, unsigned int quadCount, int argCount)> R_ReserveCodeSurfQuads{ 0x140DDA600 };
	WEAK game::symbol<GfxCodeSurfQuad*(GfxCodeSurfGlob* codeSurfGlob, unsigned int quadIndex)> R_CodeSurfQuadIter_Begin{ 0x140DDA2B0 };
	WEAK game::symbol<void(FxDrawState* draw, float* s0, float* ds, float* t0, float* dt, float* nextS0, float* nextT0, float* blend)> FX_GetSpriteTexCoordsAtlasBlend{ 0x140A95220 };
	WEAK game::symbol<void(GfxCodeSurfGlob* codeSurfGlob, const FxCamera* camera, __m128* origin, const __m128* tangent, const __m128* bitangent,
		__m128* sizeX, __m128* sizeY, __m128* rotation, unsigned int lightingType, float* coordX, float* coordY, float* scale)> FX_SetupLighting{ 0x140A95F30 };
	WEAK game::symbol<bool()> R_UseBakedLighting{ 0x140E0B370 };
	WEAK game::symbol<void(GfxCodeSurfGlob* codeSurfGlob, GfxCodeSurfArgs* args)> R_AddCodeSurf{ 0x140DD9B40 };
	// outputs position (3), size scale (2), unused (2), rotation, color scale (4), normalized lifetime
	WEAK game::symbol<void(const FxParticleSimAnimation* anim, const FxParticleSimAnimationParticleData* particle, float frameLerp,
		float* out)> FX_ParticleSimAnimation_EvaluateParticle{ 0x140A95550 };
	WEAK game::symbol<game::dvar_t*> fx_gpu_lighting{ 0x145AAC160 };

	WEAK game::symbol<float> fx_randomTable{ 0x1414DFB00 };
	WEAK game::symbol<VectorFieldInstance> s_vectorFieldInstancePool{ 0x144DE1B10 };
	WEAK game::symbol<int> g_autoAddElemForUpdate{ 0x145C51498 };
	WEAK game::symbol<char> cg_fxEntitiesArray{ 0x143BD78F0 };
	WEAK game::symbol<char> particleManagers{ 0x147716080 };

	constexpr auto PARTICLE_MANAGER_SIZE = 498000;
}
