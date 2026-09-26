#pragma once

#include "game/game.hpp"

namespace fx
{
	using namespace game;

	constexpr auto FX_EFFECT_LIMIT = 2048;
	constexpr auto FX_EFFECT_HANDLE_MASK = FX_EFFECT_LIMIT - 1;
	constexpr auto FX_UPDATE_LIST_LIMIT = 2048;
	constexpr auto FX_TRAIL_ELEM_DRAW_LIMIT = 256;
	constexpr auto FX_RANDOM_SEED_MODULUS = 0x3FE9;

	constexpr auto FX_ELEM_FLAG2_GPU_LIGHTING = 0x10;
	constexpr auto FX_ELEM_FLAG2_GPU_LIGHTING_TYPE = 0x20;

	constexpr auto FX_DOBJ_HANDLE_NONE = 4095;
	constexpr auto FX_BONE_INDEX_NONE = 2047;
	constexpr auto FX_ENTITYNUM_NONE = 2047;

	enum FxEffectStatus : std::uint32_t
	{
		FX_STATUS_REF_COUNT_MASK = 0x3FFF,
		FX_STATUS_HIDDEN = 0x4000,
		FX_STATUS_IS_LOOPING = 0x8000,
		FX_STATUS_OWNED_EFFECTS_MASK = 0x3FF0000,
		FX_STATUS_OWNED_EFFECTS_SHIFT = 16,
		FX_STATUS_SELF_OWNED = 0x4000000,
	};

	enum FxEffectFlags : std::uint16_t
	{
		FX_EFFECT_FLAG_KILL_ON_STOP = 0x200,
		FX_EFFECT_FLAG_BOLTED_TO_ENTITY = 0x400,
		FX_EFFECT_FLAG_EMITTER_DRIVEN = 0x800,
	};

	enum FxSystemFlags : std::uint32_t
	{
		FX_SYSTEM_FLAG_PAUSED = 0x1,
		FX_SYSTEM_FLAG_SKIP_DRAW = 0x2,
		FX_SYSTEM_FLAG_NEEDS_GARBAGE_COLLECTION = 0x8,
		FX_SYSTEM_FLAG_ACCEPT_REQUESTS = 0x10,
		FX_SYSTEM_FLAG_DEFERRED_UPDATE = 0x40,
	};

	enum FX_ELEM_CLASS : std::uint32_t
	{
		FX_ELEM_CLASS_SPRITE = 0x0,
		FX_ELEM_CLASS_NONSPRITE = 0x1,
		FX_ELEM_CLASS_CLOUD = 0x2,
		FX_ELEM_CLASS_COUNT = 0x3,
	};

	enum FxUpdateResult : std::int32_t
	{
		FX_UPDATE_REMOVE = 0x0,
		FX_UPDATE_KEEP = 0x1,
	};

	struct orientation_t
	{
		float origin[3];
		float axis[3][3];
	};

	struct r_double_index_t
	{
		unsigned short value[2];
	};

	struct __declspec(align(8)) FxEffect
	{
		FxEffectDef* def;
		volatile int status;
		unsigned int firstElemHandle[FX_ELEM_CLASS_COUNT];
		unsigned int firstSortedElemHandle;
		unsigned int firstTrailHandle;
		unsigned int firstSparkFountainHandle;
		unsigned short occlusionQueryHandle;
		unsigned short randomSeed;
		unsigned int owner;
		float lighting[3];
		unsigned short updateCount;
		unsigned char markViewmodelClientIndex;
		unsigned short markEntnum;
		unsigned short flags;
		unsigned short bolt;
		unsigned char runnerSortOrder;
		volatile int frameCount;
		int msecBegin;
		int msecLastUpdate;
		float distanceTravelled;
		FxSpatialFrame frameAtSpawn;
		FxSpatialFrame frameNow;
		FxSpatialFrame framePrev;
		unsigned int numVectorFields;
		unsigned int vectorFields[8];
		unsigned int pad[3];
		float occlusionFade;
	};
	assert_sizeof(FxEffect, 0xE0);
	assert_offsetof(FxEffect, occlusionQueryHandle, 0x24);
	assert_offsetof(FxEffect, owner, 0x28);
	assert_offsetof(FxEffect, flags, 0x3E);
	assert_offsetof(FxEffect, bolt, 0x40);
	assert_offsetof(FxEffect, frameCount, 0x44);
	assert_offsetof(FxEffect, msecBegin, 0x48);
	assert_offsetof(FxEffect, msecLastUpdate, 0x4C);
	assert_offsetof(FxEffect, frameAtSpawn, 0x54);
	assert_offsetof(FxEffect, frameNow, 0x70);
	assert_offsetof(FxEffect, framePrev, 0x8C);
	assert_offsetof(FxEffect, occlusionFade, 0xD8);

	struct FxElem
	{
		unsigned char defIndex : 6;
		unsigned char defer : 1;
		unsigned char valid : 1;
		unsigned char sequence;
		unsigned char atRestFraction;
		unsigned char emitResidual;
		int msecBegin;
		float baseVel[3];
		float origin[3];
	};
	assert_sizeof(FxElem, 0x20);

	struct FxTrail
	{
		unsigned int nextTrailHandle;
		unsigned int firstTrailElemHandle;
		unsigned int lastTrailElemHandle;
		char defIndex;
		char sequence;
		float distanceAccumulator;
		char pad[4];
	};
	assert_sizeof(FxTrail, 0x18);

	struct FxTrailElem
	{
		float origin[3];
		float spawnDist;
		int msecBegin;
		unsigned int nextTrailElemHandle;
		short baseVelZ;
		char basis[2][3];
		unsigned char sequence;
	};
	assert_sizeof(FxTrailElem, 0x24);

	// iw7 packs 2 temporal bits (iw6 had 1), see FXIW_SpawnEffect
	struct FxBolt
	{
		unsigned int dobjHandle : 12;
		unsigned int temporalBits : 2;
		unsigned int boneIndex : 11;
		unsigned int unused : 7;
		FxSpatialFrame offset;
	};
	assert_sizeof(FxBolt, 0x20);

	struct FxSparkFountainHeader
	{
		unsigned int nextFountainInEffect;
		short defIndex;
		unsigned char isValid;
		unsigned char clusterCount;
		unsigned char computedClusterCount;
		unsigned char computedClusterPortion;
		char pad[2];
	};

	struct FxSparkFountain
	{
		FxSparkFountainHeader header;
		unsigned int clusters[16];
	};
	assert_sizeof(FxSparkFountain, 0x4C);

	struct FxSpark
	{
		float time[4];
		float pos[4][3];
		float vel[4][3];
	};

	struct FxSparkFountainCluster
	{
		FxSpark sparks[64];
	};

	struct GfxScaledPlacement
	{
		GfxPlacement base;
		float scale;
	};

	struct GfxParticleCloud
	{
		GfxScaledPlacement placement;
		float endpos[3];
		float color[4];
		unsigned char padding[4];
		float radius[2];
		unsigned int flags;
		float timeOffset;
	};
	assert_sizeof(GfxParticleCloud, 0x50);

	struct FxSparkCloud
	{
		unsigned int frameBegin;
		unsigned int frameEnd;
		float lastTime;
		unsigned int pad;
		GfxParticleCloud frames[32];
	};
	assert_sizeof(FxSparkCloud, 0xA10);

	template <typename T>
	union FxPool
	{
		int nextFree;
		T item;
	};

	union FxElemChild
	{
		unsigned short lightingHandle;
		unsigned int sparkCloudHandle;
		unsigned int sparkFountainHandle;
		unsigned int childEffectHandle;
	};

	struct SpawnEffectInfo
	{
		const FxEffectDef* remoteDef;
		FxSpatialFrame frame;
		FxSpatialFrame boltOffset;
		int msecBegin;
		int dobjHandle;
		int boneIndex;
		int runnerSortOrder;
		unsigned int owner;
		unsigned int markEntnum;
		int markViewmodelClientIndex;
		unsigned short effectFlags;
	};

	struct FXSpawnCmd
	{
		SpawnEffectInfo info;
		int removeRef;
		unsigned int elemHandle;
		unsigned int elemParent;
		unsigned int elemClass;
		int entnum;
		int timeValue;
		int nextCmd;
	};
	assert_sizeof(FXSpawnCmd, 0x80);

	union FxAccessLock
	{
		char pad[0x80];
	};

	struct ElemDeferDelete
	{
		unsigned int effectHandle;
		unsigned int elemHandle;
		int elemDefIndex;
	};

	struct ElemDeferSpawn
	{
		unsigned int effectHandle;
		unsigned int elemHandle;
		int elemDefIndex;
		int randomSeed;
	};

	struct ElemUpdate
	{
		unsigned int effectHandle;
		unsigned int elemHandle;
	};

	struct StopBoltedEffectsRequest
	{
		const FxEffectDef* defHandle;
		unsigned int dobjHandle;
		unsigned int boneIndex : 31;
		unsigned int killFlag : 1;
	};

	struct KillEffectDefRequest
	{
		const FxEffectDef* defHandle;
		int time;
	};

	struct RetriggerEffectRequest
	{
		unsigned int effectHandle;
		int msecBegin;
	};

	struct FxCamera
	{
		float origin[3];
		volatile int isValid;
		float frustum[6][4];
		float axis[3][3];
		unsigned char frustumPlaneCount;
		bool thermal;
		bool lightmap;
		bool zeroG;
		float viewOffset[3];
		float tanHalfFov[2];
		float znear;
		float unk[4];
	};
	assert_sizeof(FxCamera, 0xC0);
	assert_offsetof(FxCamera, axis, 0x70);
	assert_offsetof(FxCamera, lightmap, 0x96);

	struct FxSystemCommand
	{
		FXSpawnCmd spawnCmdList[2048];
		volatile int spawnCmdRead;
		char __pad0[0x7C];
		volatile int spawnCmdWrite;
		char __pad1[0x7C];
		FxAccessLock lock;
		unsigned int updatePass0[FX_UPDATE_LIST_LIMIT];
		unsigned int updatePass4[FX_UPDATE_LIST_LIMIT];
		ElemDeferDelete deleteElementRequest[16384];
		ElemDeferSpawn spawnElementRequest[16384];
		ElemUpdate updateElement[16384];
		StopBoltedEffectsRequest stopBoltedEffectsRequest[2][512];
		FxEffect* throughWithEffectRequest[2][2048];
		KillEffectDefRequest killEffectDefRequest[2][64];
		RetriggerEffectRequest retriggerEffectRequest[2][2048];
		int requestIndex;
		FxCamera camera;
		int numUpdatePass0;
		int numUpdatePass4;
		int numStopBoltedEffectsRequest;
		int numThroughWithEffectIndex;
		int numKillEffectDefIndex;
		int numRetriggerEffectRequest;
		volatile int pass5iterator;
		volatile int pass5count;
	};
	assert_offsetof(FxSystemCommand, spawnCmdRead, 0x40000);
	assert_offsetof(FxSystemCommand, spawnCmdWrite, 0x40080);
	assert_offsetof(FxSystemCommand, lock, 0x40100);
	assert_offsetof(FxSystemCommand, updatePass0, 0x40180);
	assert_offsetof(FxSystemCommand, updatePass4, 0x42180);
	assert_offsetof(FxSystemCommand, deleteElementRequest, 0x44180);
	assert_offsetof(FxSystemCommand, spawnElementRequest, 0x74180);
	assert_offsetof(FxSystemCommand, updateElement, 0xB4180);
	assert_offsetof(FxSystemCommand, stopBoltedEffectsRequest, 0xD4180);
	assert_offsetof(FxSystemCommand, throughWithEffectRequest, 0xD8180);
	assert_offsetof(FxSystemCommand, killEffectDefRequest, 0xE0180);
	assert_offsetof(FxSystemCommand, retriggerEffectRequest, 0xE0980);
	assert_offsetof(FxSystemCommand, requestIndex, 0xE8980);
	assert_offsetof(FxSystemCommand, camera, 0xE8984);
	assert_offsetof(FxSystemCommand, numUpdatePass0, 0xE8A44);
	assert_offsetof(FxSystemCommand, pass5iterator, 0xE8A5C);

	struct FxSpotLightInfoData
	{
		int effectHandle;
		int elemHandle;
		short id;
		short pad;
	};

	struct FxSpotLightInfo
	{
		int index;
		FxSpotLightInfoData arr[64];
	};
	assert_sizeof(FxSpotLightInfo, 0x304);

	struct FxVisState;

	struct FxSystem
	{
		FxEffect* effects;
		FxPool<FxTrail>* trails;
		FxPool<FxBolt>* bolts;
		FxPool<FxSparkFountain>* sparkFountains;
		FxPool<FxSparkFountainCluster>* sparkFountainClusters;
		int firstFreeTrail;
		int firstFreeBolt;
		int firstFreeSparkFountain;
		int firstFreeSparkFountainCluster;
		FxPool<FxSparkCloud>* sparkClouds;
		FxPool<FxTrailElem>* trailElems;
		FxPool<FxElem>* elems;
		FxElemChild* elemChild;
		unsigned int* nextElemHandleInEffect;
		unsigned int* prevElemHandleInEffect;
		int firstFreeElem;
		int firstFreeSparkCloud;
		int firstFreeTrailElem;
		volatile int gfxCloudCount;
		FxVisState* visState;
		const FxVisState* visStateBufferRead;
		FxVisState* visStateBufferWrite;
		volatile int firstActiveEffect;
		volatile int firstNewEffect;
		volatile int firstFreeEffect;
		int pad2;
		unsigned int* allEffectHandles;
		FxSpotLightInfo spotLightInfo;
		int msecNow;
		int msecDelta;
		int msecDraw;
		int frameCount;
		int localClientNum;
		FxEffect** restartEffectsList;
		unsigned int restartCount;
		FXSpawnCmd* spawnCmdList;
		volatile int* spawnCmdRead;
		volatile int* spawnCmdWrite;
		FxAccessLock* lock;
		unsigned int systemFlags;
		FxSystemCommand* command;
		ElemDeferDelete* deleteElementRequest;
		int numDeleteElementRequest;
		ElemDeferSpawn* spawnElementRequest;
		int numSpawnElementRequest;
		ElemUpdate* updateElement;
		int numUpdateElement;
		FxCamera camera;
		FxCamera cameraPrevArray[1];
		int team;
		int singleClientOnly;
		int pad;
	};
	assert_sizeof(FxSystem, 0x5B8);
	assert_offsetof(FxSystem, firstActiveEffect, 0x90);
	assert_offsetof(FxSystem, allEffectHandles, 0xA0);
	assert_offsetof(FxSystem, msecNow, 0x3AC);
	assert_offsetof(FxSystem, localClientNum, 0x3BC);
	assert_offsetof(FxSystem, spawnCmdList, 0x3D0);
	assert_offsetof(FxSystem, lock, 0x3E8);
	assert_offsetof(FxSystem, systemFlags, 0x3F0);
	assert_offsetof(FxSystem, command, 0x3F8);
	assert_offsetof(FxSystem, updateElement, 0x420);
	assert_offsetof(FxSystem, camera, 0x42C);
	assert_offsetof(FxSystem, cameraPrevArray, 0x4EC);

	struct __declspec(align(8)) FxCmd
	{
		FxSystem* system;
		int localClientNum;
		int cgameClientNum;
		FxAccessLock* lock;
		int isLocked;
		char lockRequest;
		unsigned int workerID;
	};
	assert_sizeof(FxCmd, 0x28);

	struct FxPass0Cmd : FxCmd
	{
		unsigned int* updatePass;
		int skipBolt;
		int isDeferred;
		int particleBegin;
		int particleCount;
	};
	assert_sizeof(FxPass0Cmd, 0x40);

	struct FxPass4Cmd : FxCmd
	{
		unsigned int* createPass;
		StopBoltedEffectsRequest* stopBoltedEffectsRequest;
		FxEffect** throughWithEffectRequest;
		KillEffectDefRequest* killEffectDefRequest;
		RetriggerEffectRequest* retriggerEffectRequest;
		volatile int* effectIter;
		int numStopBoltedEffectsRequest;
		int numThroughWithEffectIndex;
		int numKillEffectDefIndex;
		int numRetriggerEffectRequest;
		void* fxEntityArray;
	};
	assert_sizeof(FxPass4Cmd, 0x70);

	struct FxPass5Cmd : FxCmd
	{
		volatile int* effectIter;
		volatile int* count;
	};
	assert_sizeof(FxPass5Cmd, 0x38);

	struct FxSpriteInfo
	{
		r_double_index_t* indices;
		Material* material;
		const char* name;
		unsigned int indexCount;
		int vertIndexBase;
		unsigned int sortOrder;
		char flags;
	};
	assert_sizeof(FxSpriteInfo, 0x28);

	struct FxElemPreVisualState
	{
		float sampleLerp;
		float sampleLerpInv;
		const FxElemDef* elemDef;
		const FxEffect* effect;
		const FxElemVisStateSample* refState;
		int randomSeed;
		float distanceFade;
		float occlusionFade;
	};
	assert_sizeof(FxElemPreVisualState, 0x30);

	struct GfxCodeSurfGlob;
	struct GfxFlareSurfGlob;

	struct FxDrawState
	{
		const FxSystem* system;
		const FxEffect* effect;
		const FxElem* elem;
		const FxElemDef* elemDef;
		FxSpriteInfo sprite;
		int pad[2];
		GfxCodeSurfGlob* codeSurfGlob;
		orientation_t orient;
		const FxCamera* camera;
		bool flaresEnabled;
		int randomSeed;
		float msecLifeSpan;
		float msecElapsed;
		float normTimeUpdateEnd;
		float posWorld[3];
		float velDirWorld[3];
		FxElemVisualState visState;
		FxElemPreVisualState preVisState;
		float physicsLerpFrac;
		int msecDraw;
		unsigned int elemHandle;
		int reactiveTurbulenceGroup;
		__m128 negCameraNormal;
		__m128 negCameraTangent;
		__m128 cameraBitangent;
		GfxFlareSurfGlob* flareSurfGlob;
		char __pad0[0xC];
		bool unk164;
		int unk168;
		int surfList;
	};
	assert_sizeof(FxDrawState, 0x170);
	assert_offsetof(FxDrawState, sprite, 0x20);
	assert_offsetof(FxDrawState, codeSurfGlob, 0x50);
	assert_offsetof(FxDrawState, orient, 0x58);
	assert_offsetof(FxDrawState, camera, 0x88);
	assert_offsetof(FxDrawState, flaresEnabled, 0x90);
	assert_offsetof(FxDrawState, randomSeed, 0x94);
	assert_offsetof(FxDrawState, posWorld, 0xA4);
	assert_offsetof(FxDrawState, velDirWorld, 0xB0);
	assert_offsetof(FxDrawState, visState, 0xBC);
	assert_offsetof(FxDrawState, preVisState, 0xE0);
	assert_offsetof(FxDrawState, msecDraw, 0x114);
	assert_offsetof(FxDrawState, reactiveTurbulenceGroup, 0x11C);
	assert_offsetof(FxDrawState, negCameraNormal, 0x120);
	assert_offsetof(FxDrawState, flareSurfGlob, 0x150);
	assert_offsetof(FxDrawState, unk164, 0x164);
	assert_offsetof(FxDrawState, surfList, 0x16C);

	struct FxUpdateElem
	{
		unsigned int effectHandle;
		FxEffect* effect;
		int elemIndex;
		int atRestFraction;
		orientation_t orient;
		int randomSeed;
		int sequence;
		float msecLifeSpan;
		int msecElemBegin;
		int msecElemEnd;
		int msecUpdateBegin;
		int msecUpdateEnd;
		float msecElapsed;
		float invMsecLifeSpan;
		float* elemOrigin;
		float* elemBaseVel;
		float posWorld[3];
		bool onGround;
		bool hasNonBoltedEmitter;
	};

	struct FxTrailSegmentDrawState
	{
		const FxTrailDef* trailDef;
		float posWorld[3];
		float basis[2][3];
		float rotation;
		float size[2];
		float uCoord;
		float color[4];
	};

	struct FXRegisteredDef
	{
		char type;
		void* fx;
	};

	struct GfxSpriteVertex
	{
		float xyz[3];
		unsigned int extraData;
		float color[4];
		unsigned int lightingCoord;
		unsigned int texCoord;
		unsigned int normal;
		unsigned int tangentBinormalSign;
	};
	assert_sizeof(GfxSpriteVertex, 0x30);

	struct GfxCodeSurfBuffers
	{
		unsigned int vertIndexBase;
		unsigned short vertIndexOffset;
		r_double_index_t* indices;
		unsigned int argOffset;
	};
	assert_offsetof(GfxCodeSurfBuffers, vertIndexOffset, 0x4);
	assert_offsetof(GfxCodeSurfBuffers, indices, 0x8);
	assert_offsetof(GfxCodeSurfBuffers, argOffset, 0x10);

	struct GfxCodeSurfArgs
	{
		Material* material;
		unsigned int vertIndexBase;
		r_double_index_t* indices;
		unsigned int indexCount;
		unsigned int argOffset;
		unsigned int argCount;
		const char* fxName;
		int reactiveTurbulenceGroup;
		unsigned int sortOrder;
		unsigned int unk;
		char flags;
	};
	assert_sizeof(GfxCodeSurfArgs, 0x40);
	assert_offsetof(GfxCodeSurfArgs, indexCount, 0x18);
	assert_offsetof(GfxCodeSurfArgs, fxName, 0x28);
	assert_offsetof(GfxCodeSurfArgs, flags, 0x3C);

	struct GfxCodeSurfQuad
	{
		float origin[3];
		float rotation;
		float normal[3];
		float intensity;
		float tangent[3];
		float atlasBlend;
		float bitangent[3];
		float lightingScale;
		float texCoordOrigin[2];
		float texCoordSize[2];
		float nextTexCoordOrigin[2];
		float lightingCoord[2];
		float color[4];
		float size[2];
	};
	assert_sizeof(GfxCodeSurfQuad, 120);

	struct GfxCodeSurfGlob
	{
		unsigned int surfCount;
		unsigned int vertCount;
		unsigned int vertIndexBase;
		unsigned int quadCount;
		unsigned int quadIndexBase;
		unsigned int indexCount;
		unsigned int argsCount;
		char __pad0[100];
		GfxSpriteVertex* verts;
		void* quads;
		unsigned short* indices;
		char __pad1[208];
	};
	assert_offsetof(GfxCodeSurfGlob, verts, 0x80);
	assert_offsetof(GfxCodeSurfGlob, indices, 0x90);

	constexpr auto VECTOR_FIELD_INSTANCE_LIMIT = 256;

	struct VectorFieldInstance
	{
		VectorField* vf;
		orientation_t orient;
		float strengthScale;
		float unk1;
		float unk2;
		float unk3;
		float sizeScale;
		volatile int inUse;
		int isRotated;
		unsigned int elemHandle;
	};
	assert_sizeof(VectorFieldInstance, 0x58);
	assert_offsetof(VectorFieldInstance, inUse, 0x4C);
	assert_offsetof(VectorFieldInstance, elemHandle, 0x54);

	// handles are byte offsets into the pools, scaled by 4 for elems and trails and by 8 for effects
	inline unsigned int FX_ElemHandleToIndex(const unsigned int elemHandle)
	{
		return (4 * elemHandle) / sizeof(FxElem);
	}

	inline FxElem* FX_ElemFromHandle(const FxSystem* system, const unsigned int elemHandle)
	{
		return reinterpret_cast<FxElem*>(reinterpret_cast<char*>(system->elems) + 4 * elemHandle);
	}

	inline unsigned int FX_NextElemHandle(const FxSystem* system, const unsigned int elemHandle)
	{
		return system->nextElemHandleInEffect[FX_ElemHandleToIndex(elemHandle)];
	}

	inline FxEffect* FX_EffectFromHandle(const FxSystem* system, const unsigned int effectHandle)
	{
		return reinterpret_cast<FxEffect*>(reinterpret_cast<char*>(system->effects) + 8 * effectHandle);
	}

	inline unsigned int FX_EffectToHandle(const FxSystem* system, const FxEffect* effect)
	{
		return static_cast<unsigned int>((reinterpret_cast<const char*>(effect) - reinterpret_cast<const char*>(system->effects)) / 8);
	}

	inline FxTrail* FX_TrailFromHandle(const FxSystem* system, const unsigned int trailHandle)
	{
		return reinterpret_cast<FxTrail*>(reinterpret_cast<char*>(system->trails) + 4 * trailHandle);
	}

	inline FxTrailElem* FX_TrailElemFromHandle(const FxSystem* system, const unsigned int trailElemHandle)
	{
		return reinterpret_cast<FxTrailElem*>(reinterpret_cast<char*>(system->trailElems) + 4 * trailElemHandle);
	}

	inline FxBolt* FX_BoltFromHandle(const FxSystem* system, const unsigned short bolt)
	{
		return &system->bolts[bolt].item;
	}
}
