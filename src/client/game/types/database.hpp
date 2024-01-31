#pragma once
#include <d3d11.h>

namespace game::database
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	struct dummy
	{
	};

	enum scr_string_t : std::int32_t
	{
	};

	enum XAssetType : std::int32_t
	{
		ASSET_TYPE_PHYSICSLIBRARY = 0,
		ASSET_TYPE_PHYSICS_SFX_EVENT_ASSET = 1,
		ASSET_TYPE_PHYSICS_VFX_EVENT_ASSET = 2,
		ASSET_TYPE_PHYSICSASSET = 3,
		ASSET_TYPE_PHYSICS_FX_PIPELINE = 4,
		ASSET_TYPE_PHYSICS_FX_SHAPE = 5,
		ASSET_TYPE_XANIMPARTS = 6,
		ASSET_TYPE_XMODEL_SURFS = 7,
		ASSET_TYPE_XMODEL = 8,
		ASSET_TYPE_MAYHEM = 9,
		ASSET_TYPE_MATERIAL = 10,
		ASSET_TYPE_COMPUTESHADER = 11,
		ASSET_TYPE_VERTEXSHADER = 12,
		ASSET_TYPE_HULLSHADER = 13,
		ASSET_TYPE_DOMAINSHADER = 14,
		ASSET_TYPE_PIXELSHADER = 15,
		ASSET_TYPE_VERTEXDECL = 16,
		ASSET_TYPE_TECHNIQUE_SET = 17,
		ASSET_TYPE_IMAGE = 18,
		ASSET_TYPE_SOUND_GLOBALS = 19,
		ASSET_TYPE_SOUND_BANK = 20,
		ASSET_TYPE_SOUND_BANK_TRANSIENT = 21,
		ASSET_TYPE_CLIPMAP = 22,
		ASSET_TYPE_COMWORLD = 23,
		ASSET_TYPE_GLASSWORLD = 24,
		ASSET_TYPE_PATHDATA = 25,
		ASSET_TYPE_NAVMESH = 26,
		ASSET_TYPE_MAP_ENTS = 27,
		ASSET_TYPE_FXWORLD = 28,
		ASSET_TYPE_GFXWORLD = 29,
		ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE = 30,
		ASSET_TYPE_IESPROFILE = 31, // unused
		ASSET_TYPE_LIGHT_DEF = 32,
		ASSET_TYPE_UI_MAP = 33, // unused
		ASSET_TYPE_ANIMCLASS = 34,
		ASSET_TYPE_PLAYERANIM = 35,
		ASSET_TYPE_GESTURE = 36,
		ASSET_TYPE_LOCALIZE_ENTRY = 37,
		ASSET_TYPE_ATTACHMENT = 38,
		ASSET_TYPE_WEAPON = 39,
		ASSET_TYPE_VFX = 40,
		ASSET_TYPE_FX = 41,
		ASSET_TYPE_IMPACT_FX = 42,
		ASSET_TYPE_SURFACE_FX = 43,
		ASSET_TYPE_AITYPE = 44, // unused
		ASSET_TYPE_MPTYPE = 45, // unused
		ASSET_TYPE_CHARACTER = 46, // unused
		ASSET_TYPE_XMODELALIAS = 47, // unused
		ASSET_TYPE_RAWFILE = 48,
		ASSET_TYPE_SCRIPTFILE = 49,
		ASSET_TYPE_STRINGTABLE = 50,
		ASSET_TYPE_LEADERBOARD = 51,
		ASSET_TYPE_VIRTUAL_LEADERBOARD = 52,
		ASSET_TYPE_STRUCTUREDDATADEF = 53,
		ASSET_TYPE_DDL = 54,
		ASSET_TYPE_TRACER = 55,
		ASSET_TYPE_VEHICLE = 56,
		ASSET_TYPE_ADDON_MAP_ENTS = 57,
		ASSET_TYPE_NET_CONST_STRINGS = 58,
		ASSET_TYPE_LUA_FILE = 59,
		ASSET_TYPE_SCRIPTABLE = 60,
		ASSET_TYPE_EQUIPMENT_SND_TABLE = 61,
		ASSET_TYPE_VECTORFIELD = 62,
		ASSET_TYPE_PARTICLE_SIM_ANIMATION = 63,
		ASSET_TYPE_STREAMING_INFO = 64,
		ASSET_TYPE_LASER = 65,
		ASSET_TYPE_TTF = 66,
		ASSET_TYPE_SUIT = 67,
		ASSET_TYPE_SUITANIMPACKAGE = 68,
		ASSET_TYPE_SPACESHIPTARGET = 69,
		ASSET_TYPE_RUMBLE = 70,
		ASSET_TYPE_RUMBLE_GRAPH = 71,
		ASSET_TYPE_ANIM_PACKAGE = 72,
		ASSET_TYPE_SFX_PACKAGE = 73,
		ASSET_TYPE_VFX_PACKAGE = 74,
		ASSET_TYPE_BEHAVIOR_TREE = 75,
		ASSET_TYPE_XANIM_ARCHETYPE = 76,
		ASSET_TYPE_XANIM_PROCEDURALBONES = 77,
		ASSET_TYPE_RETICLE = 78,
		ASSET_TYPE_GFXLIGHTMAP = 79,
		ASSET_TYPE_COUNT = 80
	};

	struct PhysicsLibrary;
	struct PhysicsSFXEventAsset;
	struct PhysicsVFXEventAsset;
	struct PhysicsAsset;
	struct PhysicsFXPipeline;
	struct PhysicsFXShape;
	struct XAnimParts;
	struct XModelSurfs;
	struct XModel;
	struct Mayhem;
	struct Material;
	struct ComputeShader;
	struct MaterialVertexShader;
	struct MaterialHullShader;
	struct MaterialDomainShader;
	struct MaterialPixelShader;
	struct MaterialVertexDeclaration;
	struct MaterialTechniqueSet;
	struct GfxImage;
	//ASSET_TYPE_SOUND_GLOBALS = 19,
	//ASSET_TYPE_SOUND_BANK = 20,
	//ASSET_TYPE_SOUND_BANK_TRANSIENT = 21,
	//ASSET_TYPE_CLIPMAP = 22,
	//ASSET_TYPE_COMWORLD = 23,
	//ASSET_TYPE_GLASSWORLD = 24,
	//ASSET_TYPE_PATHDATA = 25,
	//ASSET_TYPE_NAVMESH = 26,
	struct MapEnts;
	//ASSET_TYPE_FXWORLD = 28,
	//ASSET_TYPE_GFXWORLD = 29,
	//ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE = 30,
	//ASSET_TYPE_IESPROFILE = 31, // unused
	//ASSET_TYPE_LIGHT_DEF = 32,
	//ASSET_TYPE_UI_MAP = 33, // unused
	//ASSET_TYPE_ANIMCLASS = 34,
	//ASSET_TYPE_PLAYERANIM = 35,
	//ASSET_TYPE_GESTURE = 36,
	//ASSET_TYPE_LOCALIZE_ENTRY = 37,
	//ASSET_TYPE_ATTACHMENT = 38,
	//ASSET_TYPE_WEAPON = 39,
	//ASSET_TYPE_VFX = 40,
	//ASSET_TYPE_IMPACT_FX = 41,
	//ASSET_TYPE_SURFACE_FX = 42,
	//ASSET_TYPE_AITYPE = 43, // unused
	//ASSET_TYPE_MPTYPE = 44, // unused
	//ASSET_TYPE_CHARACTER = 45, // unused
	//ASSET_TYPE_XMODELALIAS = 46, // unused
	//ASSET_TYPE_UNKNOWN = 47, // unused
	struct RawFile;
	//ASSET_TYPE_SCRIPTFILE = 49,
	//ASSET_TYPE_STRINGTABLE = 50,
	//ASSET_TYPE_LEADERBOARD = 51,
	//ASSET_TYPE_VIRTUAL_LEADERBOARD = 52,
	//ASSET_TYPE_STRUCTUREDDATADEF = 53,
	//ASSET_TYPE_DDL = 54,
	//ASSET_TYPE_TRACER = 55,
	//ASSET_TYPE_VEHICLE = 56,
	//ASSET_TYPE_ADDON_MAP_ENTS = 57,
	//ASSET_TYPE_NET_CONST_STRINGS = 58,
	//ASSET_TYPE_LUA_FILE = 59,
	struct ScriptableDef;
	//ASSET_TYPE_EQUIPMENT_SND_TABLE = 61,
	//ASSET_TYPE_VECTORFIELD = 62,
	//ASSET_TYPE_PARTICLE_SIM_ANIMATION = 63,
	//ASSET_TYPE_STREAMING_INFO = 64,
	//ASSET_TYPE_LASER = 65,
	//ASSET_TYPE_TTF = 66,
	//ASSET_TYPE_SUIT = 67,
	//ASSET_TYPE_SUITANIMPACKAGE = 68,
	//ASSET_TYPE_SPACESHIPTARGET = 69,
	//ASSET_TYPE_RUMBLE = 70,
	//ASSET_TYPE_RUMBLE_GRAPH = 71,
	//ASSET_TYPE_ANIM_PACKAGE = 72,
	//ASSET_TYPE_SFX_PACKAGE = 73,
	//ASSET_TYPE_VFX_PACKAGE = 74,
	//ASSET_TYPE_BEHAVIOR_TREE = 75,
	//ASSET_TYPE_XANIM_ARCHETYPE = 76,
	struct XAnimProceduralBones;
	//ASSET_TYPE_RETICLE = 78,
	//ASSET_TYPE_GFXLIGHTMAP = 79,

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct PhysicsLibrary
	{
		const char* name;
		bool isMaterialList;
		bool isBodyQualityList;
		bool isMotionPropertiesList;
		bool isGlobalTypeCompendium;
		unsigned int havokDataSize;
		char* havokData;
	}; assert_sizeof(PhysicsLibrary, 0x18);

	struct PhysicsSFXEventAssetRule;
	struct PhysicsSFXEventAssetRules
	{
		int numrules;
		PhysicsSFXEventAssetRule* rules;
	};

	enum PhysicsSFXEventAssetRuleType
	{
		Types_PhysicsSFXEventRule = 0x0,
		Types_PhysicsSFXEventSoundRule = 0x1,
		Types_PhysicsSFXEventMaterialRule = 0x2,
		Types_PhysicsSFXEventAngleRule = 0x3,
		Types_PhysicsSFXEventSpeedRule = 0x4,
		Types_PhysicsSFXEventMassRule = 0x5,
		Types_PhysicsSFXEventGravityRule = 0x6,
		Types_PhysicsSFXEventAtmosphereRule = 0x7,
		Types_PhysicsSFXEventCameraDistanceRule = 0x8,
		Types_PhysicsSFXEventFrequencyRule = 0x9,
		Types_PhysicsSFXEventRuleCount = 0xA,
	};

	struct PhysicsSFXEventAssetSoundRule
	{
		char* hitSoundAlias;
		char* scrapeSoundAlias;
		float scrapeSoundVolumeSpeedThresholdMin;
		float scrapeSoundVolumeSpeedThresholdMax;
		float scrapeSoundVolumeBlendSpeed;
	};

	struct PhysicsSFXEventAssetMaterialEntry
	{
		int surfaceType;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetMaterialRule
	{
		int numentries;
		PhysicsSFXEventAssetMaterialEntry* entries;
	};

	struct PhysicsSFXEventAssetAngleEntry
	{
		float angle;
		float hitSoundVolumeModifierMin;
		float hitSoundVolumeModifierMax;
		float hitSoundPitchModifierMin;
		float hitSoundPitchModifierMax;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetAngleRule
	{
		int numentries;
		PhysicsSFXEventAssetAngleEntry* entries;
	};

	struct PhysicsSFXEventAssetSpeedEntry
	{
		float speedThreshold;
		float hitSoundVolumeModifierMin;
		float hitSoundVolumeModifierMax;
		float hitSoundPitchModifierMin;
		float hitSoundPitchModifierMax;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetSpeedRule
	{
		int numentries;
		PhysicsSFXEventAssetSpeedEntry* entries;
	};

	struct PhysicsSFXEventAssetMassEntry
	{
		float massThreshold;
		float hitSoundVolumeModifierMin;
		float hitSoundVolumeModifierMax;
		float hitSoundPitchModifierMin;
		float hitSoundPitchModifierMax;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetMassRule
	{
		int numentries;
		PhysicsSFXEventAssetMassEntry* entries;
	};

	struct PhysicsSFXEventAssetGravityEntry
	{
		bool gravity;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetGravityRule
	{
		int numentries;
		PhysicsSFXEventAssetGravityEntry* entries;
	};

	struct PhysicsSFXEventAssetAtmosphereEntry
	{
		bool atmosphere;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetAtmosphereRule
	{
		int numentries;
		PhysicsSFXEventAssetAtmosphereEntry* entries;
	};

	struct PhysicsSFXEventAssetCameraDistanceEntry
	{
		float distance;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetCameraDistanceRule
	{
		int numentries;
		PhysicsSFXEventAssetCameraDistanceEntry* entries;
	};

	struct PhysicsSFXEventAssetFrequencyEntry
	{
		int maxEvents;
		PhysicsSFXEventAssetRules rules;
	};

	struct PhysicsSFXEventAssetFrequencyRule
	{
		int numentries;
		PhysicsSFXEventAssetFrequencyEntry* entries;
	};

	union PhysicsSFXEventAssetRuleUnion
	{
		PhysicsSFXEventAssetSoundRule soundRule;
		PhysicsSFXEventAssetMaterialRule materialRule;
		PhysicsSFXEventAssetAngleRule angleRule;
		PhysicsSFXEventAssetSpeedRule speedRule;
		PhysicsSFXEventAssetMassRule massRule;
		PhysicsSFXEventAssetGravityRule gravityRule;
		PhysicsSFXEventAssetAtmosphereRule atmosphereRule;
		PhysicsSFXEventAssetCameraDistanceRule cameraDistanceRule;
		PhysicsSFXEventAssetFrequencyRule frequencyRule;
	};

	struct PhysicsSFXEventAssetRule
	{
		PhysicsSFXEventAssetRuleType type;
		PhysicsSFXEventAssetRuleUnion u;
	};

	struct PhysicsSFXEventAsset
	{
		const char* name;
		int priority;
		PhysicsSFXEventAssetRules rules;
	}; assert_sizeof(PhysicsSFXEventAsset, 0x20);

	struct PhysicsVFXEventAssetRule;
	struct PhysicsVFXEventAssetRules
	{
		int numrules;
		PhysicsVFXEventAssetRule* rules;
	};

	enum PhysicsVFXEventAssetRuleType
	{
		Types_PhysicsVFXEventRule = 0x0,
		Types_PhysicsVFXEventParticleEffectRule = 0x1,
		Types_PhysicsVFXEventMaterialRule = 0x2,
		Types_PhysicsVFXEventAngleRule = 0x3,
		Types_PhysicsVFXEventSpeedRule = 0x4,
		Types_PhysicsVFXEventMassRule = 0x5,
		Types_PhysicsVFXEventGravityRule = 0x6,
		Types_PhysicsVFXEventAtmosphereRule = 0x7,
		Types_PhysicsVFXEventCameraDistanceRule = 0x8,
		Types_PhysicsVFXEventFrequencyRule = 0x9,
		Types_PhysicsVFXEventRuleCount = 0xA,
	};

	struct PhysicsVFXEventAssetParticleEffectRule
	{
		char* hitParticleEffectAlias;
		char* scrapeParticleEffectAlias;
	};

	struct PhysicsVFXEventAssetMaterialEntry
	{
		int surfaceType;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetMaterialRule
	{
		int numentries;
		PhysicsVFXEventAssetMaterialEntry* entries;
	};

	struct PhysicsVFXEventAssetAngleEntry
	{
		float angle;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetAngleRule
	{
		int numentries;
		PhysicsVFXEventAssetAngleEntry* entries;
	};

	struct PhysicsVFXEventAssetSpeedEntry
	{
		float speedThreshold;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetSpeedRule
	{
		int numentries;
		PhysicsVFXEventAssetSpeedEntry* entries;
	};

	struct PhysicsVFXEventAssetMassEntry
	{
		float massThreshold;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetMassRule
	{
		int numentries;
		PhysicsVFXEventAssetMassEntry* entries;
	};

	struct PhysicsVFXEventAssetGravityEntry
	{
		bool gravity;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetGravityRule
	{
		int numentries;
		PhysicsVFXEventAssetGravityEntry* entries;
	};

	struct PhysicsVFXEventAssetAtmosphereEntry
	{
		bool atmosphere;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetAtmosphereRule
	{
		int numentries;
		PhysicsVFXEventAssetAtmosphereEntry* entries;
	};

	struct PhysicsVFXEventAssetCameraDistanceEntry
	{
		float distance;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetCameraDistanceRule
	{
		int numentries;
		PhysicsVFXEventAssetCameraDistanceEntry* entries;
	};

	struct PhysicsVFXEventAssetFrequencyEntry
	{
		int maxEvents;
		PhysicsVFXEventAssetRules rules;
	};

	struct PhysicsVFXEventAssetFrequencyRule
	{
		int numentries;
		PhysicsVFXEventAssetFrequencyEntry* entries;
	};

	union PhysicsVFXEventAssetRuleUnion
	{
		PhysicsVFXEventAssetParticleEffectRule particleEffectRule;
		PhysicsVFXEventAssetMaterialRule materialRule;
		PhysicsVFXEventAssetAngleRule angleRule;
		PhysicsVFXEventAssetSpeedRule speedRule;
		PhysicsVFXEventAssetMassRule massRule;
		PhysicsVFXEventAssetGravityRule gravityRule;
		PhysicsVFXEventAssetAtmosphereRule atmosphereRule;
		PhysicsVFXEventAssetCameraDistanceRule cameraDistanceRule;
		PhysicsVFXEventAssetFrequencyRule frequencyRule;
	};

	struct PhysicsVFXEventAssetRule
	{
		PhysicsVFXEventAssetRuleType type;
		PhysicsVFXEventAssetRuleUnion u;
	};

	struct PhysicsVFXEventAsset
	{
		const char* name;
		int priority;
		PhysicsVFXEventAssetRules rules;
	}; assert_sizeof(PhysicsVFXEventAsset, 0x20);

	struct PhysicsAsset
	{
		const char* name;
		unsigned int havokDataSize;
		char* havokData;
		char __pad0[8];
		int numConstraints;
		int numSFXEventAssets;
		void** sfxEventAssets;
		int numVFXEventAssets;
		void** vfxEventAssets;
		char __pad1[16];
	}; assert_sizeof(PhysicsAsset, 0x50);

	struct PhysicsFXPipeline
	{
		const char* name;
		char __pad0[24];
	}; assert_sizeof(PhysicsFXPipeline, 0x20);

	struct PhysicsFXShape
	{
		const char* name;
		unsigned int size;
		vec3_t* vecs;
	}; assert_sizeof(PhysicsFXShape, 0x18);

	union XAnimIndices
	{
		char* _1;
		unsigned short* _2;
		void* data;
	};

	struct XAnimNotifyInfo
	{
		scr_string_t name;
		float time;
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned short(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		char _1[1];
		unsigned short _2[1];
	};

	struct XAnimPartTransFrames
	{
		vec3_t mins;
		vec3_t size;
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		vec3_t frame0;
	};

	struct XAnimPartTrans
	{
		unsigned short size;
		char smallTrans;
		XAnimPartTransData u;
	};

	struct XAnimDeltaPartQuatDataFrames2
	{
		short(*frames)[2];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		short frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned short size;
		XAnimDeltaPartQuatData2 u;
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		short(*frames)[4];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		short frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned short size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat2* quat2;
		XAnimDeltaPartQuat* quat;
	};

	struct XAnimParts
	{
		const char* name; // 0
		scr_string_t* names; // 8
		char* dataByte; // 16
		short* dataShort; // 24
		int* dataInt; // 32
		short* randomDataShort; // 40
		char* randomDataByte; // 48
		int* randomDataInt; // 56
		XAnimIndices indices; // 64
		XAnimNotifyInfo* notify; // 72
		XAnimDeltaPart* deltaPart; // 80
		unsigned int randomDataShortCount; // 88
		unsigned int randomDataByteCount; // 92
		unsigned int indexCount; // 96
		float framerate; // 100
		float frequency; // 104
		unsigned short dataByteCount; // 108
		unsigned short dataShortCount; // 110
		unsigned short dataIntCount; // 112
		unsigned short randomDataIntCount; // 114
		unsigned short numframes; // 116
		char flags; // 118
		char boneCount[10]; // 119
		char notifyCount; // 129
		char assetType; // 130
		char ikType; // 131
	}; assert_sizeof(XAnimParts, 0x88);

	struct XSurfaceCollisionAabb
	{
		unsigned short mins[3];
		unsigned short maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned short childBeginIndex;
		unsigned short childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned short triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode* nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf* leafs;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	union PackedUnitVec
	{
		unsigned int packed;
	};

	union PackedTexCoords
	{
		unsigned int packed;
	};

	union GfxColor
	{
		unsigned char array[4];
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct GfxPackedMotionVertex
	{
		float xyz[3];
		float binormalSignAndHeight;
		GfxColor pieceIndex;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	union GfxVertexUnion0
	{
		GfxPackedVertex* packedVerts0;
		GfxPackedMotionVertex* packedMotionVerts0;
		void* verts0;
	};

	struct Face
	{
		unsigned short v1;
		unsigned short v2;
		unsigned short v3;
	};

	typedef unsigned int XBlendInfo;

	struct XSubdivRigidVertList
	{
		unsigned int firstFace;
		unsigned int faceCount;
		unsigned int firstRegularPatch;
		unsigned int regularPatchCount;
	};

	struct XSurfaceSubdivLevel
	{
		XSubdivRigidVertList* rigidVertLists;
		unsigned int faceCount;
		unsigned int regularPatchCount;
		unsigned int regularPatchOffset;
		unsigned int facePointCount;
		unsigned int facePointValence4Count;
		unsigned int facePointBufferSize;
		unsigned int edgePointCount;
		unsigned int edgePointSmoothEnd;
		unsigned int edgePointUVBorderEnd;
		unsigned int vertexPointCount;
		unsigned int vertexPointValence4Count;
		unsigned int vertexPointBufferSize;
		unsigned int normalCount;
		unsigned int normalBufferSize;
		unsigned int transitionPointCount;
		unsigned int vertCount;
		unsigned int vertOffset;
		unsigned short* faceIndices;
		unsigned short* regularPatchIndices;
		unsigned int* regularPatchFlags;
		unsigned int* facePoints;
		unsigned int* edgePoints;
		unsigned int* vertexPoints;
		unsigned int* normals;
		unsigned int* transitionPoints;
		float* regularPatchCones;
		ID3D11Buffer* regularPatchIndexBuffer;
		ID3D11Buffer* faceIndexBuffer;
		ID3D11ShaderResourceView* regularPatchIndexBufferView;
		ID3D11ShaderResourceView* regularPatchFlagsView;
		ID3D11ShaderResourceView* facePointsView;
		ID3D11ShaderResourceView* edgePointsView;
		ID3D11ShaderResourceView* vertexPointsView;
		ID3D11ShaderResourceView* normalsView;
		ID3D11ShaderResourceView* transitionPointsView;
		ID3D11ShaderResourceView* regularPatchConesView;
	}; assert_sizeof(XSurfaceSubdivLevel, 0xE8);

	struct GfxSubdivCache
	{
		unsigned int size;
		ID3D11Buffer* subdivCacheBuffer;
		ID3D11ShaderResourceView* subdivCacheView;
	}; assert_sizeof(GfxSubdivCache, 0x18);

	struct XSurfaceSubdivInfo
	{
		XSurfaceSubdivLevel* levels;
		char __pad0[24];
		GfxSubdivCache cache;
	}; assert_sizeof(XSurfaceSubdivInfo, 0x38);

	struct TensionData
	{
		char __pad0[32];
	};

	struct SHProbeSimplexData0
	{
		char __pad0[32];
	};

	struct SHProbeSimplexData1
	{
		char __pad0[8];
	};

	struct SHProbeSimplexData2
	{
		char __pad0[24];
	};

	union SHProbeSimplexDataUnion
	{
		SHProbeSimplexData0 data0;
		SHProbeSimplexData1 data1;
		SHProbeSimplexData2 data2;
	};

	struct XSurface
	{
		unsigned short flags; // 0
		unsigned short vertCount; // 2
		unsigned short triCount; // 4
		char rigidVertListCount; // 6
		char subdivLevelCount; // 7
		short blendVertCounts[8]; // 8
		int unkBlendCount; // 24
		char __pad0[4]; // 28
		GfxVertexUnion0 verts0; // 32
		Face* triIndices; // 40
		ID3D11Buffer* vb0; // 48
		ID3D11ShaderResourceView* vb0View; // 56
		ID3D11Buffer* indexBuffer; // 64
		XRigidVertList* rigidVertLists; // 72
		char* unkBlend; // 80
		ID3D11Buffer* blendVertsBuffer; // 88
		ID3D11ShaderResourceView* blendVertsView; // 96
		__int64* lmapUnwrap; // 104
		ID3D11Buffer* vblmapBuffer; // 112
		ID3D11ShaderResourceView* vblmapView; // 120
		XSurfaceSubdivInfo* subdiv; // 128
		XBlendInfo* blendVerts; // 136
		TensionData* tensionData; // 144
		void* tensionAccumTable; // 152
		ID3D11Buffer* tensionAccumTableBuffer; // 160
		ID3D11ShaderResourceView* tensionAccumTableView; // 168
		ID3D11Buffer* tensionDataBuffer; // 176
		ID3D11ShaderResourceView* tensionDataView; // 184
		ID3D11ShaderResourceView* indexBufferView; // 192
		SHProbeSimplexDataUnion* shProbeSimplexVertData; // 200
		ID3D11Buffer* shProbeSimplexVertBuffer; // 208
		ID3D11ShaderResourceView* shProbeSimplexVertBufferView; // 216
		int partBits[8]; // 224
	}; assert_sizeof(XSurface, 0x100);

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfs;
		unsigned short numsurfs;
		int partBits[8];
	}; assert_sizeof(XModelSurfs, 0x38);

	struct XModelAngle
	{
		short x;
		short y;
		short z;
		short base;
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct ReactiveMotionModelPart
	{
		float center[3];
		float stiffness;
	};

	struct ReactiveMotionModelTweaks
	{
		float scale[4];
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned short numsurfs;
		unsigned short surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[8];
		XSurface* surfs;
		int unk; // subdivLodValidMask?
		char flags;
		char pad[3];
	}; assert_sizeof(XModelLodInfo, 0x40);

	struct XModelCollSurf_s
	{
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	}; assert_sizeof(XModelCollSurf_s, 36);

	struct XBoneInfo
	{
		Bounds bounds;
		union
		{
			float radiusSquared;
			unsigned int radiusSquaredAsInt;
		};
	};

	struct XModel
	{
		const char* name; // 0
		char __pad0[12];
		unsigned char numBones; // 20
		unsigned char numRootBones; // 21
		unsigned char numReactiveMotionParts; // 22
		char b;
		unsigned short numClientBones; // 24
		unsigned char numsurfs; // 26
		char __pad1[5];
		unsigned short numCollSurfs; // 32
		char __pad2[38];
		unsigned char unk1Count; // 72
		char __pad3[15];
		ScriptableDef* scriptableMoverDef; // 88
		XAnimProceduralBones* proceduralBones; // 96
		scr_string_t* aimAssistBones; // 104
		unsigned char numAimAssistBones; // 112
		char __pad4[39];
		scr_string_t* boneNames; // 152
		unsigned char* parentList; // 160
		XModelAngle* tagAngles; // 168
		XModelTagPos* tagPositions; // 176
		unsigned char* partClassification; // 184
		DObjAnimMat* baseMat; // 192
		ReactiveMotionModelPart* reactiveMotionParts; // 200
		ReactiveMotionModelTweaks* reactiveMotionTweaks; // 208
		Material** materialHandles; // 216
		XModelLodInfo lodInfo[6]; // 224
		XModelCollSurf_s* collSurfs; // 608
		XBoneInfo* boneInfo; // 616
		unsigned short* invHighMipRadius; // 624
		PhysicsAsset* physAsset; // 632
		PhysicsFXShape* physFxShape; // 640
		char* unk1; // 648
		unsigned short unkNamesCount; // 656
		char __pad5[6];
		scr_string_t* unkNames; // 664
		char __pad6[16];
		int unk2Count;
		char __pad8[4];
		char* unk2;
		char __pad9[1];
		unsigned char unkVec3Count;
		char __pad10[6];
		vec3_t* unkVec3;
		char __pad11[1];
		char unk3Count;
		char __pad12[6];
		void* unk3;
	}; assert_sizeof(XModel, 0x2E0); // 736

	struct MayhemModel
	{
		XModel* model;
	};

	struct MayhemTransBounds
	{
		vec3_t mins;
		vec3_t size;
	};

	struct GfxWrappedBaseBuffer
	{
		ID3D11Resource* buffer;
		//ID3D11ShaderResourceView* view;
	};

	struct MayhemAnimFramesSplineCompressed
	{
		char* diskQuat;
		char* diskPos;
		unsigned __int16* diskQuatFrames;
		unsigned __int16* diskPosFrames;
		unsigned __int16* numDiskQuatFrames;
		unsigned __int16* numDiskPosFrames;
		MayhemTransBounds* transBounds;
		GfxWrappedBaseBuffer quat;
		GfxWrappedBaseBuffer pos;
		unsigned int quatStride;
		unsigned int posStride;
		unsigned int totalCompressedQuatFrames;
		unsigned int totalCompressedPosFrames;
	}; assert_sizeof(MayhemAnimFramesSplineCompressed, 0x58);

	struct MayhemAnimFramesUncompressed
	{
		char* diskQuat;
		char* diskPos;
		MayhemTransBounds* transBounds;
		GfxWrappedBaseBuffer quat;
		GfxWrappedBaseBuffer pos;
		unsigned int quatStride;
		unsigned int posStride;
	}; assert_sizeof(MayhemAnimFramesUncompressed, 0x30);

	union MayhemFramesUnion
	{
		void* data;
		MayhemAnimFramesSplineCompressed* splineCompressedFrames;
		MayhemAnimFramesUncompressed* uncompressedFrames;
	};

	struct MayhemDataKeysSplineCompressed
	{
		char* keys;
		unsigned __int16* numKeys;
		unsigned __int16* keyFrames;
		unsigned int totalCompressedKeyFrames;
		unsigned int numStreams;
		unsigned int keyStride;
	}; assert_sizeof(MayhemDataKeysSplineCompressed, 0x28);

	struct MayhemDataKeysUncompressed
	{
		char* keys;
		unsigned int numStreams;
		unsigned int keyStride;
	}; assert_sizeof(MayhemDataKeysUncompressed, 0x10);

	union MayhemDataChannelsUnion
	{
		void* data;
		MayhemDataKeysSplineCompressed* splineCompressedKeys;
		MayhemDataKeysUncompressed* uncompressedKeys;
	};

	struct MayhemAnim
	{
		MayhemFramesUnion frames;
		unsigned int numBones;
		unsigned int numFrames;
		float framerate;
		float frequency;
		unsigned int notifyCount;
		unsigned int dataChannelCount;
		bool isLooping;
		bool isSplineCompressed;
		bool quantizeQuats;
		bool quantizeTrans;
		bool quantizeChannels;
		XAnimNotifyInfo* notify;
		MayhemDataChannelsUnion dataChannels;
	}; assert_sizeof(MayhemAnim, 56);

	struct MayhemObject
	{
		unsigned int modelIndex;
		unsigned int animIndex;
	};

	struct MayhemData
	{
		const char* name;
		XModel* preModel;
		XModel* postModel;
		unsigned int numModels;
		MayhemModel* models;
		unsigned int numAnims;
		MayhemAnim* anims;
		unsigned int numObjects;
		MayhemObject* objects;
		float length;
	}; assert_sizeof(MayhemData, 0x50);

	enum MaterialTechniqueType
	{
		TECHNIQUE_UNLIT = 8,
		TECHNIQUE_EMISSIVE = 10,
		TECHNIQUE_LIT = 13,
	};

	struct Packed128
	{
		std::uint64_t p0;
		std::uint64_t p1;
	};

	struct GfxDrawSurfFields
	{

	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		Packed128 packed;
	};

	enum SurfaceTypeBits : std::uint64_t
	{

	};

	struct MaterialInfo
	{
		const char* name;
		unsigned char gameFlags;
		unsigned char sortKey;
		unsigned char textureAtlasRowCount;
		unsigned char textureAtlasColumnCount;
		unsigned char textureAtlasFrameBlend;
		unsigned char textureAtlasAsArray;
		unsigned char renderFlags;
		GfxDrawSurf drawSurf;
		SurfaceTypeBits surfaceTypeBits;
		unsigned int hashIndex;
	}; assert_sizeof(MaterialInfo, 48);

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned char samplerState;
		unsigned char semantic;
		GfxImage* image;
	}; assert_sizeof(MaterialTextureDef, 16);

	struct MaterialConstantDef
	{
		unsigned int nameHash;
		char name[12];
		float literal[4];
	}; assert_sizeof(MaterialConstantDef, 32);

	struct GfxStateBits
	{
		unsigned int loadBits[6]; // loadbits[3], blendstatebits[3]
		unsigned short zone;
		unsigned char depthStencilState[14];
		unsigned char blendState;
		unsigned char rasterizerState;
	}; assert_sizeof(GfxStateBits, 44);
	assert_offsetof(GfxStateBits, zone, 24);

	struct MaterialConstantBufferDef
	{
		unsigned int vsDataSize;
		unsigned int hsDataSize;
		unsigned int dsDataSize;
		unsigned int psDataSize;
		unsigned int vsOffsetDataSize;
		unsigned int hsOffsetDataSize;
		unsigned int dsOffsetDataSize;
		unsigned int psOffsetDataSize;
		unsigned char* vsData;
		unsigned char* hsData;
		unsigned char* dsData;
		unsigned char* psData;
		unsigned short* vsOffsetData;
		unsigned short* hsOffsetData;
		unsigned short* dsOffsetData;
		unsigned short* psOffsetData;
		ID3D11Buffer* vsConstantBuffer;
		ID3D11Buffer* hsConstantBuffer;
		ID3D11Buffer* dsConstantBuffer;
		ID3D11Buffer* psConstantBuffer;
	};

	struct Material
	{
		union
		{
			const char* name;
			MaterialInfo info;
		};
		unsigned char textureCount;
		unsigned char constantCount;
		unsigned char stateFlags;
		unsigned char cameraRegion;
		unsigned char materialType;
		unsigned char layerCount;
		unsigned char assetFlags;
		char pad;
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		MaterialConstantDef* constantTable;
		unsigned char stateBitsEntry[152];
		unsigned char stateBitsCount;
		GfxStateBits* stateBitsTable;
		unsigned char* constantBufferIndex; // [152]
		MaterialConstantBufferDef* constantBufferTable;
		unsigned char constantBufferCount;
		const char** subMaterials;
	}; assert_sizeof(Material, 0x118);
	assert_offsetof(Material, techniqueSet, 56);
	assert_offsetof(Material, textureCount, 48);
	assert_offsetof(Material, constantCount, 49);
	assert_offsetof(Material, stateBitsTable, 240);
	assert_offsetof(Material, constantBufferIndex, 248);
	assert_offsetof(Material, stateBitsCount, 232);
	assert_offsetof(Material, constantBufferTable, 256);
	assert_offsetof(Material, constantBufferCount, 264);
	assert_offsetof(Material, subMaterials, 272);
	assert_offsetof(Material, layerCount, 53);

	struct GfxComputeShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
	};

	struct ComputeShaderProgram
	{
		ID3D11ComputeShader* cs;
		GfxComputeShaderLoadDef loadDef;
	};

	struct ComputeShader
	{
		const char* name;
		const char* debugName;
		ComputeShaderProgram prog;
	}; assert_sizeof(ComputeShader, 0x28);

	struct GfxPixelShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		unsigned int microCodeCrc;
	};

	struct MaterialPixelShaderProgram
	{
		ID3D11PixelShader* ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char* name;
		const char* debugName;
		MaterialPixelShaderProgram prog;
	}; assert_sizeof(MaterialPixelShader, 0x28);

	struct GfxVertexShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
		unsigned int microCodeCrc;
	};

	struct MaterialVertexShaderProgram
	{
		ID3D11VertexShader* vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char* name;
		const char* debugName;
		MaterialVertexShaderProgram prog;
	}; assert_sizeof(MaterialVertexShader, 0x28);

	struct GfxHullShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
	};

	struct MaterialHullShaderProgram
	{
		ID3D11HullShader* hs;
		GfxHullShaderLoadDef loadDef;
	};

	struct MaterialHullShader
	{
		const char* name;
		const char* debugName;
		MaterialHullShaderProgram prog;
	}; assert_sizeof(MaterialHullShader, 0x28);

	struct GfxDomainShaderLoadDef
	{
		unsigned char* program;
		unsigned int programSize;
	};

	struct MaterialDomainShaderProgram
	{
		ID3D11DomainShader* ds;
		GfxDomainShaderLoadDef loadDef;
	};

	struct MaterialDomainShader
	{
		const char* name;
		const char* debugName;
		MaterialDomainShaderProgram prog;
	}; assert_sizeof(MaterialDomainShader, 0x28);

	struct MaterialStreamRouting
	{
		unsigned char source;
		unsigned char dest;
		unsigned char mask;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[34];
		ID3D11InputLayout* decl[270];
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		unsigned char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	}; assert_sizeof(MaterialVertexDeclaration, 0x8E8);

	struct MaterialArgumentCodeConst
	{
		unsigned short index;
		unsigned char firstRow;
		unsigned char rowCount;
	};

	union MaterialArgumentDef
	{
		const float* literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	enum MaterialShaderArgumentType : std::uint8_t
	{

	};

	struct MaterialShaderArgument
	{
		unsigned char type;
		unsigned char shader;
		unsigned short dest;
		MaterialArgumentDef u;
	}; assert_sizeof(MaterialShaderArgument, 0x10);

	struct MaterialPass
	{
		MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialHullShader* hullShader;
		MaterialDomainShader* domainShader;
		MaterialPixelShader* pixelShader;
		unsigned char pixelOutputMask;
		unsigned char perPrimArgCount;
		unsigned char perObjArgCount;
		unsigned char stableArgCount;
		unsigned short perPrimArgSize;
		unsigned short perObjArgSize;
		unsigned short stableArgSize;
		unsigned short zone;
		char __pad0[44];
		MaterialShaderArgument* args;
	}; assert_sizeof(MaterialPass, 104);
	assert_offsetof(MaterialPass, zone, 50);
	assert_offsetof(MaterialPass, args, 96);

	struct MaterialTechniqueHeader
	{
		const char* name;
		unsigned short flags;
		unsigned short passCount;
	}; assert_sizeof(MaterialTechniqueHeader, 0x10);

	struct MaterialTechnique
	{
		MaterialTechniqueHeader hdr;
		MaterialPass passArray[1];
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		unsigned short flags;
		unsigned char worldVertFormat;
		unsigned char preDisplacementOnlyCount;
		unsigned int techniqueCount;
		char __pad0[32];
		MaterialTechnique* techniques;
	}; assert_sizeof(MaterialTechniqueSet, 0x38);
	assert_offsetof(MaterialTechniqueSet, techniqueCount, 12);
	assert_offsetof(MaterialTechniqueSet, techniques, 48);

	enum MapType : std::uint8_t
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_1D = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_ARRAY = 0x6,
		MAPTYPE_CUBE_ARRAY = 0x7,
		MAPTYPE_COUNT = 0x8,
	};

	struct GfxImageStreamData
	{
		unsigned short width;
		unsigned short height;
		unsigned int pixelSize;
	};

	struct GfxImage
	{
		char __pad0[24];
		DXGI_FORMAT imageFormat; // 24
		char flags; // 28
		char __pad1[3];
		MapType mapType; // 32
		char sematic; // 33
		char category; // 34
		unsigned char platform[2]; // 35
		char __pad2[3];
		unsigned int dataLen1; // 40
		unsigned int dataLen2; // 44
		unsigned short width; // 48
		unsigned short height; // 50
		unsigned short depth; // 52
		unsigned short numElements; // 54
		unsigned char mipmapCount; // 56
		bool streamed; // 57
		char __pad3[6];
		char* pixelData; // 64
		GfxImageStreamData streams[4]; // 72
		const char* name; // 104
	}; assert_sizeof(GfxImage, 0x70);

	struct snd_volmod_info_t
	{
		char name[64];
		unsigned int id;
		float value;
	};

	struct SndGlobals
	{
		const char* name;
		float globalVolumeModifier;
		float binkVolumeModifier;
		unsigned int volmodinfoCount;
		snd_volmod_info_t* volmodinfo;
		char __pad0[176];
	}; assert_sizeof(SndGlobals, 0xD0);

	struct SndAlias
	{
		const char* aliasName;
		char __pad0[192];
	}; assert_sizeof(SndAlias, 200);

	struct SndAliasList
	{
		const char* aliasName;
		unsigned int id;
		SndAlias* head;
		int count;
		int sequence;
	}; assert_sizeof(SndAliasList, 32);
	assert_offsetof(SndAliasList, head, 16);
	assert_offsetof(SndAliasList, count, 24);

	struct SndIndexEntry
	{
		unsigned short value;
		unsigned short next;
	}; assert_sizeof(SndIndexEntry, 4);

	struct SndSendEffectParams
	{
		char __pad0[136];
	}; assert_sizeof(SndSendEffectParams, 136);

	struct SoundTable
	{
		char __pad0[248];
	}; assert_sizeof(SoundTable, 248);

	struct SndDuck
	{
		char __pad0[144];
	}; assert_sizeof(SndDuck, 144);

	struct SndMusicState
	{
		char __pad0[384];
	}; assert_sizeof(SndMusicState, 384);

	struct SndMusicSet
	{
		char name[64];
		unsigned int id;
		unsigned int stateCount;
		SndMusicState* states;
	}; assert_sizeof(SndMusicSet, 80);
	assert_offsetof(SndMusicSet, stateCount, 68);
	assert_offsetof(SndMusicSet, states, 72);

	struct SndBank
	{
		const char* name;
		const char* zone;
		const char* gameLanguage;
		const char* soundLanguage;
		unsigned int aliasCount;
		SndAliasList* alias;
		SndIndexEntry* aliasIndex;
		unsigned int sendEffectCount;
		SndSendEffectParams* sendEffects;
		SoundTable soundTable;
		unsigned int duckCount;
		SndDuck* ducks;
		unsigned int musicSetCount;
		SndMusicSet* musicSets;
	};
	assert_offsetof(SndBank, name, 0);
	assert_offsetof(SndBank, zone, 8);
	assert_offsetof(SndBank, gameLanguage, 16);
	assert_offsetof(SndBank, soundLanguage, 24);
	assert_offsetof(SndBank, aliasCount, 32);
	assert_offsetof(SndBank, aliasIndex, 48);
	assert_offsetof(SndBank, sendEffectCount, 56);
	assert_offsetof(SndBank, sendEffects, 64);
	assert_offsetof(SndBank, soundTable, 72);
	assert_offsetof(SndBank, duckCount, 320);
	assert_offsetof(SndBank, ducks, 328);
	assert_offsetof(SndBank, musicSetCount, 336);
	assert_offsetof(SndBank, musicSets, 344);

	struct SndBankResident
	{
		SndBank bank;
	}; assert_sizeof(SndBankResident, 0x160);

	struct SndBankTransient
	{
		const char* name;
		unsigned int elementCount;
		unsigned int* elements;
	}; assert_sizeof(SndBankTransient, 0x18);

	struct LocalizeEntry
	{
		const char* name;
		const char* value;
	};

	struct RawFile
	{
		const char* name;
		int compressedLen;
		int len;
		const char* buffer;
	};

	struct ScriptFile
	{
		const char* name;
		int compressedLen;
		int len;
		int bytecodeLen;
		const char* buffer;
		char* bytecode;
	};

	struct StringTableCell
	{
		const char* string;
		int hash;
	};

	struct StringTable
	{
		const char* name;
		int columnCount;
		int rowCount;
		StringTableCell* values;
	};

	enum NetConstStringType
	{
	};

	enum NetConstStringSource
	{
	};

	struct NetConstStrings
	{
		const char* name;
		NetConstStringType stringType;
		NetConstStringSource sourceType;
		unsigned int flags;
		unsigned int entryCount;
		const char** stringList;
	}; assert_sizeof(NetConstStrings, 0x20);

	struct LuaFile
	{
		const char* name;
		int len;
		char strippingType;
		const char* buffer;
	};

	struct TTFDef
	{
		const char* name;
		int fileLen;
		const char* file;
		void* ftFace;
	}; assert_sizeof(TTFDef, 0x20);

	union XAssetHeader
	{
		void* data;
		PhysicsLibrary* physicsLibrary;
		PhysicsSFXEventAsset* physicsSFXEventAsset;
		PhysicsVFXEventAsset* physicsVFXEventAsset;
		PhysicsAsset* physicsAsset;
		PhysicsFXPipeline* physicsFXPipeline;
		PhysicsFXShape* physicsFXShape;
		XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		MayhemData* mayhem;
		Material* material;
		//ComputeShader* computeShader;
		//MaterialVertexShader* vertexShader;
		//MaterialHullShader* hullShader;
		//MaterialDomainShader* domainShader;
		//MaterialPixelShader* pixelShader;
		//MaterialVertexDeclaration* vertexDecl;
		//MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		SndGlobals* soundGlobals;
		SndBankResident* soundBankResident;
		SndBankTransient* soundBankTransient;
		//clipMap_t* clipMap;
		//ComWorld* comWorld;
		//GlassWorld* glassWorld;
		//PathData* pathData;
		//NavMeshData* navMeshData;
		//MapEnts* mapEnts;
		//FxWorld* fxWorld;
		//GfxWorld* gfxWorld;
		//GfxWorldTransientZone* gfxWorldTransientZone;
		//GfxIESProfile* iesProfile;
		//GfxLightDef* lightDef;
		//void* uiMap;
		//AnimationClass* animClass;
		//PlayerAnimScript* playerAnim;
		//Gesture* gesture;
		LocalizeEntry* localize;
		//WeaponAttachment* attachment;
		//WeaponCompleteDef* weapon;
		//ParticleSystemDef* vfx;
		//FxImpactTable* impactFx;
		//SurfaceFxTable* surfaceFx;
		//void* aiType;
		//void* mpType;
		//void* character;
		//void* modelAlias;
		//void* unknown;
		RawFile* rawfile;
		ScriptFile* scriptfile;
		StringTable* stringTable;
		//LeaderboardDef* leaderboardDef;
		//VirtualLeaderboardDef* virtualLeaderboardDef;
		//DDLFile* ddlFile;
		//TracerDef* tracerDef;
		//VehicleDef* vehDef;
		//AddonMapEnts* addonMapEnts;
		NetConstStrings* netConstStrings;
		LuaFile* luaFile;
		//ScriptableDef* scriptable;
		//EquipmentSoundTable* equipSndTable;
		//VectorField* vectorField;
		//FxParticleSimAnimation* particleSimAnimation;
		//StreamingInfo* streamingInfo;
		//LaserDef* laserDef;
		TTFDef* ttfDef;
		//SuitDef* suitDef;
		//SuitAnimPackage* suitAnimPackage;
		//SpaceshipTargetDef* spaceshipTargetDef;
		//RumbleInfo* rumble;
		//RumbleGraph* rumbleGraph;
		//WeaponAnimPackage* weaponAnimPackage;
		//WeaponSFXPackage* weaponSFXPackage;
		//WeaponVFXPackage* weaponVFXPackage;
		//BehaviorTree* behaviorTree;
		//XAnimArcheType* archeType;
		//XAnimProceduralBones *proceduralBones;
		//ReticleDef* reticleDef;
		//GfxLightMap* lightMap;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XAssetEntry
	{
		XAssetHeader header;
		int nextHash;
		int nextOverride; // or nextPoolEntry
		char inuseMask;
		unsigned char type;
		unsigned short zoneIndex;
	}; assert_sizeof(XAssetEntry, 0x18);

	struct ScriptStringList
	{
		int count;
		const char** strings;
	};

	union GfxZoneTableEntry
	{
		char* dataPtr;
		void* voidPtr;
		ID3D11Buffer* buffer;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11BlendState* blendState;
	};

	typedef std::uint32_t GfxBlendStateBits[3];

	struct XGfxGlobals
	{
		unsigned int depthStencilStateCount;
		unsigned int blendStateCount;
		std::uint64_t* depthStencilStateBits;
		GfxBlendStateBits* blendStateBits;
		GfxZoneTableEntry* depthStencilStates;
		GfxZoneTableEntry* blendStates;
		unsigned int perPrimConstantBufferCount;
		unsigned int perObjConstantBufferCount;
		unsigned int stableConstantBufferCount;
		unsigned int* perPrimConstantBufferSizes;
		unsigned int* perObjConstantBufferSizes;
		unsigned int* stableConstantBufferSizes;
		GfxZoneTableEntry* perPrimConstantBuffers;
		GfxZoneTableEntry* perObjConstantBuffers;
		GfxZoneTableEntry* stableConstantBuffers;
	};

	struct XGlobals
	{
		XGfxGlobals* gfxGlobals;
	};

	struct XAssetList
	{
		ScriptStringList stringList;
		int assetCount;
		XAsset* assets;
		XGlobals* globals;
	};

	enum DBSyncMode : std::int8_t
	{
		DB_LOAD_SYNC = 0x0,
		DB_LOAD_ASYNC = 0x1,
	};

	enum DBAllocFlags : std::int32_t
	{
		DB_ZONE_NONE = 0x0,
		DB_ZONE_COMMON = 0x1,
		DB_ZONE_GAME = 0x4, // maybe
		DB_ZONE_LOAD = 0x100,
		DB_ZONE_CUSTOM = 0x1000000 // added for custom zone loading
	};

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
	};

	struct DBFile
	{
		char name[64];
		void* handle;
		char __pad0[46];
		bool isSecure;
		char __pad1[49];
	}; assert_sizeof(DBFile, 0xA8);
	assert_offsetof(DBFile, isSecure, 118);

	struct db_internal_state
	{
		int dummy;
	};

	struct db_z_stream_s
	{
		unsigned char* next_in;
		unsigned int avail_in;
		unsigned int total_in;
		unsigned char* next_out;
		unsigned int avail_out;
		unsigned int total_out;
		char* msg;
		db_internal_state* state;
		unsigned __int8* (__fastcall* zalloc)(unsigned __int8*, unsigned int, unsigned int);
		void(__fastcall* zfree)(unsigned __int8*, unsigned __int8*);
		unsigned __int8* opaque;
		int data_type;
		unsigned long adler;
		unsigned long reserved;
	};

	enum DB_CompressorType
	{
		DB_COMPRESSOR_INVALID = 0x0,
		DB_COMPRESSOR_PASSTHROUGH = 0x1,
		DB_COMPRESSOR_BLOCK = 0x2,
	};

	struct DB_ReadStream
	{
		unsigned char* next_in;
		unsigned __int64 avail_in;
		unsigned __int64 total_in;
		unsigned char* next_out;
		unsigned __int64 avail_out;
		unsigned __int64 total_out;
		DB_CompressorType compressor;
	};

	struct DB_LoadData
	{
		DBFile* file;
		unsigned __int8* fileBuffer;
		unsigned __int64 readSize;
		unsigned __int64 completedReadSize;
		unsigned __int64 offset;
		unsigned __int8* start_in;
		DB_ReadStream stream;
	};

	enum XFileBlock
	{
		XFILE_BLOCK_TEMP = 0x0,
		XFILE_BLOCK_UNK1 = 0x1,
		XFILE_BLOCK_UNK2 = 0x2,
		XFILE_BLOCK_IMAGE_STREAM = 0x3,
		XFILE_BLOCK_SHARED_STREAM = 0x4,
		XFILE_BLOCK_UNK5 = 0x5,
		XFILE_BLOCK_RUNTIME = 0x6,
		XFILE_BLOCK_UNK7 = 0x7,
		XFILE_BLOCK_VIRTUAL = 0x8,
		XFILE_BLOCK_SCRIPT = 0x9,
		MAX_XFILE_COUNT = 0xA,
	};

	struct XBlock
	{
		char* alloc;
		unsigned __int64 size;
	};

	struct XZoneMemory
	{
		XBlock blocks[MAX_XFILE_COUNT];
		char __pad0[112];
		void* shared_ff_data;
		unsigned int shared_ff_count;
		int padding1;
		void* unknown; // always 0
		void* image_ff_data;
		unsigned int image_ff_count;
		int padding2;
		char __pad1[0x100]; // unk size
	};

	struct XFileSignedInfo // sub_1409E6100
	{
		char magic[8]; // IWffs100
		std::uint32_t unk;
		char key[32];
		char sha[256];
		char name[32];
		char pad[4];
	};

	struct XFileStreamData
	{
		std::uint64_t size;
		std::uint64_t unk1;
		std::uint64_t unk2;
		std::uint64_t block_size[MAX_XFILE_COUNT];
		std::uint64_t unk_arr[8];
	}; assert_sizeof(XFileStreamData, 168);

	struct XFileHeader
	{
		char header[8];
		std::uint32_t version;
		std::uint8_t unused; // (unused)
		std::uint8_t has_no_image_fastfile;
		std::uint8_t has_no_shared_fastfile;
		std::uint8_t unk1;
		std::uint32_t fileTimeHigh; // (unused)
		std::uint32_t fileTimeLow; // (unused)
		XFileStreamData stream_data;
		std::uint32_t shared_ff_hash; // shared fastfile // (unused)
		std::uint32_t shared_ff_count; // shared fastfile // (unused)
		std::uint32_t image_ff_hash; // image fastfile // (unused)
		std::uint32_t image_ff_count; // image fastfile // (unused)
		// image streams 24 * sharedcount
		// image streams 24 * imagecount
		std::uint64_t fileLen;
		std::uint64_t fileLenUnk1;
		std::uint64_t fileLenUnk2;
		// if signed: XFileSignedInfo info;
	};

	struct XFileReadData
	{
		bool header_parsed;
		std::uint8_t has_no_image_fastfile;
		std::uint8_t has_no_shared_fastfile;
		std::uint8_t unk1;
		char __pad0[4];
		XFileStreamData stream_data;
		std::uint32_t pad;
		std::uint32_t shared_ff_hash;
		std::uint32_t shared_ff_count;
		std::uint64_t fileLenUnk1;
		std::uint32_t image_ff_hash;
		std::uint32_t image_ff_count;
		std::uint64_t fileLen;
		std::uint64_t fileLenUnk2;
	}; assert_sizeof(XFileReadData, 224);
}