#pragma once
#include <d3d11.h>

namespace database
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

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
	struct MayhemData;
	struct Material;
	struct ComputeShader;
	struct MaterialVertexShader;
	struct MaterialHullShader;
	struct MaterialDomainShader;
	struct MaterialPixelShader;
	struct MaterialVertexDeclaration;
	struct MaterialTechniqueSet;
	struct GfxImage;
	struct SndGlobals;
	struct SndBank;
	struct SndBankTransient;
	struct clipMap_t;
	struct ComWorld;
	struct GlassWorld;
	struct PathData;
	struct NavMeshData;
	struct MapEnts;
	struct FxWorld;
	struct GfxWorld;
	struct GfxWorldTransientZone;
	//ASSET_TYPE_IESPROFILE = 31, // unused
	struct GfxLightDef;
	//ASSET_TYPE_UI_MAP = 33, // unused
	struct AnimationClass;
	struct PlayerAnimScript;
	struct Gesture;
	struct LocalizeEntry;
	struct WeaponAttachment;
	struct WeaponCompleteDef;
	struct ParticleSystemDef;
	struct FxEffectDef;
	struct FxImpactTable;
	struct SurfaceFxTable;
	//ASSET_TYPE_AITYPE = 44, // unused
	//ASSET_TYPE_MPTYPE = 45, // unused
	//ASSET_TYPE_CHARACTER = 46, // unused
	//ASSET_TYPE_XMODELALIAS = 47, // unused
	struct RawFile;
	struct ScriptFile;
	struct StringTable;
	struct LeaderBoardDef;
	struct VirtualLeaderboardDef;
	struct StructuredDataDefSet;
	struct DDLFile;
	struct TracerDef;
	struct VehicleDef;
	struct AddonMapEnts;
	struct NetConstStrings;
	struct LuaFile;
	struct ScriptableDef;
	struct EquipmentSoundTable;
	struct VectorField;
	struct FxParticleSimAnimation;
	struct StreamingInfo;
	struct LaserDef;
	struct TTFDef;
	struct SuitDef;
	struct SuitAnimPackage;
	struct SpaceshipTargetDef;
	struct RumbleInfo;
	struct RumbleGraph;
	struct WeaponAnimPackage;
	struct WeaponSFXPackage;
	struct WeaponVFXPackage;
	struct BehaviorTree;
	struct XAnimArcheType;
	struct XAnimProceduralBones;
	struct ReticleDef;
	struct GfxLightMap;

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct ExtentBounds
	{
		vec3_t mins;
		vec3_t maxs;
	};

	union FxCombinedUnion
	{
		FxEffectDef* fx;
		ParticleSystemDef* vfx;
		void* data;
	};

	enum FxCombinedType : std::uint8_t
	{
		FX_COMBINED_FX,
		FX_COMBINED_VFX,
	};

	struct FxCombinedDef
	{
		FxCombinedUnion u;
		FxCombinedType type;
	};

	typedef const char* SndAliasLookup;

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

	struct PhysicsAssetUsageCounter
	{
		int serverEnt;
		int clientEnt;
		int dynEnt;
	};

	struct PhysicsAsset
	{
		const char* name;
		unsigned int havokDataSize;
		char* havokData;
		int numRigidBodies;
		int numConstraints;
		int numSFXEventAssets;
		PhysicsSFXEventAsset** sfxEventAssets;
		int numVFXEventAssets;
		PhysicsVFXEventAsset** vfxEventAssets;
		PhysicsAssetUsageCounter physicsUsageCounter;
		char __pad0[4]; // unused
	}; assert_sizeof(PhysicsAsset, 0x50);

	struct unk_1453E1188
	{
		char __pad0[68];
	}; assert_sizeof(unk_1453E1188, 68);

	struct unk_1453E1068
	{
		int numUnk;
		unk_1453E1188* unk;
	}; assert_sizeof(unk_1453E1068, 16);

	struct PhysicsFXPipeline
	{
		const char* name;
		char __pad0[8];
		unk_1453E1068 unk;
	}; assert_sizeof(PhysicsFXPipeline, 0x20);

	struct PhysicsFXShape
	{
		const char* name;
		unsigned int size;
		vec3_t* vecs;
	}; assert_sizeof(PhysicsFXShape, 0x18);

	union XAnimIndices
	{
		unsigned char* _1;
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
		unsigned char(*_1)[3];
		unsigned short(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		unsigned char _1[1];
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
		unsigned char smallTrans;
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
		unsigned char* dataByte; // 16
		short* dataShort; // 24
		int* dataInt; // 32
		short* randomDataShort; // 40
		unsigned char* randomDataByte; // 48
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
		unsigned char flags; // 118
		unsigned char boneCount[10]; // 119
		unsigned char notifyCount; // 129
		unsigned char assetType; // 130
		unsigned char unk1; // 131
		unsigned char unk2; // 132
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
		unsigned short boneOffsetIndex;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	union PackedUnitVec
	{
		unsigned int packed;
		unsigned char array[4];
	};

	union PackedTexCoords
	{
		unsigned int packed;
		unsigned char array[4];
	};

	union GfxColor
	{
		unsigned char array[4];
		unsigned int packed;
	};

	struct PackedSelfVisibility
	{
		unsigned int packed;
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		PackedSelfVisibility selfVisibility;
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
		unsigned int flags;
		char __pad0[20];
		GfxSubdivCache cache;
	}; assert_sizeof(XSurfaceSubdivInfo, 0x38);

	struct SHProbeSimplexData2
	{
		char __pad0[24];
	};

	struct SHProbeSimplexData1
	{
		char __pad0[8];
	};

	struct SHProbeSimplexData0
	{
		SHProbeSimplexData2 a; // coulbereversed?
		SHProbeSimplexData1 b; // ^
	};

	union SHProbeSimplexDataUnion
	{
		void* data;
		SHProbeSimplexData0* data0;
		SHProbeSimplexData1* data1;
		SHProbeSimplexData2* data2;
	};

	typedef unsigned short XBlendInfo;

	enum XSurfaceFlagBits
	{
		SURF_FLAG_BIT_VERTCOL_GREY = 0x0,
		SURF_FLAG_BIT_VERTCOL_NONE = 0x1,
		SURF_FLAG_BIT_SKINNED = 0x2,
		SURF_FLAG_BIT_REACTIVE_MOTION = 0x3,
		SURF_FLAG_BIT_LIGHTMAP_COORDS = 0x4,
		SURF_FLAG_BIT_SUBDIV = 0x5,
		SURF_FLAG_BIT_TENSION = 0x6,
		SURF_FLAG_BIT_SELF_VISIBILITY = 0x7,
		SURF_FLAG_BIT_SECONDUV = 0x8,
		SURF_FLAG_BIT_MAYHEM_CUSTOM_CHANNELS = 0x9,
		SURF_FLAG_BIT_MAYHEM_SELFVIS = 0xA,
		SURF_FLAG_BIT_COUNT = 0xB,
	};

	enum XSurfaceFlags
	{
		SURF_FLAG_NONE = 0x0,
		SURF_FLAG_VERTCOL_GREY = 0x1,
		SURF_FLAG_VERTCOL_NONE = 0x2,
		SURF_FLAG_SKINNED = 0x4,
		SURF_FLAG_REACTIVE_MOTION = 0x8,
		SURF_FLAG_LIGHTMAP_COORDS = 0x10,
		SURF_FLAG_SUBDIV = 0x20,
		SURF_FLAG_TENSION = 0x40,
		SURF_FLAG_SELF_VISIBILITY = 0x80,
		SURF_FLAG_SECONDUV = 0x100,
		SURF_FLAG_MAYHEM_CUSTOM_CHANNELS = 0x200,
		SURF_FLAG_MAYHEM_SELFVIS = 0x400,
	};

	struct XSurface
	{
		unsigned short flags;
		unsigned short vertCount;
		unsigned short triCount;
		unsigned char rigidVertListCount;
		unsigned char subdivLevelCount;
		unsigned short blendVertCounts[8];
		unsigned int blendVertSize;
		char __pad0[4];
		GfxVertexUnion0 verts0;
		Face* triIndices;
		ID3D11Buffer* vb0;
		ID3D11ShaderResourceView* vb0View;
		ID3D11Buffer* indexBuffer;
		XRigidVertList* rigidVertLists;
		XBlendInfo* blendVerts;
		ID3D11Buffer* blendVertsBuffer;
		ID3D11ShaderResourceView* blendVertsView;
		float(*lmapUnwrap)[2];
		ID3D11Buffer* vblmapBuffer;
		ID3D11ShaderResourceView* vblmapView;
		XSurfaceSubdivInfo* subdiv;
		float* tensionData;
		unsigned short* tensionAccumTable;
		ID3D11Buffer* tensionAccumTableBuffer;
		ID3D11ShaderResourceView* tensionAccumTableView;
		ID3D11Buffer* tensionDataBuffer;
		ID3D11ShaderResourceView* tensionDataView;
		ID3D11ShaderResourceView* indexBufferView;
		int unk; // vertexLightingIndex
		int pad;
		SHProbeSimplexDataUnion shProbeSimplexVertData;
		ID3D11Buffer* shProbeSimplexVertBuffer;
		ID3D11ShaderResourceView* shProbeSimplexVertBufferView;
		int partBits[8];
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

	enum XModelLodInfoFlags
	{
		XMODEL_LODINFO_FLAG_NONE = 0x0,
		XMODEL_LODINFO_FLAG_SUBDIV = 0x1,
		XMODEL_LODINFO_FLAG_SUBDIV_NON_ADAPTIVE = 0x2,
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned short numsurfs;
		unsigned short surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[8];
		XSurface* surfs;
		int subdivLodValidMask;
		char flags;
	}; assert_sizeof(XModelLodInfo, 0x40);

	enum SurfaceFlags : std::uint32_t
	{
		SURFACE_FLAG_NONE = 0x00000000,
		SURFACE_FLAG_BARK = 0x00080000,
		SURFACE_FLAG_BRICK = 0x00100000,
		SURFACE_FLAG_CARPET_SOLID = 0x00180000,
		SURFACE_FLAG_CLOTH = 0x00200000,
		SURFACE_FLAG_CONCRETE_DRY = 0x00280000,
		SURFACE_FLAG_DIRT = 0x00300000,
		SURFACE_FLAG_FLESH = 0x00380000,
		SURFACE_FLAG_FOLIAGE_DRY = 0x00400000,
		SURFACE_FLAG_GLASS_PANE = 0x00480000,
		SURFACE_FLAG_GRASS_SHORT = 0x00500000,
		SURFACE_FLAG_GRAVEL = 0x00580000,
		SURFACE_FLAG_ICE_SOLID = 0x00600000,
		SURFACE_FLAG_METAL_THICK = 0x00680000,
		SURFACE_FLAG_METAL_GRATE = 0x00700000,
		SURFACE_FLAG_MUD = 0x00780000,
		SURFACE_FLAG_PAPER = 0x00800000,
		SURFACE_FLAG_PLASTER = 0x00880000,
		SURFACE_FLAG_ROCK = 0x00900000,
		SURFACE_FLAG_SAND = 0x00980000,
		SURFACE_FLAG_SNOW = 0x00A00000,
		SURFACE_FLAG_WATER = 0x00A80000,
		SURFACE_FLAG_WOOD_SOLID = 0x00B00000,
		SURFACE_FLAG_ASPHALT_DRY = 0x00B80000,
		SURFACE_FLAG_CERAMIC = 0x00C00000,
		SURFACE_FLAG_PLASTIC = 0x00C80000,
		SURFACE_FLAG_RUBBER = 0x00D00000,
		SURFACE_FLAG_FRUIT = 0x00D80000,
		SURFACE_FLAG_METAL_PAINTED = 0x00E00000,
		SURFACE_FLAG_RIOTSHIELD = 0x00E80000,
		SURFACE_FLAG_SLUSH = 0x00F00000,
		SURFACE_FLAG_ASPHALT_WET = 0x00F80000,
		SURFACE_FLAG_CARPET_WOOD = 0x01000000,
		SURFACE_FLAG_CARPET_METAL = 0x01080000,
		SURFACE_FLAG_CONCRETE_WET = 0x01100000,
		SURFACE_FLAG_CUSHION = 0x01180000,
		SURFACE_FLAG_DEFAULT = 0x01200000,
		SURFACE_FLAG_FOLIAGE_WET = 0x01280000,
		SURFACE_FLAG_GLASS_BROKEN = 0x01300000,
		SURFACE_FLAG_GLASS_SOLID = 0x01380000,
		SURFACE_FLAG_GLASS_VEHICLE = 0x01400000,
		SURFACE_FLAG_GRASS_TALL = 0x01480000,
		SURFACE_FLAG_ICE_THIN = 0x01500000,
		SURFACE_FLAG_METAL_THIN = 0x01580000,
		SURFACE_FLAG_ROBOT_HEAD = 0x01600000,
		SURFACE_FLAG_ROBOT_TORSO = 0x01680000,
		SURFACE_FLAG_ROBOT_LIMB = 0x01700000,
		SURFACE_FLAG_ROBOT_ARMOR = 0x01780000,
		SURFACE_FLAG_BIG_ROBOT_HEAD = 0x01800000,
		SURFACE_FLAG_BIG_ROBOT_LIMB = 0x01880000,
		SURFACE_FLAG_WATER_KNEE = 0x01900000,
		SURFACE_FLAG_WATER_WAIST = 0x01980000,
		SURFACE_FLAG_WOOD_FLOOR = 0x01A00000,
		SURFACE_FLAG_USER_TERRAIN_1 = 0x01A80000,
		SURFACE_FLAG_USER_TERRAIN_2 = 0x01B00000,
		SURFACE_FLAG_USER_TERRAIN_3 = 0x01B80000,
		SURFACE_FLAG_USER_TERRAIN_4 = 0x01C00000,
		SURFACE_FLAG_USER_TERRAIN_5 = 0x01C80000,
		SURFACE_FLAG_USER_TERRAIN_6 = 0x01D00000,
		SURFACE_FLAG_USER_TERRAIN_7 = 0x01D80000,
		SURFACE_FLAG_USER_TERRAIN_8 = 0x01E00000,
		SURFACE_FLAG_USER_TERRAIN_9 = 0x01E80000,
		SURFACE_FLAG_USER_TERRAIN_10 = 0x01F00000,
		SURFACE_FLAG_CODE_RESERVED = 0x01F80000,
		SURFACE_FLAG_OPAQUEGLASS = 0x01380000,
		SURFACE_FLAG_CLIPMISSILE = 0x00000000,
		SURFACE_FLAG_AI_NOSIGHT = 0x00000000,
		SURFACE_FLAG_CLIPSHOT = 0x00000000,
		SURFACE_FLAG_PLAYERCLIP = 0x00000000,
		SURFACE_FLAG_MONSTERCLIP = 0x00000000,
		SURFACE_FLAG_AICLIPALLOWDEATH = 0x00000000,
		SURFACE_FLAG_VEHICLECLIP = 0x00000000,
		SURFACE_FLAG_ITEMCLIP = 0x00000000,
		SURFACE_FLAG_NODROP = 0x00000000,
		SURFACE_FLAG_NONSOLID = 0x00004000,
		SURFACE_FLAG_DETAIL = 0x00000000,
		SURFACE_FLAG_STRUCTURAL = 0x00000000,
		SURFACE_FLAG_PORTAL = 0x80000000,
		SURFACE_FLAG_CANSHOOTCLIP = 0x00000000,
		SURFACE_FLAG_ORIGIN = 0x00000000,
		SURFACE_FLAG_SKY = 0x00000004,
		SURFACE_FLAG_NOCASTSHADOW = 0x00040000,
		SURFACE_FLAG_PHYSICSGEOM = 0x00000000,
		SURFACE_FLAG_LIGHTPORTAL = 0x00000000,
		SURFACE_FLAG_OUTDOORBOUNDS = 0x00000000,
		SURFACE_FLAG_SLICK = 0x00000002,
		SURFACE_FLAG_NOIMPACT = 0x00000010,
		SURFACE_FLAG_NOMARKS = 0x00000020,
		SURFACE_FLAG_NOPENETRATE = 0x00000100,
		SURFACE_FLAG_LADDER = 0x00000008,
		SURFACE_FLAG_NODAMAGE = 0x00000001,
		SURFACE_FLAG_MANTLEON = 0x02000000,
		SURFACE_FLAG_MANTLEOVER = 0x04000000,
		SURFACE_FLAG_STAIRS = 0x00000200,
		SURFACE_FLAG_SOFT = 0x00001000,
		SURFACE_FLAG_NOSTEPS = 0x00002000,
		SURFACE_FLAG_NODRAW = 0x00000080,
		SURFACE_FLAG_NOLIGHTMAP = 0x00000400,
		SURFACE_FLAG_NODLIGHT = 0x00020000,
		SURFACE_FLAG_NAVINCLUSIONVOLUME = 0x00000000,
		SURFACE_FLAG_NAVEXCLUSIONVOLUME = 0x00000000,
		SURFACE_FLAG_NAV3DVOLUME = 0x00000000,
	};

	enum SurfaceTypeBits : std::uint64_t
	{
		SURFACE_TYPE_BIT_NONE = 0x0,
		SURFACE_TYPE_BIT_BARK = 0x1,
		SURFACE_TYPE_BIT_BRICK = 0x2,
		SURFACE_TYPE_BIT_CARPET_SOLID = 0x4,
		SURFACE_TYPE_BIT_CLOTH = 0x8,
		SURFACE_TYPE_BIT_CONCRETE_DRY = 0x10,
		SURFACE_TYPE_BIT_DIRT = 0x20,
		SURFACE_TYPE_BIT_FLESH = 0x40,
		SURFACE_TYPE_BIT_FOLIAGE_DRY = 0x80,
		SURFACE_TYPE_BIT_GLASS_PANE = 0x100,
		SURFACE_TYPE_BIT_GRASS_SHORT = 0x200,
		SURFACE_TYPE_BIT_GRAVEL = 0x400,
		SURFACE_TYPE_BIT_ICE_SOLID = 0x800,
		SURFACE_TYPE_BIT_METAL_THICK = 0x1000,
		SURFACE_TYPE_BIT_METAL_GRATE = 0x2000,
		SURFACE_TYPE_BIT_MUD = 0x4000,
		SURFACE_TYPE_BIT_PAPER = 0x8000,
		SURFACE_TYPE_BIT_PLASTER = 0x10000,
		SURFACE_TYPE_BIT_ROCK = 0x20000,
		SURFACE_TYPE_BIT_SAND = 0x40000,
		SURFACE_TYPE_BIT_SNOW = 0x80000,
		SURFACE_TYPE_BIT_WATER = 0x100000,
		SURFACE_TYPE_BIT_WOOD_SOLID = 0x200000,
		SURFACE_TYPE_BIT_ASPHALT_DRY = 0x400000,
		SURFACE_TYPE_BIT_CERAMIC = 0x800000,
		SURFACE_TYPE_BIT_PLASTIC = 0x1000000,
		SURFACE_TYPE_BIT_RUBBER = 0x2000000,
		SURFACE_TYPE_BIT_FRUIT = 0x4000000,
		SURFACE_TYPE_BIT_METAL_PAINTED = 0x8000000,
		SURFACE_TYPE_BIT_RIOTSHIELD = 0x10000000,
		SURFACE_TYPE_BIT_SLUSH = 0x20000000,
		SURFACE_TYPE_BIT_ASPHALT_WET = 0x40000000,
		SURFACE_TYPE_BIT_CARPET_WOOD = 0x80000000,
		SURFACE_TYPE_BIT_CARPET_METAL = 0x100000000,
		SURFACE_TYPE_BIT_CONCRETE_WET = 0x200000000,
		SURFACE_TYPE_BIT_CUSHION = 0x400000000,
		SURFACE_TYPE_BIT_DEFAULT = 0x800000000,
		SURFACE_TYPE_BIT_FOLIAGE_WET = 0x1000000000,
		SURFACE_TYPE_BIT_GLASS_BROKEN = 0x2000000000,
		SURFACE_TYPE_BIT_GLASS_SOLID = 0x4000000000,
		SURFACE_TYPE_BIT_GLASS_VEHICLE = 0x8000000000,
		SURFACE_TYPE_BIT_GRASS_TALL = 0x10000000000,
		SURFACE_TYPE_BIT_ICE_THIN = 0x20000000000,
		SURFACE_TYPE_BIT_METAL_THIN = 0x40000000000,
		SURFACE_TYPE_BIT_ROBOT_HEAD = 0x80000000000,
		SURFACE_TYPE_BIT_ROBOT_TORSO = 0x100000000000,
		SURFACE_TYPE_BIT_ROBOT_LIMB = 0x200000000000,
		SURFACE_TYPE_BIT_ROBOT_ARMOR = 0x400000000000,
		SURFACE_TYPE_BIT_BIG_ROBOT_HEAD = 0x800000000000,
		SURFACE_TYPE_BIT_BIG_ROBOT_LIMB = 0x1000000000000,
		SURFACE_TYPE_BIT_WATER_KNEE = 0x2000000000000,
		SURFACE_TYPE_BIT_WATER_WAIST = 0x4000000000000,
		SURFACE_TYPE_BIT_WOOD_FLOOR = 0x8000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_1 = 0x10000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_2 = 0x20000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_3 = 0x40000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_4 = 0x80000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_5 = 0x100000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_6 = 0x200000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_7 = 0x400000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_8 = 0x800000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_9 = 0x1000000000000000,
		SURFACE_TYPE_BIT_USER_TERRAIN_10 = 0x2000000000000000,
		SURFACE_TYPE_BIT_CODE_RESERVED = 0x4000000000000000,
		SURFACE_TYPE_BIT_OPAQUEGLASS = 0x8000000000000000,
	};

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

	struct unk_1453E14D8
	{
		Bounds bounds;
	};

	enum XModelCharCollBoundsType : std::uint8_t
	{
		CharCollBoundsType_None = 0x0,
		CharCollBoundsType_Human = 0x1,
		CharCollBoundsType_C12 = 0x2,
		CharCollBoundsType_C8 = 0x3,
		CharCollBoundsType_Seeker = 0x4,
		CharCollBoundsType_Zombie = 0x5,
		CharCollBoundsType_LargeZombie = 0x6,
		CharCollBoundsType_Alien = 0x7,
		CharCollBoundsType_LargeAlien = 0x8,
		CharCollBoundsType_ExtraLargeZombie = 0x9,
		CharCollBoundsType_Num = 0xA,
	};

	enum XModelFlags
	{
		XMODEL_FLAG_NONE = 0x0,
		XMODEL_FLAG_ANIMATED_VERTS = 0x1,
		XMODEL_FLAG_REACTIVE_MOTION = 0x8,
		XMODEL_FLAG_COMPOSITE = 0x400,
		XMODEL_FLAG_HAS_DEFAULT_MODEL = 0x8000,
		XMODEL_FLAG_HAS_ANY_DEFAULT_SURFS = 0x10000,
		XMODEL_FLAG_HAS_ALL_DEFAULT_SURFS = 0x20000,
		XMODEL_FLAG_HAS_RIGID_VERT_LIST = 0x80000,
		XMODEL_FLAG_HAS_MAYHEM_SELFVIS = 0x8000000,
	};

	struct XModelPhysicsUsageCounter
	{
		int serverEnt;
		int clientEnt;
		int dynEnt;
	};

	struct XModel
	{
		const char* name;
		char unused01;
		bool hasLods;
		unsigned char maxLoadedLod;
		unsigned char numLods;
		unsigned char collLod;
		unsigned char shadowCutoffLod;
		unsigned char characterCollBoundsType;
		int flags;
		unsigned char numBones;
		unsigned char numRootBones;
		unsigned char numReactiveMotionParts;
		unsigned short numClientBones;
		unsigned short numsurfs;
		float scale;
		unsigned short numCollSurfs;
		int contents;
		float radius;
		Bounds bounds;
		int memUsage;
		unsigned int physicsLODDataSize;
		XModelPhysicsUsageCounter physicsUsageCounter;
		ScriptableDef* scriptableMoverDef;
		XAnimProceduralBones* proceduralBones;
		scr_string_t* aimAssistBones;
		unsigned char numAimAssistBones;
		float edgeLength;
		unsigned int noScalePartBits[8];
		scr_string_t* boneNames;
		unsigned char* parentList;
		XModelAngle* tagAngles;
		XModelTagPos* tagPositions;
		unsigned char* partClassification;
		DObjAnimMat* baseMat;
		ReactiveMotionModelPart* reactiveMotionParts;
		ReactiveMotionModelTweaks* reactiveMotionTweaks;
		Material** materialHandles;
		XModelLodInfo lodInfo[6];
		XModelCollSurf_s* collSurfs;
		XBoneInfo* boneInfo;
		unsigned short* invHighMipRadius;
		PhysicsAsset* physicsAsset;
		PhysicsFXShape* physicsFXShape;
		char* physicsLODData;
		unsigned int physicsLODDataNameCount;
		scr_string_t* physicsLODDataNames;
		unsigned char impactType;
		char unk_03[15]; // unknown data
		unsigned char unknown03Count;
		char* unknown03;
		unsigned char unknownIndex;
		unsigned char unknownVec3Count;
		char unk_04[6]; // unknown data
		vec3_t* unknownVec3;
		unsigned char unknownIndex2;
		unsigned char unknown04Count;
		char unk_05[6]; // unknown data
		unk_1453E14D8* unknown04;
	}; assert_sizeof(XModel, 0x2E0); // 736
	assert_offsetof(XModel, lodInfo, 224);

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
		unsigned short* diskQuatFrames;
		unsigned short* diskPosFrames;
		unsigned short* numDiskQuatFrames;
		unsigned short* numDiskPosFrames;
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
		unsigned short* numKeys;
		unsigned short* keyFrames;
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
		TECHNIQUE_DEPTH_PREPASS = 0,
		TECHNIQUE_DEPTH_PREPASS_VELOCITY_RIGID = 1,
		TECHNIQUE_DEPTH_PREPASS_VELOCITY_SKINNED = 2,
		TECHNIQUE_DEPTH_PREPASS_OBJECTID = 3,
		TECHNIQUE_DEPTH_PREPASS_OBJECTID_VELOCITY_RIGID = 1,
		TECHNIQUE_DEPTH_PREPASS_OBJECTID_VELOCITY_SKINNED = 2,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 6,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 7,
		TECHNIQUE_UNLIT = 8,
		TECHNIQUE_UNLIT_UNK = 9,
		TECHNIQUE_EMISSIVE = 10,
		TECHNIQUE_EMISSIVE_UNK = 11,
		TECHNIQUE_EMISSIVE_DEBUG = 12,
		TECHNIQUE_LIT = 13,
		//TECHNIQUE_LIT_ = 14
		//TECHNIQUE_LIT_ = 15
		//TECHNIQUE_LIT_ = 16
		//TECHNIQUE_LIT_ = 17
		//TECHNIQUE_LIT_ = 18
		//TECHNIQUE_LIT_ = 19
		//TECHNIQUE_LIT_ = 20
		//TECHNIQUE_LIT_ = 21
		//TECHNIQUE_LIT_ = 22
		//TECHNIQUE_LIT_ = 23
		//TECHNIQUE_LIT_ = 24
		//TECHNIQUE_LIT_ = 25
		//TECHNIQUE_LIT_ = 26
		TECHNIQUE_THERMAL = 27,
		TECHNIQUE_VELOCITY_RIGID = 28,
		TECHNIQUE_VELOCITY_SKINNED = 29,
		TECHNIQUE_DEBUG_BUMPMAP = 30,
		// 31
		// 32
		// 33
		// 34
		// 35
		// 36
		// 37
		TECHNIQUE_INSTANCED_DEPTH_PREPASS = 38,
		TECHNIQUE_INSTANCED_DEPTH_PREPASS_VELOCITY_RIGID = 39,
		TECHNIQUE_INSTANCED_DEPTH_PREPASS_VELOCITY_SKINNED = 40,
		TECHNIQUE_INSTANCED_DEPTH_PREPASS_OBJECTID = 41,
		TECHNIQUE_INSTANCED_DEPTH_PREPASS_OBJECTID_VELOCITY_RIGID = 42,
		TECHNIQUE_INSTANCED_DEPTH_PREPASS_OBJECTID_VELOCITY_SKINNED = 43,
		TECHNIQUE_INSTANCED_BUILD_SHADOWMAP_DEPTH = 44,
		TECHNIQUE_INSTANCED_BUILD_SHADOWMAP_COLOR = 45,
		TECHNIQUE_INSTANCED_UNLIT = 46,
		TECHNIQUE_INSTANCED_UNLIT_UNK = 47,
		TECHNIQUE_INSTANCED_EMISSIVE = 48,
		TECHNIQUE_INSTANCED_EMISSIVE_UNK = 49,
		TECHNIQUE_INSTANCED_EMISSIVE_DEBUG = 50,
		TECHNIQUE_INSTANCED_LIT = 51,
		//TECHNIQUE_INSTANCED_LIT_ = 52
		//TECHNIQUE_INSTANCED_LIT_ = 53
		//TECHNIQUE_INSTANCED_LIT_ = 54
		//TECHNIQUE_INSTANCED_LIT_ = 55
		//TECHNIQUE_INSTANCED_LIT_ = 56
		//TECHNIQUE_INSTANCED_LIT_ = 57
		//TECHNIQUE_INSTANCED_LIT_ = 58
		//TECHNIQUE_INSTANCED_LIT_ = 59
		//TECHNIQUE_INSTANCED_LIT_ = 60
		//TECHNIQUE_INSTANCED_LIT_ = 61
		//TECHNIQUE_INSTANCED_LIT_ = 62
		//TECHNIQUE_INSTANCED_LIT_ = 63
		//TECHNIQUE_INSTANCED_LIT_ = 64
		TECHNIQUE_INSTANCED_THERMAL = 65,
		TECHNIQUE_INSTANCED_VELOCITY_RIGID = 66,
		TECHNIQUE_INSTANCED_VELOCITY_SKINNED = 67,
		TECHNIQUE_INSTANCED_DEBUG_BUMPMAP = 68,
		// 69
		// 70
		// 71
		// 72
		// 73
		// 74
		// 75
		TECHNIQUE_SUBDIV_DEPTH_PREPASS = 76,
		TECHNIQUE_SUBDIV_DEPTH_PREPASS_VELOCITY_RIGID = 77,
		TECHNIQUE_SUBDIV_DEPTH_PREPASS_VELOCITY_SKINNED = 78,
		TECHNIQUE_SUBDIV_DEPTH_PREPASS_OBJECTID = 79,
		TECHNIQUE_SUBDIV_DEPTH_PREPASS_OBJECTID_VELOCITY_RIGID = 80,
		TECHNIQUE_SUBDIV_DEPTH_PREPASS_OBJECTID_VELOCITY_SKINNED = 81,
		TECHNIQUE_SUBDIV_BUILD_SHADOWMAP_DEPTH = 82,
		TECHNIQUE_SUBDIV_BUILD_SHADOWMAP_COLOR = 83,
		TECHNIQUE_SUBDIV_UNLIT = 84,
		TECHNIQUE_SUBDIV_UNLIT_UNK = 85,
		TECHNIQUE_SUBDIV_EMISSIVE = 86,
		TECHNIQUE_SUBDIV_EMISSIVE_UNK = 87,
		TECHNIQUE_SUBDIV_EMISSIVE_DEBUG = 88,
		TECHNIQUE_SUBDIV_LIT = 89,
		//TECHNIQUE_SUBDIV_LIT_ = 90
		//TECHNIQUE_SUBDIV_LIT_ = 91
		//TECHNIQUE_SUBDIV_LIT_ = 92
		//TECHNIQUE_SUBDIV_LIT_ = 93
		//TECHNIQUE_SUBDIV_LIT_ = 94
		//TECHNIQUE_SUBDIV_LIT_ = 95
		//TECHNIQUE_SUBDIV_LIT_ = 96
		//TECHNIQUE_SUBDIV_LIT_ = 97
		//TECHNIQUE_SUBDIV_LIT_ = 98
		//TECHNIQUE_SUBDIV_LIT_ = 99
		//TECHNIQUE_SUBDIV_LIT_ = 100
		//TECHNIQUE_SUBDIV_LIT_ = 101
		//TECHNIQUE_SUBDIV_LIT_ = 102
		TECHNIQUE_SUBDIV_THERMAL = 103,
		TECHNIQUE_SUBDIV_VELOCITY_RIGID = 104,
		TECHNIQUE_SUBDIV_VELOCITY_SKINNED = 105,
		TECHNIQUE_SUBDIV_DEBUG_BUMPMAP = 106,
		// 107
		// 108
		// 109
		// 110
		// 111
		// 112
		// 113
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS = 114,
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS_VELOCITY_RIGID = 115,
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS_VELOCITY_SKINNED = 116,
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS_OBJECTID = 117,
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS_OBJECTID_VELOCITY_RIGID = 118,
		TECHNIQUE_NO_DISPLACEMENT_DEPTH_PREPASS_OBJECTID_VELOCITY_SKINNED = 119,
		TECHNIQUE_NO_DISPLACEMENT_BUILD_SHADOWMAP_DEPTH = 120,
		TECHNIQUE_NO_DISPLACEMENT_BUILD_SHADOWMAP_COLOR = 121,
		TECHNIQUE_NO_DISPLACEMENT_UNLIT = 122,
		TECHNIQUE_NO_DISPLACEMENT_UNLIT_UNK = 123,
		TECHNIQUE_NO_DISPLACEMENT_EMISSIVE = 124,
		TECHNIQUE_NO_DISPLACEMENT_EMISSIVE_UNK = 125,
		TECHNIQUE_NO_DISPLACEMENT_EMISSIVE_DEBUG = 126,
		TECHNIQUE_NO_DISPLACEMENT_LIT = 127,
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 128
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 129
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 130
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 131
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 132
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 133
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 134
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 135
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 136
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 137
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 138
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 139
		//TECHNIQUE_NO_DISPLACEMENT_LIT_ = 140
		TECHNIQUE_NO_DISPLACEMENT_THERMAL = 141,
		TECHNIQUE_NO_DISPLACEMENT_VELOCITY_RIGID = 142,
		TECHNIQUE_NO_DISPLACEMENT_VELOCITY_SKINNED = 143,
		TECHNIQUE_NO_DISPLACEMENT_DEBUG_BUMPMAP = 144,
		// 145
		// 146
		// 147
		// 148
		// 149
		// 150
		// 151
		TECHNIQUE_COUNT = 152,
	};

	struct Packed128
	{
		std::uint64_t p0;
		std::uint64_t p1;
	};

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16; // p0 >> 0
		unsigned __int64 pad0 : 32;
		unsigned __int64 tessellation : 2; // p0 >> 48
		unsigned __int64 pad1 : 14;
		unsigned __int64 pad2 : 64;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		Packed128 packed;
	};

	enum TechsetdefRenderFlags : std::uint8_t
	{
		TECHSETDEF_RENDER_FLAGS_NONE = 0x0,
		TECHSETDEF_RENDER_FLAGS_2D = 0x1,
		TECHSETDEF_RENDER_FLAGS_CASTS_SHADOWS = 0x2,
	};

	struct MaterialInfo
	{
		const char* name;
		unsigned char gameFlags;
		unsigned char unkFlags; // runtimeFlags?
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
		unsigned char perPrimConstantBuffer;
		unsigned char perObjConstantBuffer;
		unsigned char stableConstantBuffer;
		unsigned int customBufferFlags;
		unsigned char customSamplerFlags;
		unsigned char precompiledIndex;
		unsigned char stageConfig;
		char __pad0[33];
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

	enum TechniqueMask
	{
		TECHNIQUE_MASK = 0x3F,
		NUM_TECHNIQUE_MASK_ELEMS = 3, // TECHNIQUE_COUNT / 64
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		unsigned short flags;
		unsigned char worldVertFormat;
		unsigned char preDisplacementOnlyCount;
		unsigned short techniqueCount;
		short techniqueMaskStartIndex[3];
		short unk[2];
		unsigned __int64 techniqueMask[3];
		MaterialTechnique** techniques;
	}; assert_sizeof(MaterialTechniqueSet, 0x38);
	assert_offsetof(MaterialTechniqueSet, techniqueCount, 12);
	assert_offsetof(MaterialTechniqueSet, techniques, 48);

	enum GfxImageFlags : std::uint32_t
	{
		IMG_FLAG_NONE = 0x0,
		IMG_DISK_FLAG_NOPICMIP = 0x1,
		IMG_DISK_FLAG_NOMIPMAPS = 0x2,
		IMG_DISK_FLAG_UNUSED = 0x4,
		IMG_DISK_FLAG_NORMAL_OCCLUSON_GLOSS = 0x8,
		IMG_DISK_FLAG_CLAMP_U = 0x10,
		IMG_DISK_FLAG_CLAMP_V = 0x20,
		IMG_DISK_FLAG_STREAMED = 0x40,
		IMG_DISK_FLAG_USE_OODLE_COMPRESSION = 0x80,
		IMG_DISK_FLAG_GAMMA_SRGB = 0x100,
		IMG_DISK_FLAG_PACKED_ATLAS = 0x200,
		IMG_CREATE_FLAG_UNTILED = 0x400,
		IMG_CREATE_FLAG_CPU_READ = 0x800,
		IMG_CREATE_FLAG_CPU_WRITE = 0x1000,
		IMG_DISK_FLAG_AUTOMETALNESS = 0x2000,
		IMG_DISK_FLAG_AUTODISPLACEMENT = 0x4000,
		IMG_DISK_FLAG_MAPTYPE_2D = 0x0,
		IMG_DISK_FLAG_MAPTYPE_CUBE = 0x8000,
		IMG_DISK_FLAG_MAPTYPE_3D = 0x10000,
		IMG_DISK_FLAG_MAPTYPE_1D = 0x18000,
		IMG_DISK_FLAG_MAPTYPE_ARRAY = 0x20000,
		IMG_DISK_FLAG_MAPTYPE_CUBE_ARRAY = 0x28000,
		IMG_DISK_FLAG_INVERT_ALPHA = 0x40000,
		IMG_DISK_FLAG_PREMUL_ALPHA = 0x80000,
		IMG_DISK_FLAG_MIPGEN_ORIGINAL = 0x0,
		IMG_DISK_FLAG_MIPGEN_LANCZOS3 = 0x100000,
		IMG_DISK_FLAG_MIPGEN_CATMULL_ROM = 0x200000,
		IMG_DISK_FLAG_MIPGEN_CUBIC_BSPLINE = 0x300000,
		IMG_DISK_FLAG_MIPGEN_BOX = 0x400000,
		IMG_DISK_FLAG_MIPGEN_COVERAGE_PRESERVING = 0x500000,
		IMG_CREATE_FLAG_RW_VIEW = 0x800000,
		IMG_CREATE_FLAG_DYNAMIC = 0x1000000,
		IMG_DISK_FLAG_PREMUL_KEEP_ALPHA = 0x2000000,
		IMG_DISK_FLAG_RTT = 0x4000000,
		IMG_DISK_FLAG_EXTRACT_ALPHA = 0x8000000,
		IMG_DISK_FLAG_OCTAHEDRON = 0x10000000,
		IMG_CREATE_FLAG_STAGING = 0x20000000,
		IMG_CREATE_FLAG_VOLUMETRIC_LAYOUT_OVERRIDE = 0x40000000,
		IMG_CREATE_FLAG_TYPELESS = 0x80000000,
	};

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

	enum TextureSemantic : std::uint8_t
	{
		TS_2D = 0x0,
		TS_FUNCTION = 0x1,
		TS_COLOR_MAP = 0x2,
		TS_DETAIL_MAP = 0x3,
		TS_UNUSED_4 = 0x4,
		TS_NORMAL_MAP = 0x5,
		TS_UNUSED_6 = 0x6,
		TS_UNUSED_7 = 0x7,
		TS_SPECULAR_MAP = 0x8,
		TS_SPECULAR_OCCLUSION_MAP = 0x9,
		TS_UNUSED_10 = 0xA,
		TS_THINFILM_MAP = 0xB,
		TS_DISPLACEMENT_MAP = 0xC,
		TS_PARALLAX_MAP = 0xD,
		TS_COLOR_SPECULAR_MAP = 0xE,
		TS_NORMAL_OCCLUSSION_GLOSS_MAP = 0xF,
		TS_ALPHA_REVEAL_THICKNESS_MAP = 0x10,
		TS_COUNT = 0x11,
	};

	enum GfxImageCategory : std::uint8_t
	{
		IMG_CATEGORY_UNKNOWN = 0x0,
		IMG_CATEGORY_AUTO_GENERATED = 0x1,
		IMG_CATEGORY_LIGHTMAP = 0x2,
		IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
		IMG_CATEGORY_RAW = 0x4,
		IMG_CATEGORY_FIRST_UNMANAGED = 0x5,
		IMG_CATEGORY_RENDERTARGET = 0x5,
		IMG_CATEGORY_TEMP = 0x6,
	};

	struct GfxImageLoadDef
	{
		char levelCount;
		char numElements;
		char pad[2];
		int flags;
		int format;
		int resourceSize;
		char data[1];
	};

	struct GfxTexture
	{
		union
		{
			ID3D11Texture1D* linemap;
			ID3D11Texture2D* map;
			ID3D11Texture3D* volmap;
			ID3D11Texture2D* cubemap;
			GfxImageLoadDef* loadDef;
		};
		ID3D11ShaderResourceView* shaderView;
		ID3D11ShaderResourceView* shaderViewAlternate;
	};

	struct PicMip
	{
		unsigned char platform[2];
	};

	struct CardMemory
	{
		unsigned char platform[2];
	};

	struct GfxImageStreamLevelCountAndSize
	{
		unsigned int pixelSize : 26;
		unsigned int levelCount : 6;
	};

	struct GfxImageStreamData
	{
		unsigned short width;
		unsigned short height;
		GfxImageStreamLevelCountAndSize levelCountAndSize;
	};

	struct GfxImage
	{
		GfxTexture texture;
		DXGI_FORMAT imageFormat;
		unsigned int flags;
		MapType mapType;
		TextureSemantic semantic;
		GfxImageCategory category;
		PicMip picmip;
		CardMemory cardMemory;
		unsigned int dataLen1;
		unsigned int dataLen2;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		unsigned short numElements;
		unsigned char levelCount;
		unsigned char streamed;
		unsigned char unknown1;
		unsigned char* pixelData;
		GfxImageStreamData streams[4];
		const char* name;
	}; assert_sizeof(GfxImage, 0x70);

	typedef uint32_t SndStringHash;

	struct snd_volmod_info_t
	{
		char name[64];
		SndStringHash id;
		float value;
	};

	enum SndEntchannelSpaitalType
	{
		SND_ENTCHAN_TYPE_2D = 0x0,
		SND_ENTCHAN_TYPE_3D = 0x1,
		SND_ENTCHAN_TYPE_PA_SPEAKER = 0x2,
	};

	enum SndLimitBehavior
	{
		LIMIT_BEHAVIOR_NO_NEW = 0x0,
		LIMIT_BEHAVIOR_REPLACE_QUIETER = 0x1,
		LIMIT_BEHAVIOR_REPLACE_OLDEST = 0x2,
		LIMIT_BEHAVIOR_REPLACE_FARTHEST = 0x3,
		LIMIT_BEHAVIOR_COUNT = 0x4,
	};

	enum SndFarReverbBehavior
	{
		SND_FAR_REVERB_NONE = 0x0,
		SND_FAR_REVERB_FULL = 0x1,
		SND_FAR_REVERB_WET_ONLY = 0x2,
		SND_FAR_REVERB_UNK3 = 3,
		SND_FAR_REVERB_UNK4 = 4,
		SND_FAR_REVERB_UNK5 = 5,
	};

	struct snd_entchannel_info_t
	{
		char name[64];
		SndStringHash id;
		int priority;
		SndEntchannelSpaitalType spatialType;
		bool isRestricted;
		bool isPausable;
		int maxVoices;
		SndLimitBehavior limitBehavior;
		bool centerSpeakerPan;
		SndFarReverbBehavior mpFarReverbBehavior;
		SndFarReverbBehavior spFarReverbBehavior;
		SndFarReverbBehavior cpFarReverbBehavior;
	}; assert_sizeof(snd_entchannel_info_t, 104);

	struct DopplerPreset
	{
		char name[64];
		SndStringHash id;
		float speedOfSound;
		float playerVelocityScale;
		float minPitch;
		float maxPitch;
		float smoothing;
	}; assert_sizeof(DopplerPreset, 88);

	struct OcclusionShape
	{
		char name[64];
		SndStringHash id;
		float unk;
		float innerAngleVolume;
		float outerAngleVolume;
		float occludedVolume;
		float innerAngleLPF;
		float outerAngleLPF;
		float occludedLPF;
	}; assert_sizeof(OcclusionShape, 96);

	struct SndCurve
	{
		char name[64];
		SndStringHash id;
		int numPoints;
		vec2_t points[16];
	};
	assert_sizeof(SndCurve, 200);

	struct SpeakerMap
	{
		char name[64];
		SndStringHash id;
		float monoToStereo[2];
		float stereoToStereo[4];
		float monoTo51[6];
		float stereoTo51[12];
		float monoTo71[8];
		float stereoTo71[16];
	}; assert_sizeof(SpeakerMap, 260);

	struct SndContext
	{
		char* type;
		SndStringHash typeId;
		int numValues;
		int startIndex;
		bool allowDefaults;
		bool includeDefaultsInRandom;
		bool canBlend;
	}; assert_sizeof(SndContext, 24);

	struct SndContextValue
	{
		char* value;
		SndStringHash valueId;
	};

	struct SndMasterEqParam
	{
		int enabled;
		float gain;
		float frequency;
		float q;
	};

	struct SndMasterEqParamBank
	{
		SndMasterEqParam low;
		SndMasterEqParam peak1;
		SndMasterEqParam peak2;
		SndMasterEqParam high;
	};

	struct SndDynamicsParams
	{
		int enable;
		float preGain;
		float postGain;
		float threshold;
		float ratio;
		float timeAttack;
		float timeRelease;
		float spread;
		int linear;
		int ITU1770;
	};

	struct SndMaster
	{
		char name[64];
		SndStringHash id;
		SndMasterEqParamBank dialogEq;
		SndMasterEqParamBank weapExpScenesEq;
		SndMasterEqParamBank sfxEq;
		SndMasterEqParamBank mainEq;
		SndDynamicsParams dialogDyn;
		SndDynamicsParams sfxDyn;
		SndDynamicsParams mainDyn;
		float busDialogGain;
		float busWeapExpSceneGain;
		float busSfxGain;
		float busReverbGain;
		float busMusicGain;
		float busMovieGain;
		float busReferenceGain;
		int busDialogEnable;
		int busWeapExpSceneEnable;
		int busSfxEnable;
		int busReverbEnable;
		int busMusicEnable;
		int busMovieEnable;
		int busReferenceEnable;
	}; assert_sizeof(SndMaster, 500);

	struct SndGlobals
	{
		const char* name;
		float globalVolumeModifier;
		float binkVolumeModifier;
		unsigned int volmodinfoCount;
		snd_volmod_info_t* volmodinfo;
		unsigned int entchannelCount;
		snd_entchannel_info_t* entchannelInfo;
		unsigned int dopplerPresetCount;
		DopplerPreset* dopplerPresets;
		unsigned int occlusionShapeCount;
		OcclusionShape* occlusionShapes;
		unsigned int vfCurveCount;
		SndCurve* vfCurves;
		unsigned int lpfCurveCount;
		SndCurve* lpfCurves;
		unsigned int hpfCurveCount;
		SndCurve* hpfCurves;
		unsigned int rvbCurveCount;
		SndCurve* rvbCurves;
		unsigned int speakerMapCount;
		SpeakerMap* speakerMaps;
		unsigned int contextCount;
		SndContext* contexts;
		unsigned int contextValueCount;
		SndContextValue* contextValues;
		unsigned int masterCount;
		SndMaster* masters;
	}; assert_sizeof(SndGlobals, 0xD0);

	enum SndVolMod
	{
		SND_VOLMOD_HUD = 0,
		SND_VOLMOD_INTERFACE = 1,
		SND_VOLMOD_INTERFACE_MUSIC = 2,
		SND_VOLMOD_MUSIC = 3,
		SND_VOLMOD_MUSIC_EMITTER = 4,
		SND_VOLMOD_MUSIC_EMITTER_PA = 5,
		SND_VOLMOD_AMBIENCE = 6,
		SND_VOLMOD_AMBIENCE_DIST = 7,
		SND_VOLMOD_ELEMENT = 8,
		SND_VOLMOD_EMITTER = 9,
		SND_VOLMOD_PHYSICS = 10,
		SND_VOLMOD_BODYFALL = 11,
		SND_VOLMOD_FOLEY_PLR = 12,
		SND_VOLMOD_FOLEYMP_PLR = 13,
		SND_VOLMOD_FOLEY_NPC = 14,
		SND_VOLMOD_FOLEYMP_NPC = 15,
		SND_VOLMOD_FOLEY_WPN_PLR = 16,
		SND_VOLMOD_FOLEY_WPN_NPC = 17,
		SND_VOLMOD_FOOTSTEP_PLR = 18,
		SND_VOLMOD_FOOTSTEP_NPC = 19,
		SND_VOLMOD_FOOTSTEPMP_PLR = 20,
		SND_VOLMOD_FOOTSTEPMP_NPC = 21,
		SND_VOLMOD_MELEE_PLR = 22,
		SND_VOLMOD_MELEE_NPC = 23,
		SND_VOLMOD_CHATTERAL = 24,
		SND_VOLMOD_CHATTERAX = 25,
		SND_VOLMOD_REACTIONAL = 26,
		SND_VOLMOD_REACTIONAX = 27,
		SND_VOLMOD_VOICEOVER = 28,
		SND_VOLMOD_VOICEOVER_RADIO = 29,
		SND_VOLMOD_VOICEOVER_RADIO_3D = 30,
		SND_VOLMOD_VOICEOVER_CRITICAL = 31,
		SND_VOLMOD_VOICEOVER_AMB = 32,
		SND_VOLMOD_VOICEOVER_PLR = 33,
		SND_VOLMOD_DESTRUCT = 34,
		SND_VOLMOD_EXPLOSION = 35,
		SND_VOLMOD_EXPLOSION_GRENADE = 36,
		SND_VOLMOD_EXPLOSION_FLASHBANG = 37,
		SND_VOLMOD_EXPLOSION_ROCKET = 38,
		SND_VOLMOD_EXPLOSION_CAR = 39,
		SND_VOLMOD_REX_EMITTERS = 40,
		SND_VOLMOD_IMPACT = 41,
		SND_VOLMOD_IMPACT_PLR = 42,
		SND_VOLMOD_IMPACT_NPC = 43,
		SND_VOLMOD_IMPACT_PLR_TO_NPC = 44,
		SND_VOLMOD_IMPACTMP = 45,
		SND_VOLMOD_IMPACTMP_PLR = 46,
		SND_VOLMOD_IMPACTMP_NPC = 47,
		SND_VOLMOD_IMPACTMP_PLR_TO_NPC = 48,
		SND_VOLMOD_WHIZBY = 49,
		SND_VOLMOD_WHIZBYMP = 50,
		SND_VOLMOD_VEHICLE_PLR = 51,
		SND_VOLMOD_VEHICLE_NPC = 52,
		SND_VOLMOD_VEHICLE_WPN_PLR = 53,
		SND_VOLMOD_VEHICLE_WPN_NPC = 54,
		SND_VOLMOD_VEHICLE = 55,
		SND_VOLMOD_GRENADEBOUNCE = 56,
		SND_VOLMOD_GRENADEBOUNCEMP = 57,
		SND_VOLMOD_SHELLCASINGS = 58,
		SND_VOLMOD_SHELLCASINGSMP = 59,
		SND_VOLMOD_WPN_PLR = 60,
		SND_VOLMOD_WPNMP_PLR = 61,
		SND_VOLMOD_WPN_NPC = 62,
		SND_VOLMOD_WPNMP_NPC = 63,
		SND_VOLMOD_WPN_PROJECTILE = 64,
		SND_VOLMOD_WPNMP_PROJECTILE = 65,
		SND_VOLMOD_WPN_REFLECTIONS = 66,
		SND_VOLMOD_WPN_REFLECTIONS_MID = 67,
		SND_VOLMOD_WPN_REFLECTIONS_FAR = 68,
		SND_VOLMOD_NA = 69,
		SND_VOLMOD_MAX = 70,
		SND_VOLMOD_SCRIPTED1 = 71,
		SND_VOLMOD_SCRIPTED2 = 72,
		SND_VOLMOD_SCRIPTED3 = 73,
		SND_VOLMOD_SCRIPTED4 = 74,
		SND_VOLMOD_SCRIPTED5 = 75,
		SND_VOLMOD_FULLVOLUME = 76,
		SND_VOLMOD_PERKMP_QUIET = 77,
		SND_VOLMOD_DEATHSDOOR = 78,
		SND_VOLMOD_DEFAULT = 79,
	};

	enum SndChannel
	{
		SND_CHANNEL_SCN_FX_UNRES_3D = 0,
		SND_CHANNEL_SCN_LFE_UNRES_3D = 1,
		SND_CHANNEL_SCN_FX_UNRES_2D = 2,
		SND_CHANNEL_SCN_LFE_UNRES_2D = 3,
		SND_CHANNEL_SCN_FX_SPECIAL_UNRES_3D = 4,
		SND_CHANNEL_SCN_FX_SPECIAL_UNRES_2D = 5,
		SND_CHANNEL_SCN_FX_UNRES_3D_LIM = 6,
		SND_CHANNEL_SCN_FX_UNRES_2D_LIM = 7,
		SND_CHANNEL_SCN_FX_RES_3D = 8,
		SND_CHANNEL_SCN_FX_RES_2D = 9,
		SND_CHANNEL_MELEE_PLR_2D = 10,
		SND_CHANNEL_WEAP_PLR_FIRE_1_2D = 11,
		SND_CHANNEL_WEAP_PLR_FIRE_2_2D = 12,
		SND_CHANNEL_WEAP_PLR_FIRE_3_2D = 13,
		SND_CHANNEL_WEAP_PLR_FIRE_4_2D = 14,
		SND_CHANNEL_WEAP_PLR_FIRE_OVERLAP_2D = 15,
		SND_CHANNEL_WEAP_PLR_FIRE_LFE_2D = 16,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_1_2D = 17,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_2_2D = 18,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_3_2D = 19,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_4_2D = 20,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_OVERLAP_2D = 21,
		SND_CHANNEL_WEAP_PLR_FIRE_ALT_LFE_2D = 22,
		SND_CHANNEL_FOLEY_PLR_STEP_2D = 23,
		SND_CHANNEL_FOLEY_PLR_STEP_UNRES_2D = 24,
		SND_CHANNEL_FOLEY_PLR_MVMT_UNRES_2D_LIM = 25,
		SND_CHANNEL_FOLEY_PLR_WEAP_2D_LIM = 26,
		SND_CHANNEL_RELOAD_PLR_RES_2D = 27,
		SND_CHANNEL_RELOAD_PLR_UNRES_2D = 28,
		SND_CHANNEL_RELOAD_PLR_AKIMBO_RES_2D = 29,
		SND_CHANNEL_RELOAD_PLR_AKIMBO_UNRES_2D = 30,
		SND_CHANNEL_SPEAR_REFL_CLOSE_UNRES_3D_LIM = 31,
		SND_CHANNEL_SPEAR_REFL_UNRES_3D_LIM = 32,
		SND_CHANNEL_WEAP_NPC_MAIN_3D = 33,
		SND_CHANNEL_WEAP_NPC_MECH_3D = 34,
		SND_CHANNEL_WEAP_NPC_MID_3D = 35,
		SND_CHANNEL_WEAP_NPC_LFE_3D = 36,
		SND_CHANNEL_WEAP_NPC_DIST_3D = 37,
		SND_CHANNEL_WEAP_NPC_LO_3D = 38,
		SND_CHANNEL_MELEE_NPC_3D = 39,
		SND_CHANNEL_PLR_USE_MISC_UNRES_2D = 40,
		SND_CHANNEL_PLR_UI_INGAME_UNRES_2D = 41,
		SND_CHANNEL_PLR_USE_PS4SPKR_RES_2D = 42,
		SND_CHANNEL_EQUIP_USE_UNRES_3D = 43,
		SND_CHANNEL_EQUIP_UI_INGAME_UNRES_3D = 44,
		SND_CHANNEL_ABILITY_PLR_UNRES_2D = 45,
		SND_CHANNEL_ABILITY_NPC_UNRES_3D = 46,
		SND_CHANNEL_EXPLO_1_3D = 47,
		SND_CHANNEL_EXPLO_2_3D = 48,
		SND_CHANNEL_EXPLO_3_3D = 49,
		SND_CHANNEL_EXPLO_4_3D = 50,
		SND_CHANNEL_EXPLO_5_3D = 51,
		SND_CHANNEL_EXPLO_LFE_3D = 52,
		SND_CHANNEL_EXPLO_DIST_1_3D = 53,
		SND_CHANNEL_EXPLO_DIST_2_3D = 54,
		SND_CHANNEL_FOLEY_C12_STEP_3D = 55,
		SND_CHANNEL_FOLEY_C12_LFE_3D = 56,
		SND_CHANNEL_PROJECTILE_LOOP_CLOSE = 57,
		SND_CHANNEL_PROJECTILE_LOOP_MID = 58,
		SND_CHANNEL_PROJECTILE_LOOP_DIST = 59,
		SND_CHANNEL_WHIZBY_IN_UNRES_3D_LIM = 60,
		SND_CHANNEL_WHIZBY_OUT_UNRES_3D_LIM = 61,
		SND_CHANNEL_BULLETIMPACT_UNRES_3D_LIM = 62,
		SND_CHANNEL_BEAMIMPACT_UNRES_3D_LIM = 63,
		SND_CHANNEL_BULLETIMPACT_LO_UNRES_3D_LIM = 64,
		SND_CHANNEL_BULLET_RICOCHETS_UNRES_3D_LIM = 65,
		SND_CHANNEL_BULLET_RICO_REFLECT_UNRES_3D_LIM = 66,
		SND_CHANNEL_BULLETFLESH_1_UNRES_3D_LIM = 67,
		SND_CHANNEL_BULLETFLESH_2_UNRES_3D_LIM = 68,
		SND_CHANNEL_BULLETFLESH_LFE_UNRES_2D_LIM = 69,
		SND_CHANNEL_BULLETFLESH_NPC_1_UNRES_3D_LIM = 70,
		SND_CHANNEL_BULLETFLESH_NPC_2_UNRES_3D_LIM = 71,
		SND_CHANNEL_BULLETFLESH_NPCNPC1_UNRES_3D_LIM = 72,
		SND_CHANNEL_BULLETFLESH_NPCNPC2_UNRES_3D_LIM = 73,
		SND_CHANNEL_MELEE_IMPACT_3D = 74,
		SND_CHANNEL_FOLEY_NPC_STEP_3D = 75,
		SND_CHANNEL_FOLEY_NPC_STEP_UNRES_3D = 76,
		SND_CHANNEL_FOLEY_NPC_ZMB_STEP_3D_LIM = 77,
		SND_CHANNEL_FOLEY_NPC_MVMT_UNRES_3D_LIM = 78,
		SND_CHANNEL_FOLEY_NPC_WEAP_UNRES_3D_LIM = 79,
		SND_CHANNEL_RELOAD_NPC_RES_3D = 80,
		SND_CHANNEL_RELOAD_NPC_UNRES_3D = 81,
		SND_CHANNEL_VOICE_AIR_3D = 82,
		SND_CHANNEL_VOICE_RADIO_3D = 83,
		SND_CHANNEL_VOICE_RADIO_2D = 84,
		SND_CHANNEL_VOICE_NARRATION_2D = 85,
		SND_CHANNEL_VOICE_SPECIAL_2D = 86,
		SND_CHANNEL_VOICE_WALLA_1_3D = 87,
		SND_CHANNEL_VOICE_BCHATTER_1_3D = 88,
		SND_CHANNEL_VOICE_BCHATTER_LO_UNRES_3D_LIM = 89,
		SND_CHANNEL_VOICE_PLR_2D = 90,
		SND_CHANNEL_VOICE_PLR_EFFORTS_2D = 91,
		SND_CHANNEL_VOICE_PLR_BREATH_2D = 92,
		SND_CHANNEL_VOICE_ANIMAL_1_3D = 93,
		SND_CHANNEL_AMB_BED_2D = 94,
		SND_CHANNEL_AMB_ELM_UNRES_3D = 95,
		SND_CHANNEL_AMB_ELM_INT_UNRES_3D = 96,
		SND_CHANNEL_AMB_ELM_EXT_UNRES_3D = 97,
		SND_CHANNEL_AMB_ELM_UNRES_2D_LIM = 98,
		SND_CHANNEL_AMB_ELM_UNRES_2D = 99,
		SND_CHANNEL_AMB_ELM_INT_SPECIAL_UNRES_3D = 100,
		SND_CHANNEL_AMB_ELM_EXT_SPECIAL_UNRES_3D = 101,
		SND_CHANNEL_BODYFALLS_HI_UNRES_3D_LIM = 102,
		SND_CHANNEL_VEHICLE_AIR_FLYBY_CLOSE_3D_LIM = 103,
		SND_CHANNEL_VEHICLE_AIR_LOOPS_3D_LIM = 104,
		SND_CHANNEL_VEHICLE_AIR_FLYBY_MIDFAR_3D_LIM = 105,
		SND_CHANNEL_VEHICLE_WEAP_CLOSE_3D_LIM = 106,
		SND_CHANNEL_VEHICLE_HI_RES_3D = 107,
		SND_CHANNEL_VEHICLE_HI_UNRES_3D = 108,
		SND_CHANNEL_VEHICLE_LFE_3D = 109,
		SND_CHANNEL_VEHICLE_LO_3D_LIM = 110,
		SND_CHANNEL_VEHICLE_UNRES_2D = 111,
		SND_CHANNEL_VEHICLE_UNRES_2D_LIM = 112,
		SND_CHANNEL_SHOCK_NOFILTER_2D = 113,
		SND_CHANNEL_HURT_NOFILTER_2D = 114,
		SND_CHANNEL_SHOCK1_NOFILTER_3D = 115,
		SND_CHANNEL_SHOCK2_NOFILTER_3D = 116,
		SND_CHANNEL_MUSIC_LR = 117,
		SND_CHANNEL_MUSIC_LSRS = 118,
		SND_CHANNEL_MUSICNOPAUSE_LR = 119,
		SND_CHANNEL_MUSICNOPAUSE_LSRS = 120,
		SND_CHANNEL_MUS_EMITTER_3D = 121,
		SND_CHANNEL_MENU_UNRES_2D = 122,
		SND_CHANNEL_MENU_1_2D_LIM = 123,
		SND_CHANNEL_MENU_2_2D_LIM = 124,
		SND_CHANNEL_PHYSICS_HI_UNRES_3D_LIM = 125,
		SND_CHANNEL_PHYSICS_LO_UNRES_3D_LIM = 126,
		SND_CHANNEL_BODYFALLS_LO_UNRES_3D_LIM = 127,
		SND_CHANNEL_SPECIAL_HI_UNRES_1_3D = 128,
		SND_CHANNEL_SPECIAL_HI_UNRES_1_2D = 129,
		SND_CHANNEL_SPECIAL_LO_UNRES_1_3D = 130,
		SND_CHANNEL_SPECIAL_LO_UNRES_1_2D = 131,
		SND_CHANNEL_PA_SPEAKER = 132,
		SND_CHANNEL_PA_VOICE = 133,
	};

	enum SndAliasType
	{
		SAT_LOADED = 0x0,
		SAT_REV_VEHICLE = 0x1,
		SAT_HYBRID_PCM = 0x2,
		SAT_STREAMED = 0x3,
		SAT_PRIMED = 0x4,
		SAT_COUNT = 0x5,
	};

	struct SndAliasFlags
	{
		unsigned int looping : 3; // 0
		unsigned int reverb : 1; // 3
		unsigned int shape : 3; // 4
		unsigned int type : 2; // 7
		unsigned int channel : 8; // 9
		unsigned int unused : 1; // 17
		unsigned int ignoreDistanceCheck : 1; // 18
		unsigned int precached : 1; // 19
		unsigned int MusicContent : 2; // 20
		unsigned int GPadOutput : 2; // 22
		unsigned int ForceSubtitle : 1; // 24
		unsigned int pad : 7; // 26
	};

	struct SndAlias
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* stopAliasName;
		const char* assetFileName;
		SndStringHash id;
		SndStringHash secondaryId;
		SndStringHash stopAliasID;
		SndStringHash assetId;
		SndStringHash contextType;
		SndStringHash contextValue;
		SndStringHash duck;
		int sequence;
		float volMin;
		float volMax;
		int volModIndex;
		float pitchMin;
		float pitchMax;
		float donutFadeEnd;
		float distMin;
		float distMax;
		float velocityMin;
		SndAliasFlags flags;
		int masterPriority;
		float masterPercentage;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		unsigned char volumeFalloffCurveIndex;
		unsigned char lpfCurveIndex;
		unsigned char hpfCurveIndex;
		unsigned char reverbSendCurveIndex;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		unsigned char speakerMapIndex;
		char pad1[3];
		float reverbMultiplier;
		float farReverbMultiplier;
		unsigned char occlusionShapeIndex;
		unsigned char dopplerPresetIndex;
		char pad2[2];
		float smartPanDistance2d;
		float smartPanDistance3d;
		float smartPanAttenuation2d;
		float stereoSpreadMinDist;
		float stereoSpreadMaxDist;
		float stereoSpreadMidPoint;
		int stereoSpreadMaxAngle;
	}; assert_sizeof(SndAlias, 200);
	assert_offsetof(SndAlias, flags, 108);

	struct SndAliasList
	{
		const char* aliasName;
		SndStringHash id;
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
		char name[64];
		SndStringHash id;
		float smoothing;
		float earlyTime;
		float lateTime;
		float earlyGain;
		float lateGain;
		float lateGainProx;
		float returnGain;
		float earlyLpf;
		float lateLpf;
		float inputLpf;
		float dampLpf;
		float wallReflect;
		float dryGain;
		float earlySize;
		float lateSize;
		float diffusion;
		float rearLevel;
	}; assert_sizeof(SndSendEffectParams, 136);

	enum ADSRCurve
	{
		ADSR_CURVE_LINEAR = 0x0,
		ADSR_CURVE_LOG_FAST = 0x1,
		ADSR_CURVE_LOG_SLOW = 0x2,
		ADSR_CURVE_FLAT_ENDED = 0x3,
		ADSR_CURVE_FLAT_MIDDLE = 0x4,
	};

	struct ADSRSetting
	{
		const char* name;
		SndStringHash id;
		float attackLength;
		ADSRCurve attackCurve;
		float decayLength;
		ADSRCurve decayCurve;
		float releaseLength;
		ADSRCurve releaseCurve;
		float sustainLevel;
		float sustainLength;
	};

	struct AmbientDef
	{
		const char* name;
		SndStringHash id;
		unsigned short ambientEventIndex;
		unsigned short numEvents;
		float timeMin;
		float timeMax;
	};

	struct AmbientEvent
	{
		SndStringHash id;
		unsigned short ambientElementIndex;
		float weight;
	};

	struct AmbientElement
	{
		SndStringHash id;
		const char* unusedNameField;
		const char* aliasName;
		float rangeMin;
		float rangeMax;
		float coneMin;
		float coneMax;
	};

	enum SND_EQTYPE
	{
		SND_EQTYPE_FIRST = 0x0,
		SND_EQTYPE_LOWPASS = 0x0,
		SND_EQTYPE_HIGHPASS = 0x1,
		SND_EQTYPE_LOWSHELF = 0x2,
		SND_EQTYPE_HIGHSHELF = 0x3,
		SND_EQTYPE_BELL = 0x4,
		SND_EQTYPE_LAST = 0x4,
		SND_EQTYPE_COUNT = 0x5,
		SND_EQTYPE_INVALID = 0x5,
	};

	struct FilterDef
	{
		SndStringHash id;
		unsigned char entChannelIdx;
		unsigned char band;
		SND_EQTYPE type;
		float freq;
		float gain;
		float q;
	};

	struct MixDef
	{
		SndStringHash id;
		unsigned short volModIndex;
		float volume;
		float fade;
	};

	struct AdsrZoneEntry
	{
		const char* name;
		SndStringHash id;
		const char* weaponName;
		int adsrIdx;
		unsigned int weaponIdx;
	};

	struct OccludeDef
	{
		SndStringHash id;
		unsigned short entChannelIdx;
		float cutoffLevel1;
		float cutoffLevel2;
		float cutoffLevel3;
		float cutoffLevel4;
		float attnLevel1;
		float attnLevel2;
		float attnLevel3;
		float attnLevel4;
	};

	struct ReverbDef
	{
		SndStringHash id;
		SndStringHash roomTypeId;
		char roomType[64];
		float dryLevel;
		float wetLevel;
		float fadeTime;
	}; assert_sizeof(ReverbDef, 84);

	struct TimescaleEntry
	{
		SndStringHash id;
		SndStringHash presetName;
		int entChannelIdx;
		float scale;
	}; assert_sizeof(TimescaleEntry, 16);

	struct ZoneDef
	{
		SndStringHash id;
		SndStringHash stateId;
		short reverbIndex;
		short startOcclusionIndex;
		short numOcclusion;
		short startFilterIndex;
		short numFilter;
		short startMixIndex;
		short numMix;
		const char* ambientStream;
		short startAdsrZoneIndex;
		short numAdsrZone;
		short startNPCAdsrZoneIndex;
		short numNPCAdsrZone;
		short ambientDefIndex;
		short unkIndex; // idk
		short startFullOccIndex;
		short numDisableFullOcc;
		short startPlayerBreathStateIndex;
		short numPlayerBreathStates;
		SndStringHash weapReflId;
		//bool exterior;
		const char* zoneName;
		const char* stateName;
		const char* ambientName;
		const char* mixName;
		const char* reverbName;
		const char* filterName;
		const char* occlusionName;
		const char* plrAdsrName;
		const char* npcAdsrName;
		const char* weapReflName;
		const char* fullOccName;
		const char* playerBreathStateName;
		SndStringHash contextType1;
		SndStringHash contextValue1;
		SndStringHash contextType2;
		SndStringHash contextValue2;
		SndStringHash contextType3;
		SndStringHash contextValue3;
		SndStringHash contextType4;
		SndStringHash contextValue4;
		//SndStringHash duck;
	}; assert_sizeof(ZoneDef, 184);

	struct unk_1453E2FD8
	{
		SndStringHash id;
		const char* name;
		float unk3;
		float unk4;
		float unk5;
		float unk6;
		int unk7;
		int unk8;
		int unk9;
		float unk10;
		float unk11;
		int unk12;
	}; assert_sizeof(unk_1453E2FD8, 56);

	struct FullOcclusionDef
	{
		SndStringHash id;
		SndStringHash presetName;
		int entChannelIdx;
	};

	struct PlayerBreathStateDef
	{
		SndStringHash stateType;
		const char* inhaleAlias;
		const char* exhaleAlias;
		int minBreaths;
	};

	struct SoundTable
	{
		SndStringHash id;
		ADSRSetting* adsrSettings;
		unsigned int adsrCount;
		AmbientDef* ambientDefs;
		unsigned int ambientDefCount;
		AmbientEvent* ambientEvents;
		unsigned int ambientEventCount;
		AmbientElement* ambientElements;
		unsigned int ambientElementsCount;
		FilterDef* filters;
		unsigned int filterCount;
		MixDef* mixes;
		unsigned int mixCount;
		AdsrZoneEntry* npcADSRZones;
		unsigned int npcADSRZoneCount;
		OccludeDef* occlusionFilters;
		unsigned int occlusionFilterCount;
		AdsrZoneEntry* playerADSRZones;
		unsigned int playerADSRZoneCount;
		ReverbDef* reverbs;
		unsigned int reverbCount;
		TimescaleEntry* timeScaleSettings;
		unsigned int timeScaleSettingCount;
		ZoneDef* zones;
		unsigned int zoneCount;
		unk_1453E2FD8* unk;
		unsigned int unkCount;
		FullOcclusionDef* fullOcclusionDefs;
		unsigned int fullOcclusionDefCount;
		PlayerBreathStateDef* plrBreathStateDefs;
		unsigned int plrBreathStateDefCount;
	}; assert_sizeof(SoundTable, 248);

	struct SndDuck
	{
		char name[64];
		SndStringHash id;
		float fadeIn;
		float fadeOut;
		float startDelay;
		float minDistance;
		float distance;
		float length;
		SndStringHash fadeInCurve;
		SndStringHash fadeOutCurve;
		float* attenuation;
		float* lpf;
		bool updateWhilePaused;
		bool trackAmplitude;
		bool disableInSplitscreen;
		float unused[2];
		float aliasAttenuation;
		float aliasLpf;
		SndStringHash duckAlias;
	}; assert_sizeof(SndDuck, 144);

	enum SndMusicAssetType
	{
		SND_MUSIC_INTRO = 0x0,
		SND_MUSIC_LOOP = 0x1,
		SND_MUSIC_EXIT = 0x2,
	};

	struct SndMusicAsset
	{
		char alias[64];
		SndStringHash aliasId;
		int looping;
		int completeLoop;
		int removeAfterPlay;
		int firstRandom;
		int startSync;
		int stopSync;
		int completeOnStop;
		unsigned int loopStartOffset;
		int bpm;
		int assetType;
		int loopNumber;
		int order;
		int startDelayBeats;
		int startFadeBeats;
		int stopDelayBeats;
		int stopFadeBeats;
		int startOffsetFrames;
		int meter;
	}; assert_sizeof(SndMusicAsset, 140);

	enum SndMusicStateStatus
	{
		SND_MUSIC_STATE_INACTIVE = 0x0,
		SND_MUSIC_STATE_ACTIVE = 0x1,
	};

	struct SndMusicState
	{
		char name[64];
		SndStringHash id;
		SndMusicAsset intro;
		SndMusicAsset exit;
		unsigned int loopCount;
		SndMusicAsset* loops;
		int order;
		int interruptPriority;
		int persistPriority;
		bool isRandom;
		bool isSequential;
		bool skipPreviousExit;
		bool wallClockSync;
		unsigned int refCount;
		SndMusicStateStatus status;
	}; assert_sizeof(SndMusicState, 384);

	struct SndMusicSet
	{
		char name[64];
		SndStringHash id;
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

	struct cplane_s
	{
		float normal[3];
		float dist;
		unsigned char type;
		unsigned char pad[3];
	}; assert_sizeof(cplane_s, 20);

	struct ClipInfo
	{
		int planeCount;
		cplane_s* planes;
	}; assert_sizeof(ClipInfo, 16);

	struct cStaticModel_s
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float unk[2];
	}; assert_sizeof(cStaticModel_s, 0x40);

	struct unk_1453E2338
	{
		unsigned int num;
		int* unk;
	};

	struct Stage
	{
		const char* name;
		vec3_t origin;
		unsigned short triggerIndex;
		char sunPrimaryLightIndex;
		unsigned int entityUID;
		char unk[20];
	}; assert_sizeof(Stage, 48);

	struct PhysicsCapacities
	{
		int maxNumRigidBodiesServer;
		int maxNumDetailRigidBodiesServer;
		int maxNumConstraintsServer;
		int maxNumMotionsServer;
		int maxNumRigidBodiesClient;
		int maxNumDetailRigidBodiesClient;
		int maxNumConstraintsClient;
		int maxNumMotionsClient;
	};

	struct CollisionHeatmapEntry
	{
		int vertexCount;
		float vertexDensity;
		vec3_t minExtent;
		vec3_t maxExtent;
	};

	struct TopDownMapDataHeader
	{
		int version;
		int samplesPerAxis;
		vec2_t bottomLeft;
		vec2_t topRight;
	};

	struct TopDownMapDataContent
	{
		char navMeshVisible[8193];
	};

	struct TopDownMapData
	{
		TopDownMapDataHeader header;
		TopDownMapDataContent content;
	}; assert_sizeof(TopDownMapData, 8220);

	struct TriggerModel
	{
		int contents;
		unsigned short hullCount;
		unsigned short firstHull;
		unsigned short windingCount;
		unsigned short firstWinding;
		unsigned int flags;
		PhysicsAsset* physicsAsset;
		unsigned short physicsShapeOverrideIdx;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned short slabCount;
		unsigned short firstSlab;
	};

	struct TriggerSlab
	{
		vec3_t dir;
		float midPoint;
		float halfSize;
	};

	struct TriggerWinding
	{
		vec3_t dir;
		unsigned short pointCount;
		unsigned short firstPoint;
		unsigned int flags;
	};

	struct TriggerWindingPoint
	{
		vec3_t loc;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
		unsigned int windingCount;
		TriggerWinding* windings;
		unsigned int windingPointCount;
		TriggerWindingPoint* windingPoints;
	}; assert_sizeof(MapTriggers, 0x50);

	struct clipMap_t // __declspec(align(128))
	{
		const char* name;
		int isInUse;
		ClipInfo info;
		ClipInfo* pInfo;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unk_1453E2338 unk01;
		unsigned int numUnk02;
		unk_1453E2338* unk02;
		MapEnts* mapEnts;
		Stage* stages;
		unsigned char stageCount;
		MapTriggers stageTrigger;
		vec3_t broadphaseMin;
		vec3_t broadphaseMax;
		PhysicsCapacities physicsCapacities;
		unsigned int havokWorldShapeDataSize;
		char* havokWorldShapeData;
		unsigned int numCollisionHeatmapEntries;
		CollisionHeatmapEntry* collisionHeatmap;
		TopDownMapData* topDownMapData;
		unsigned int checksum;
		char padding[88]; // alignment pad
	}; assert_sizeof(clipMap_t, 0x180);

	enum GfxLightType : std::uint8_t
	{
		GFX_LIGHT_TYPE_NONE = 0x0,
		GFX_LIGHT_TYPE_DIR = 0x1,
		GFX_LIGHT_TYPE_SPOT = 0x2,
		GFX_LIGHT_TYPE_OMNI = 0x3,
		GFX_LIGHT_TYPE_COUNT = 0x4,
		GFX_LIGHT_TYPE_DIR_SHADOWMAP = 0x4,
		GFX_LIGHT_TYPE_SPOT_SHADOWMAP = 0x5,
		GFX_LIGHT_TYPE_OMNI_SHADOWMAP = 0x6,
		GFX_LIGHT_TYPE_COUNT_WITH_SHADOWMAP_VERSIONS = 0x7,
		GFX_LIGHT_TYPE_SPOT_SHADOWMAP_CUCOLORIS = 0x7,
		GFX_LIGHT_TYPE_COUNT_WITH_ALL_VERSIONS = 0x8,
	};

	struct ComPrimaryLight
	{
		GfxLightType type;
		unsigned char canUseShadowMap;
		unsigned char needsDynamicShadows;
		unsigned char isVolumetric;
		unsigned char exponent;
		char unused[3];
		unsigned int transientZoneList;
		unsigned int entityId;
		float uvIntensity;
		float irIntensity;
		float color[3];
		float dir[3];
		float up[3];
		float origin[3];
		float radius;
		vec2_t fadeOffsetRt;
		float bulbRadius;
		vec3_t bulbLength;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float shadowSoftness;
		float shadowBias;
		float shadowArea;
		float distanceFalloff;
		float rotationLimit;
		float translationLimit;
		const char* defName;
	}; assert_sizeof(ComPrimaryLight, 144);

	struct ComPrimaryLightEnv
	{
		unsigned short primaryLightIndices[4];
		unsigned char numIndices;
	}; assert_sizeof(ComPrimaryLightEnv, 10);

	struct ComChangeListInfo
	{
		unsigned int changeListNumber;
		int time;
		const char* userName;
	}; assert_sizeof(ComChangeListInfo, 16);

	struct ComWorld
	{
		const char* name;
		int isInUse;
		int useForwardPlus;
		unsigned int bakeQuality;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
		unsigned int scriptablePrimaryLightCount;
		unsigned int firstScriptablePrimaryLight;
		unsigned int primaryLightEnvCount;
		ComPrimaryLightEnv* primaryLightEnvs;
		ComChangeListInfo changeListInfo;
		unsigned int numUmbraGates;
		const char** umbraGateNames;
		int umbraGateInitialStates[4];
	}; assert_sizeof(ComWorld, 0x68);

	struct G_GlassPiece
	{
		unsigned short damageTaken;
		unsigned short collapseTime;
		int lastStateChangeTime;
		char impactDir;
		char impactPos[2];
	};

	struct G_GlassName
	{
		char* nameStr;
		scr_string_t name;
		unsigned short pieceCount;
		unsigned short* pieceIndices;
	};

	struct G_GlassData
	{
		G_GlassPiece* glassPieces;
		unsigned int pieceCount;
		unsigned short damageToWeaken;
		unsigned short damageToDestroy;
		unsigned int glassNameCount;
		G_GlassName* glassNames;
	};

	struct GlassWorld
	{
		const char* name;
		G_GlassData* g_glassData;
	};

	struct pathnode_t;
	struct pathnode_tree_t;

	struct pathnode_yaworient_t
	{
		float fLocalAngle;
		vec2_t localForward;
	};

	union PathNodeOrientationUnion
	{
		pathnode_yaworient_t yaw_orient;
		vec3_t angles;
	};

	union PathNodeParentUnion
	{
		scr_string_t name;
		unsigned short index;
	};

	enum PathNodeErrorCode
	{
		PNERR_NONE = 0x0,
		PNERR_INSOLID = 0x1,
		PNERR_FLOATING = 0x2,
		PNERR_NOLINK = 0x3,
		PNERR_DUPLICATE = 0x4,
		PNERR_NOSTANCE = 0x5,
		PNERR_INVALIDDOOR = 0x6,
		PNERR_NOANGLES = 0x7,
		PNERR_NOPEEKOUT = 0x8,
		PNERR_NEARSOLID = 0x9,
		PNERR_NOCOVER = 0xA,
		NUM_PATH_NODE_ERRORS = 0xB,
	};

	union $96C58BBB7F3ED2CCC5F07A7E763FE6AE
	{
		float minUseDistSq;
		PathNodeErrorCode error;
	};

	struct pathlink_s
	{
		float fDist;
		unsigned short nodeNum;
		char disconnectCount;
		char negotiationLink;
		char flags;
		char ubBadPlaceCount[3];
	};

	enum nodeType : std::uint16_t
	{
		NODE_ERROR = 0x0,
		NODE_PATHNODE = 0x1,
		NODE_COVER_STAND = 0x2,
		NODE_COVER_CROUCH = 0x3,
		NODE_COVER_CROUCH_WINDOW = 0x4,
		NODE_COVER_PRONE = 0x5,
		NODE_COVER_RIGHT = 0x6,
		NODE_COVER_LEFT = 0x7,
		NODE_AMBUSH = 0x8,
		NODE_EXPOSED = 0x9,
		NODE_CONCEALMENT_STAND = 0xA,
		NODE_CONCEALMENT_CROUCH = 0xB,
		NODE_CONCEALMENT_PRONE = 0xC,
		NODE_DOOR = 0xD,
		NODE_DOOR_INTERIOR = 0xE,
		NODE_SCRIPTED = 0xF,
		NODE_NEGOTIATION_BEGIN = 0x10,
		NODE_NEGOTIATION_END = 0x11,
		NODE_TURRET = 0x12,
		NODE_GUARD = 0x13,
		NODE_PATHNODE_3D = 0x14,
		NODE_COVER_3D = 0x15,
		NODE_COVER_STAND_3D = 0x16,
		UNUSED_0 = 0x17,
		UNUSED_1 = 0x18,
		NODE_EXPOSED_3D = 0x19,
		NODE_SCRIPTED_3D = 0x1A,
		NODE_NEGOTIATION_BEGIN_3D = 0x1B,
		NODE_NEGOTIATION_END_3D = 0x1C,
		NODE_JUMP = 0x1D,
		NODE_JUMP_ATTACK = 0x1E,
		NODE_COVER_MULTI = 0x1F,
		NODE_NUMTYPES = 0x20,
	};

	struct pathnode_constant_t
	{
		unsigned short type;
		unsigned int spawnflags;
		scr_string_t targetname;
		scr_string_t script_linkName;
		scr_string_t script_noteworthy;
		scr_string_t target;
		scr_string_t animscript;
		int animscriptfunc;
		vec3_t vLocalOrigin;
		PathNodeOrientationUnion orientation;
		PathNodeParentUnion parent;
		$96C58BBB7F3ED2CCC5F07A7E763FE6AE ___u11;
		unsigned short wOverlapNode[2];
		unsigned short totalLinkCount;
		pathlink_s* Links;
	}; assert_sizeof(pathnode_constant_t, 80);
	assert_offsetof(pathnode_constant_t, parent, 56);
	assert_offsetof(pathnode_constant_t, animscript, 24);
	assert_offsetof(pathnode_constant_t, Links, 72);

	struct SentientHandle
	{
		unsigned short number;
		unsigned short infoIndex;
	};

	struct pathnode_dynamic_t
	{
		SentientHandle pOwner;
		int iFreeTime;
		int iValidTime[3];
		short wLinkCount;
		short wOverlapCount;
		short turretEntNumber;
		unsigned char userCount;
		unsigned char hasBadPlaceLink;
		int spreadUsedTime[2];
		short flags;
		short dangerousCount;
		int recentUseProxTime;
	};

	union $73F238679C0419BE2C31C6559E8604FC
	{
		float nodeCost;
		int linkIndex;
	};

	struct pathnode_transient_t
	{
		int iSearchFrame;
		pathnode_t* pNextOpen;
		pathnode_t* pPrevOpen;
		pathnode_t* pParent;
		float fCost;
		float fHeuristic;
		$73F238679C0419BE2C31C6559E8604FC ___u6;
	};

	struct pathnode_t
	{
		pathnode_constant_t constant;
		pathnode_dynamic_t dynamic;
		pathnode_transient_t transient;
	}; assert_sizeof(pathnode_t, 176);

	struct pathnode_tree_nodes_t
	{
		int nodeCount;
		unsigned short* nodes;
	};

	union pathnode_tree_info_t
	{
		pathnode_tree_t* child[2];
		pathnode_tree_nodes_t s;
	};

	struct pathnode_tree_t
	{
		int axis;
		float dist;
		pathnode_tree_info_t u;
	};

	struct PathDynamicNodeGroup
	{
		unsigned short parentIndex;
		int nodeTreeCount;
		pathnode_tree_t* nodeTree;
	};

	struct PathData
	{
		const char* name;
		unsigned int nodeCount;
		pathnode_t* nodes;
		bool parentIndexResolved;
		unsigned short version;
		int visBytes;
		char* pathVis;
		int nodeTreeCount;
		pathnode_tree_t* nodeTree;
		int dynamicNodeGroupCount;
		PathDynamicNodeGroup* dynamicNodeGroups;
		int exposureBytes;
		char* pathExposure;
		int noPeekVisBytes;
		char* pathNoPeekVis;
		int zoneCount;
		int zonesBytes;
		char* pathZones;
		int fixedNodeCount;
		int maxDynamicSpawnedNodeCount;
		int dynStatesBytes;
		char* pathDynStates;
	}; assert_sizeof(PathData, 0x90);

	struct nav_resource_s;

	struct nav_resource_s
	{
		scr_string_t targetName;
		int modelIdx;
		int targetEntNum;
		void* pSpace;
		int graphSize;
		vec3_t localOffsetPos;
		vec4_t localOffsetRot;
		int offsetLayer;
		nav_resource_s* pPrev;
		nav_resource_s* pNext;
		//int buildError;
		unsigned int layerFlags;
		bool bDockable;
		bool bIsVolume;
		char* pGraphBuffer;
		char* pWorkingGraph;
	}; assert_sizeof(nav_resource_s, 104);
	assert_offsetof(nav_resource_s, pGraphBuffer, 88);

	struct nav_boundary_plane_s
	{
		vec3_t m_Normal;
		float m_DistFromCenter;
	};

	struct nav_obstacle_hull_s
	{
		nav_boundary_plane_s* m_Boundaries;
		int m_NumBoundaries;
		float m_BoundsX;
		float m_BoundsY;
		float m_BoundsZ;
		vec3_t m_Offset;
		bool m_bUseBounds;
	};

	struct nav_obstacle_bounds_s
	{
		nav_obstacle_hull_s* m_Hulls;
		int m_NumHulls;
	};

	struct nav_glass_bounds_s
	{
		nav_boundary_plane_s* m_Boundaries;
		int m_NumBoundaries;
		vec3_t m_Pos;
		unsigned int m_GlassPieceIndex;
	};

	struct nav_modifier_s
	{
		nav_boundary_plane_s* m_Boundaries;
		int m_NumBoundaries;
		vec3_t m_Pos;
		scr_string_t m_TargetName;
		//scr_string_t m_Target;
		//scr_string_t m_ScriptNoteworthy;
		//scr_string_t m_ScriptLinkName;
		unsigned int m_UserFlags;
		unsigned int m_LayerFlags;
		float m_Weight;
		bool m_bActiveOnLoad;
	}; assert_sizeof(nav_modifier_s, 48);
	assert_offsetof(nav_modifier_s, m_TargetName, 24);

	struct nav_link_point_s
	{
		vec3_t m_Pt1;
		vec3_t m_Pt2;
	};

	struct nav_link_creation_data_s
	{
		nav_link_point_s m_Start;
		nav_link_point_s m_End;
		scr_string_t m_Animscript;
		scr_string_t m_Target;
		scr_string_t m_Parent;
		unsigned int m_UsageFlags;
		float m_PenaltyMult;
		bool m_bBidirectional;
	}; assert_sizeof(nav_link_creation_data_s, 72);
	assert_offsetof(nav_link_creation_data_s, m_Animscript, 48);

	struct nav_raw_volume_s
	{
		vec3_t m_MidPoint;
		vec3_t m_HalfSize;
		float m_MinVoxelSize;
		float m_ClearanceXY;
		float m_ClearanceZ;
	}; assert_sizeof(nav_raw_volume_s, 36);

	struct nav_raw_custom_volume_tri_s
	{
		vec3_t m_Verts[3];
	}; assert_sizeof(nav_raw_custom_volume_tri_s, 36);

	struct nav_raw_custom_volume_s
	{
		nav_raw_custom_volume_tri_s* m_Tris;
		int m_NumTris;
	}; assert_sizeof(nav_raw_custom_volume_s, 16);

	struct NavMeshData
	{
		const char* name;
		int version;
		int hasExposureData;
		int numNavResources;
		nav_resource_s* navResources;
		int numObstacleBounds;
		nav_obstacle_bounds_s* obstacleBounds;
		int numGlassBounds;
		nav_glass_bounds_s* glassBounds;
		int numModifiers;
		nav_modifier_s* modifiers;
		int numLinkCreationData;
		nav_link_creation_data_s* linkCreationData;
		int numVolumeSeeds;
		vec3_t* volumeSeeds;
		int numRawVolumes;
		nav_raw_volume_s* rawVolumes;
		int numRawCustomVolumes;
		nav_raw_custom_volume_s* rawCustomVolumes;
	}; assert_sizeof(NavMeshData, 0x90);
	assert_offsetof(NavMeshData, rawCustomVolumes, 136);

	struct CTAudRvbPanInfo
	{
		bool hasCustomPosition;
		vec3_t position;
		float maxWallDistance;
		float minReverbVolume;
	};

	struct ClientEntityLinkToDef
	{
		int anchorIndex;
		vec3_t originOffset;
		vec3_t angleOffset;
	};

	struct ClientTriggers
	{
		MapTriggers trigger;
		unsigned int triggerStringLength;
		char* triggerString;
		short* visionSetTriggers;
		unsigned char* triggerType;
		vec3_t* origins;
		float* scriptDelay;
		short* audioTriggers;
		short* blendLookup;
		short* npcTriggers;
		short* audioStateIds;
		CTAudRvbPanInfo* audioRvbPanInfo;
		short* transientIndex; // guess
		ClientEntityLinkToDef** linkTo;
	}; assert_sizeof(ClientTriggers, 0xB8);

	struct ClientTriggerBlendNode
	{
		vec3_t pointA;
		vec3_t pointB;
		unsigned short triggerA;
		unsigned short triggerB;
	};

	struct ClientTriggerBlend
	{
		unsigned short numClientTriggerBlendNodes;
		ClientTriggerBlendNode* blendNodes;
	};

	struct SpawnPointEntityRecord
	{
		unsigned short index;
		scr_string_t name;
		scr_string_t target;
		scr_string_t script_noteworthy;
		vec3_t origin;
		vec3_t angles;
	};

	struct SpawnPointRecordList
	{
		unsigned short spawnsCount;
		SpawnPointEntityRecord* spawns;
	};

	struct SplinePointEntityRecord
	{
		int splineId;
		int splineNodeId;
		scr_string_t splineNodeLabel;
		scr_string_t targetname;
		scr_string_t target;
		scr_string_t string;
		float speed;
		float splineNodeTension;
		vec3_t origin;
		vec3_t angles;
		float throttle;
		vec2_t corridorDims;
		vec3_t tangent;
		float distToNextNode;
		vec3_t positionCubic[4];
		vec3_t tangentQuadratic[3];
	};

	struct SplinePointRecordList
	{
		unsigned short splinePointCount;
		float splineLength;
		SplinePointEntityRecord* splinePoints;
	};

	struct SplineRecordList
	{
		unsigned short splineCount;
		SplinePointRecordList* splines;
	};

	struct cmodel_t
	{
		Bounds bounds;
		float radius;
		ClipInfo* info;
		PhysicsAsset* physicsAsset;
		unsigned short physicsShapeOverrideIdx;
		unsigned short navObstacleIdx;
		unsigned int edgeFirstIndex;
		//unsigned int edgeTotalCount;
	}; assert_sizeof(cmodel_t, 56);

	enum DynEntityType : std::int8_t
	{
		DYNENT_TYPE_INVALID = 0x0,
		DYNENT_TYPE_CLUTTER = 0x1,
		DYNENT_TYPE_CLUTTER_NOSHADOW = 0x2,
		DYNENT_TYPE_HINGE = 0x3,
		DYNENT_TYPE_SCRIPTABLEINST = 0x4,
		DYNENT_TYPE_SCRIPTABLEPHYSICS = 0x5,
		DYNENT_TYPE_LINKED = 0x6,
		DYNENT_TYPE_LINKED_NOSHADOW = 0x7,
		DYNENT_TYPE_COUNT = 0x8,
	};

	enum DynEntityBasis
	{
		DYNENT_BASIS_MODEL = 0x0,
		DYNENT_BASIS_BRUSH = 0x1,
		DYNENT_BASIS_COUNT = 0x2,
	};

	struct GfxPlacement
	{
		vec4_t quat;
		vec3_t origin;
	};

	struct DynEntityLinkToDef
	{
		int anchorIndex;
		float originOffset[3];
		float angleOffset[3];
	};

	struct DynEntityDef
	{
		DynEntityType type;
		char __pad0[31];
		GfxPlacement pose;
		char __pad1[4];
		XModel* baseModel;
		unsigned short brushModel;
		bool spawnActive;
		char __pad2[1];
		short instanceIndex;
		char __pad3[10];
		DynEntityLinkToDef* linkTo;
		bool noPhysics;
		bool unk1;
		bool unk2;
		bool distantShadows;
		bool noSpotShadows;
		bool isTransient;
		bool transientZoneLoaded;
		char unk3;
		char priority;
		char __pad6[7];
	}; assert_sizeof(DynEntityDef, 112);
	assert_offsetof(DynEntityDef, baseModel, 64);
	assert_offsetof(DynEntityDef, linkTo, 88);

	struct GpuLightGridRequestRecord
	{
		unsigned int lgvFrame;
		unsigned int lgvHistoryEntry;
		unsigned int lgvNumProbes;
	};

	struct DynEntityPose
	{
		GfxPlacement pose;
		float radius;
		GpuLightGridRequestRecord lastGpuLightGridRequest;
		unsigned int numPoses;
		GfxPlacement* poses;
		char* unk;
		char __pad1[1];
		bool cachedActive;
		char __pad2[6];
	}; assert_sizeof(DynEntityPose, 72);
	assert_offsetof(DynEntityPose, unk, 56);

	struct DynEntityClient
	{
		unsigned short flags;
		char __pad0[6];
		XModel* activeModel;
		int physicsSystemId;
		int physicsSystemDetailId;
		char numPhysicsBodies;
		int singlePhysicsBody;
		int detailBoundBody;
		int unkId;
		int dynEntDefId;
		int locked;
		char __pad3[8];
	}; assert_sizeof(DynEntityClient, 56);

	struct DynEntityGlobalId
	{
		char basis;
		unsigned int id;
	}; assert_sizeof(DynEntityGlobalId, 8);

	struct unk_1453E4280
	{
		char __pad0[8];
	}; assert_sizeof(unk_1453E4280, 8);

	struct unk_1453E4298
	{
		char __pad0[28];
		int unkIndex;
		unsigned int unk01Count;
		unk_1453E4280* unk01;
	}; assert_sizeof(unk_1453E4298, 48);

	struct unk_1453E42A8
	{
		int unkIndex;
		int unk;
	}; assert_sizeof(unk_1453E42A8, 8);

	struct unk_1453E1130
	{
		char unk;
	}; assert_sizeof(unk_1453E1130, 1);

	struct ClientEntAnchor
	{
		scr_string_t name;
		int entNum;
	}; assert_sizeof(ClientEntAnchor, 8);

	struct unk_1453E24B0
	{
		XModel* model;
		bool neverMoves;
		bool dynamicSimulation;
		char __pad0[6];
	}; assert_sizeof(unk_1453E24B0, 16);

	struct unk_1453E2510
	{
		ScriptableDef* def;
		char __pad0[52];
		short entityId;
		char __pad1[10];
		unk_1453E24B0 unk01;
		unsigned int eventStreamBufferSize;
		char* eventStreamBuffer;
		char __pad2[4];
	}; assert_sizeof(unk_1453E2510, 112);

	struct unk_1453E2520
	{
		unk_1453E2510 unk01;
		char __pad0[16];
	}; assert_sizeof(unk_1453E2520, 128);

	struct unk_1453E2530
	{
		unk_1453E2510 unk01;
		char __pad0[40];
	}; assert_sizeof(unk_1453E2530, 152);

	struct ScriptableInstance
	{
		unk_1453E2520 unk01;
		unk_1453E2530 unk02[2];
		scr_string_t unk03;
		int flags;
		char __pad0[8];
		const char* unk04;
		scr_string_t targetname;
	}; assert_sizeof(ScriptableInstance, 464);

	struct ScriptableReservedDynent
	{
		unsigned int dynentId;
		short next;
	};

	struct ScriptableReservedDynents
	{
		short freeHead;
		unsigned int numReservedDynents;
		ScriptableReservedDynent* reservedDynents;
	};

	struct unk_1453E2558
	{
		int id;
	};

	struct unk_1453E2560
	{
		int unk01Count;
		unk_1453E2558* unk01;
		int unk02Count;
		unk_1453E2558* unk02_1;
		unk_1453E2558* unk02_2;
	};

	struct Scriptable_EventSun_Data
	{
		int startTime;
		int transTime;
		vec3_t startColorLinearSrgb;
		float startIntensity;
		float targetIntensity;
		vec3_t startAngles;
		vec3_t targetAngles;
	};

	struct ScriptableMapEnts
	{
		unsigned int totalInstanceCount;
		unsigned int runtimeInstanceCount;
		unsigned int reservedInstanceCount;
		unsigned int pad;
		ScriptableInstance* instances;
		unk_1453E2560 unk;
		Scriptable_EventSun_Data sunClientDatas[2];
		ScriptableReservedDynents reservedDynents[2];
		unsigned int ffMemCost;
	}; assert_sizeof(ScriptableMapEnts, 0xD0);
	assert_offsetof(ScriptableMapEnts, unk, 24);
	assert_offsetof(ScriptableMapEnts, reservedDynents, 168);

	struct MayhemInstance
	{
		GfxPlacement initialPose;
		MayhemData* mayhem;
		ClientEntityLinkToDef* linkTo;
		scr_string_t scriptName;
		float curTime;
		float oldTime;
		unsigned int initialFlags;
		unsigned int flags;
		unsigned int mapEntLookup;
		//unsigned short transientIndexStored;
	}; assert_sizeof(MayhemInstance, 72);

	enum SpawnerFieldType : std::uint8_t // VariableType
	{
		SF_TYPE_UNDEFINED = 0x0,
		SF_TYPE_STRING = 0x2,
		SF_TYPE_ISTRING = 0x3,
		SF_TYPE_VECTOR = 0x4,
		SF_TYPE_FLOAT = 0x5,
		SF_TYPE_INTEGER = 0x6,
	};

	struct SpawnerField
	{
		scr_string_t key;
		unsigned int keyCanonical;
		scr_string_t value;
		unsigned char type;
	}; assert_sizeof(SpawnerField, 16);

	struct Spawner
	{
		vec3_t origin;
		vec3_t angles;
		SpawnerField* fields;
		int flags;
		int count;
		unsigned int numFields;
		scr_string_t targetname;
		scr_string_t classname;
	}; assert_sizeof(Spawner, 56);

	struct SpawnerList
	{
		unsigned int spawnerCount;
		Spawner* spawnerList;
	};

	struct AudioPASpeaker
	{
		vec3_t origin;
		unsigned int nameHash;
		unsigned int entChannelHash;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		ClientTriggers clientTrigger;
		ClientTriggerBlend clientTriggerBlend;
		SpawnPointRecordList spawnList;
		SplineRecordList splineList;
		unsigned int havokEntsShapeDataSize;
		char* havokEntsShapeData;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned short dynEntCount[2];
		unsigned short dynEntCountTotal;
		DynEntityDef* dynEntDefList[2];
		DynEntityPose* dynEntPoseList[2][2];
		DynEntityClient* dynEntClientList[2][2]; // runtime data
		short unkIndexes[8];
		DynEntityGlobalId* dynEntGlobalIdList[2];
		char __pad1[8];
		unsigned int unk2Count;
		unk_1453E4298* unk2;
		unk_1453E42A8* unk2_1[2];
		unk_1453E1130* unk2_2[2];
		unsigned int unk3Count;
		unsigned int* unk3;
		unsigned int clientEntAnchorCount;
		ClientEntAnchor* clientEntAnchors;
		ScriptableMapEnts scriptableMapEnts;
		unsigned int numMayhemScenes;
		MayhemInstance* mayhemScenes;
		SpawnerList spawners;
		unsigned int audioPASpeakerCount;
		AudioPASpeaker* audioPASpeakers;
	}; assert_sizeof(MapEnts, 0x340);
	assert_offsetof(MapEnts, havokEntsShapeDataSize, 336);
	assert_offsetof(MapEnts, cmodels, 360);
	assert_offsetof(MapEnts, dynEntDefList, 376);
	assert_offsetof(MapEnts, dynEntGlobalIdList, 472);
	assert_offsetof(MapEnts, unk2, 504);
	assert_offsetof(MapEnts, scriptableMapEnts, 576);
	assert_offsetof(MapEnts, audioPASpeakers, 824);

	struct FxGlassDef
	{
		float halfThickness;
		vec2_t texVecs[2];
		GfxColor color;
		Material* material;
		Material* materialShattered;
		PhysicsAsset* physicsAsset;
		FxCombinedDef pieceBreakEffect;
		FxCombinedDef shatterEffect;
		FxCombinedDef shatterSmallEffect;
		FxCombinedDef crackDecalEffect;
		SndAliasLookup damagedSound;
		SndAliasLookup destroyedSound;
		SndAliasLookup destroyedQuietSound;
		float highMipRadiusInvSq;
		float shatteredHighMipRadiusInvSq;
		int numCrackRings;
		bool isOpaque;
	};

	struct FxSpatialFrame
	{
		vec4_t quat;
		vec3_t origin;
	};

	struct $03A8A7B39FA20F64B5AB79125E07CD62
	{
		FxSpatialFrame frame;
		float radius;
	};

	union FxGlassPiecePlace
	{
		$03A8A7B39FA20F64B5AB79125E07CD62 __s0;
		unsigned int nextFree;
	};

	struct FxGlassPieceState
	{
		float texCoordOrigin[2];
		unsigned int supportMask;
		unsigned short initIndex;
		unsigned short geoDataStart;
		unsigned short lightingIndex;
		unsigned char defIndex;
		unsigned char pad[3];
		unsigned char vertCount;
		unsigned char holeDataCount;
		unsigned char crackDataCount;
		unsigned char fanDataCount;
		unsigned short flags;
		float areaX2;
	};

	struct FxGlassPieceDynamics
	{
		int fallTime;
		unsigned int physicsInstance;
		unsigned int physicsDetailInstance;
		vec3_t vel;
		vec3_t avel;
		bool pendingCreation;
	};

	struct FxGlassVertex
	{
		short x;
		short y;
	};

	struct FxGlassHoleHeader
	{
		unsigned short uniqueVertCount;
		unsigned char touchVert;
		unsigned char pad[1];
	};

	struct FxGlassCrackHeader
	{
		unsigned short uniqueVertCount;
		unsigned char beginVertIndex;
		unsigned char endVertIndex;
	};

	union FxGlassGeometryData
	{
		FxGlassVertex vert;
		FxGlassHoleHeader hole;
		FxGlassCrackHeader crack;
		unsigned char asBytes[4];
		short anonymous[2];
	};

	struct FxGlassInitPieceState
	{
		FxSpatialFrame frame;
		float radius;
		vec2_t texCoordOrigin;
		unsigned int supportMask;
		float areaX2;
		unsigned short lightingIndex;
		unsigned char defIndex;
		unsigned char vertCount;
		unsigned char fanDataCount;
		unsigned char pad[1];
	};

	struct FxGlassSystem
	{
		int time;
		int prevTime;
		unsigned int defCount;
		unsigned int pieceLimit;
		unsigned int pieceWordCount;
		unsigned int initPieceCount;
		unsigned int cellCount;
		unsigned int activePieceCount;
		unsigned int firstFreePiece;
		unsigned int geoDataLimit;
		unsigned int geoDataCount;
		unsigned int initGeoDataCount;
		FxGlassDef* defs;
		FxGlassPiecePlace* piecePlaces;
		FxGlassPieceState* pieceStates;
		FxGlassPieceDynamics* pieceDynamics;
		FxGlassGeometryData* geoData;
		unsigned int* isInUse;
		unsigned int* cellBits;
		unsigned char* visData;
		vec3_t* linkOrg;
		float* halfThickness;
		unsigned short* lightingHandles;
		FxGlassInitPieceState* initPieceStates;
		FxGlassGeometryData* initGeoData;
		bool needToCompactData;
		unsigned char initCount;
		float effectChanceAccum;
		int lastPieceDeletionTime;
	};

	struct FxWorld
	{
		const char* name;
		FxGlassSystem glassSys;
	};

	struct GfxPortal;

	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		unsigned char skySamplerState;
	}; assert_sizeof(GfxSky, 32);

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s* planes;
		unsigned short* nodes;
		unsigned int* sceneEntCellBits;
	}; assert_sizeof(GfxWorldDpvsPlanes, 32);

	struct GfxPortalWritable
	{
		bool isQueued;
		bool isAncestor;
		char recursionDepth;
		char hullPointCount;
		vec2_t* hullPoints;
		GfxPortal* queuedParent;
	};

	struct DpvsPlane
	{
		vec4_t coeffs;
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		vec3_t* vertices;
		unsigned short cellIndex;
		unsigned short closeDistance;
		char vertexCount;
		vec3_t hullAxis[2];
	};

	struct GfxCell
	{
		Bounds bounds;
		unsigned short portalCount;
		GfxPortal* portals;
	};

	struct GfxCellTransientInfo
	{
		unsigned short aabbTreeIndex;
		unsigned short transientZone;
	};

	struct GfxReflectionProbe
	{
		char* livePath;
		vec3_t origin;
		vec3_t angles;
		unsigned int* probeInstances;
		unsigned int probeInstanceCount;
		unsigned int probeRelightingIndex;
	}; assert_sizeof(GfxReflectionProbe, 48);

	struct GfxReflectionProbeRelightingData
	{
		unsigned int reflectionProbeIndex;
		unsigned short relightingFlags;
		unsigned int gBufferAlbedoImageIndex;
		unsigned int gBufferNormalImageIndex;
		//unsigned int gBufferSecondaryDiffuseImageIndex;
		float relightingScale;
	}; assert_sizeof(GfxReflectionProbeRelightingData, 20);

	struct GfxReflectionProbeObb
	{
		vec3_t center;
		vec3_t xAxis;
		vec3_t yAxis;
		vec3_t zAxis;
		vec3_t halfSize;
	};

	struct GfxReflectionProbeInstance
	{
		char* livePath;
		char* livePath2;
		vec3_t probePosition;
		unsigned short probeImageIndex;
		char lodLevel;
		char flags;
		vec4_t probeRotation;
		GfxReflectionProbeObb volumeObb;
		float priority;
		vec3_t feather;
		vec3_t expandProjectionNeg;
		vec3_t expandProjectionPos;
	}; assert_sizeof(GfxReflectionProbeInstance, 152);

	struct GfxRawTexture : GfxTexture
	{
	}; assert_sizeof(GfxRawTexture, 24);

	struct GfxReflectionProbeSampleData
	{
		float unk[8];
	}; assert_sizeof(GfxReflectionProbeSampleData, 32);

	struct GfxWorldReflectionProbeData
	{
		unsigned int reflectionProbeCount;
		unsigned int sharedReflectionProbeCount;
		GfxReflectionProbe* reflectionProbes;
		GfxImage* reflectionProbeArrayImage;
		unsigned int probeRelightingCount;
		GfxReflectionProbeRelightingData* probeRelightingData;
		unsigned int reflectionProbeGBufferImageCount;
		GfxImage** reflectionProbeGBufferImages;
		GfxRawTexture* reflectionProbeGBufferTextures;
		unsigned int reflectionProbeInstanceCount;
		GfxReflectionProbeInstance* reflectionProbeInstances;
		GfxReflectionProbeSampleData* reflectionProbeLightgridSampleData;
		void* reflectionProbeLightgridSampleDataBuffer;
		void* reflectionProbeLightgridSampleDataBufferView;
		void* reflectionProbeLightgridSampleDataBufferRWView;
	}; assert_sizeof(GfxWorldReflectionProbeData, 112);

	struct GfxWorldLightmapReindexDataElement
	{
		unsigned int index;
		unsigned int posX;
		unsigned int posY;
		unsigned int width;
		unsigned int height;
	}; assert_sizeof(GfxWorldLightmapReindexDataElement, 20);

	struct GfxWorldPackedLightmap
	{
		unsigned int imageWidth;
		unsigned int imageHeight;
	};

	struct GfxWorldLightmapReindexData
	{
		unsigned int imagePixelSize;
		unsigned int reindexCount;
		GfxWorldLightmapReindexDataElement* reindexElement;
		unsigned int packedLightmapCount;
		GfxWorldPackedLightmap* packedLightmap;
	}; assert_sizeof(GfxWorldLightmapReindexData, 32);

	struct GfxDecalVolumeCollection
	{
		char __pad0[388];
	}; assert_sizeof(GfxDecalVolumeCollection, 388);

	struct GfxOrientedBoundingBox
	{
		vec3_t center;
		vec3_t xAxis;
		vec3_t yAxis;
		vec3_t zAxis;
		vec3_t halfSize;
	};

	enum VolumetricMaskType
	{
		VOLUMETRIC_MASK_UNUSED = 0x0,
		VOLUMETRIC_MASK_HEIGHTMAP = 0x1,
		VOLUMETRIC_MASK_OPACITYMAP = 0x2,
		VOLUMETRIC_MASK_TYPE_COUNT = 0x3,
	};

	enum VolumetricAxis
	{
		VOLUMETRIC_X_AXIS = 0x0,
		VOLUMETRIC_Y_AXIS = 0x1,
		VOLUMETRIC_Z_AXIS = 0x2,
		VOLUMETRIC_AXIS_COUNT = 0x3,
		VOLUMETRIC_NEGATVE_AXIS_FLAG = 0x4,
	};

	struct GfxVolumetricMask
	{
		VolumetricMaskType type;
		VolumetricAxis axis;
		GfxImage* image;
		vec2_t scale;
		vec2_t offset;
		vec2_t scroll;
	}; assert_sizeof(GfxVolumetricMask, 40);

	struct GfxVolumetric
	{
		char* livePath;
		unsigned int flags;
		GfxOrientedBoundingBox obb;
		float density;
		float falloff;
		GfxVolumetricMask masks[4];
	}; assert_sizeof(GfxVolumetric, 240);
	assert_offsetof(GfxVolumetric, masks, 80);

	struct GfxWorldVolumetrics
	{
		unsigned int volumetricCount;
		GfxVolumetric* volumetrics;
	};

	struct GfxWorldDraw
	{
		GfxWorldReflectionProbeData reflectionProbeData;
		GfxWorldLightmapReindexData lightmapReindexData;
		GfxImage* iesLookupTexture;
		unsigned int decalVolumeCollectionCount;
		GfxDecalVolumeCollection* decalVolumeCollections;
		GfxImage* lightmapOverridePrimary;
		GfxImage* lightmapOverrideSecondary;
		unsigned int lightMapCount;
		GfxLightMap** lightMaps;
		GfxTexture* lightmapTextures;
		void* unused1;
		void* unused2;
		void* unused3;
		unsigned int transientZoneCount;
		GfxWorldTransientZone* transientZones[32];
		unsigned int indexCount;
		unsigned short* indices;
		ID3D11Buffer* indexBuffer;
		GfxWorldVolumetrics volumetrics;
	}; assert_sizeof(GfxWorldDraw, 536);
	assert_offsetof(GfxWorldDraw, iesLookupTexture, 144);
	assert_offsetof(GfxWorldDraw, decalVolumeCollectionCount, 152);
	assert_offsetof(GfxWorldDraw, decalVolumeCollections, 160);
	assert_offsetof(GfxWorldDraw, transientZones, 240);
	assert_offsetof(GfxWorldDraw, indices, 504);
	assert_offsetof(GfxWorldDraw, indexBuffer, 512);
	assert_offsetof(GfxWorldDraw, volumetrics, 520);

	struct GfxLightGridTree
	{
		unsigned char maxDepth;
		int nodeCount;
		int leafCount;
		int coordMinGridSpace[3];
		int coordMaxGridSpace[3];
		int coordHalfSizeGridSpace[3];
		int defaultColorIndexBitCount;
		int defaultLightIndexBitCount;
		unsigned int* p_nodeTable;
		int leafTableSize;
		unsigned char* p_leafTable;
	};

	struct GfxLightGridColorsHDR
	{
		float rgb[56][3];
	}; assert_sizeof(GfxLightGridColorsHDR, 672);

	struct GfxProbeData
	{
		unsigned int data[16];
	}; assert_sizeof(GfxProbeData, 64);

	struct GfxGpuLightGridProbePosition
	{
		float origin[3];
	}; assert_sizeof(GfxGpuLightGridProbePosition, 12);

	struct GfxSHProbeData
	{
		unsigned __int16 coeffs[29];
		unsigned __int16 pad[3];
	};

	struct GfxGpuLightGridZone
	{
		unsigned int numProbes;
		unsigned int firstProbe;
		unsigned int numTetrahedrons;
		unsigned int firstTetrahedron;
		unsigned int firstVoxelTetrahedronIndex;
		//unsigned int voxelTetrahedronInternalNodeShift;
		unsigned int numVoxelTetrahedronIndices;
		GfxSHProbeData fallbackProbeData;
	}; assert_sizeof(GfxGpuLightGridZone, 88);

	struct GfxGpuLightGridTetrahedron
	{
		unsigned int indexFlags[4];
	}; assert_sizeof(GfxGpuLightGridTetrahedron, 16);

	struct GfxGpuLightGridTetrahedronNeighbors
	{
		unsigned int neighbors[4];
	}; assert_sizeof(GfxGpuLightGridTetrahedronNeighbors, 16);

	struct GfxGpuLightGridTetrahedronVisibility
	{
		unsigned int visibility[16];
	}; assert_sizeof(GfxGpuLightGridTetrahedronVisibility, 64);

	struct GfxGpuLightGridVoxelStartTetrahedron
	{
		unsigned int index;
	}; assert_sizeof(GfxGpuLightGridVoxelStartTetrahedron, 4);

	struct GfxLightGridProbeData
	{
		unsigned int gpuVisibleProbesCount;
		GfxGpuLightGridProbePosition* gpuVisibleProbePositions;
		GfxProbeData* gpuVisibleProbesData; // 64 * (count * 0x2000)
		void* gpuVisibleProbesBuffer;
		void* gpuVisibleProbesView;
		void* gpuVisibleProbesRWView;
		unsigned int probeCount;
		GfxProbeData* probes; // 64 * count
		void* probesBuffer;
		void* probesView;
		void* probesRWView;
		GfxGpuLightGridProbePosition* probePositions;
		void* probePositionsBuffer;
		void* probePositionsView;
		unsigned int zoneCount;
		GfxGpuLightGridZone* zones;
		unsigned int tetrahedronCount;
		unsigned int tetrahedronCountVisible;
		GfxGpuLightGridTetrahedron* tetrahedrons;
		void* tetrahedronBuffer;
		void* tetrahedronView;
		GfxGpuLightGridTetrahedronNeighbors* tetrahedronNeighbors;
		void* tetrahedronNeighborsBuffer;
		void* tetrahedronNeighborsView;
		GfxGpuLightGridTetrahedronVisibility* tetrahedronVisibility; // 64 * count
		void* tetrahedronVisibilityBuffer;
		void* tetrahedronVisibilityView;
		unsigned int voxelStartTetrahedronCount;
		GfxGpuLightGridVoxelStartTetrahedron* voxelStartTetrahedron;
		void* voxelStartTetrahedronBuffer;
		void* voxelStartTetrahedronView;
	}; assert_sizeof(GfxLightGridProbeData, 240);

	struct GfxLightGrid
	{
		int unk[9];
		int tableVersion;
		int paletteVersion;
		char rangeExponent8BitsEncoding;
		char rangeExponent12BitsEncoding;
		char rangeExponent16BitsEncoding;
		unsigned char stageCount;
		float* stageLightingContrastGain;
		unsigned int paletteEntryCount;
		int* paletteEntryAddress;
		unsigned int paletteBitstreamSize;
		unsigned char* paletteBitstream;
		GfxLightGridColorsHDR skyLightGridColors;
		GfxLightGridColorsHDR defaultLightGridColors;
		GfxLightGridTree tree;
		GfxLightGridProbeData probeData;
	}; assert_sizeof(GfxLightGrid, 0x6D8);
	assert_offsetof(GfxLightGrid, stageCount, 47);

	struct GfxVoxelTreeHeader
	{
		int rootNodeDimension[4];
		int nodeCoordBitShift[4];
		vec4_t boundMin;
		vec4_t boundMax;
	}; assert_sizeof(GfxVoxelTreeHeader, 0x40);

	struct GfxVoxelTopDownViewNode
	{
		int firstNodeIndex;
		int zMin;
		int zMax;
	}; assert_sizeof(GfxVoxelTopDownViewNode, 12);

	struct GfxVoxelInternalNode
	{
		int firstNodeIndex[2];
		int childNodeMask[2];
	}; assert_sizeof(GfxVoxelInternalNode, 16);

	struct GfxVoxelLeafNode
	{
		unsigned short lightListAddress;
	};

	struct GfxVoxelTree
	{
		Bounds zoneBound;
		int voxelTopDownViewNodeCount;
		int voxelInternalNodeCount;
		int voxelLeafNodeCount;
		int lightListArraySize;
		GfxVoxelTreeHeader* voxelTreeHeader;
		GfxVoxelTopDownViewNode* voxelTopDownViewNodeArray;
		GfxVoxelInternalNode* voxelInternalNodeArray;
		GfxVoxelLeafNode* voxelLeafNodeArray;
		unsigned short* lightListArray;
		unsigned int* voxelInternalNodeDynamicLightList; // 2 * count
		char __pad0[24];
	}; assert_sizeof(GfxVoxelTree, 112);
	assert_offsetof(GfxVoxelTree, voxelTreeHeader, 40);
	assert_offsetof(GfxVoxelTree, voxelInternalNodeDynamicLightList, 80);

	struct GfxFrustumLights
	{
		unsigned int indexCount;
		unsigned short* indices;
		unsigned int vertexCount;
		char* vertices; // 32 * count
		void* indexBuffer;
		void* vertexBuffer;
	}; assert_sizeof(GfxFrustumLights, 48);

	struct GfxLightViewFrustum
	{
		unsigned int planeCount;
		vec4_t* planes;
		unsigned int indexCount;
		unsigned short* indices;
		unsigned int vertexCount;
		vec3_t* vertices;
	}; assert_sizeof(GfxLightViewFrustum, 48);

	struct GfxHeightfield
	{
		Bounds bounds;
		vec4_t lookupMatrix[4];
		GfxImage* image;
	}; assert_sizeof(GfxHeightfield, 96);

	struct unk_1453E4B08
	{
		unsigned int unk01Count;
		unsigned int* unk01; // unused
		unsigned int unk02Count;
		unsigned int* unk02; // unused
		unsigned int unk03Count;
		unsigned short* unk03;
	}; assert_sizeof(unk_1453E4B08, 48);

	struct GfxBrushModelWritable
	{
		Bounds bounds;
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable;
		Bounds bounds;
		float radius;
		unsigned int startSurfIndex;
		unsigned short surfaceCount;
	}; assert_sizeof(GfxBrushModel, 60);

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct sunflare_t
	{
		bool hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	}; assert_sizeof(sunflare_t, 112);

	struct XModelDrawInfo
	{
		unsigned int hasGfxEntIndex : 1;
		unsigned int lod : 4;
		unsigned int materialLod : 1;
		unsigned int surfBufSize : 24;
		unsigned int surfId : 24;
	};

#pragma pack(push, 2)
	struct GfxSceneDynModel
	{
		XModelDrawInfo info;
		unsigned short dynEntId;
	};
#pragma pack(pop)

	struct BModelDrawInfo
	{
		unsigned int surfId;
	};

#pragma pack(push, 2)
	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned short dynEntId;
	};
#pragma pack(pop)

	struct GfxShadowGeometry
	{
		unsigned short surfaceCount;
		unsigned short smodelCount;
		unsigned int* sortedSurfIndex;
		unsigned short* smodelIndex;
	}; assert_sizeof(GfxShadowGeometry, 24);

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis* axis;
	};

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull* hulls;
	};

	struct GfxLightAABBNode
	{
		Bounds bound;
		unsigned short firstChild;
		unsigned short childCount;
	}; assert_sizeof(GfxLightAABBNode, 28);

	struct GfxLightAABB
	{
		unsigned short nodeCount;
		unsigned short lightCount;
		GfxLightAABBNode* nodeArray;
		unsigned short* lightArray;
	}; assert_sizeof(GfxLightAABB, 24);

	struct GfxStaticModelInst
	{
		Bounds bounds;
		float lightingOrigin[3];
	};

	struct srfTriangles_t
	{
		unsigned int vertexLayerData;
		unsigned int firstVertex;
		float maxEdgeLength;
		unsigned short vertexCount;
		unsigned short triCount;
		unsigned int baseIndex;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material* material;
		unsigned char lightmapIndex;
		unsigned char flags;
		unsigned short sortKey;
		unsigned char unk1;
		unsigned char unk2;
		unsigned char unk3;
		unsigned char unk4;
		unsigned char transientZone;
	}; assert_sizeof(GfxSurface, 48);
	assert_offsetof(GfxSurface, material, 24);

	struct GfxPackedPlacement
	{
		float origin[3];
		float axis[3][3];
		float scale;
	};

	struct GfxStaticModelVertexLighting
	{
		unsigned char visibility[4];
		unsigned short ambientColorFloat16[4];
		unsigned short highlightColorFloat16[4];
	};

	struct GfxStaticModelVertexLightingInfo
	{
		GfxStaticModelVertexLighting* lightingValues;
		ID3D11Buffer* smodelLightingVb;
		ID3D11Buffer* lightingValuesVb;
		GfxSubdivCache cache;
		int flags;
		unsigned int numLightingValues;
	}; assert_sizeof(GfxStaticModelVertexLightingInfo, 56);

	struct GfxStaticModelLightmapInfo
	{
		float offset[2];
		float scale[2];
		int lightmapIndex;
	};

	enum StaticModelFlag : std::int32_t
	{
		// scale modifiers: 
		// 0 = 4.0f
		// 1 = 2.66667f
		// 2 = 2.0f
		// 3 = 1.6f
		// 4 = 1.33333f
		// 5 = 1.14286f
		// 6 = 1.0f
		// 7 = 0.888889f
		// 8 = 0.8f
		// 9 = 0.666667f
		// 10 = 0.571429f
		// 11 = 0.5f
		// 12 = 0.4f
		// 13 = 0.333333f
		// 14 = 0.285714f
		// 15 = 0.25f
		STATIC_MODEL_FLAG_SCALE_MODIFIER_MASK = 0xF,
		STATIC_MODEL_FLAG_NO_CAST_SHADOW = 0x10,
		STATIC_MODEL_FLAG_GROUND_LIGHTING = 0x20,
		STATIC_MODEL_FLAG_LIGHTGRID_LIGHTING = 0x40,
		STATIC_MODEL_FLAG_VERTEXLIT_LIGHTING = 0x80,
		STATIC_MODEL_FLAG_LIGHTMAP_LIGHTING = 0x100,
		STATIC_MODEL_FLAG_ALLOW_FXMARK = 0x200,
		STATIC_MODEL_FLAG_REACTIVEMOTION = 0x400,
		STATIC_MODEL_FLAG_ANIMATED_VERTS = 0x800,
	};

	struct GfxStaticModelDrawInst
	{
		GfxPackedPlacement placement;
		XModel* model;
		GfxStaticModelVertexLightingInfo vertexLightingInfo;
		GfxStaticModelLightmapInfo modelLightmapInfo;
		unsigned short unk0;
		unsigned short unk1;
		unsigned short unk2;
		unsigned short unk3;
		unsigned short lightingHandle;
		unsigned short unk5;
		unsigned short unk6[4];
		unsigned short cullDist;
		unsigned short flags;
		unsigned short unk9;
		unsigned short unk10;
		unsigned short unk11;
		unsigned char unk13;
		unsigned char unk14;
		unsigned char unk15[4];
		unsigned short primaryLightEnvIndex;
		unsigned char reflectionProbeIndex;
		unsigned char firstMtlSkinIndex;
		unsigned char sunShadowFlags;
		unsigned char transientZone;
		unsigned char unk21;
	}; assert_sizeof(GfxStaticModelDrawInst, 184);
	assert_offsetof(GfxStaticModelDrawInst, model, 56);
	assert_offsetof(GfxStaticModelDrawInst, vertexLightingInfo, 64);
	assert_offsetof(GfxStaticModelDrawInst, cullDist, 160);

	struct GfxSurfaceBounds
	{
		Bounds bounds;
		unsigned int unk[3];
	}; assert_sizeof(GfxSurfaceBounds, 36);

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int litOpaqueSurfsBegin;
		unsigned int litOpaqueSurfsEnd;
		unsigned int litDecalSurfsBegin;
		unsigned int litDecalSurfsEnd;
		unsigned int litTransSurfsBegin;
		unsigned int litTransSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		unsigned int primaryLightVisDataCount;
		unsigned int reflectionProbeVisDataCount;
		unsigned int volumetricVisDataCount;
		unsigned int decalVisDataCount;
		unsigned int* smodelVisData[30];
		unsigned int* surfaceVisData[30];
		unsigned int* lodData;
		unsigned int* primaryLightVisData[1];
		unsigned int* reflectionProbeVisData[1];
		unsigned int* volumetricVisData[1];
		unsigned int* decalVisData[1];
		unsigned int* tessellationCutoffVisData[30]; // only [0] used
		unsigned int* sortedSurfIndex;
		GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		GfxSurfaceBounds* surfacesBounds;
		GfxStaticModelDrawInst* smodelDrawInsts;
		GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		unsigned short* smodelUnk;
		unsigned int sunShadowOptCount;
		unsigned int sunSurfVisDataCount;
		unsigned int* surfaceCastsSunShadowOpt;
		char** constantBuffers;
		int usageCount;
	}; assert_sizeof(GfxWorldDpvsStatic, 920);

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int* dynEntCellBits[2];
		unsigned char* dynEntVisData[2][30];
	}; assert_sizeof(GfxWorldDpvsDynamic, 512);

	struct GfxHeroOnlyLight
	{
		unsigned char type;
		unsigned char unused[3];
		float color[3];
		float dir[3];
		float up[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	typedef void* umbraTomePtr_t;

	struct UmbraGate
	{
		unsigned int objID;
		Bounds bounds;
		float closeDistance;
	};

	struct UmbraGate2
	{
		char __pad0[40];
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		unsigned int bspVersion;
		int planeCount;
		int nodeCount;
		unsigned int surfaceCount;
		int skyCount;
		GfxSky* skies;
		unsigned int lastSunPrimaryLightIndex;
		unsigned int primaryLightCount;
		unsigned int movingScriptablePrimaryLightCount;
		unsigned int sortKeyLitDecal;
		unsigned int sortKeyEffectDecal;
		unsigned int sortKeyTopDecal;
		unsigned int sortKeyEffectAuto;
		unsigned int sortKeyDistortion;
		unsigned int sortKeyEffectDistortion;
		unsigned int sortKey2D;
		unsigned int sortKeyOpaqueBegin;
		unsigned int sortKeyOpaqueEnd;
		unsigned int sortKeyDecalBegin;
		unsigned int sortKeyDecalEnd;
		unsigned int sortKeyTransBegin;
		unsigned int sortKeyTransEnd;
		unsigned int sortKeyEmissiveBegin;
		unsigned int sortKeyEmissiveEnd;
		GfxWorldDpvsPlanes dpvsPlanes;
		GfxCellTransientInfo* cellTransientInfos;
		GfxCell* cells;
		GfxWorldDraw draw;
		GfxLightGrid lightGrid;
		GfxFrustumLights* frustumLights;
		GfxLightViewFrustum* lightViewFrustums;
		int voxelTreeCount;
		GfxVoxelTree* voxelTree;
		int heightfieldCount;
		GfxHeightfield* heightfields;
		unk_1453E4B08 unk01; // something related to surfaces
		int modelCount;
		GfxBrushModel* models;
		Bounds bounds;
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory* materialMemory;
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		Material* dustMaterial;
		float materialLod0SizeThreshold;
		unsigned int* cellCasterBits;
		unsigned int* cellHasSunLitSurfsBits;
		GfxSceneDynModel* sceneDynModel;
		GfxSceneDynBrush* sceneDynBrush;
		unsigned int primaryLightMotionDetectBitsEntries;
		unsigned int* primaryLightMotionDetectBits;
		unsigned int entityMotionBitsEntries;
		unsigned int* entityMotionBits;
		unsigned int staticSpotOmniPrimaryLightCountAligned;
		unsigned int numPrimaryLightEntityShadowVisEntries;
		unsigned int* primaryLightEntityShadowVis;
		unsigned int dynEntMotionBitsEntries[2];
		unsigned int* dynEntMotionBits[2];
		unsigned int numPrimaryLightDynEntShadowVisEntries[2];
		unsigned int* primaryLightDynEntShadowVis[2];
		GfxShadowGeometry* shadowGeomOptimized;
		GfxLightRegion* lightRegion;
		GfxLightAABB lightAABB;
		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;
		unsigned int mapVtxChecksum;
		unsigned int heroOnlyLightCount;
		GfxHeroOnlyLight* heroOnlyLights;
		unsigned int numUmbraGates;
		UmbraGate* umbraGates;
		unsigned int umbraTomeSize;
		char* umbraTomeData;
		umbraTomePtr_t umbraTomePtr;
		unsigned int numUmbraGates2;
		UmbraGate2* umbraGates2;
		unsigned int umbraTomeSize2;
		char* umbraTomeData2;
		umbraTomePtr_t umbraTomePtr2;
		unsigned int umbraUnkSize;
		char* umbraUnkData;
	}; assert_sizeof(GfxWorld, 0x11A8);
	assert_offsetof(GfxWorld, dpvsPlanes, 120);
	assert_offsetof(GfxWorld, voxelTreeCount, 2472);
	assert_offsetof(GfxWorld, models, 2560);
	assert_offsetof(GfxWorld, materialMemory, 2600);
	assert_offsetof(GfxWorld, outdoorImage, 2784);
	assert_offsetof(GfxWorld, cellHasSunLitSurfsBits, 2816);
	assert_offsetof(GfxWorld, sceneDynModel, 2824);
	assert_offsetof(GfxWorld, shadowGeomOptimized, 2936);
	assert_offsetof(GfxWorld, lightAABB, 2952);
	assert_offsetof(GfxWorld, heroOnlyLights, 4416);
	assert_offsetof(GfxWorld, numUmbraGates, 4424);
	assert_offsetof(GfxWorld, numUmbraGates2, 4464);
	assert_offsetof(GfxWorld, umbraUnkData, 4512);

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	}; assert_sizeof(GfxWorldVertex, 44);

	union GfxWorldVertex0Union
	{
		GfxWorldVertex* vertices;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		ID3D11Buffer* worldVb;
	}; assert_sizeof(GfxWorldVertexData, 16);

	struct GfxWorldVertexLayerData
	{
		unsigned char* data;
		ID3D11Buffer* layerVb;
	}; assert_sizeof(GfxWorldVertexLayerData, 16);

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
	};

	struct GfxAabbTree
	{
		Bounds bounds;
		unsigned short childCount;
		unsigned short surfaceCount;
		unsigned int startSurfIndex;
		unsigned short smodelIndexCount;
		unsigned short* smodelIndexes;
		int childrenOffset;
	}; assert_sizeof(GfxAabbTree, 56);

	struct GfxCellTree
	{
		GfxAabbTree* aabbTree;
	};

	struct GfxWorldTransientZone
	{
		const char* name;
		unsigned int transientZoneIndex;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		unsigned int cellCount;
		GfxCellTreeCount* aabbTreeCounts;
		GfxCellTree* aabbTrees;
	}; assert_sizeof(GfxWorldTransientZone, 0x50);

	struct GfxLightDef
	{
		const char* name;
		const char* iesProfile;
		unsigned int unk0;
		unsigned int unk1;
		float coordOffset;
		float coordScale;
		float unk2;
		int pad;
	}; assert_sizeof(GfxLightDef, 0x28);

	struct AnimationEntry
	{
		scr_string_t animName;
		int aimSetIndex;
	};

	struct AnimAliasInfo
	{
		char animIndex;
		float animWeight;
	};

	struct AnimAlias
	{
		scr_string_t aliasName;
		char animCount;
		AnimAliasInfo* aliasInfo;
	};

	struct AnimationState
	{
		scr_string_t name;
		scr_string_t notify;
		float blendTime;
		float blendOutTime;
		char flags;
		char entryCount;
		char aliasCount;
		int aimSetIndex;
		AnimationEntry* animEntries;
		unsigned __int64* animIndices;
		AnimAlias* aliasList;
	};

	struct AnimationAimSet
	{
		scr_string_t name;
		scr_string_t rootName;
		int animCount;
		scr_string_t* animName;
		unsigned __int64 rootIndex;
		unsigned __int64* animIndices;
		unsigned __int64* aimNodeIndices;
	};

	struct AnimationStateMachine
	{
		scr_string_t name;
		unsigned short stateCount;
		unsigned short aimSetCount;
		AnimationState* states;
		AnimationAimSet* aimSets;
	};

	enum AnimationController
	{
		ANIMCTRL_NONE = 0x0,
		ANIMCTRL_PLAYER = 0x1,
		ANIMCTRL_DOG = 0x2,
		ANIMCTRL_NUM = 0x3,
	};

	struct AnimationClass
	{
		const char* className;
		AnimationStateMachine* stateMachine;
		AnimationController animCtrl;
		scr_string_t animTree;
		ScriptableDef* scriptable;
		unsigned short soundCount;
		unsigned short effectCount;
		scr_string_t* soundNotes;
		scr_string_t* soundNames;
		scr_string_t* soundOptions;
		scr_string_t* effectNotes;
		FxCombinedDef* effectDefs;
		scr_string_t* effectTags;
	}; assert_sizeof(AnimationClass, 0x58);

	struct PlayerAnimEntry
	{
		scr_string_t entryName;
		scr_string_t animName;
		scr_string_t shadowAnimName;
		char __pad0[68];
	}; assert_sizeof(PlayerAnimEntry, 80);

	struct PlayerAnimScriptEntry
	{
		unsigned int flags;
		unsigned int itemCount;
		unsigned int transitionCount;
		unsigned int* items;
		unsigned int* transitions;
	};

	struct PlayerAnimScriptCondition
	{
		unsigned int index;
		unsigned int value[4];
	}; assert_sizeof(PlayerAnimScriptCondition, 20);

	struct PlayerAnimScriptCommand
	{
		unsigned short bodyPart;
		unsigned short animIndex;
		unsigned short animDuration;
	};

	struct PlayerAnimScriptItem
	{
		unsigned int conditionCount;
		unsigned int commandCount;
		PlayerAnimScriptCondition* conditions;
		PlayerAnimScriptCommand* commands;
	};

	enum PlayerAnimScriptTransitionTriggerType
	{
		ANIM_TRANSITIONTRIGGERTYPE_MOVETYPE = 0x0,
		ANIM_TRANSITIONTRIGGERTYPE_MOVETYPE_NOT_STRAFING = 0x1,
		ANIM_TRANSITIONTRIGGERTYPE_ANIMCOMPLETE = 0x2,
		ANIM_TRANSITIONTRIGGERTYPE_ANIMCHANGE = 0x3,
		NUM_ANIM_TRANSITION_TRIGGER_TYPES = 0x4,
	};

	enum PlayerAnimScriptMoveType
	{
		ANIM_MT_UNUSED = 0x0,
		ANIM_MT_IDLE = 0x1,
		ANIM_MT_IDLECR = 0x2,
		ANIM_MT_IDLEPRONE = 0x3,
		ANIM_MT_WALK = 0x4,
		ANIM_MT_WALKBK = 0x5,
		ANIM_MT_WALKCR = 0x6,
		ANIM_MT_WALKCRBK = 0x7,
		ANIM_MT_WALKPRONE = 0x8,
		ANIM_MT_WALKPRONEBK = 0x9,
		ANIM_MT_RUN = 0xA,
		ANIM_MT_RUNBK = 0xB,
		ANIM_MT_RUNCR = 0xC,
		ANIM_MT_RUNCRBK = 0xD,
		ANIM_MT_CLIMBUP = 0xE,
		ANIM_MT_LADDERSLIDE = 0xF,
		ANIM_MT_CLIMBDOWN = 0x10,
		ANIM_MT_JOG = 0x11,
		ANIM_MT_SPRINT = 0x12,
		ANIM_MT_SUPERSPRINT = 0x13,
		ANIM_MT_MANTLE = 0x14,
		ANIM_MT_IDLELASTSTAND = 0x15,
		ANIM_MT_CRAWLLASTSTAND = 0x16,
		ANIM_MT_CRAWLLASTSTANDBK = 0x17,
		ANIM_MT_SLIDE = 0x18,
		ANIM_MT_SLIDEBK = 0x19,
		ANIM_MT_MOUNTED_TOP = 0x1A,
		ANIM_MT_MOUNTED_LEFT = 0x1B,
		ANIM_MT_MOUNTED_RIGHT = 0x1C,
		ANIM_MT_BOOSTAIRDODGE = 0x1D,
		ANIM_MT_BOOSTAIRDODGELEFT = 0x1E,
		ANIM_MT_BOOSTAIRDODGERIGHT = 0x1F,
		ANIM_MT_BOOSTAIRDODGEBACK = 0x20,
		ANIM_MT_WALLRUN = 0x21,
		ANIM_MT_AIR = 0x22,
		ANIM_MT_KNOCKBACK = 0x23,
		ANIM_MT_KNOCKBACKBK = 0x24,
		ANIM_MT_PARACHUTE = 0x25,
		NUM_ANIM_CODEMOVETYPES = 0x26,
		FIRST_TRANSITION_MOVETYPE = 0x26,
		NUM_ANIM_MOVETYPES = 0x80,
	};

	struct PlayerAnimTriggerParam
	{
		PlayerAnimScriptMoveType moveType;
	};

	struct PlayerAnimScriptTransition
	{
		PlayerAnimScriptTransitionTriggerType triggerType;
		PlayerAnimTriggerParam triggerParam;
		PlayerAnimScriptMoveType targetMoveType;
	};

	struct PlayerAnimScriptIdleTurn
	{
		unsigned short turnleft;
		unsigned short turnright;
	};

	struct PlayerAnimScriptIdleTwitch
	{
		unsigned int twitchCount;
		float cycleLength;
		unsigned short* twitches;
	};

	struct PlayerAnimScriptAimSet
	{
		unsigned short aim_1;
		unsigned short aim_2;
		unsigned short aim_3;
		unsigned short aim_4;
		unsigned short aim_6;
		unsigned short aim_7;
		unsigned short aim_8;
		unsigned short aim_9;
	};

	struct PlayerAnimScriptLeanSet
	{
		unsigned short lean_left;
		unsigned short lean_right;
	};

	struct unk_1453E3788
	{
		char __pad0[48];
	};

	struct PlayerAnimScript
	{
		const char* scriptName;
		unsigned int animationCount;
		unsigned int scriptItemCount;
		unsigned int scriptTransitionCount;
		unsigned int scriptIdleTurnCount;
		unsigned int scriptIdleTwitchCount;
		unsigned int scriptAimSetCount;
		unsigned int scriptLeanSetCount;
		unsigned int unkCount;
		unsigned int torsoAnimCount;
		unsigned int legsAnimCount;
		PlayerAnimEntry* animations;
		PlayerAnimScriptEntry* scriptAnims; // count: 121
		PlayerAnimScriptEntry* scriptEvents; // count: 55
		PlayerAnimScriptItem* scriptItems;
		PlayerAnimScriptTransition* scriptTransitions;
		PlayerAnimScriptIdleTurn* scriptIdleTurns;
		PlayerAnimScriptIdleTwitch* scriptIdleTwitches;
		PlayerAnimScriptAimSet* scriptAimSets;
		PlayerAnimScriptLeanSet* scriptLeanSets;
		unk_1453E3788* unk;
		unsigned short* torsoAnimPackMap;
		unsigned short* torsoAnimUnpackMap;
		unsigned short* legsAnimPackMap;
		unsigned short* legsAnimUnpackMap;
		unsigned int xAnimCount;
		XAnimParts** xAnims;
	}; assert_sizeof(PlayerAnimScript, 0xB0);

	enum GestureType
	{
		GESTURE_TYPE_NON_DIRECTIONAL = 0x0,
		GESTURE_TYPE_DIRECTIONAL = 0x1,
		GESTURE_TYPE_LOOK_AROUND = 0x2,
		GESTURE_TYPE_IK_TARGET = 0x3,
		GESTURE_TYPE_NUM = 0x4,
	};

	enum GesturePriority
	{
		GESTURE_PRIORITY_OFFHAND_SHIELD = 0x0,
		GESTURE_PRIORITY_OFFHAND_THROWN_WEAPON = 0x1,
		GESTURE_PRIORITY_OFFHAND_SCRIPT_WEAPON = 0x2,
		GESTURE_PRIORITY_SPACE_JUMP = 0x3,
		GESTURE_PRIORITY_SCRIPT = 0x4,
		GESTURE_PRIORITY_SLIDE = 0x5,
		GESTURE_PRIORITY_MANTLE = 0x6,
		GESTURE_PRIORITY_GROUND_POUND = 0x7,
		GESTURE_PRIORITY_WALL_RUN = 0x8,
		GESTURE_PRIORITY_DEMEANOR = 0x9,
		GESTURE_PRIORITY_COUNT = 0xC,
	};

	struct GestureWeaponSettings
	{
		unsigned short blendToStates;
		bool hideReticle;
		float fireDelay;
		float sprintDelay;
		bool useLeftIdleAkimbo;
		bool splitAnimsAkimbo;
		bool blendToDemeanorLoop;
		bool blendOutRaise;
	};

	struct GestureDirectionalSettings
	{
		float maxAngle;
		float lerpAtMaxAngle;
		float widthCushionAngle;
		float lerpAtMinCushionAngle;
		float lerpAtMaxCushionAngle;
		float limitLeft;
		float limitRight;
		float limitUp;
		float limitDown;
	};

	struct FootstepTime
	{
		float time;
		bool isLeft;
	};

	struct MovementTime
	{
		float time;
		bool isLeadIn;
	};

	struct FootstepAnim
	{
		int leftCount;
		FootstepTime step[4];
		MovementTime movement[4];
	};

	struct GestureLookAroundSettings
	{
		float yawLerpIn;
		float yawLerpOut;
		unsigned short walkTime;
		FootstepAnim walkFootStepAnim;
	};

	enum GestureDirectionalAssetIndex
	{
		GESTURE_ASSET_LEFT_CENTER = 0,
		GESTURE_ASSET_LEFT_DOWN = 1,
		GESTURE_ASSET_LEFT_LEFT = 2,
		GESTURE_ASSET_LEFT_RIGHT = 3,
		GESTURE_ASSET_LEFT_UP = 4,
		GESTURE_ASSET_RIGHT_CENTER = 5,
		GESTURE_ASSET_RIGHT_DOWN = 6,
		GESTURE_ASSET_RIGHT_LEFT = 7,
		GESTURE_ASSET_RIGHT_RIGHT = 8,
		GESTURE_ASSET_RIGHT_UP = 9,
		GESTURE_ASSET_IN = 10,
		GESTURE_ASSET_OUT = 11,
		GESTURE_ASSET_IN_ADDITIVE = 12,
		GESTURE_ASSET_OUT_ADDITIVE = 13,
		GESTURE_ASSET_PITCH = 14,
		GESTURE_ASSET_YAW = 15,
		GESTURE_ASSET_WEAPON_CHECK = 16,
		GESTURE_ASSET_WALK_ADDITIVE = 17,
		GESTURE_ASSET_NUM = 18,
	};

	struct Gesture
	{
		const char* name;
		GestureType type;
		GesturePriority priority;
		bool looping;
		XAnimParts** anims; // array: 18
		GestureWeaponSettings weaponSettings;
		GestureDirectionalSettings directionalSettings;
		GestureLookAroundSettings lookAroundSettings;
	}; assert_sizeof(Gesture, 0xA8);
	assert_offsetof(Gesture, anims, 24);

	struct LocalizeEntry
	{
		const char* value;
		const char* name;
	};

	enum OffhandGestureTypes
	{
		OHGT_WEAPON,
		OHGT_PULLBACK,
		OHGT_THROW,
		OHGT_DETONATE,
		OHGT_SHIELD_DEPLOY,
		OHGT_SHIELD_DEPLOY_WHILE_FIRING,
		OHGT_SHIELD_FIRE_WEAPON,
		OHGT_SHIELD_RETRACT_WHILE_FIRING,
		OHGT_SHIELD_BASH,
		OHGT_NUM_TYPES,
		OHGT_ANY_TYPE,
	};

	enum AdsAltSwitchInterpType : std::int32_t
	{
		ADS_ALT_SWITCH_INTERP_TYPE_LINEAR = 0x0,
		ADS_ALT_SWITCH_INTERP_TYPE_COSINE = 0x1,
		ADS_ALT_SWITCH_INTERP_TYPE_SINE = 0x2,
		ADS_ALT_SWITCH_INTERP_TYPE_COUNT = 0x3,
	};

	enum AttachmentType : std::int32_t
	{
		ATTACHMENT_SCOPE = 0x0,
		ATTACHMENT_UNDERBARREL = 0x1,
		ATTACHMENT_OTHER = 0x2,
		ATTACHMENT_COUNT = 0x3,
	};

	enum weapType_t : std::int32_t
	{
		WEAPTYPE_NONE = 0x0,
		WEAPTYPE_BULLET = 0x1,
		WEAPTYPE_GRENADE = 0x2,
		WEAPTYPE_PROJECTILE = 0x3,
		WEAPTYPE_RIOTSHIELD = 0x4,
		WEAPTYPE_SCRIPT = 0x5,
		WEAPTYPE_SHIELD = 0x6,
		WEAPTYPE_CHARGE_SHIELD = 0x7,
		WEAPTYPE_UNK8 = 0x8,
		WEAPTYPE_UNK9 = 0x9,
		WEAPTYPE_UNK10 = 0xA,
		WEAPTYPE_UNK11 = 0xB,
		WEAPTYPE_EQUIP_DEPLOY = 0xC,
		WEAPTYPE_NUM = 0xD,
	};

	enum weapClass_t : std::int32_t
	{
		WEAPCLASS_RIFLE = 0x0,
		WEAPCLASS_SNIPER = 0x1,
		WEAPCLASS_MG = 0x2,
		WEAPCLASS_SMG = 0x3,
		WEAPCLASS_SPREAD = 0x4,
		WEAPCLASS_PISTOL = 0x5,
		WEAPCLASS_GRENADE = 0x6,
		WEAPCLASS_ROCKETLAUNCHER = 0x7,
		WEAPCLASS_TURRET = 0x8,
		WEAPCLASS_THROWINGKNIFE = 0x9,
		WEAPCLASS_NON_PLAYER = 0xA,
		WEAPCLASS_ITEM = 0xB,
		WEAPCLASS_BEAM = 0xC,
		WEAPCLASS_BALL = 0xD,
		WEAPCLASS_NONE = 0xE,
		WEAPCLASS_NUM = 0xF,
	};

	enum weapFireType_t : std::int32_t
	{
		WEAPON_FIRETYPE_FULLAUTO = 0x0,
		WEAPON_FIRETYPE_SINGLESHOT = 0x1,
		WEAPON_FIRETYPE_BURST = 0x2,
		WEAPON_FIRETYPE_DOUBLEBARREL = 0x3,
		WEAPON_FIRETYPE_BEAM = 0x4,
		WEAPON_FIRETYPECOUNT = 0x5,
	};

	enum ImpactType : std::int32_t
	{
		IMPACT_TYPE_NONE = 0,
		IMPACT_TYPE_COUNT = 37,
	};

	enum MeleeImpactType : std::int32_t
	{
		MELEE_IMPACT_TYPE_NONE = 0,
	};

	enum tracerStyle_t : std::int32_t
	{
		TRACERSTYLE_TRACER = 0x0,
		TRACERSTYLE_BEAM = 0x1,
		TRACERSTYLE_NUM = 0x2,
	};

	enum PenetrateType : std::int32_t
	{
		PENETRATE_TYPE_NONE = 0x0,
		PENETRATE_TYPE_SMALL = 0x1,
		PENETRATE_TYPE_MEDIUM = 0x2,
		PENETRATE_TYPE_LARGE = 0x3,
		PENETRATE_TYPE_MAXIMUM = 0x4,
		PENETRATE_TYPE_TABLE_ENTRY_COUNT = 0x4,
		PENETRATE_TYPE_RICOCHET = 0x5,
		PENETRATE_TYPE_COUNT = 0x6,
	};

	enum ReticleType : std::int32_t
	{
		RETICLE_TYPE_FOUR_SIDED = 0x0,
		RETICLE_TYPE_ONE_PIECE = 0x1,
		RETICLE_TYPE_COUNT = 0x2,
	};

	enum targetAssistType_t : std::int32_t
	{
		TARGET_ASSISTTYPPE_DEFAULT = 0x0,
		TARGET_ASSISTTYPPE_CONE = 0x1,
		TARGET_ASSISTTYPPE_COUNT = 0x2,
	};

	enum targetAssistBehavior_t : std::int32_t
	{
		TARGET_ASSISTBEHAVE_DEFAULT = 0x0,
		TARGET_ASSISTBEHAVE_DIRECT_DAMAGE = 0x1,
		TARGET_ASSISTBEHAVE_TARGET_ONLY = 0x2,
		TARGET_ASSISTBEHAVE_BEAM = 0x3,
		TARGET_ASSISTBEHAVE_SMART_PELLET = 0x4,
		TARGET_ASSISTBEHAVE_COUNT = 0x5,
	};

	enum AnimDamageType : std::int32_t
	{
		ANIM_DAMAGETYPE_NONE = 0x0,
		ANIM_DAMAGETYPE_EXPLOSION_LIGHT = 0x1,
		ANIM_DAMAGETYPE_EXPLOSION = 0x2,
		ANIM_DAMAGETYPE_MELEE = 0x3,
		ANIM_DAMAGETYPE_MELEE_DOG = 0x4,
		ANIM_DAMAGETYPE_MELEE_ALIEN = 0x5,
		ANIM_DAMAGETYPE_ENERGY_BULLET = 0x6,
		ANIM_DAMAGETYPE_FIRE_EXPLOSION = 0x7,
		ANIM_DAMAGETYPE_BULLET_SM = 0x8,
		ANIM_DAMAGETYPE_BULLET_MD = 0x9,
		ANIM_DAMAGETYPE_BULLET_LG = 0xA,
		ANIM_DAMAGETYPE_FALL = 0xB,
		ANIM_DAMAGETYPE_COUNT = 0xC,
	};

	enum weapOverlayReticle_t : std::int32_t
	{
		WEAPOVERLAYRETICLE_NONE = 0x0,
		WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
		WEAPOVERLAYRETICLE_NUM = 0x2,
	};

	enum ammoCounterClipType_t : std::int32_t
	{
		AMMO_COUNTER_CLIP_NONE = 0x0,
		AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
		AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
		AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
		AMMO_COUNTER_CLIP_ROCKET = 0x4,
		AMMO_COUNTER_CLIP_BELTFED = 0x5,
		AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
		AMMO_COUNTER_CLIP_COUNT = 0x7,
	};

	enum weapProjExposion_t : std::int32_t
	{
		WEAPPROJEXP_GRENADE = 0x0,
		WEAPPROJEXP_ROCKET = 0x1,
		WEAPPROJEXP_FLASHBANG = 0x2,
		WEAPPROJEXP_NONE = 0x3,
		WEAPPROJEXP_DUD = 0x4,
		WEAPPROJEXP_SMOKE = 0x5,
		WEAPPROJEXP_HEAVY = 0x6,
		WEAPPROJEXP_NUM = 0x7,
	};

	enum WeapStickinessType : std::int32_t
	{
		WEAPSTICKINESS_NONE = 0x0,
		WEAPSTICKINESS_ALL = 0x1,
		WEAPSTICKINESS_ALL_ORIENT = 0x2,
		WEAPSTICKINESS_GROUND = 0x3,
		WEAPSTICKINESS_GROUND_WITH_YAW = 0x4,
		WEAPSTICKINESS_GROUND_ORIENT = 0x5,
		WEAPSTICKINESS_KNIFE = 0x6,
		WEAPSTICKINESS_COUNT = 0x7,
	};

	enum WeapOverlayInteface_t : std::int32_t
	{
		WEAPOVERLAYINTERFACE_NONE = 0x0,
		WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
		WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
		WEAPOVERLAYINTERFACECOUNT = 0x3,
	};

	enum weaponIconRatioType_t : std::int32_t
	{
		WEAPON_ICON_RATIO_1TO1 = 0x0,
		WEAPON_ICON_RATIO_2TO1 = 0x1,
		WEAPON_ICON_RATIO_4TO1 = 0x2,
		WEAPON_ICON_RATIO_COUNT = 0x3,
	};

	enum guidedMissileType_t : std::int32_t
	{
		MISSILE_GUIDANCE_NONE = 0x0,
		MISSILE_GUIDANCE_SIDEWINDER = 0x1,
		MISSILE_GUIDANCE_HELLFIRE = 0x2,
		MISSILE_GUIDANCE_JAVELIN = 0x3,
		MISSILE_GUIDANCE_ROBOTECH = 0x4,
		MISSILE_GUIDANCE_SOFTLOCKON = 0x5,
		MISSILE_GUIDANCE_COUNT = 0x6,
	};

	enum offhandShieldMaxSpeed_t : std::int32_t
	{
		WEAP_OFFHAND_SHIELD_SPEED_WALK = 0x0,
		WEAP_OFFHAND_SHIELD_SPEED_RUN = 0x1,
		WEAP_OFFHAND_SHIELD_SPEED_SPRINT = 0x2,
		WEAP_OFFHAND_SHIELD_SPEED_COUNT = 0x3,
	};

	enum WeaponSlot : std::int32_t
	{
		WEAPON_SLOT_NONE = 0x0,
		WEAPON_SLOT_PRIMARY = 0x1,
		WEAPON_SLOT_HEAVY = 0x2,
		WEAPON_SLOT_MELEE = 0x3,
		WEAPON_SLOT_EXECUTION = 0x4,
		WEAPON_SLOT_ACCESSORY = 0x5,
		WEAPON_SLOT_NUM = 0x6,
	};

	enum weapInventoryType_t : std::int32_t
	{
		WEAPINVENTORY_PRIMARY = 0x0,
		WEAPINVENTORY_OFFHAND = 0x1,
		WEAPINVENTORY_ITEM = 0x2,
		WEAPINVENTORY_ALTMODE = 0x3,
		WEAPINVENTORY_EXCLUSIVE = 0x4,
		WEAPINVENTORY_SCAVENGER = 0x5,
		WEAPINVENTORY_MODEL_ONLY = 0x6,
		WEAPINVENTORYCOUNT = 0x7,
	};

	enum OffhandClass : std::int32_t
	{
		OFFHAND_CLASS_NONE = 0x0,
		OFFHAND_CLASS_FRAG_GRENADE = 0x1,
		OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
		OFFHAND_CLASS_FLASH_GRENADE = 0x3,
		OFFHAND_CLASS_THROWINGKNIFE = 0x4,
		OFFHAND_CLASS_SHIELD = 0x5,
		OFFHAND_CLASS_OTHER = 0x6,
		OFFHAND_CLASS_COUNT = 0x7,
	};

	enum activeReticleType_t : std::int32_t
	{
		VEH_ACTIVE_RETICLE_NONE = 0x0,
		VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
		VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
		VEH_ACTIVE_RETICLE_COUNT = 0x3,
	};

	enum DualWieldType : std::int32_t
	{
		DUAL_WIELD_TYPE_DEFAULT = 0x0,
		DUAL_WIELD_TYPE_ALT_MODE = 0x1,
		DUAL_WIELD_TYPE_COUNT = 0x2,
	};

	enum weapStance_t : std::int32_t
	{
		WEAPSTANCE_STAND = 0x0,
		WEAPSTANCE_DUCK = 0x1,
		WEAPSTANCE_PRONE = 0x2,
		WEAPSTANCE_NUM = 0x3,
	};

	enum scriptedAnimEvent_t : std::int32_t
	{
		SCRIPTEDANIMEVENT_POWER_ACTIVE = 0x0,
		SCRIPTEDANIMEVENT_POWER_ACTIVE_CP = 0x1,
		SCRIPTEDANIMEVENT_COUNT = 0x2,
	};

	enum playerAnimType_t : std::int32_t
	{
		PLAYERANIMTYPE_NONE,
		PLAYERANIMTYPE_OTHER,
		PLAYERANIMTYPE_PISTOL,
		PLAYERANIMTYPE_SMG,
		PLAYERANIMTYPE_AUTORIFLE,
		PLAYERANIMTYPE_MG,
		PLAYERANIMTYPE_SNIPER,
		PLAYERANIMTYPE_ROCKETLAUNCHER,
		PLAYERANIMTYPE_EXPLOSIVE,
		PLAYERANIMTYPE_GRENADE,
		PLAYERANIMTYPE_TURRET,
		PLAYERANIMTYPE_C4,
		PLAYERANIMTYPE_M203,
		PLAYERANIMTYPE_HOLD,
		PLAYERANIMTYPE_BRIEFCASE,
		PLAYERANIMTYPE_RIOTSHIELD,
		PLAYERANIMTYPE_LAPTOP,
		PLAYERANIMTYPE_THROWINGKNIFE,
		PLAYERANIMTYPE_MORTAR,
		PLAYERANIMTYPE_KNIFE,
		PLAYERANIMTYPE_CLAYMORE,
		PLAYERANIMTYPE_MINIGUN,
		PLAYERANIMTYPE_DRILL,
		PLAYERANIMTYPE_KILLSTREAKTRIGGER,
		PLAYERANIMTYPE_TROPHYSYSTEM,
		PLAYERANIMTYPE_GRENADE_TROPHY,
		PLAYERANIMTYPE_GRENADE_FRISBEE,
		PLAYERANIMTYPE_GRENADE_LARGE,
		PLAYERANIMTYPE_GRENADE_UNDERHAND,
		PLAYERANIMTYPE_GRENADE_CHUCK,
		PLAYERANIMTYPE_GRENADE_BASKETBALL,
		PLAYERANIMTYPE_GRENADE_SKEEBALL,
		PLAYERANIMTYPE_GRENADE_BASEBALL,
		PLAYERANIMTYPE_CHARGESHIELD,
		PLAYERANIMTYPE_AXE,
		PLAYERANIMTYPE_CLAW,
		PLAYERANIMTYPE_SMG_ENERGY,
		PLAYERANIMTYPE_ERAD,
		PLAYERANIMTYPE_AR57,
		PLAYERANIMTYPE_KBS,
		PLAYERANIMTYPE_CRB,
		PLAYERANIMTYPE_M4,
		PLAYERANIMTYPE_ACR,
		PLAYERANIMTYPE_M1,
		PLAYERANIMTYPE_SPAS,
		PLAYERANIMTYPE_DEVASTATOR,
		PLAYERANIMTYPE_NRG,
		PLAYERANIMTYPE_GAUNTLET,
		PLAYERANIMTYPE_BALL,
		PLAYERANIMTYPE_FHR,
		PLAYERANIMTYPE_REVOLVER,
		PLAYERANIMTYPE_AKE,
		PLAYERANIMTYPE_RIPPER,
		PLAYERANIMTYPE_FMG,
		PLAYERANIMTYPE_SDFLMG,
		PLAYERANIMTYPE_LMGTURRET,
		PLAYERANIMTYPE_SDFSHOTTY,
		PLAYERANIMTYPE_SONIC,
		PLAYERANIMTYPE_LONGBOW,
		PLAYERANIMTYPE_M200,
		PLAYERANIMTYPE_M8,
		PLAYERANIMTYPE_CHARGESHOT,
		PLAYERANIMTYPE_ROLLERCOASTERSEATED,
		PLAYERANIMTYPE_CANDYBOX,
		PLAYERANIMTYPE_C6_SUPER,
		PLAYERANIMTYPE_UMP45,
		PLAYERANIMTYPE_SDFAR,
		PLAYERANIMTYPE_SDFAR_LEGENDARY,
		PLAYERANIMTYPE_REVOLVER_LEGENDARY,
		PLAYERANIMTYPE_ACR_LEGENDARY,
		PLAYERANIMTYPE_AR57_LEGENDARY,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_01,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_02,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_03,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_04,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_05,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_06,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_07,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_08,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_09,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_10,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_11,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_12,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_13,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_14,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_15,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_16,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_17,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_18,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_19,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_20,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_21,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_22,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_23,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_24,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_25,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_26,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_27,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_28,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_29,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_30,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_31,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_32,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_33,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_34,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_35,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_36,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_37,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_38,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_39,
		PLAYERANIMTYPE_ADDITIONAL_WEAPON_40,
		PLAYERANIMTYPE_NUM,
	};

	enum scriptedAnimType_t : std::int32_t
	{
		SCRIPTEDANIMTYPE_NONE,
		SCRIPTEDANIMTYPE_OVERCHARGE,
		SCRIPTEDANIMTYPE_ADRENALINE,
		SCRIPTEDANIMTYPE_MULTIVISOR,
		SCRIPTEDANIMTYPE_SURGE,
		SCRIPTEDANIMTYPE_KINETICWAVE,
		SCRIPTEDANIMTYPE_PHASESHIFT,
		SCRIPTEDANIMTYPE_OPTICWAVE,
		SCRIPTEDANIMTYPE_CAMO,
		SCRIPTEDANIMTYPE_TELEPORT,
		SCRIPTEDANIMTYPE_REWIND,
		SCRIPTEDANIMTYPE_KILLSTREAKCALLOUT,
		SCRIPTEDANIMTYPE_GESTURE000,
		SCRIPTEDANIMTYPE_GESTURE001,
		SCRIPTEDANIMTYPE_GESTURE002,
		SCRIPTEDANIMTYPE_GESTURE003,
		SCRIPTEDANIMTYPE_GESTURE004,
		SCRIPTEDANIMTYPE_GESTURE005,
		SCRIPTEDANIMTYPE_GESTURE006,
		SCRIPTEDANIMTYPE_GESTURE007,
		SCRIPTEDANIMTYPE_GESTURE008,
		SCRIPTEDANIMTYPE_GESTURE009,
		SCRIPTEDANIMTYPE_GESTURE010,
		SCRIPTEDANIMTYPE_GESTURE011,
		SCRIPTEDANIMTYPE_GESTURE012,
		SCRIPTEDANIMTYPE_GESTURE013,
		SCRIPTEDANIMTYPE_GESTURE014,
		SCRIPTEDANIMTYPE_GESTURE015,
		SCRIPTEDANIMTYPE_GESTURE016,
		SCRIPTEDANIMTYPE_GESTURE017,
		SCRIPTEDANIMTYPE_GESTURE018,
		SCRIPTEDANIMTYPE_GESTURE019,
		SCRIPTEDANIMTYPE_GESTURE020,
		SCRIPTEDANIMTYPE_GESTURE021,
		SCRIPTEDANIMTYPE_GESTURE022,
		SCRIPTEDANIMTYPE_GESTURE023,
		SCRIPTEDANIMTYPE_GESTURE024,
		SCRIPTEDANIMTYPE_GESTURE025,
		SCRIPTEDANIMTYPE_GESTURE026,
		SCRIPTEDANIMTYPE_GESTURE027,
		SCRIPTEDANIMTYPE_GESTURE028,
		SCRIPTEDANIMTYPE_GESTURE029,
		SCRIPTEDANIMTYPE_GESTURE030,
		SCRIPTEDANIMTYPE_GESTURE031,
		SCRIPTEDANIMTYPE_GESTURE032,
		SCRIPTEDANIMTYPE_GESTURE033,
		SCRIPTEDANIMTYPE_GESTURE034,
		SCRIPTEDANIMTYPE_GESTURE035,
		SCRIPTEDANIMTYPE_GESTURE036,
		SCRIPTEDANIMTYPE_GESTURE037,
		SCRIPTEDANIMTYPE_GESTURE038,
		SCRIPTEDANIMTYPE_GESTURE039,
		SCRIPTEDANIMTYPE_GESTURE040,
		SCRIPTEDANIMTYPE_GESTURE041,
		SCRIPTEDANIMTYPE_GESTURE042,
		SCRIPTEDANIMTYPE_GESTURE043,
		SCRIPTEDANIMTYPE_GESTURE044,
		SCRIPTEDANIMTYPE_GESTURE045,
		SCRIPTEDANIMTYPE_GESTURE046,
		SCRIPTEDANIMTYPE_GESTURE047,
		SCRIPTEDANIMTYPE_GESTURE048,
		SCRIPTEDANIMTYPE_GESTURE049,
		SCRIPTEDANIMTYPE_GESTURE050,
		SCRIPTEDANIMTYPE_GESTURE051,
		SCRIPTEDANIMTYPE_GESTURE052,
		SCRIPTEDANIMTYPE_GESTURE053,
		SCRIPTEDANIMTYPE_GESTURE054,
		SCRIPTEDANIMTYPE_GESTURE055,
		SCRIPTEDANIMTYPE_GESTURE056,
		SCRIPTEDANIMTYPE_GESTURE057,
		SCRIPTEDANIMTYPE_GESTURE058,
		SCRIPTEDANIMTYPE_GESTURE059,
		SCRIPTEDANIMTYPE_GESTURE060,
		SCRIPTEDANIMTYPE_GESTURE061,
		SCRIPTEDANIMTYPE_GESTURE062,
		SCRIPTEDANIMTYPE_GESTURE063,
		SCRIPTEDANIMTYPE_GESTURE064,
		SCRIPTEDANIMTYPE_GESTURE065,
		SCRIPTEDANIMTYPE_GESTURE066,
		SCRIPTEDANIMTYPE_GESTURE067,
		SCRIPTEDANIMTYPE_GESTURE068,
		SCRIPTEDANIMTYPE_GESTURE069,
		SCRIPTEDANIMTYPE_GESTURE070,
		SCRIPTEDANIMTYPE_GESTURE071,
		SCRIPTEDANIMTYPE_GESTURE072,
		SCRIPTEDANIMTYPE_GESTURE073,
		SCRIPTEDANIMTYPE_GESTURE074,
		SCRIPTEDANIMTYPE_GESTURE075,
		SCRIPTEDANIMTYPE_GESTURE076,
		SCRIPTEDANIMTYPE_GESTURE077,
		SCRIPTEDANIMTYPE_GESTURE078,
		SCRIPTEDANIMTYPE_GESTURE079,
		SCRIPTEDANIMTYPE_GESTURE080,
		SCRIPTEDANIMTYPE_GESTURE081,
		SCRIPTEDANIMTYPE_GESTURE082,
		SCRIPTEDANIMTYPE_GESTURE083,
		SCRIPTEDANIMTYPE_GESTURE084,
		SCRIPTEDANIMTYPE_GESTURE085,
		SCRIPTEDANIMTYPE_GESTURE086,
		SCRIPTEDANIMTYPE_GESTURE087,
		SCRIPTEDANIMTYPE_GESTURE088,
		SCRIPTEDANIMTYPE_GESTURE089,
		SCRIPTEDANIMTYPE_GESTURE090,
		SCRIPTEDANIMTYPE_GESTURE091,
		SCRIPTEDANIMTYPE_GESTURE092,
		SCRIPTEDANIMTYPE_GESTURE093,
		SCRIPTEDANIMTYPE_GESTURE094,
		SCRIPTEDANIMTYPE_GESTURE095,
		SCRIPTEDANIMTYPE_GESTURE096,
		SCRIPTEDANIMTYPE_GESTURE097,
		SCRIPTEDANIMTYPE_GESTURE098,
		SCRIPTEDANIMTYPE_GESTURE099,
		SCRIPTEDANIMTYPE_GESTURE100,
		SCRIPTEDANIMTYPE_GESTURE101,
		SCRIPTEDANIMTYPE_GESTURE102,
		SCRIPTEDANIMTYPE_GESTURE103,
		SCRIPTEDANIMTYPE_GESTURE104,
		SCRIPTEDANIMTYPE_GESTURE105,
		SCRIPTEDANIMTYPE_GESTURE106,
		SCRIPTEDANIMTYPE_GESTURE107,
		SCRIPTEDANIMTYPE_GESTURE108,
		SCRIPTEDANIMTYPE_GESTURE109,
		SCRIPTEDANIMTYPE_GESTURE110,
		SCRIPTEDANIMTYPE_GESTURE111,
		SCRIPTEDANIMTYPE_GESTURE112,
		SCRIPTEDANIMTYPE_GESTURE113,
		SCRIPTEDANIMTYPE_GESTURE114,
		SCRIPTEDANIMTYPE_GESTURE115,
		SCRIPTEDANIMTYPE_NUM,
	};

	enum weapAnimFiles_t
	{
		WEAP_ANIM_ROOT = 0,
		WEAP_ANIM_INNER_ROOT = 1,
		WEAP_ANIM_GESTURE_IK_TARGET_1 = 2,
		WEAP_ANIM_GESTURE_IK_TARGET_2 = 3,
		WEAP_ANIM_GESTURE_IK_TARGET_3 = 4,
		WEAP_ANIM_GESTURE_IK_TARGET_4 = 5,
		WEAP_ANIM_IK_NODE = 6,
		WEAP_ANIM_PROC_NODE = 7,
		WEAP_ANIM_IK_NODE_VOID = 8,
		WEAP_ANIM_PROC_NODE_VOID = 9,
		WEAP_ANIM_RELATIVE_ROOT = 10,
		WEAP_ANIM_ADDITIVE_ADS_ROOT = 11,
		WEAP_ANIM_ADDITIVE_ADS_UP = 12,
		WEAP_ANIM_ADDITIVE_DRAG_LEFT_ROOT = 13,
		WEAP_ANIM_ADDITIVE_DRAG_LEFT = 14,
		WEAP_ANIM_ADDITIVE_DRAG_RIGHT_ROOT = 15,
		WEAP_ANIM_ADDITIVE_DRAG_RIGHT = 16,
		WEAP_ANIM_ADDITIVE_DRAG_UP_ROOT = 17,
		WEAP_ANIM_ADDITIVE_DRAG_UP = 18,
		WEAP_ANIM_ADDITIVE_DRAG_DOWN_ROOT = 19,
		WEAP_ANIM_ADDITIVE_DRAG_DOWN = 20,
		WEAP_ANIM_ADDITIVE_SWIM_FORWARD_ROOT = 21,
		WEAP_ANIM_ADDITIVE_SWIM_FORWARD = 22,
		WEAP_ANIM_ADDITIVE_SWIM_BACKWARD_ROOT = 23,
		WEAP_ANIM_ADDITIVE_SWIM_BACKWARD = 24,
		WEAP_ANIM_ADDITIVE_JUMP_ROOT = 25,
		WEAP_ANIM_ADDITIVE_JUMP = 26,
		WEAP_ANIM_ADDITIVE_JUMP_BOOST = 27,
		WEAP_ANIM_ADDITIVE_JUMP_LAND_ROOT = 28,
		WEAP_ANIM_ADDITIVE_JUMP_LAND = 29,
		WEAP_ANIM_ADDITIVE_WALK_ROOT = 30,
		WEAP_ANIM_ADDITIVE_WALK = 31,
		WEAP_ANIM_ADDITIVE_WALK_GESTURE_1 = 32,
		WEAP_ANIM_ADDITIVE_WALK_GESTURE_2 = 33,
		WEAP_ANIM_ADDITIVE_SPRINT_ROOT = 34,
		WEAP_ANIM_ADDITIVE_SPRINT_IN = 35,
		WEAP_ANIM_ADDITIVE_SPRINT_LOOP = 36,
		WEAP_ANIM_ADDITIVE_SUPER_SPRINT_LOOP = 37,
		WEAP_ANIM_ADDITIVE_SPRINT_OUT = 38,
		WEAP_ANIM_ADDITIVE_SPRINT_OFFSET_ROOT = 39,
		WEAP_ANIM_ADDITIVE_SPRINT_OFFSET = 40,
		WEAP_ANIM_ADDITIVE_CRAWL_IN_ROOT = 41,
		WEAP_ANIM_ADDITIVE_CRAWL_IN = 42,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP_ROOT = 43,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP = 44,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP_LEFT_ROOT = 45,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP_LEFT = 46,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP_RIGHT_ROOT = 47,
		WEAP_ANIM_ADDITIVE_CRAWL_LOOP_RIGHT = 48,
		WEAP_ANIM_ADDITIVE_PRONE_DROP_ROOT = 49,
		WEAP_ANIM_ADDITIVE_PRONE_DROP = 50,
		WEAP_ANIM_ADDITIVE_FINGER_POSE_LEFT_ROOT = 51,
		WEAP_ANIM_ADDITIVE_FINGER_POSE_LEFT = 52,
		WEAP_ANIM_ADDITIVE_FINGER_POSE_RIGHT_ROOT = 53,
		WEAP_ANIM_ADDITIVE_FINGER_POSE_RIGHT = 54,
		WEAP_ANIM_ADDITIVE_DEMEANOR_OFFSET_ROOT = 55,
		WEAP_ANIM_ADDITIVE_DEMEANOR_OFFSET_SAFE = 56,
		WEAP_ANIM_ADDITIVE_GESTURE_1 = 57,
		WEAP_ANIM_ADDITIVE_GESTURE_2 = 58,
		WEAP_ANIM_ADDITIVE_GESTURE_3 = 59,
		WEAP_ANIM_ADDITIVE_GESTURE_4 = 60,
		WEAP_ANIM_ADDITIVE_GESTURE_5 = 61,
		WEAP_ANIM_ADDITIVE_GESTURE_6 = 62,
		WEAP_ANIM_ADDITIVE_GESTURE_7 = 63,
		WEAP_ANIM_ADDITIVE_GESTURE_8 = 64,
		WEAP_ANIM_ADDITIVE_GESTURE_9 = 65,
		WEAP_ANIM_ADDITIVE_GESTURE_10 = 66,
		WEAP_ANIM_ADDITIVE_GESTURE_11 = 67,
		WEAP_ANIM_ADDITIVE_GESTURE_12 = 68,
		WEAP_ANIM_ADDITIVE_GESTURE_13 = 69,
		WEAP_ANIM_ADDITIVE_GESTURE_14 = 70,
		WEAP_ANIM_ADDITIVE_GESTURE_15 = 71,
		WEAP_ANIM_ADDITIVE_GESTURE_16 = 72,
		WEAP_ANIM_ADDITIVE_GESTURE_17 = 73,
		WEAP_ANIM_ADDITIVE_GESTURE_18 = 74,
		WEAP_ANIM_ADDITIVE_GESTURE_19 = 75,
		WEAP_ANIM_ADDITIVE_GESTURE_20 = 76,
		WEAP_ANIM_ADDITIVE_GESTURE_21 = 77,
		WEAP_ANIM_ADDITIVE_GESTURE_22 = 78,
		WEAP_ANIM_ADDITIVE_GESTURE_23 = 79,
		WEAP_ANIM_ADDITIVE_GESTURE_24 = 80,
		WEAP_ANIM_ADDITIVE_GESTURE_25 = 81,
		WEAP_ANIM_ADDITIVE_GESTURE_26 = 82,
		WEAP_ANIM_ADDITIVE_GESTURE_27 = 83,
		WEAP_ANIM_ADDITIVE_GESTURE_28 = 84,
		WEAP_ANIM_ADDITIVE_GESTURE_29 = 85,
		WEAP_ANIM_ADDITIVE_GESTURE_30 = 86,
		WEAP_ANIM_ADDITIVE_GESTURE_31 = 87,
		WEAP_ANIM_ADDITIVE_GESTURE_32 = 88,
		WEAP_ANIM_ADDITIVE_GESTURE_33 = 89,
		WEAP_ANIM_ADDITIVE_GESTURE_34 = 90,
		WEAP_ANIM_ADDITIVE_GESTURE_35 = 91,
		WEAP_ANIM_ADDITIVE_GESTURE_36 = 92,
		WEAP_ANIM_ADDITIVE_GESTURE_37 = 93,
		WEAP_ANIM_ADDITIVE_GESTURE_38 = 94,
		WEAP_ANIM_ADDITIVE_GESTURE_39 = 95,
		WEAP_ANIM_ADDITIVE_GESTURE_40 = 96,
		WEAP_ANIM_ADDITIVE_SETTLE_ROOT = 97,
		WEAP_ANIM_ADDITIVE_SETTLE_ADS = 98,
		WEAP_ANIM_ADDITIVE_REGENERATE_ROOT = 99,
		WEAP_ANIM_ADDITIVE_REGENERATE = 100,
		WEAP_ANIM_ADDITIVE_MAGAZINE_BULLETS_ROOT = 101,
		WEAP_ANIM_ADDITIVE_MAGAZINE_BULLETS = 102,
		WEAP_ANIM_ADDITIVE_RECOIL_ROOT = 103,
		WEAP_ANIM_ADDITIVE_RECOIL = 104,
		WEAP_ANIM_ADDITIVE_EMPTY_ROOT = 105,
		WEAP_ANIM_ADDITIVE_EMPTY = 106,
		WEAP_ANIM_IDLE = 107,
		WEAP_ANIM_EMPTY_IDLE = 108,
		WEAP_ANIM_FIRE = 109,
		WEAP_ANIM_HOLD_FIRE = 110,
		WEAP_ANIM_LASTSHOT = 111,
		WEAP_ANIM_RECHAMBER = 112,
		WEAP_ANIM_GRENADE_PRIME = 113,
		WEAP_ANIM_GRENADE_PRIME_READY_TO_THROW = 114,
		WEAP_ANIM_MELEE_FATAL = 115,
		WEAP_ANIM_MELEE_HIT = 116,
		WEAP_ANIM_MELEE_MISS = 117,
		WEAP_ANIM_MELEE_SWIPE = 118,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_FATAL = 119,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_HIT = 120,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_MISS = 121,
		WEAP_ANIM_MELEE_FATAL_2 = 122,
		WEAP_ANIM_MELEE_HIT_2 = 123,
		WEAP_ANIM_MELEE_MISS_2 = 124,
		WEAP_ANIM_MELEE_SWIPE_2 = 125,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_FATAL_2 = 126,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_HIT_2 = 127,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_MISS_2 = 128,
		WEAP_ANIM_MELEE_FATAL_3 = 129,
		WEAP_ANIM_MELEE_HIT_3 = 130,
		WEAP_ANIM_MELEE_MISS_3 = 131,
		WEAP_ANIM_MELEE_SWIPE_3 = 132,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_FATAL_3 = 133,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_HIT_3 = 134,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_MISS_3 = 135,
		WEAP_ANIM_MELEE_FATAL_4 = 136,
		WEAP_ANIM_MELEE_HIT_4 = 137,
		WEAP_ANIM_MELEE_MISS_4 = 138,
		WEAP_ANIM_MELEE_SWIPE_4 = 139,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_FATAL_4 = 140,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_HIT_4 = 141,
		WEAP_ANIM_MELEE_VICTIM_CROUCHING_MISS_4 = 142,
		WEAP_ANIM_ALT_MELEE_FATAL = 143,
		WEAP_ANIM_ALT_MELEE_HIT = 144,
		WEAP_ANIM_ALT_MELEE_MISS = 145,
		WEAP_ANIM_ALT_MELEE_SWIPE = 146,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_FATAL = 147,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_HIT = 148,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_MISS = 149,
		WEAP_ANIM_ALT_MELEE_FATAL_2 = 150,
		WEAP_ANIM_ALT_MELEE_HIT_2 = 151,
		WEAP_ANIM_ALT_MELEE_MISS_2 = 152,
		WEAP_ANIM_ALT_MELEE_SWIPE_2 = 153,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_FATAL_2 = 154,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_HIT_2 = 155,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_MISS_2 = 156,
		WEAP_ANIM_ALT_MELEE_FATAL_3 = 157,
		WEAP_ANIM_ALT_MELEE_HIT_3 = 158,
		WEAP_ANIM_ALT_MELEE_MISS_3 = 159,
		WEAP_ANIM_ALT_MELEE_SWIPE_3 = 160,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_FATAL_3 = 161,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_HIT_3 = 162,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_MISS_3 = 163,
		WEAP_ANIM_ALT_MELEE_FATAL_4 = 164,
		WEAP_ANIM_ALT_MELEE_HIT_4 = 165,
		WEAP_ANIM_ALT_MELEE_MISS_4 = 166,
		WEAP_ANIM_ALT_MELEE_SWIPE_4 = 167,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_FATAL_4 = 168,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_HIT_4 = 169,
		WEAP_ANIM_ALT_MELEE_VICTIM_CROUCHING_MISS_4 = 170,
		WEAP_ANIM_RELOAD = 171,
		WEAP_ANIM_RELOAD_EMPTY = 172,
		WEAP_ANIM_RELOAD_START = 173,
		WEAP_ANIM_RELOAD_END = 174,
		WEAP_ANIM_FAST_RELOAD = 175,
		WEAP_ANIM_FAST_RELOAD_EMPTY = 176,
		WEAP_ANIM_FAST_RELOAD_START = 177,
		WEAP_ANIM_FAST_RELOAD_END = 178,
		WEAP_ANIM_RAISE = 179,
		WEAP_ANIM_FIRST_RAISE = 180,
		WEAP_ANIM_BREACH_RAISE = 181,
		WEAP_ANIM_DROP = 182,
		WEAP_ANIM_ALT_RAISE = 183,
		WEAP_ANIM_ALT_RAISE_ADS = 184,
		WEAP_ANIM_ALT_RAISE_AKIMBO = 185,
		WEAP_ANIM_ALT_DROP = 186,
		WEAP_ANIM_ALT_DROP_ADS = 187,
		WEAP_ANIM_ALT_DROP_AKIMBO = 188,
		WEAP_ANIM_ALT_OVERRIDE = 189,
		WEAP_ANIM_TOGGLE_OVERRIDE = 190,
		WEAP_ANIM_SCOPE_TOGGLE_ON = 191,
		WEAP_ANIM_SCOPE_TOGGLE_OFF = 192,
		WEAP_ANIM_QUICK_RAISE = 193,
		WEAP_ANIM_QUICK_DROP = 194,
		WEAP_ANIM_EMPTY_RAISE = 195,
		WEAP_ANIM_EMPTY_DROP = 196,
		WEAP_ANIM_SPRINT_IN = 197,
		WEAP_ANIM_SPRINT_IN_CANCEL = 198,
		WEAP_ANIM_SPRINT_LOOP = 199,
		WEAP_ANIM_SPRINT_OUT = 200,
		WEAP_ANIM_STUNNED_START = 201,
		WEAP_ANIM_STUNNED_LOOP = 202,
		WEAP_ANIM_STUNNED_END = 203,
		WEAP_ANIM_DETONATE = 204,
		WEAP_ANIM_NIGHTVISION_WEAR = 205,
		WEAP_ANIM_NIGHTVISION_REMOVE = 206,
		WEAP_ANIM_ADS_FIRE = 207,
		WEAP_ANIM_ADS_LASTSHOT = 208,
		WEAP_ANIM_ADS_RECHAMBER = 209,
		WEAP_ANIM_BLAST_FRONT = 210,
		WEAP_ANIM_BLAST_RIGHT = 211,
		WEAP_ANIM_BLAST_BACK = 212,
		WEAP_ANIM_BLAST_LEFT = 213,
		WEAP_ANIM_DODGE_GROUND_BACK = 214,
		WEAP_ANIM_DODGE_GROUND_LEFT = 215,
		WEAP_ANIM_DODGE_GROUND_RIGHT = 216,
		WEAP_ANIM_DODGE_AIR_FORWARD = 217,
		WEAP_ANIM_DODGE_AIR_BACK = 218,
		WEAP_ANIM_DODGE_AIR_LEFT = 219,
		WEAP_ANIM_DODGE_AIR_RIGHT = 220,
		WEAP_ANIM_LEAP_IN = 221,
		WEAP_ANIM_LEAP_LOOP = 222,
		WEAP_ANIM_LEAP_CANCEL = 223,
		WEAP_ANIM_LEAP_OUT = 224,
		WEAP_ANIM_CHARGE_IN = 225,
		WEAP_ANIM_CHARGE_LOOP = 226,
		WEAP_ANIM_CHARGE_OUT = 227,
		WEAP_ANIM_ADS_CHARGE_IN = 228,
		WEAP_ANIM_ADS_CHARGE_LOOP = 229,
		WEAP_ANIM_ADS_CHARGE_OUT = 230,
		WEAP_ANIM_RECOIL_SETTLE = 231,
		WEAP_ANIM_SWIM_LOOP = 232,
		WEAP_ANIM_RELOAD_MULTIPLE_1 = 233,
		WEAP_ANIM_RELOAD_MULTIPLE_2 = 234,
		WEAP_ANIM_RELOAD_MULTIPLE_3 = 235,
		WEAP_ANIM_RELOAD_MULTIPLE_4 = 236,
		WEAP_ANIM_RELOAD_MULTIPLE_5 = 237,
		WEAP_ANIM_RELOAD_MULTIPLE_6 = 238,
		WEAP_ANIM_RELOAD_MULTIPLE_7 = 239,
		WEAP_ANIM_RELOAD_MULTIPLE_8 = 240,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_1 = 241,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_2 = 242,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_3 = 243,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_4 = 244,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_5 = 245,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_6 = 246,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_7 = 247,
		WEAP_ANIM_RELOAD_MULTIPLE_FAST_8 = 248,
		WEAP_ANIM_ADS_UP = 249,
		WEAP_ANIM_ADS_DOWN = 250,
		WEAP_ANIM_SHIELD_UP = 251,
		WEAP_ANIM_SHIELD_IDLE = 252,
		WEAP_ANIM_SHIELD_DOWN = 253,
		WEAP_ANIM_RECOIL = 254,
		WEAP_ALT_ANIM_ADJUST = 255,
		WEAP_ANIM_UNUSED = 256,
		WEAP_ANIM_3P_FINGER_POSE_LEFT = 257,
		WEAP_ANIM_3P_FINGER_POSE_RIGHT = 258,
		WEAP_ANIM_3P_ADDITIVE_ALT_OVERRIDE = 259,
		NUM_WEAP_ANIMS = 260,
	};

	struct AttAmmoGeneral
	{
		ImpactType vfxImpactType;
		ImpactType sfxImpactType;
		MeleeImpactType meleeImpactType;
		weapFireType_t fireType;
		int burstCount;
	}; assert_sizeof(AttAmmoGeneral, 0x14);

	struct AttAmmoTracer
	{
		TracerDef* tracerType;
		tracerStyle_t tracerStyle;
	}; assert_sizeof(AttAmmoTracer, 0x10);

	struct AttPenetration
	{
		PenetrateType penetrateType;
		float penetrateMultiplier;
		bool rifleBullet;
		bool armorPiercing;
		bool impaling;
	}; assert_sizeof(AttPenetration, 0xC);

	struct AttSight
	{
		float blurSceneAdsInFraction;
		float blurSceneAdsOutFraction;
		bool aimDownSight;
		bool adsFire;
		bool rechamberWhileAds;
		bool noAdsWhenMagEmpty;
		bool canVariableZoom;
		bool hideRailWithThisScope;
		bool forceIronSightDown;
		bool useScopeDrift;
		bool useDualFOV;
	}; assert_sizeof(AttSight, 0x14);

	struct AttHoldBreath
	{
		bool canHoldBreath;
	};

	struct AttReload
	{
		bool noPartialReload;
		bool segmentedReload;
		bool reloadDisabled;
	};

	struct AttAddOns
	{
		bool motionTracker;
		bool silenced;
		bool riotShield;
	};

	struct AttGeneral
	{
		bool boltAction;
		bool inheritsPerks;
		bool reticleSpin45;
		bool adsShouldShowCrosshair;
		float enemyCrosshairRange;
		ReticleType reticleType;
		Material* reticleCenter;
		Material* reticleSide;
		Material* reticleOnePiece;
		int reticleCenterSize;
		int reticleSideSize;
		const char* szLUICrosshairWidget;
	}; assert_sizeof(AttGeneral, 0x38);

	struct AttLaser
	{
		LaserDef* laserTypeFriendly;
		LaserDef* laserTypeEnemy;
		bool forceLaserOn;
	}; assert_sizeof(AttLaser, 0x18);

	struct AttAimAssist
	{
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimAssistPitchSlowdown;
		float aimAssistPitchSlowdownAds;
		float aimAssistYawSlowdown;
		float aimAssistYawSlowdownAds;
		float aimAssistLockonStrength;
	}; assert_sizeof(AttAimAssist, 0x20);

	struct AttTargetAssist
	{
		targetAssistType_t targetAssistType;
		targetAssistBehavior_t targetAssistBehavior;
		float targetAssistRange;
		float targetAssistAngle;
		float targetAssistLosOffsetForward;
		float targetAssistLosOffsetRight;
		float targetAssistLosOffsetUp;
		bool targetAssistOnlyAvailableInAds;
	}; assert_sizeof(AttTargetAssist, 0x20);

	struct AttUnknown01
	{
		Material* material;
		float unk_0;
		int unk_1;
		int unk_2;
		float unk_3;
		float unk_4;
		float unk_5;
		int unk_6;
		float unk_7;
		float unk_8;
		int unk_9;
		int unk_10;
		int unk_11;
		int unk_12;
		float unk_13;
		float unk_14;
		float unk_15;
		float unk_16;
		float unk_17;
		float unk_18;
		float unk_19;
		float unk_20;
		float unk_21;
		float unk_22;
		float unk_23;
		float unk_24;
		float unk_25;
		float unk_26;
		float unk_27;
		float unk_28;
		float unk_29;
		float unk_30;
		float unk_31;
		float unk_32;
		float unk_33;
		float unk_34;
		float unk_35;
		float unk_36;
		float unk_37;
		float unk_38;
		int unk_39;
		int unk_40;
		int unk_41;
		int unk_42;
		int unk_43;
		int unk_44;
		int unk_45;
		int unk_46;
		bool unk_47;
		bool unk_48;
		bool pad0;
		bool pad1;
	}; assert_sizeof(AttUnknown01, 0xC8);

	struct AttAmmunition
	{
		int maxAmmo;
		int startAmmo;
		int clipSize;
		int shotCount;
		int reloadAmmoAdd;
		int reloadStartAdd;
		bool requireAmmoUsedPerShot;
	}; assert_sizeof(AttAmmunition, 0x1C);

	struct AttDamage
	{
		int damage;
		int mid1Damage;
		int mid2Damage;
		int mid3Damage;
		int minDamage;
		int meleeDamage;
		float maxDamageRange;
		float mid1DamageRange;
		float mid2DamageRange;
		float mid3DamageRange;
		float minDamageRange;
		int playerDamage;
		int mid1PlayerDamage;
		int mid2PlayerDamage;
		int mid3PlayerDamage;
		int minPlayerDamage;
		AnimDamageType deathAnimDamageType;
	}; assert_sizeof(AttDamage, 0x44);

	struct AttIdleSettings
	{
		float hipIdleAmount;
		float hipIdleSpeed;
		float idleCrouchFactor;
		float idleProneFactor;
		float adsIdleLerpStartTime;
		float adsIdleLerpTime;
	}; assert_sizeof(AttIdleSettings, 0x18);

	struct AttADSSettings
	{
		float adsSpread;
		float adsAimPitch;
		float adsTransInTime;
		float adsTransOutTime;
		int adsReloadTransTime;
		float adsCrosshairInFrac;
		float adsCrosshairOutFrac;
		float adsInCrosshairAlphaStart;
		float adsInCrosshairAlphaEnd;
		float adsOutCrosshairAlphaStart;
		float adsOutCrosshairAlphaEnd;
		float adsZoomFov;
		float adsZoomInFrac;
		float adsZoomOutFrac;
		float adsFovLerpTime;
		float adsBobFactor;
		float adsViewBobMult;
		float adsFireRateScale;
		float adsDamageRangeScale;
		float adsFireAnimFrac;
		float fireTimerLerpToAdsScale;
		bool alwaysFireAtMaxRangeInAds;
		bool adsAlignEnabled;
		bool disableTagAlignX;
	}; assert_sizeof(AttADSSettings, 0x58);

	struct AttScopeDriftSettings
	{
		float fScopeDriftDelay;
		float fScopeDriftLerpInTime;
		float fScopeDriftSteadyTime;
		float fScopeDriftLerpOutTime;
		float fScopeDriftSteadyFactor;
		float fScopeDriftUnsteadyFactor;
	}; assert_sizeof(AttScopeDriftSettings, 0x18);

	struct AttHipSpread
	{
		float hipSpreadStandMin;
		float hipSpreadDuckedMin;
		float hipSpreadProneMin;
		float hipSpreadSprintMin;
		float hipSpreadInAirMin;
		float hipSpreadMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float hipSpreadSprintMax;
		float hipSpreadInAirMax;
		float hipSpreadFireAdd;
		float hipSpreadTurnAdd;
		float hipSpreadMoveAdd;
		float hipSpreadDecayRate;
		float hipSpreadDuckedDecay;
		float hipSpreadProneDecay;
		float hipSpreadSprintDecay;
		float hipSpreadInAirDecay;
	}; assert_sizeof(AttHipSpread, 0x48);

	struct AttSlideSpread
	{
		float slideSpreadMin;
		float slideSpreadMax;
		float slideSpreadDecayRate;
		float slideSpreadFireAdd;
		float slideSpreadTurnAdd;
	}; assert_sizeof(AttSlideSpread, 0x14);

	struct AttGunKick
	{
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float hipGunKickPitchMin;
		float hipGunKickPitchMax;
		float hipGunKickYawMin;
		float hipGunKickYawMax;
		float hipGunKickMagMin;
		float hipGunKickAccel;
		float hipGunKickSpeedMax;
		float hipGunKickSpeedDecay;
		float hipGunKickStaticDecay;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float adsGunKickPitchMin;
		float adsGunKickPitchMax;
		float adsGunKickYawMin;
		float adsGunKickYawMax;
		float adsGunKickMagMin;
		float adsGunKickAccel;
		float adsGunKickSpeedMax;
		float adsGunKickSpeedDecay;
		float adsGunKickStaticDecay;
	}; assert_sizeof(AttGunKick, 0x58);

	struct AttViewKick
	{
		float hipViewKickPitchMin;
		float hipViewKickPitchMax;
		float hipViewKickYawMin;
		float hipViewKickYawMax;
		float hipViewKickMagMin;
		float hipViewKickCenterSpeed;
		float adsViewKickPitchMin;
		float adsViewKickPitchMax;
		float adsViewKickYawMin;
		float adsViewKickYawMax;
		float adsViewKickMagMin;
		float adsViewKickCenterSpeed;
		float kickAlignedInputScalar;
		float kickOpposedInputScalar;
	}; assert_sizeof(AttViewKick, 0x38);

	struct ADSOverlay
	{
		Material* shader;
		Material* shaderLowRes;
		Material* shaderEMP;
		Material* shaderEMPLowRes;
		weapOverlayReticle_t reticle;
		float width;
		float height;
		float widthSplitscreen;
		float heightSplitscreen;
	}; assert_sizeof(ADSOverlay, 0x38);

	struct AttADSOverlay
	{
		ADSOverlay overlay;
		bool hybridToggle;
		bool thermalScope;
		bool thermalToggle;
		bool outlineEnemies;
	}; assert_sizeof(AttADSOverlay, 0x40);

	struct AttOutline
	{
		vec4_t outlineColor;
		vec4_t outlineAltColor;
		vec4_t outlineFillColor0;
		vec4_t outlineFillColor1;
		vec4_t outlineOccludedOutlineColor;
		vec4_t outlineOccludedInlineColor;
		vec4_t outlineOccludedInteriorColor;
		float outlineWidth;
		bool outlineEnemies;
		bool outlineDepthTest;
		bool outlineFill;
		bool outlineScriptControlled;
		bool outlineVehicles;
		bool outlineTurrets;
		bool outlineCharacters;
		bool outlineAgents;
		bool outlineScriptMovers;
		bool outlineLockOn;
	}; assert_sizeof(AttOutline, 0x80);

	struct AttUI
	{
		Material* hudIcon;
		Material* pickupIcon;
		Material* dangerIcon;
		Material* throwBackIcon;
		bool hideWarningIcons;
		float warningIconsDelay;
		Material* dpadIcon;
		Material* ammoCounterIcon;
		ammoCounterClipType_t ammoCounterClip;
		float grenadeDangerIconDistance;
	}; assert_sizeof(AttUI, 0x40);

	struct AttRumbles
	{
		RumbleInfo* fireRumble;
		RumbleInfo* meleeImpactRumble;
	}; assert_sizeof(AttRumbles, 0x10);

	struct AttProjectile
	{
		int explosionRadius;
		int explosionForceRadius;
		int explosionInnerDamage;
		int explosionOuterDamage;
		float damageConeAngle;
		int projectileSpeed;
		int projectileSpeedUp;
		int projectileActivateDist;
		float projectileLifetime;
		float projectileLifetimeStdDeviation;
		XModel* projectileModel;
		weapProjExposion_t projExplosionType;
		FxCombinedDef projExplosionEffect;
		bool projExplosionEffectForceNormalUp;
		SndAliasLookup projExplosionSound;
		FxCombinedDef projDudEffect;
		SndAliasLookup projDudSound;
		bool projImpactExplode;
		bool disableProjectileCrumpleCheck;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		FxCombinedDef projBodyEffect;
		FxCombinedDef projTrailEffect;
		FxCombinedDef projBeaconEffect;
		int projIgnitionDelay;
		FxCombinedDef projIgnitionEffect;
		SndAliasLookup projIgnitionSound;
		WeapStickinessType stickiness;
		int fuseTime;
		int aiFuseTime;
		bool stickToPlayers;
		bool stickToVehicles;
		bool stickToTurrets;
		bool stickToNonStick;
		bool timedDetonation;
		bool isPredictedProjectile;
		bool useSteppedExplosionDamage;
		float reactiveMotionRadiusScale;
		float reactiveMotionFrequencyScale;
		float reactiveMotionAmplitudeScale;
		float reactiveMotionFalloff;
		float reactiveMotionLifetime;
	}; assert_sizeof(AttProjectile, 0xF8);

	enum WeaponChargeType : std::int32_t
	{
	};

	struct AttCharged
	{
		WeaponChargeType chargeType;
		float chargeGain;
		float chargeCostPerShot;
		float chargeLossWhenIdle;
		float chargeEmptyCooldown;
		float chargeFireAtMaxDamageMultiplier;
		int chargeMeterEffectBoneCount;
		FxCombinedDef chargeMeterEffect;
		SndAliasLookup chargeUpSound;
		SndAliasLookup chargeDownSound;
		SndAliasLookup chargeDownToUpSound;
		SndAliasLookup chargeUpToDownSound;
		SndAliasLookup chargeMaxSound;
		SndAliasLookup chargeUpSoundPlayer;
		SndAliasLookup chargeDownSoundPlayer;
		SndAliasLookup chargeDownToUpSoundPlayer;
		SndAliasLookup chargeUpToDownSoundPlayer;
		SndAliasLookup chargeMaxSoundPlayer;
		bool chargeHudReveal;
		RumbleInfo* chargeRumble;
	}; assert_sizeof(AttCharged, 0x90);

	struct AdsAltSwitch
	{
		float fovInterpTimeSecMainToAlt;
		float fovInterpDelaySecMainToAlt;
		float dualFovSwitchRateMainToAlt;
		AdsAltSwitchInterpType fovInterpTypeMainToAlt;
		float fovInterpTimeSecAltToMain;
		float fovInterpDelaySecAltToMain;
		float dualFovSwitchRateAltToMain;
		AdsAltSwitchInterpType fovInterpTypeAltToMain;
	}; assert_sizeof(AdsAltSwitch, 0x20);

	struct AttRegeneration
	{
		bool regenerationEnabled;
		int regenerationTimeMs;
		int regenerationAddTimeMs;
		int regenerationAmount;
		bool regenerationConsumeStock;
		bool regenerationDisableWhileFiring;
	}; assert_sizeof(AttRegeneration, 0x14);

	struct AttMovement
	{
		float moveSpeedScale;
		float adsMoveSpeedScale;
		bool fastADS3PAnims;
	}; assert_sizeof(AttMovement, 0xC);

	struct AttBurst
	{
		int burstFireCooldown;
		bool burstFireAuto;
	}; assert_sizeof(AttBurst, 8);

	struct WeaponAttachment
	{
		union
		{
			const char* szInternalName;
			const char* name;
		};
		const char* szDisplayName;
		scr_string_t internalName;
		bool papAttachment; // packapunch
		AttachmentType type;
		weapType_t weaponType;
		weapClass_t weapClass;
		int numReticles;
		int numAltReticles;
		int unk;
		XModel* worldModelCamo;
		XModel* viewModelCamo;
		XModel** worldModelVariations;
		XModel** viewModelVariations;
		unsigned int numModelVariations;
		Material** worldModelCamoMaterials;
		Material** viewModelCamoMaterials;
		unsigned int numWorldModelCamoMaterials;
		unsigned int numViewModelCamoMaterials;
		scr_string_t attachPoint;
		ReticleDef** reticles;
		ReticleDef** altReticles;
		AttAmmoGeneral* ammogeneral;
		AttAmmoTracer* tracer;
		AttPenetration* penetration;
		AttSight* sight;
		AttHoldBreath* holdBreath;
		AttReload* reload;
		AttAddOns* addOns;
		AttLaser* laser;
		AttGeneral* general;
		AttAimAssist* aimAssist;
		AttTargetAssist* targetAssist;
		AttUnknown01* unkAtt01;
		AttAmmunition* ammunition;
		AttDamage* damage;
		AttIdleSettings* idleSettings;
		AttADSSettings* adsSettings;
		AttADSSettings* adsSettingsMain;
		AttScopeDriftSettings* scopeDriftSettings;
		AttScopeDriftSettings* scopeDriftSettingsMain;
		AttHipSpread* hipSpread;
		AttSlideSpread* slideSpread;
		AttGunKick* gunKick;
		AttViewKick* viewKick;
		AttADSOverlay* adsOverlay;
		AttOutline* outline;
		AttUI* ui;
		AttRumbles* rumbles;
		AttProjectile* projectile;
		AttCharged* charged;
		AdsAltSwitch* adsAltSwitch;
		AttRegeneration* regeneration;
		AttMovement* movement;
		AttBurst* burst;
		float* locationDamage; // arr: 22
		float ammunitionScale;
		float damageScale;
		float stateTimersScale;
		float reloadTimersScale;
		int fireTimeOverride;
		int fireTimeAkimboOverride;
		float fireTimersScale;
		float burstFireCooldownScale;
		float idleHipMotionScale;
		float idleADSMotionScale;
		float idleADSMotionTimeScale;
		float adsSettingsScale;
		float adsSettingsScaleMain;
		float hipSpreadScale;
		float slideSpreadScale;
		float gunKickScale;
		float viewKickScale;
		float viewCenterScale;
		float adsSwayScale;
		float adsSwayScale2;
		float adsSwayScale3;
		int iAmmoIndex;
		int loadIndex;
		bool useAlternateViewModelLaser;
		bool hideIronSightsWithThisAttachment;
		bool shareAmmoWithAlt;
		bool useLeftTriggerAltFire;
		bool containsIKTag;
		bool isAkimboAttachment;
		bool attachLeftWeaponAkimbo;
		bool attachRightWeaponAkimbo;
		bool luiWeaponInfoWidgetUsesScopeStencil;
		const char* szAltModeName;
		const char* szLUIWeaponInfoWidgetName;
		const char* szLUIWeaponInfoWidgetTag;
	}; assert_sizeof(WeaponAttachment, 0x218);

	struct AnimOverride
	{
		scr_string_t attachment1;
		scr_string_t attachment2;
		WeaponAnimPackage* overrides;
		WeaponAnimPackage* overridesAlt;
	}; assert_sizeof(AnimOverride, 24);

	struct SFXOverride
	{
		scr_string_t attachment1;
		scr_string_t attachment2;
		WeaponSFXPackage* overrides;
		WeaponSFXPackage* overridesAlt;
	}; assert_sizeof(SFXOverride, 24);

	struct VFXOverride
	{
		scr_string_t attachment1;
		scr_string_t attachment2;
		WeaponVFXPackage* overrides;
		WeaponVFXPackage* overridesAlt;
	}; assert_sizeof(VFXOverride, 24);

	enum GrenadeRotationDirection : std::int32_t
	{
		GRD_RANDOM = 0x0,
		GRD_CLOCKWISE = 0x1,
		GRD_COUNTERCLOCKWISE = 0x2,
		GRD_COUNT = 0x3,
	};

	struct GrenadeRotationParams
	{
		float initialPitch;
		float initialYaw;
		float initialRoll;
		GrenadeRotationDirection rotationPitchDir;
		int rotationPitchMin;
		int rotationPitchMax;
		GrenadeRotationDirection rotationYawDir;
		int rotationYawMin;
		int rotationYawMax;
		GrenadeRotationDirection rotationRollDir;
		int rotationRollMin;
		int rotationRollMax;
		bool rotate;
	};

	enum hitLocation_t
	{
		HITLOC_NONE = 0x0,
		HITLOC_HELMET = 0x1,
		HITLOC_HEAD = 0x2,
		HITLOC_NECK = 0x3,
		HITLOC_TORSO_UPR = 0x4,
		HITLOC_TORSO_LWR = 0x5,
		HITLOC_R_ARM_UPR = 0x6,
		HITLOC_L_ARM_UPR = 0x7,
		HITLOC_R_ARM_LWR = 0x8,
		HITLOC_L_ARM_LWR = 0x9,
		HITLOC_R_HAND = 0xA,
		HITLOC_L_HAND = 0xB,
		HITLOC_R_LEG_UPR = 0xC,
		HITLOC_L_LEG_UPR = 0xD,
		HITLOC_R_LEG_LWR = 0xE,
		HITLOC_L_LEG_LWR = 0xF,
		HITLOC_R_FOOT = 0x10,
		HITLOC_L_FOOT = 0x11,
		HITLOC_GUN = 0x12,
		HITLOC_SHIELD = 0x13,
		HITLOC_ARMOR = 0x14,
		HITLOC_SOFT = 0x15,
		HITLOC_NUM = 0x16,
		HITLOC_LIMB_START = 0x6,
		HITLOC_LIMB_END = 0x11,
	};

	struct WeaponDef
	{
		const char* szOverlayName;
		XModel* gunXModel;
		XModel* gunXModelLeftHand;
		XModel* gunXModelRightHand;
		XModel* defaultViewModel;
		XModel* defaultWorldModelLeftHand;
		XModel* defaultWorldModelRightHand;
		XModel* worldModel;
		XModel* worldXModelLeftHand;
		XModel* worldXModelRightHand;
		XModel* defaultWorldModel;
		XModel* playerShadowModel;
		XModel* playerShadowModelLeftHand;
		XModel* playerShadowModelRightHand;
		XModel* handXModel;
		XModel** complexWorldModel;
		XModel** complexWorldModelLeftHand;
		XModel** complexWorldModelRightHand;
		XModel** complexGunXModel;
		XModel** complexGunXModelLeftHand;
		XModel** complexGunXModelRightHand;
		unsigned int numComplexModels;
		int pad; // padding
		Material* camoWorldModelMaterialOverride[16];
		Material* camoViewModelMaterialOverride[16];
		float unk_0; // set but unused?
		float unk_1; // set but unused?
		float unk_2; // set but unused?
		float unk_3; // set but unused?
		WeaponAnimPackage* szXAnims;
		WeaponAnimPackage* szXAnimsRightHanded;
		WeaponAnimPackage* szXAnimsLeftHanded;
		bool unk_4_0; // meleeAnims? // 1407448B0
		bool unk_4_1; // weaponCamo? // 140718630
		char pad17[2]; // padding
		int iFireTime;
		int iFireDelay;
		int iFireTimeAkimbo;
		int iFireDelayAkimbo;
		int pad3; // padding
		const char* szModeName;
		scr_string_t* notetrackRumbleMapKeys; //  array: 16
		scr_string_t* notetrackRumbleMapValues; //  array: 16
		scr_string_t* notetrackFXMapKeys; //  array: 16
		FxCombinedDef* notetrackFXMapValues; //  array: 16
		scr_string_t* notetrackFXMapTagValues; //  array: 16
		playerAnimType_t playerAnimType;
		scriptedAnimEvent_t scriptedAnimEvent;
		scriptedAnimType_t scriptedAnimType;
		weapType_t weapType;
		unsigned int whizbyType; // guessed // 140747580
		weapClass_t weapClass;
		PenetrateType penetrateType;
		AnimDamageType deathAnimDamageType;
		weapInventoryType_t inventoryType;
		weapFireType_t fireType;
		int burstCount;
		targetAssistType_t targetAssistType;
		targetAssistBehavior_t targetAssistBehavior;
		float pad28; // padding
		Material* unkMaterial; // -start AttUnknown01 // 140101130
		float unk_18;
		int unk_19;
		float unk_20;
		float unk_21;
		float unk_22;
		float unk_23;
		float unk_24;
		float unk_25;
		float unk_26;
		float unk_27;
		float unk_28;
		float unk_29;
		float unk_30;
		float unk_31;
		float unk_32;
		float unk_33;
		float unk_34;
		float unk_35;
		float unk_36;
		float unk_37;
		float unk_38;
		float unk_39;
		float unk_40;
		float unk_41;
		float unk_42;
		float unk_43;
		float unk_44;
		float unk_45;
		float unk_46;
		float unk_47;
		float unk_48;
		float unk_49;
		float unk_50;
		float unk_51;
		int unk_52;
		int unk_53;
		int unk_54;
		int unk_55;
		int unk_56;
		int unk_57;
		int unk_58;
		int unk_59;
		int unk_60;
		int unk_61;
		float unk_62;
		float unk_63;
		float unk_64;
		bool unk_65_0;
		bool unk_65_1;
		bool unk_65_2;
		bool unk_65_3; // -end
		float targetAssistRange;
		float targetAssistAngle;
		float targetAssistLosOffsetForward;
		float targetAssistLosOffsetRight;
		float targetAssistLosOffsetUp;
		bool targetAssistOnlyAvailableInAds;
		bool unk_71_1; // unused
		bool unk_71_2; // unused
		bool unk_71_3; // unused
		bool bSegmentedReload;
		bool unk_72_1; // unused/padding
		bool unk_72_2; // unused/padding
		bool unk_72_3; // unused/padding
		float burstFireCooldown;
		bool burstFireAuto;
		bool pad27[3]; // padding
		OffhandClass offhandClass;
		weapStance_t stance;
		float pad18; // padding
		WeaponVFXPackage* vfxPackage;
		WeaponSFXPackage* sfxPackage;
		ReticleType reticleType;
		float pad8; // padding
		Material* reticleCenter;
		Material* reticleSide;
		Material* reticleOnePiece;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		bool reticleCenterPulse;
		char pad25[3]; // padding
		activeReticleType_t activeReticleType;
		float vStandMove[3];
		float vStandRot[3];
		float strafeMove[3];
		float strafeRot[3];
		float vDuckedOfs[3];
		float vDuckedMove[3];
		float vDuckedRot[3];
		float vProneOfs[3];
		float vProneMove[3];
		float vProneRot[3];
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		float pad4; // padding
		XModel* worldClipModel;
		XModel* rocketModel;
		XModel* knifeModel;
		XModel* worldKnifeModel;
		Material* hudIcon;
		Material* pickupIcon;
		Material* dangerIcon;
		Material* throwBackIcon;
		bool hideWarningIcons;
		char pad6[3]; // padding
		float warningIconsDelay;
		Material* ammoCounterIcon;
		ammoCounterClipType_t ammoCounterClip;
		int iStartAmmo;
		int iPerkStartAmmo;
		int iPerkMaxAmmo;
		const char* szAmmoName;
		int iAmmoIndex; // runtime data
		int iMaxAmmo;
		int shotCount;
		const char* szSharedAmmoCapName;
		int iSharedAmmoCapIndex; // runtime data
		int iSharedAmmoCap;
		int iAmmoUsedPerShot;
		bool requireAmmoUsedPerShot;
		char pad24[3]; // padding
		float lowAmmoWarningThreshold;
		bool disableNoAmmoWarning;
		char pad19[3]; // padding
		int damage;
		int playerDamage;
		int meleeDamage;
		int iDamageType;
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimAssistPitchSlowdown;
		float aimAssistPitchSlowdownAds;
		float aimAssistYawSlowdown;
		float aimAssistYawSlowdownAds;
		float aimAssistLockonStrength;
		float aimPadding;
		float enemyCrosshairRange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		float sprintRestoreDelay;
		float fAdsZoomInFrac;
		float fAdsZoomOutFrac;
		ADSOverlay overlay;
		WeapOverlayInteface_t overlayInterface;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadSprintMin;
		float hipSpreadInAirMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float hipSpreadSprintMax;
		float hipSpreadInAirMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float hipSpreadSprintDecay;
		float hipSpreadInAirDecay;
		float fHipReticleSidePos;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float adsIdleLerpStartTime;
		float adsIdleLerpTime;
		float slideSpreadMin;
		float slideSpreadMax;
		float slideSpreadDecayRate;
		float slideSpreadFireAdd;
		float slideSpreadTurnAdd;
		float swayMaxAngleSteadyAim;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSway_UNKNOWN_Scale; // set but unused
		float adsSwayPitchScale;
		float adsSwayYawScale;
		float adsSwayHorizScale;
		float adsSwayVertScale;
		float adsFireRateScale;
		float adsDamageRangeScale;
		float adsFireAnimFrac;
		float fireTimerLerpToAdsScale;
		bool alwaysFireAtMaxRangeInAds;
		char pad23[3]; // padding
		float dualWieldViewModelOffset;
		float fScopeDriftDelay;
		float fScopeDriftLerpInTime;
		float fScopeDriftSteadyTime;
		float fScopeDriftLerpOutTime;
		float fScopeDriftSteadyFactor;
		float fScopeDriftUnsteadyFactor;
		weaponIconRatioType_t killIconRatio;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		int ammoDropStockMin;
		int ammoDropClipPercentMin;
		int ammoDropClipPercentMax;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iExplosionForceRadius;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		float bulletExplDmgMult;
		float bulletExplRadiusMult;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedForward;
		int iProjectileActivateDist;
		int iProjectileDetonationRadius;
		float projLifetime;
		float projLifetimeStdDeviation;
		float timeToAccelerate;
		float projectileCurvature;
		float pad2; // padding
		XModel* projectileModel;
		weapProjExposion_t projExplosion;
		FxCombinedDef projExplosionEffect;
		FxCombinedDef projDudEffect;
		WeapStickinessType stickiness;
		float ricochetChance;
		bool riotShieldEnableDamage;
		int riotShieldHealth;
		float riotShieldDamageMult;
		float pad7; // padding
		float* parallelBounce; // array: 64
		float* perpendicularBounce; // array: 64
		FxCombinedDef projBodyEffect;
		FxCombinedDef projTrailEffect;
		FxCombinedDef projBeaconEffect;
		vec3_t vProjectileColor;
		guidedMissileType_t guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		FxCombinedDef projIgnitionEffect;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		float adsInCrosshairAlphaStart;
		float adsInCrosshairAlphaEnd;
		float adsOutCrosshairAlphaStart;
		float adsOutCrosshairAlphaEnd;
		bool adsShouldShowCrosshair;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickMagMin;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsViewKickMagMin;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int iVisibilityAxis;
		float fVisibilityUpOffset;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickMagMin;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewKickMagMin;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float multipleReloadClipPercentage;
		float kickAlignedInputScalar;
		float kickOpposedInputScalar;
		float fightDist;
		float maxDist;
		const char* accuracyGraphName[2];
		vec2_t* originalAccuracyGraphKnots[2];
		unsigned short originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		bool softLeftRightArc;
		char pad20[3]; // padding
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char* szUseHintString;
		const char* dropHintString;
		unsigned int iUseHintStringIndex; // runtime data
		unsigned int dropHintStringIndex; // runtime data
		float horizViewJitter;
		float vertViewJitter;
		float scanSpeed;
		float scanAccel;
		int scanPauseTime;
		const char* szScript;
		float adsSpeedMs[2];
		int minDamage;
		int minPlayerDamage;
		int mid1Damage;
		int mid2Damage;
		int mid3Damage;
		int mid1PlayerDamage;
		int mid2PlayerDamage;
		int mid3PlayerDamage;
		float maxDamageRange;
		float mid1DamageRange;
		float mid2DamageRange;
		float mid3DamageRange;
		float minDamageRange;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float robotechMaxPitch;
		float robotechMaxYaw;
		float robotechFrequency;
		float robotechVariancePitch;
		float robotechVarianceYaw;
		float robotechVarianceFrequency;
		float* locationDamageMultipliers; // arr: 22
		unsigned char* hitLocPriorityMap; // arr: 22
		float unittypeMultipliers[9];
		float pad30;
		RumbleInfo* fireRumble;
		RumbleInfo* meleeImpactRumble;
		tracerStyle_t tracerStyle;
		TracerDef* tracerType;
		TracerDef* overchargeTracerType;
		LaserDef* laserTypeFriendly;
		LaserDef* laserTypeEnemy;
		bool turretADSEnabled;
		float turretADSTime;
		float turretFov;
		float turretFovADS;
		float turretScopeZoomRate;
		float turretScopeZoomMin;
		float turretScopeZoomMax;
		float turretOverheatUpRate;
		float turretOverheatDownRate;
		float turretOverheatPenalty;
		SndAliasLookup turretOverheatSound;
		FxCombinedDef turretOverheatEffect;
		RumbleInfo* turretBarrelSpinRumble;
		float turretBarrelSpinSpeed;
		float turretBarrelSpinUpTime;
		float turretBarrelSpinDownTime;
		SndAliasLookup turretBarrelSpinMaxSnd;
		SndAliasLookup turretBarrelSpinUpSnd[4];
		SndAliasLookup turretBarrelSpinDownSnd[4];
		SndAliasLookup missileConeSoundAlias;
		SndAliasLookup missileConeSoundAliasAtBase;
		float missileConeSoundRadiusAtTop;
		float missileConeSoundRadiusAtBase;
		float missileConeSoundHeight;
		float missileConeSoundOriginOffset;
		float missileConeSoundVolumescaleAtCore;
		float missileConeSoundVolumescaleAtEdge;
		float missileConeSoundVolumescaleCoreSize;
		float missileConeSoundPitchAtTop;
		float missileConeSoundPitchAtBottom;
		float missileConeSoundPitchTopSize;
		float missileConeSoundPitchBottomSize;
		float missileConeSoundCrossfadeTopSize;
		float missileConeSoundCrossfadeBottomSize;
		scr_string_t knifeAttachTagOverride;
		scr_string_t knifeAttachTagOverride2;
		bool knifeAlwaysAttached;
		bool meleeOverrideValues;
		char pad26[2]; // padding
		float aim_automelee_lerp;
		float aim_automelee_region_height;
		float aim_automelee_region_width;
		float aim_automelee_maxPitchMovement;
		float aim_automelee_maxYawMovement;
		float player_meleeHeight;
		float player_meleeWidth;
		float playerMeleeRangeStanding;
		float playerMeleeRangeCrouched;
		float playerMeleeRangeProne;
		float playerMeleeRangeChargeStanding;
		float playerMeleeRangeChargeCrouched;
		float playerMeleeChargeHeightTolerance;
		bool shieldAllowFiring;
		bool shieldUnkBool; // shieldAllowGesture? // 14004CCE0
		char pad13[2]; // padding
		offhandShieldMaxSpeed_t shieldMaxSpeed;
		bool shieldAlwaysDisplay;
		char pad14[3]; // padding
		Gesture* shieldDeployGesture;
		Gesture* shieldFireWeapGesture;
		Gesture* shieldDeployWhileFiring;
		Gesture* shieldRetractWhileFiring;
		Gesture* shieldBashGesture;
		FxCombinedDef shieldMeleeFx;
		float shieldMeleeFxDelay;
		float HitEarthquakeScale;
		float HitEarthquakeDuration;
		float HitEarthquakeRadius;
		RumbleInfo* shieldHitRumble;
		float MissEarthquakeScale;
		float MissEarthquakeDuration;
		float MissEarthquakeRadius;
		RumbleInfo* shieldMissRumble;
		int shieldDeployButton;
		bool shieldUsesEnergy;
		float shieldMaxEnergy;
		float shieldConsumptionRate;
		float shieldMeleeEnergyCost;
		float shieldMeleeHitEnergyCost;
		float reactiveMotionRadiusScale;
		float reactiveMotionFrequencyScale;
		float reactiveMotionAmplitudeScale;
		float reactiveMotionFalloff;
		float reactiveMotionLifetime;
		bool hasAnyTransientModels;
		bool sharedAmmo;
		bool lockonSupported;
		bool requireLockonToFire;
		bool isAirburstWeapon;
		bool bigExplosion;
		bool noAdsWhenMagEmpty;
		bool avoidDropCleanup;
		bool allowGrenadeSwitching;
		bool inheritsPerks;
		bool crosshairColorChange;
		bool bRifleBullet;
		bool bEnergyBullet;
		bool armorPiercing;
		bool impaling;
		bool bBoltAction;
		bool aimDownSight;
		bool canHoldBreath;
		bool meleeOnly;
		bool supportsAlternateMelee;
		bool canVariableZoom;
		bool bRechamberWhileAds;
		bool bBulletExplosiveDamage;
		bool bCookOffHold;
		bool reticleSpin45;
		bool bClipOnly;
		bool bDoesNotConsumeAmmo;
		bool bRemoveWeaponOnEmpty;
		bool noAmmoPickup;
		bool adsFireOnly;
		bool cancelAutoHolsterWhenEmpty;
		bool disableSwitchToWhenEmpty;
		bool suppressAmmoPrimaryDisplay;
		bool suppressAmmoReserveDisplay;
		bool laserSightDuringNightvision;
		bool markableViewmodel;
		DualWieldType dualWieldType;
		bool flipKillIcon;
		bool bNoPartialReload;
		bool reloadDisabled;
		bool blocksProne;
		bool silenced;
		bool isRollingGrenade;
		bool dropGrenadeHeldOnDeath;
		bool projExplosionEffectForceNormalUp;
		bool projExplosionEffectInheritParentDirection;
		bool bProjImpactExplode;
		bool disableProjectileCrumpleCheck;
		bool bProjTrajectoryEvents;
		bool bProjWhizByEnabled;
		bool stickToPlayers;
		bool stickToVehicles;
		bool stickToTurrets;
		bool stickToNonStick;
		bool projEnableMissileStickiness;
		bool thrownSideways;
		bool hasDetonator;
		bool disableFiring;
		bool firesOnWeaponSwitch;
		bool disableHolding;
		bool timedDetonation;
		int explosiveDamageDelay;
		int fuseTime;
		int aiFuseTime;
		int maxHoldTime;
		GrenadeRotationParams rotationParams;
		bool holdButtonToThrow;
		bool infiniteHold;
		bool freezeMovementWhenFiring;
		bool offhandAllowsSprint;
		bool thermalScope;
		bool thermalToggle;
		bool outlineEnemies;
		bool outlineDepthTest;
		bool outlineFill;
		char pad10[3]; // padding
		float enemyOutlineR;
		float enemyOutlineG;
		float enemyOutlineB;
		float enemyOutlineA;
		float allyOutlineR;
		float allyOutlineG;
		float allyOutlineB;
		float allyOutlineA;
		bool depthScan;
		char pad11[3]; // padding
		float depthScanThickness;
		float depthScanR;
		float depthScanG;
		float depthScanB;
		float depthScanA;
		float depthScanOutlineThickness;
		float depthScanOutlineR;
		float depthScanOutlineG;
		float depthScanOutlineB;
		float depthScanOutlineA;
		bool depthScanOverlay;
		char pad12[3]; // padding
		float depthScanOverlayStrength;
		float depthScanOverlayXTiles;
		float depthScanOverlayYTiles;
		float depthScanOverlayXScroll;
		float depthScanOverlayYScroll;
		float blurSceneAdsInFraction;
		float blurSceneAdsOutFraction;
		bool altModeSameWeapon;
		bool turretBarrelSpinEnabled;
		bool missileConeSoundEnabled;
		bool missileConeSoundPitchshiftEnabled;
		bool missileConeSoundCrossfadeEnabled;
		bool offhandHoldIsCancelable;
		bool doNotAllowAttachmentsToOverrideSpread;
		bool useScopeDrift;
		bool alwaysShatterGlassOnImpact;
		bool oldWeapon;
		bool jumpSpread;
		bool noFullViewmodelAnimations;
		float killcamOffset;
		bool useDualFOV;
		bool disableDrop;
		bool preferredDrop;
		char pad21[5]; // pad
		Gesture* gestureAnimation;
		float gestureFireStateTime;
		Gesture* gesturePullback;
		float minPullbackTime;
		Gesture* gestureThrow;
		float gestureFireTime;
		Gesture* gestureDetonate;
		float gestureDetonationTime;
		float unk_433; // 1400688A0
		const char* unknownString1;
		float unk_434; // 1400688A0
		char pad22[4]; // pad
		const char* unknownString2;
		bool gesturesDisablePrimary;
		char pad15[7]; // pad
		FxCombinedDef cameraFireEffect;
		float cameraFireEffectDurationSec;
		float changedFireTime;
		float changedFireTimeAkimbo;
		int changedFireTimeNumBullets;
		WeaponChargeType chargeType;
		float chargeGain;
		float chargeCostPerShot;
		float chargeLossWhenIdle;
		float chargeEmptyCooldown;
		float chargeFireAtMaxDamageMultiplier;
		int chargeMeterEffectBoneCount;
		FxCombinedDef chargeMeterEffect;
		SndAliasLookup chargeUpSound;
		SndAliasLookup chargeDownSound;
		SndAliasLookup chargeUpSoundPlayer;
		SndAliasLookup chargeDownSoundPlayer;
		SndAliasLookup chargeDownToUpSound;
		SndAliasLookup chargeDownToUpSoundPlayer;
		SndAliasLookup chargeUpToDownSound;
		SndAliasLookup chargeUpToDownSoundPlayer;
		SndAliasLookup chargeMaxSound;
		SndAliasLookup chargeMaxSoundPlayer;
		bool chargeHudReveal;
		RumbleInfo* chargeRumble;
		scr_string_t stowTag;
		XModel* stowOffsetModel;
		WeaponSlot slot;
		float maxTargetAngle;
		bool spaceshipSecondaryWeapon;
		float impulseFieldRadius;
		float impulseFieldInitialSpeed;
		float impulseFieldMaxSpeed;
		float impulseFieldAcceleration;
		float impulseFieldInAirImpulseMultiplier;
		float impulseFieldInAirImpulseMultiplierInterpTime;
		float impulseFieldSlideMultiplier;
		float impulseFieldSlideMultiplierInterpTime;
		bool impulseFieldIsPush;
		bool impulseFieldAffectsFriendlies;
		bool impulseFieldAffectsSelf;
		bool impulseFieldAffectsProne;
		bool regenerationEnabled;
		int regenerationTimeMs;
		int regenerationAddTimeMs;
		int regenerationAmount;
		bool regenerationConsumeStock;
		bool regenerationDisableWhileFiring;
		bool deployRequireOnWalkableSurface;
		bool deployRequireOnNavmesh;
		bool deployRequireSkyAbove;
		bool deployRequireNoOverhang;
		bool deployAlwaysUpright;
		bool deployEdgeSnap;
		float deployCylinderRadius;
		float deployCylinderHeight;
		float deployMaxDistance;
		float pad16;  // pad
		FxCombinedDef deployEffect;
		int deployValidGroupId;
		int deployIndoorGroupId;
		int deployInvalidGroupId;
		int deployOutOfRangeGroupId;
		AnimOverride animOverrides[21];
		SFXOverride sfxOverrides[21];
		VFXOverride vfxOverrides[21];
		float reactiveEmitterDelay;
		float grenadeDangerIconDistance;
		bool unk_466_0; // unk // 14074E570
		char pad29[7]; // pad
	}; assert_sizeof(WeaponDef, 0x1418);
	assert_offsetof(WeaponDef, handXModel, 112);
	assert_offsetof(WeaponDef, camoWorldModelMaterialOverride, 176);
	assert_offsetof(WeaponDef, camoViewModelMaterialOverride, 304);
	assert_offsetof(WeaponDef, szXAnims, 448);
	assert_offsetof(WeaponDef, szModeName, 496);
	assert_offsetof(WeaponDef, notetrackFXMapTagValues, 536);
	assert_offsetof(WeaponDef, reticleOnePiece, 888);
	assert_offsetof(WeaponDef, hudIcon, 1112);
	assert_offsetof(WeaponDef, szAmmoName, 1176);
	assert_offsetof(WeaponDef, szSharedAmmoCapName, 1200);
	assert_offsetof(WeaponDef, projDudEffect, 1752);
	assert_offsetof(WeaponDef, parallelBounce, 1792);
	assert_offsetof(WeaponDef, projIgnitionEffect, 1880);
	assert_offsetof(WeaponDef, accuracyGraphName, 2104);
	assert_offsetof(WeaponDef, szUseHintString, 2216);
	assert_offsetof(WeaponDef, szScript, 2264);
	assert_offsetof(WeaponDef, locationDamageMultipliers, 2368);
	assert_offsetof(WeaponDef, fireRumble, 2424);
	assert_offsetof(WeaponDef, laserTypeEnemy, 2472);
	assert_offsetof(WeaponDef, turretOverheatSound, 2520);
	assert_offsetof(WeaponDef, missileConeSoundAliasAtBase, 2648);
	assert_offsetof(WeaponDef, knifeAttachTagOverride, 2708);
	assert_offsetof(WeaponDef, shieldDeployGesture, 2784);
	assert_offsetof(WeaponDef, shieldHitRumble, 2856);
	assert_offsetof(WeaponDef, shieldMissRumble, 2880);
	assert_offsetof(WeaponDef, gestureAnimation, 3208);
	assert_offsetof(WeaponDef, gestureDetonate, 3256);
	assert_offsetof(WeaponDef, unknownString2, 3288);
	assert_offsetof(WeaponDef, cameraFireEffect, 3304);
	assert_offsetof(WeaponDef, chargeMeterEffect, 3368);
	assert_offsetof(WeaponDef, chargeRumble, 3472);
	assert_offsetof(WeaponDef, stowOffsetModel, 3488);

	struct NoteTrackToSoundEntry
	{
		unsigned int numSoundMappings;
		scr_string_t attachment;
		scr_string_t* notetrackSoundMapKeys;
		scr_string_t* notetrackSoundMapValues;
	};

	enum SuitAnimType : std::int32_t
	{
		ANIM_SUIT_HUMAN = 0x0,
		ANIM_SUIT_ROBOTIC = 0x1,
		ANIM_SUIT_C8 = 0x2,
		ANIM_SUIT_REAPER = 0x3,
		NUM_ANIM_SUIT_STATE = 0x4,
		ANIM_SUIT_INVALID = 0x4,
	};

	struct NoteTrackToSuitSoundEntry
	{
		unsigned int numSoundMappings;
		SuitAnimType suitAnimState;
		scr_string_t* notetrackSoundMapKeys;
		scr_string_t* notetrackSoundMapValues;
	};

	struct WeaponCompleteDef
	{
		union
		{
			const char* szInternalName;
			const char* name;
		};
		WeaponDef* weapDef;
		const char* szDisplayName;
		const char* szLootTable;
		scr_string_t* hideTags; // arr: 32
		unsigned int numAttachments;
		unsigned int numAttachments2;
		unsigned int numAttachments3; // underbarrel
		unsigned int numAttachments4;
		unsigned int numAttachments5; // cosmetic
		unsigned int numAttachments6;
		int numLootVariants; // runtime data
		int pad; // padding
		WeaponAttachment** attachments;
		WeaponAttachment** attachments2;
		WeaponAttachment** attachments3;
		WeaponAttachment** attachments4;
		WeaponAttachment** attachments5;
		WeaponAttachment** attachments6;
		unsigned int numNotetrackSoundMappings;
		int pad2; // padding
		scr_string_t* notetrackSoundMapKeys;
		scr_string_t* notetrackSoundMapValues;
		unsigned int numNotetrackOverrides;
		NoteTrackToSoundEntry* notetrackOverrides;
		unsigned int numNotetrackSuitEntries;
		NoteTrackToSuitSoundEntry* notetrackSuitEntries;
		float fAdsZoomFov;
		int iAdsTransInTime;
		int iAdsTransOutTime;
		int iClipSize;
		ImpactType vfxImpactType;
		ImpactType sfxImpactType;
		MeleeImpactType meleeImpactType;
		float penetrateMultiplier;
		float fAdsViewKickCenterSpeed;
		float fHipViewKickCenterSpeed;
		const char* szAltWeaponName;
		Material* killIcon;
		Material* dpadIcon;
		int ammoDropStockMax;
		float adsDofStart;
		float adsDofEnd;
		unsigned short accuracyGraphKnotCount[2];
		vec2_t* accuracyGraphKnots[2];
		bool motionTracker;
		bool enhanced;
		bool dpadIconShowsAmmo;
		bool luiWeaponInfoWidgetUsesScopeStencil;
		const char* szAdsrBaseSetting;
		const char* szLUIWeaponInfoWidgetName;
		const char* szLUIWeaponInfoWidgetTag;
		const char* szLUICrosshairWidget;
	}; assert_sizeof(WeaponCompleteDef, 0x138);

	enum PARTICLE_MODULE_FLAG : std::uint32_t
	{
		PARTICLE_MODULE_FLAG_DISABLED = 0x1,
		PARTICLE_MODULE_FLAG_HAS_CURVES = 0x2,
		PARTICLE_MODULE_FLAG_USE_EMITTER_LIFE = 0x4,
		PARTICLE_MODULE_FLAG_HAS_ASSETS = 0x8,
		PARTICLE_MODULE_FLAG_RANDOMIZE_BETWEEN_CURVES = 0x10,
		PARTICLE_MODULE_FLAG_USE_NON_UNIFORM_INTERPOLATION = 0x20,
		PARTICLE_MODULE_FLAG_NO_LERP = 0x40,
		PARTICLE_MODULE_FLAG_USE_WORLD_SPACE = 0x80,
		PARTICLE_MODULE_FLAG_GRAPH_BY_LENGTH = 0x100,
		PARTICLE_MODULE_FLAG_UPDATE_EXTERNAL = 0x200,
		PARTICLE_MODULE_FLAG_GRAPH_ADDITIVE = 0x400,
		PARTICLE_MODULE_FLAG_HAS_LIGHT_DEFS = 0x800,
		PARTICLE_MODULE_FLAG_GRAPH_IS_LINEAR = 0x1000,
		PARTICLE_MODULE_FLAG_MODEL_IGNORE_EMITTER_ORIENTATION = 0x2000,
		PARTICLE_MODULE_FLAG_USE_OCCLUSION_QUERY_WORLD_SIZE = 0x4000,
		PARTICLE_MODULE_FLAG_EMISSION_CURVE_SCRUBBED_BY_GAME = 0x8000,
		PARTICLE_MODULE_FLAG_ON_IMPACT_BOLT_TO_ENTITY = 0x10000,
		PARTICLE_MODULE_FLAG_RAYCAST_COLLIDE_WITH_WZ_TRAIN = 0x20000,
		PARTICLE_MODULE_FLAGS_ALL = 0xFFFFFFFF,
	};

	enum PARTICLE_STATE_DEF_FLAG : std::uint64_t
	{
		PARTICLE_STATE_DEF_FLAG_DISABLED = 0x1,
		PARTICLE_STATE_DEF_FLAG_HAS_CAMERA_OFFSET = 0x2,
		PARTICLE_STATE_DEF_FLAG_HAS_SPAWN_SHAPE = 0x4,
		PARTICLE_STATE_DEF_FLAG_HAS_POSITION_CURVE = 0x8,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_1D_CURVE = 0x10,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_3D_CURVE = 0x20,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_1D_INIT = 0x40,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_3D_INIT = 0x80, // c
		PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_LOCAL1 = 0x100,
		PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_WORLD1 = 0x200,
		PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_LOCAL2 = 0x400,
		PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE_WORLD2 = 0x800,
		PARTICLE_STATE_DEF_FLAG_USE_PHYSICS = 0x1000, // c
		PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_HORIZONTALLY = 0x2000, // c
		PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_HORIZONTALLY_RANDOM = 0x4000, // c
		PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_VERTICALLY = 0x8000, // c
		PARTICLE_STATE_DEF_FLAG_MIRROR_TEXTURE_VERTICALLY_RANDOM = 0x10000, // c
		PARTICLE_STATE_DEF_FLAG_SORT_PARTICLES = 0x20000,
		PARTICLE_STATE_DEF_FLAG_HANDLE_ON_IMPACT = 0x40000,
		PARTICLE_STATE_DEF_FLAG_PLAYER_FACING = 0x80000, // c
		PARTICLE_STATE_DEF_FLAG_PLAYER_FACING_LOCK_UP_VECTOR = 0x100000, // c
		PARTICLE_STATE_DEF_FLAG_USE_OCCLUSION_QUERY = 0x200000, // c

		PARTICLE_STATE_DEF_FLAG_HAS_COLOR = 0x400000,
		PARTICLE_STATE_DEF_FLAG_HAS_RAY_CAST_PHYSICS = 0x800000,
		PARTICLE_STATE_DEF_FLAG_0x1000000 = 0x1000000,

		PARTICLE_STATE_DEF_FLAG_HAS_EMISSIVE_CURVE = 0x2000000, // c
		PARTICLE_STATE_DEF_FLAG_HAS_INTENSITY_CURVE = 0x4000000,
		PARTICLE_STATE_DEF_FLAG_USE_VECTOR_FIELDS = 0x8000000,
		PARTICLE_STATE_DEF_FLAG_INHERIT_PARENT_VELOCITY = 0x10000000,
		PARTICLE_STATE_DEF_FLAG_DRAW_WITH_VIEW_MODEL = 0x20000000,
		PARTICLE_STATE_DEF_FLAG_PLAY_SOUNDS = 0x40000000,
		PARTICLE_STATE_DEF_FLAG_HAS_CAMERA_OFFSET_POSITION_ONLY = 0x40000000, // c
		PARTICLE_STATE_DEF_FLAG_ON_IMPACT_USE_SURFACE_TYPE = 0x80000000,
		PARTICLE_STATE_DEF_FLAG_IS_SPRITE = 0x100000000, // c
		PARTICLE_STATE_DEF_FLAG_HAS_TRANS_SHADOWS = 0x200000000, // c
		PARTICLE_STATE_DEF_FLAG_HAS_CHILD_EFFECTS = 0x400000000,
		PARTICLE_STATE_DEF_FLAG_BLOCKS_SIGHT = 0x0,
		PARTICLE_STATE_DEF_FLAG_HANDLE_TIME_IN_STATE = 0x0,
		PARTICLE_STATE_DEF_FLAG_SCALE_BY_DISTANCE = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_VECTOR_FIELD_CURVE = 0x0,
		PARTICLE_STATE_DEF_FLAG_USE_LOCAL_VECTOR_FIELDS_ONLY = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_SHADER_CURVE = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_SIZE_CURVE = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_SIZE_LERP = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_TEMPERATURE_CURVE = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_LIGHTING_FRACTION_CURVE = 0x0,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_CURVE = 0x30,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_1D = 0x50,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION_3D = 0xA0,
		PARTICLE_STATE_DEF_FLAG_HAS_ROTATION = 0xF0,
		PARTICLE_STATE_DEF_FLAG_HAS_MIRROR_TEXTURE = 0x1E000, // c
		PARTICLE_STATE_DEF_FLAG_HAS_VELOCITY_CURVE = 0x300,
		PARTICLE_STATE_DEF_FLAG_REQUIRES_WORLD_COLLISION = 0x801000,
	};

	enum PARTICLE_EMITTER_DEF_FLAG : std::uint32_t
	{
		PARTICLE_EMITTER_DEF_FLAG_DISABLED = 0x1,
		PARTICLE_EMITTER_DEF_FLAG_USE_BURST_MODE = 0x2,
		PARTICLE_EMITTER_DEF_FLAG_DRAW_PAST_FOG = 0x4,
		PARTICLE_EMITTER_DEF_FLAG_INFINITE_PARTICLE_LIFE = 0x8,
		PARTICLE_EMITTER_DEF_FLAG_LOOP_INFINITE_PARTICLES = 0x10,
		PARTICLE_EMITTER_DEF_FLAG_HAS_PHYSICS_HEAVY = 0x20,
		PARTICLE_EMITTER_DEF_FLAG_HAS_SCRIPTED_INPUTS = 0x40,
		PARTICLE_EMITTER_DEF_FLAG_HAS_LIGHTS = 0x80,
		PARTICLE_EMITTER_DEF_FLAG_USE_SCRIPTED_VELOCITY = 0x100,
		PARTICLE_EMITTER_DEF_FLAG_EMIT_BY_DISTANCE = 0x200,
		PARTICLE_EMITTER_DEF_FLAG_USE_OCCLUSION_QUERY = 0x400,
		PARTICLE_EMITTER_DEF_FLAG_HAS_TRANS_SHADOWS = 0x800,
		PARTICLE_EMITTER_DEF_FLAG_GROUP_ONLY_WHEN_NVG_ON = 0x1000,
		PARTICLE_EMITTER_DEF_FLAG_GROUP_ONLY_WHEN_NVG_OFF = 0x2000,
		PARTICLE_EMITTER_DEF_FLAG_GROUP_ONLY_WHEN_THERMAL_ON = 0x4000,
		PARTICLE_EMITTER_DEF_FLAG_GROUP_ONLY_WHEN_THERMAL_OFF = 0x8000,
		PARTICLE_EMITTER_DEF_FLAG_DISABLE_IN_SP = 0x10000,
		PARTICLE_EMITTER_DEF_FLAG_DISABLE_IN_MP_LPC = 0x20000,
		PARTICLE_EMITTER_DEF_FLAG_DISABLE_IN_MP_HPC = 0x40000,
		PARTICLE_EMITTER_DEF_FLAG_DISABLE_IN_CP = 0x80000,
		PARTICLE_EMITTER_DEF_FLAG_HAS_INSTANCE_POOL = 0x100000,
		PARTICLE_EMITTER_DEF_FLAG_FORCE_HALF_RES = 0x200000,
		PARTICLE_EMITTER_DEF_FLAG_FORCE_FULL_RES = 0x400000,
		PARTICLE_EMITTER_DEF_FLAG_IGNORE_MODIFIERS = 0x800000,
		PARTICLE_EMITTER_DEF_FLAG_HAS_BARREL_HEAT_SCALE = 0x1000000,
		PARTICLE_EMITTER_DEF_FLAG_IGNORE_FOV_SCALE = 0x2000000,
	};

	enum PARTICLE_SYSTEM_DEF_FLAG : std::uint32_t
	{
		PARTICLE_SYSTEM_DEF_FLAG_HAS_SPRITES = 0x1,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_NON_SPRITES = 0x2,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_LIGHTS = 0x4,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_PHYSICS_HEAVY = 0x8,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_PHYSICS_LIGHT = 0x10,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_SCRIPTED_INPUTS = 0x20,
		PARTICLE_SYSTEM_DEF_FLAG_USE_OCCLUSION_QUERY = 0x40, // c
		PARTICLE_SYSTEM_DEF_FLAG_USE_OCCLUSION_QUERY_OVERRIDE_POS = 0x80,
		PARTICLE_SYSTEM_DEF_FLAG_KILL_ON_OWNER_ENTITY_DEATH = 0x100,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_TRANS_SHADOWS = 0x200,
		PARTICLE_SYSTEM_DEF_FLAG_ALIGN_TO_SUN = 0x400,
		PARTICLE_SYSTEM_DEF_FLAG_KILL_ON_KILLCAM_TRANSITION = 0x800,
		PARTICLE_SYSTEM_DEF_FLAG_CANNOT_PRE_ROLL = 0x1000,
		PARTICLE_SYSTEM_DEF_FLAG_ALWAYS_UPDATE_BONE_INDEX = 0x2000,
		PARTICLE_SYSTEM_DEF_FLAG_AFFECTS_GAMEPLAY = 0x4000,
		PARTICLE_SYSTEM_DEF_FLAG_UPDATE_CHILDREN_AFTER_BOLTING = 0x8000,
		PARTICLE_SYSTEM_DEF_FLAG_PRE_ROLL_CHILD_EFFECTS = 0x10000,
		PARTICLE_SYSTEM_DEF_FLAG_KILL_ON_KILLCAM_ENTITY_TRANSITION = 0x20000,
		PARTICLE_SYSTEM_DEF_FLAG_KILL_STOPPED_INFINITE_EFFECTS = 0x40000,
		PARTICLE_SYSTEM_DEF_FLAG_PARENT_UPDATES_CHILD = 0x80000,
		PARTICLE_SYSTEM_DEF_FLAG_STOP_ON_DYNAMIC_BOLT_DEATH = 0x100000,
		PARTICLE_SYSTEM_DEF_FLAG_DISABLE_IN_SPLIT_SCREEN = 0x200000,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_EMITTER_GROUP_IDS = 0x400000,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_EMITTER_NVG_OR_THERMAL = 0x800000,
		PARTICLE_SYSTEM_DEF_FLAG_HAS_INSTANCE_POOL = 0x1000000,
		PARTICLE_SYSTEM_DEF_FLAG_HIDE_IF_BONE_IS_HIDDEN = 0x2000000,
		PARTICLE_SYSTEM_DEF_FLAG_FOV_CULL = 0x4000000,
		PARTICLE_SYSTEM_DEF_FLAG_USE_OCCLUSION_QUERY_OVERRIDE_LOC = 0x8000000,
	};

	enum ParticleModuleType : std::uint16_t
	{
		PARTICLE_MODULE_INIT_ATLAS = 0x0,
		PARTICLE_MODULE_INIT_ATTRIBUTES = 0x1,
		PARTICLE_MODULE_INIT_BEAM = 0x2,
		PARTICLE_MODULE_INIT_CAMERA_OFFSET = 0x3,
		PARTICLE_MODULE_INIT_CLOUD = 0x4,
		PARTICLE_MODULE_INIT_DECAL = 0x5,
		PARTICLE_MODULE_INIT_FLARE = 0x6,
		PARTICLE_MODULE_INIT_GEO_TRAIL = 0x7,
		PARTICLE_MODULE_INIT_LIGHT_OMNI = 0x8,
		PARTICLE_MODULE_INIT_LIGHT_SPOT = 0x9,
		PARTICLE_MODULE_INIT_MATERIAL = 0xA,
		PARTICLE_MODULE_INIT_MIRROR_TEXTURE = 0xB,
		PARTICLE_MODULE_INIT_MODEL = 0xC,
		PARTICLE_MODULE_INIT_OCCLUSION_QUERY = 0xD,
		PARTICLE_MODULE_INIT_ORIENTED_SPRITE = 0xE,
		PARTICLE_MODULE_INIT_PARTICLE_SIM = 0xF,
		PARTICLE_MODULE_INIT_PLAYER_FACING = 0x10,
		PARTICLE_MODULE_INIT_RELATIVE_VELOCITY = 0x11,
		PARTICLE_MODULE_INIT_ROTATION = 0x12,
		PARTICLE_MODULE_INIT_ROTATION_3D = 0x13,
		PARTICLE_MODULE_INIT_RUNNER = 0x14,
		PARTICLE_MODULE_INIT_SOUND = 0x15,
		PARTICLE_MODULE_INIT_SPAWN = 0x16,
		PARTICLE_MODULE_INIT_SPAWN_SHAPE_BOX = 0x17,
		PARTICLE_MODULE_INIT_SPAWN_SHAPE_CYLINDER = 0x18,
		PARTICLE_MODULE_INIT_SPAWN_SHAPE_ELLIPSOID = 0x19,
		PARTICLE_MODULE_INIT_SPAWN_SHAPE_MESH = 0x1A,
		PARTICLE_MODULE_INIT_SPAWN_SHAPE_SPHERE = 0x1B,
		PARTICLE_MODULE_INIT_TAIL = 0x1C,
		PARTICLE_MODULE_INIT_VECTOR_FIELD = 0x1D,
		PARTICLE_MODULE_INIT_VOLUMETRIC = 0x1E,
		PARTICLE_MODULE_ATTRACTOR = 0x1F,
		PARTICLE_MODULE_COLOR_GRAPH = 0x20,
		PARTICLE_MODULE_COLOR_LERP = 0x21,
		PARTICLE_MODULE_EMISSION_GRAPH = 0x22,
		PARTICLE_MODULE_EMISSIVE_GRAPH = 0x23,
		PARTICLE_MODULE_FORCE = 0x24,
		PARTICLE_MODULE_GRAVITY = 0x25,
		PARTICLE_MODULE_INTENSITY_GRAPH = 0x26,
		PARTICLE_MODULE_PARENT_VELOCITY_GRAPH = 0x27,
		PARTICLE_MODULE_PHYSICS_LIGHT = 0x28,
		PARTICLE_MODULE_PHYSICS_RAY_CAST = 0x29,
		PARTICLE_MODULE_POSITION_GRAPH = 0x2A,
		PARTICLE_MODULE_ROTATION_GRAPH = 0x2B,
		PARTICLE_MODULE_ROTATION_GRAPH_3D = 0x2C,
		PARTICLE_MODULE_SIZE_GRAPH = 0x2D,
		PARTICLE_MODULE_SIZE_LERP = 0x2E,
		PARTICLE_MODULE_VELOCITY_GRAPH = 0x2F,
		PARTICLE_MODULE_TEST_AGE = 0x30,
		PARTICLE_MODULE_TEST_BIRTH = 0x31,
		PARTICLE_MODULE_TEST_DEATH = 0x32,
		PARTICLE_MODULE_TEST_GRAVITY = 0x33,
		PARTICLE_MODULE_TEST_IMPACT = 0x34,
		PARTICLE_MODULE_TEST_POS = 0x35,
		PARTICLE_MODULE_COUNT = 0x36,
		PARTICLE_MODULE_INVALID = 0xFF,
	};

	enum PARTICLE_ELEMENT_TYPE : std::uint32_t
	{
		PARTICLE_ELEMENT_TYPE_BILLBOARD_SPRITE = 0,
		PARTICLE_ELEMENT_TYPE_BEAM = 1,
		PARTICLE_ELEMENT_TYPE_CLOUD = 2,
		PARTICLE_ELEMENT_TYPE_SPARK_CLOUD = 3,
		PARTICLE_ELEMENT_TYPE_DECAL = 4,
		PARTICLE_ELEMENT_TYPE_FLARE = 5,
		PARTICLE_ELEMENT_TYPE_GEO_TRAIL = 6,
		PARTICLE_ELEMENT_TYPE_LIGHT_OMNI = 7,
		PARTICLE_ELEMENT_TYPE_LIGHT_SPOT = 8,
		PARTICLE_ELEMENT_TYPE_MODEL = 9,
		PARTICLE_ELEMENT_TYPE_ORIENTED_SPRITE = 10,
		PARTICLE_ELEMENT_TYPE_RUNNER = 11,
		PARTICLE_ELEMENT_TYPE_TAIL = 12,
		PARTICLE_ELEMENT_TYPE_VECTOR_FIELD = 13,
		PARTICLE_ELEMENT_TYPE_VOLUMETRIC = 14,
	};

	struct float4
	{
		float v[4];
	};

	struct ParticleFloatRange
	{
		float min;
		float max;
	};

	struct ParticleIntRange
	{
		int min;
		int max;
	};

	struct ParticlePhysicsFXData
	{
		PhysicsFXPipeline* physicsFXPipeline;
		PhysicsFXShape* physicsFXShape;
	};

	struct ParticleMarkVisuals
	{
		Material* materials[3];
	};

	union ParticleLinkedAssetDef
	{
		Material* material;
		XModel* model;
		ParticleSystemDef* particleSystem;
		ParticlePhysicsFXData physicsFXData;
		FxParticleSimAnimation* particleSim;
		ParticleMarkVisuals decal;
		const char* sound;
		VectorField* vectorField;
		GfxLightDef* lightDef;
		char pad[32];
	}; assert_sizeof(ParticleLinkedAssetDef, 32);

	struct ParticleModule
	{
		ParticleModuleType type;
		unsigned int m_flags;
	};

	struct ParticleCurveControlPointDef
	{
		float time;
		float value;
		float invTimeDelta;
		unsigned int pad[1];
	};

	struct ParticleCurveDef
	{
		ParticleCurveControlPointDef* controlPoints;
		int numControlPoints;
		float scale;
	};

	struct ParticleModuleInitAtlas : ParticleModule
	{
		int m_playRate;
		int m_startFrame;
		int m_loopCount;
	}; assert_sizeof(ParticleModuleInitAtlas, 20);

	struct ParticleModuleInitAttributes : ParticleModule
	{
		bool m_useNonUniformInterpolationForColor;
		bool m_useNonUniformInterpolationForSize;
		char m_pad[6];
		float4 m_sizeMin;
		float4 m_sizeMax;
		float4 m_colorMin;
		float4 m_colorMax;
		float4 m_velocityMin;
		float4 m_velocityMax;
	}; assert_sizeof(ParticleModuleInitAttributes, 112);

	enum ParticleModuleInitBeamFlags
	{
		PARTICLE_MODULE_INIT_BEAM_FLAG_TILE = 0x1,
		PARTICLE_MODULE_INIT_BEAM_FLAG_USE_CURVE_POINTS = 0x2,
		PARTICLE_MODULE_INIT_BEAM_FLAG_CAMERA_FACING = 0x4,
	};

	struct ParticleLinkedAssetListDef
	{
		ParticleLinkedAssetDef* assetList;
		int numAssets;
	};

	struct ParticleModuleInitBeam : ParticleModule
	{
		ParticleModuleInitBeamFlags m_beamFlags;
		float m_splitDistance;
		float4 m_startPos;
		float4 m_endPos;
		float4 m_offset;
		float4 m_curvePoint1;
		float4 m_curvePoint2;
		ParticleLinkedAssetListDef m_linkedAssetList;
		float m_scrollRateUV;
		float m_scrollRateSpiralGraph;
		float m_spiralRepeatDistance;
	}; assert_sizeof(ParticleModuleInitBeam, 128);

	struct ParticleModuleInitCameraOffset : ParticleModule
	{
		unsigned int m_pad[2];
	}; assert_sizeof(ParticleModuleInitCameraOffset, 16);

	struct ParticleModuleInitCloud : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef curves[2];
	}; assert_sizeof(ParticleModuleInitCloud, 48);

	struct ParticleModuleInitDecal : ParticleModule
	{
		unsigned short m_fadeInTime;
		unsigned short m_fadeOutTime;
		unsigned short m_stoppableFadeOutTime;
		unsigned short m_lerpWaitTime;
		float4 m_lerpColor;
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitDecal, 48);
	assert_offsetof(ParticleModuleInitDecal, m_linkedAssetList, 32);

	struct ParticleModuleInitFlare : ParticleModule
	{
		unsigned int m_pad[1];
		float m_position;
		float4 m_direction;
		int m_angularRotCount;
		unsigned int m_flareFlags;
		ParticleFloatRange m_depthScaleRange;
		ParticleFloatRange m_depthScaleValue;
		ParticleFloatRange m_radialRot;
		ParticleFloatRange m_radialScaleX;
		ParticleFloatRange m_radialScaleY;
		ParticleCurveDef m_curves[4];
	}; assert_sizeof(ParticleModuleInitFlare, 144);
	assert_offsetof(ParticleModuleInitFlare, m_curves, 80);

	struct ParticleModuleInitGeoTrail : ParticleModule
	{
		unsigned int m_numPointsMax;
		float m_splitDistance;
		float m_splitAngle;
		float m_centerOffset;
		unsigned int m_numSheets;
		float m_fadeInDistance;
		float m_fadeOutDistance;
		float m_tileDistance;
		ParticleFloatRange m_tileOffset;
		float m_scrollTime;
		bool m_useLocalVelocity;
		bool m_useVerticalTexture;
		bool m_cameraFacing;
		bool m_fixLeadingEdge;
		bool m_clampUVs;
		char m_pad[3];
		unsigned int m_pad2[1];
	}; assert_sizeof(ParticleModuleInitGeoTrail, 64);

	struct ParticleModuleInitLightOmni : ParticleModule
	{
		ParticleLinkedAssetListDef m_linkedAssetList;
		float m_tonemappingScaleFactor;
		float m_intensityIR;
		bool m_disableVolumetric;
		unsigned char m_exponent;
		char __pad0[2];
		float m_shadowSoftness;
		float m_shadowBias;
		float m_shadowArea;
	}; assert_sizeof(ParticleModuleInitLightOmni, 48);

	struct ParticleModuleInitLightSpot : ParticleModule
	{
		float m_fovOuter;
		float m_fovInner;
		float m_bulbRadius;
		float m_bulbLength;
		float m_brightness;
		float unk1;
		float m_intensityUV;
		float m_intensityIR;
		float m_shadowNearPlane;
		float m_toneMappingScaleFactor;
		float m_shadowSoftness;
		float m_shadowBias;
		float m_shadowArea;
		bool m_disableVolumetric;
		bool m_disableShadowMap;
		bool m_disableDynamicShadows;
		bool m_exponent;
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitLightSpot, 80);
	assert_offsetof(ParticleModuleInitLightSpot, m_linkedAssetList, 64);

	struct ParticleModuleInitMaterial : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitMaterial, 32);

	enum ParticleMirrorTextureType
	{
		PARTICLE_MIRROR_TEXTURE_TYPE_NONE = 0x0,
		PARTICLE_MIRROR_TEXTURE_TYPE_STANDARD = 0x1,
		PARTICLE_MIRROR_TEXTURE_TYPE_RANDOM = 0x2,
		PARTICLE_MIRROR_TEXTURE_TYPE_COUNT = 0x3,
	};

	struct ParticleModuleInitMirrorTexture : ParticleModule
	{
		ParticleMirrorTextureType m_mirrorHorizontal;
		ParticleMirrorTextureType m_mirrorVertical;
	}; assert_sizeof(ParticleModuleInitMirrorTexture, 16);

	struct ParticleModuleInitModel : ParticleModule
	{
		bool m_usePhysics;
		bool m_motionBlurHQ;
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitModel, 32);
	assert_offsetof(ParticleModuleInitModel, m_linkedAssetList, 16);

	struct ParticleModuleInitOcclusionQuery : ParticleModule
	{
		float m_fadeInTime;
		float m_fadeOutTime;
		ParticleFloatRange m_scale;
		vec2_t m_worldSize;
	}; assert_sizeof(ParticleModuleInitOcclusionQuery, 32);

	struct ParticleModuleInitOrientedSprite : ParticleModule
	{
		unsigned int m_pad[2];
		float4 m_orientationQuat;
	}; assert_sizeof(ParticleModuleInitOrientedSprite, 32);

	struct ParticleModuleInitParticleSim : ParticleModule
	{
		ParticleLinkedAssetListDef m_linkedAssetList;
		ParticleFloatRange m_scaleFactor;
		bool m_holdLastFrame;
		unsigned int m_pad[3];
	}; assert_sizeof(ParticleModuleInitParticleSim, 48);

	struct ParticleModuleInitPlayerFacing : ParticleModule
	{
		unsigned int m_pad[2];
	}; assert_sizeof(ParticleModuleInitPlayerFacing, 16);

	enum ParticleRelativeVelocityType
	{
		PARTICLE_RELATIVE_VELOCITY_TYPE_LOCAL = 0x0,
		PARTICLE_RELATIVE_VELOCITY_TYPE_WORLD = 0x1,
		PARTICLE_RELATIVE_VELOCITY_TYPE_RELATIVE_TO_EFFECT_ORIGIN = 0x2,
		PARTICLE_RELATIVE_VELOCITY_TYPE_COUNT = 0x3,
		PARTICLE_RELATIVE_VELOCITY_TYPE_LOCAL_WITH_BOLT_INFO = 0x3,
		PARTICLE_RELATIVE_VELOCITY_TYPE_WORLD_WITH_BOLT_INFO = 0x4,
		PARTICLE_RELATIVE_VELOCITY_TYPE_RELATIVE_TO_EFFECT_ORIGIN_WITH_BOLT_INFO = 0x5,
	};

	struct ParticleModuleInitRelativeVelocity : ParticleModule
	{
		ParticleRelativeVelocityType m_velocityType;
		bool m_useBoltInfo;
	}; assert_sizeof(ParticleModuleInitRelativeVelocity, 16);

	struct ParticleModuleInitRotation : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleFloatRange m_rotationAngle;
		ParticleFloatRange m_rotationRate;
	}; assert_sizeof(ParticleModuleInitRotation, 32);

	struct ParticleModuleInitRotation3D : ParticleModule
	{
		unsigned int m_pad[2];
		float4 m_rotationAngleMin;
		float4 m_rotationAngleMax;
		float4 m_rotationRateMin;
		float4 m_rotationRateMax;
	}; assert_sizeof(ParticleModuleInitRotation3D, 80);

	struct ParticleModuleInitRunner : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitRunner, 32);

	struct ParticleModuleInitSound : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitSound, 32);

	struct ParticleModuleInitSpawn : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef m_curves[1];
	}; assert_sizeof(ParticleModuleInitSpawn, 32);

	enum ParticleModuleAxesFlags
	{
		PARTICLE_MODULE_AXES_FLAG_POS_X = 0x1,
		PARTICLE_MODULE_AXES_FLAG_NEG_X = 0x2,
		PARTICLE_MODULE_AXES_FLAG_POS_Y = 0x4,
		PARTICLE_MODULE_AXES_FLAG_NEG_Y = 0x8,
		PARTICLE_MODULE_AXES_FLAG_POS_Z = 0x10,
		PARTICLE_MODULE_AXES_FLAG_NEG_Z = 0x20,
		PARTICLE_MODULE_AXES_FLAG_ALL_X = 0x3,
		PARTICLE_MODULE_AXES_FLAG_ALL_Y = 0xC,
		PARTICLE_MODULE_AXES_FLAG_ALL_Z = 0x30,
		PARTICLE_MODULE_AXES_FLAG_ALL_POS = 0x15,
		PARTICLE_MODULE_AXES_FLAG_ALL_NEG = 0x2A,
		PARTICLE_MODULE_AXES_FLAG_ALL = 0x3F,
	};

	enum ParticleSpawnFlags
	{
		PARTICLE_SPAWN_ONLY_ON_SURFACE = 0x1,
		PARTICLE_SPAWN_USE_CURVE_VALUES = 0x2,
		PARTICLE_SPAWN_USE_MODIFIER_SCALE = 0x4,
		PARTICLE_SPAWN_IS_DETERMINISTIC = 0x8,
		PARTICLE_SPAWN_UNIFORM = 0x10,
	};

	struct ParticleModuleInitSpawnShape : ParticleModule
	{
		char m_axisFlags;
		char m_spawnFlags;
		char m_normalAxis;
		char m_spawnType;
		float m_volumeCubeRoot;
		//float4 m_calculationOffset;
		float4 m_offset;
	};

	struct ParticleModuleInitSpawnShapeBox : ParticleModuleInitSpawnShape
	{
		bool m_useBeamInfo;
		char m_pad[15];
		float4 m_dimensionsMin;
		float4 m_dimensionsMax;
	}; assert_sizeof(ParticleModuleInitSpawnShapeBox, 80);

	struct ParticleModuleInitSpawnShapeCylinder : ParticleModuleInitSpawnShape
	{
		bool m_hasRotation;
		bool m_rotateCalculatedOffset;
		float m_halfHeight;
		ParticleFloatRange m_radius;
		float4 unk;
		float4 m_directionQuat;
	}; assert_sizeof(ParticleModuleInitSpawnShapeCylinder, 80);

	struct ParticleModuleInitSpawnShapeEllipsoid : ParticleModuleInitSpawnShape
	{
		float4 m_radiusMin;
		float4 m_radiusMax;
		unsigned int m_pad[4];
	}; assert_sizeof(ParticleModuleInitSpawnShapeEllipsoid, 80);

	struct ParticleSpawnMeshAssetDef
	{
		unsigned int m_numVertsTotal;
		unsigned int m_numTrisTotal;
		unsigned int m_pad[2];
	};

	struct ParticleModuleInitSpawnShapeMesh : ParticleModuleInitSpawnShape
	{
		unsigned int m_numMeshAssets;
		ParticleLinkedAssetListDef m_linkedAssetList;
		ParticleSpawnMeshAssetDef* m_meshAssetData;
		unsigned int m_pad[3];
	}; assert_sizeof(ParticleModuleInitSpawnShapeMesh, 80);
	assert_offsetof(ParticleModuleInitSpawnShapeMesh, m_numMeshAssets, 32);

	struct ParticleModuleInitSpawnShapeSphere : ParticleModuleInitSpawnShape
	{
		unsigned int m_pad[2];
		ParticleFloatRange m_radius;
		float4 m_offset2;
	}; assert_sizeof(ParticleModuleInitSpawnShapeSphere, 64);

	struct ParticleModuleInitTail : ParticleModule
	{
		unsigned short m_averagePastVelocities;
		unsigned short m_maxParentSpeed;
		bool m_tailLeading;
		bool m_scaleWithVelocity;
		bool m_rotateAroundPivot;
	}; assert_sizeof(ParticleModuleInitTail, 16);

	struct ParticleModuleInitVectorField : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModuleInitVectorField, 32);

	struct ParticleModuleInitVolumetric : ParticleModule
	{
		float m_density;
		float m_falloff;
		float m_noiseAmplitude;
		unsigned int m_noiseMode;
		unsigned int m_pad[2];
	}; assert_sizeof(ParticleModuleInitVolumetric, 32);

	struct ParticleModuleAttractor : ParticleModule
	{
		unsigned int m_pad[3];
		bool m_killWhenNear;
		float m_forceMag;
		float m_nearDistanceSq;
		float4 m_attractPoint;
	}; assert_sizeof(ParticleModuleAttractor, 48);

	struct ParticleModuleColorGraph : ParticleModule
	{
		bool firstCurve;
		char m_pad[3];
		bool m_modulateColorByAlpha;
		ParticleCurveDef m_curves[8];
	}; assert_sizeof(ParticleModuleColorGraph, 144);

	struct ParticleModuleColorLerp : ParticleModule
	{
		unsigned int m_pad[2];
		float4 m_colorBegin;
		float4 m_colorEnd;
	}; assert_sizeof(ParticleModuleColorLerp, 48);

	struct ParticleModuleEmissionGraph : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef m_curves[2];
	}; assert_sizeof(ParticleModuleEmissionGraph, 48);

	struct ParticleModuleEmissiveGraph : ParticleModule
	{
		bool firstCurve;
		char m_pad[7];
		ParticleCurveDef m_curves[2];
	}; assert_sizeof(ParticleModuleEmissiveGraph, 48);
	assert_offsetof(ParticleModuleEmissiveGraph, m_curves, 16);

	struct ParticleModuleForce : ParticleModule
	{
		unsigned int m_pad[2];
		float4 m_forceMin;
		float4 m_forceMax;
	}; assert_sizeof(ParticleModuleForce, 48);

	struct ParticleModuleGravity : ParticleModule
	{
		ParticleFloatRange m_gravityPercentage;
	}; assert_sizeof(ParticleModuleGravity, 16);

	struct ParticleModuleIntensityGraph : ParticleModule
	{
		bool firstCurve;
		char m_pad[7];
		ParticleCurveDef m_curves[2];
	}; assert_sizeof(ParticleModuleIntensityGraph, 48);

	struct ParticleModuleParentVelocityGraph : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef m_curves[2];
	}; assert_sizeof(ParticleModuleParentVelocityGraph, 48);

	struct ParticleModulePhysicsLight : ParticleModule
	{
		bool m_ignoreEmitterOrientation;
		bool m_useSurfaceType;
		char m_pad[6];
		ParticleLinkedAssetListDef m_linkedAssetList;
	}; assert_sizeof(ParticleModulePhysicsLight, 32);

	struct ParticleModulePhysicsRayCast : ParticleModule
	{
		ParticleFloatRange m_bounce;
		Bounds m_bounds;
		bool m_useItemClip;
		bool m_useSurfaceType;
		bool m_collideWithWater;
		bool m_ignoreContentItem;
		char m_pad[3];
	}; assert_sizeof(ParticleModulePhysicsRayCast, 48);

	struct ParticleModulePositionGraph : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef m_curves[6];
	}; assert_sizeof(ParticleModulePositionGraph, 112);

	struct ParticleModuleRotationGraph : ParticleModule
	{
		bool m_useRotationRate;
		unsigned int m_pad[1];
		ParticleCurveDef m_curves[2];
	}; assert_sizeof(ParticleModuleRotationGraph, 48);

	struct ParticleModuleRotationGraph3D : ParticleModule
	{
		bool m_useRotationRate;
		unsigned int m_pad[1];
		ParticleCurveDef m_curves[6];
	}; assert_sizeof(ParticleModuleRotationGraph3D, 112);

	struct ParticleModuleSizeGraph : ParticleModule
	{
		bool firstCurve;
		char m_pad[7];
		ParticleCurveDef m_curves[6];
		float4 m_sizeBegin;
		float4 m_sizeEnd;
	}; assert_sizeof(ParticleModuleSizeGraph, 144);

	struct ParticleModuleSizeLerp : ParticleModule
	{
		unsigned int m_pad[2];
		float4 m_sizeBegin;
		float4 m_sizeEnd;
	}; assert_sizeof(ParticleModuleSizeLerp, 48);

	struct ParticleModuleVelocityGraph : ParticleModule
	{
		unsigned int m_pad[2];
		ParticleCurveDef m_curves[6];
		float4 m_velocityBegin;
		float4 m_velocityEnd;
	}; assert_sizeof(ParticleModuleVelocityGraph, 144);

	struct ParticleModuleTestEventHandlerData
	{
		unsigned int m_nextState;
		ParticleLinkedAssetListDef m_linkedAssetList;
		bool m_kill;
		unsigned int m_pad[1];
	};

	enum ParticleUseOrientationOptions
	{
		PARTICLE_USE_ORIENTATION_OPTION_IDENTITY = 0x0,
		PARTICLE_USE_ORIENTATION_OPTION_ROTATION = 0x1,
		PARTICLE_USE_ORIENTATION_OPTION_RELATIVE_ROTATION = 0x2,
		PARTICLE_USE_ORIENTATION_OPTION_VELOCITY_Y = 0x3,
		PARTICLE_USE_ORIENTATION_OPTION_VELOCITY_X = 0x4,
		PARTICLE_USE_ORIENTATION_OPTION_VELOCITY_Z = 0x5,
		PARTICLE_USE_ORIENTATION_OPTION_RELATIVE = 0x6,
		PARTICLE_USE_ORIENTATION_OPTION_COUNT = 0x7,
	};

	enum ParticleUseScaleOptions
	{
		PARTICLE_USE_SCALE_OPTION_NONE = 0x0,
		PARTICLE_USE_SCALE_OPTION_MULT = 0x1,
		PARTICLE_USE_SCALE_OPTION_OVERRIDE = 0x2,
		PARTICLE_USE_SCALE_OPTION_COUNT = 0x3,
	};

	enum ParticleUseVelocityOptions
	{
		PARTICLE_USE_VELOCITY_OPTION_NONE = 0x0,
		PARTICLE_USE_VELOCITY_OPTION_MULT = 0x1,
		PARTICLE_USE_VELOCITY_OPTION_OVERRIDE = 0x2,
		PARTICLE_USE_VELOCITY_OPTION_COUNT = 0x3,
	};

	struct ParticleModuleTest : ParticleModule
	{
		unsigned short m_moduleIndex;
		char m_useOrientationOptions[1];
		char m_useScaleOptions[1];
		char m_useVelocityOptions[1];
		ParticleModuleTestEventHandlerData m_eventHandlerData;
	}; assert_sizeof(ParticleModuleTest, 48);
	assert_offsetof(ParticleModuleTest, m_eventHandlerData, 16);

	struct ParticleModuleTestAge : ParticleModuleTest
	{
		float m_age;
	};

	struct ParticleModuleTestBirth : ParticleModuleTest
	{
	};

	struct ParticleModuleTestDeath : ParticleModuleTest
	{
	};

	struct ParticleModuleTestGravity : ParticleModuleTest
	{
	};

	struct ParticleModuleTestImpact : ParticleModuleTest
	{
	};

	struct ParticleModuleTestPos : ParticleModuleTest
	{
	};

	union ParticleModuleTypeDef
	{
		ParticleModule moduleBase;
		ParticleModuleInitAtlas initAtlas;
		ParticleModuleInitAttributes initAttributes;
		ParticleModuleInitBeam initBeam;
		ParticleModuleInitCameraOffset initCameraOffset;
		ParticleModuleInitCloud initCloud; // maybe different
		ParticleModuleInitDecal initDecal; // maybe wrong fields
		ParticleModuleInitFlare initFlare;
		ParticleModuleInitGeoTrail initGeoTrail;
		ParticleModuleInitLightOmni initLightOmni; // maybe wrong fields
		ParticleModuleInitLightSpot initLightSpot; // maybe wrong fields
		ParticleModuleInitMaterial initMaterial;
		ParticleModuleInitMirrorTexture initMirrorTexture;
		ParticleModuleInitModel initModel;
		ParticleModuleInitOcclusionQuery initOcclusionQuery; // maybe wrong fields
		ParticleModuleInitOrientedSprite initOrientedSprite;
		ParticleModuleInitParticleSim initParticleSim;
		ParticleModuleInitPlayerFacing initPlayerFacing;
		ParticleModuleInitRelativeVelocity initRelativeVelocity;
		ParticleModuleInitRotation initRotation;
		ParticleModuleInitRotation3D initRotation3D;
		ParticleModuleInitRunner initRunner; // maybe wrong
		ParticleModuleInitSound initSound;
		ParticleModuleInitSpawn initSpawn;
		ParticleModuleInitSpawnShapeBox initSpawnShapeBox; // maybe wrong fields
		ParticleModuleInitSpawnShapeCylinder initSpawnShapeCylinder; // maybe wrong fields
		ParticleModuleInitSpawnShapeEllipsoid initSpawnShapeEllipsoid; // maybe wrong
		ParticleModuleInitSpawnShapeMesh initSpawnShapeMesh;  // maybe wrong
		ParticleModuleInitSpawnShapeSphere initSpawnShapeSphere;  // maybe wrong
		ParticleModuleInitTail initTail;
		ParticleModuleInitVectorField initVectorField; // maybe wrong
		ParticleModuleInitVolumetric initVolumetric;
		ParticleModuleAttractor attractor;
		ParticleModuleColorGraph colorGraph;
		ParticleModuleColorLerp colorLerp;
		ParticleModuleEmissionGraph emissionGraph;
		ParticleModuleEmissiveGraph emissiveGraph;
		ParticleModuleForce force;
		ParticleModuleGravity gravity;
		ParticleModuleIntensityGraph intensityGraph;
		ParticleModuleParentVelocityGraph parentVelocityGraph;
		ParticleModulePhysicsLight physicsLight;
		ParticleModulePhysicsRayCast physicsRayCast;
		ParticleModulePositionGraph positionGraph;
		ParticleModuleRotationGraph rotationGraph;
		ParticleModuleRotationGraph3D rotationGraph3D;
		ParticleModuleSizeGraph sizeGraph;
		ParticleModuleSizeLerp sizeLerp;
		ParticleModuleVelocityGraph velocityGraph;
		ParticleModuleTestAge testAge;
		ParticleModuleTestBirth testBirth;
		ParticleModuleTestDeath testDeath;
		ParticleModuleTestGravity testGravity;
		ParticleModuleTestImpact testImpact;
		ParticleModuleTestPos testPos;
	};

	struct ParticleModuleDef
	{
		ParticleModuleType moduleType;
		unsigned int m_pad[3];
		ParticleModuleTypeDef moduleData;
	}; assert_sizeof(ParticleModuleDef, 160);

	enum ParticleGravityOptions
	{
		PARTICLE_GRAVITY_OPTION_NONE = 0x0,
		PARTICLE_GRAVITY_OPTION_GRAVITY_ONLY = 0x1,
		PARTICLE_GRAVITY_OPTION_GRAVITY_NEVER = 0x2,
		PARTICLE_GRAVITY_OPTION_COUNT = 0x3,
	};

	enum ParticleModuleGroup
	{
		PARTICLE_MODULE_GROUP_INIT = 0x0,
		PARTICLE_MODULE_GROUP_UPDATE = 0x1,
		PARTICLE_MODULE_GROUP_TEST = 0x2,
		PARTICLE_MODULE_GROUP_COUNT = 0x3,
	};

	struct ParticleModuleGroupDef
	{
		ParticleModuleDef* moduleDefs;
		int numModules;
		bool disabled;
	};

	struct ParticleStateDef
	{
		ParticleModuleGroupDef* moduleGroupDefs; // size: 3
		unsigned int elementType;
		unsigned __int64 flags;
		unsigned int pad[2];
	};

	struct ParticleEmitterDef
	{
		ParticleStateDef* stateDefs;
		int numStates;
		ParticleFloatRange particleSpawnRate;
		ParticleFloatRange particleLife;
		ParticleFloatRange particleDelay;
		unsigned int particleCountMax;
		ParticleIntRange particleBurstCount;
		ParticleFloatRange emitterLife;
		ParticleFloatRange emitterDelay;
		int randomSeed;
		ParticleFloatRange spawnRangeSq;
		float fadeOutMaxDistance;
		ParticleCurveDef fadeCurveDef;
		float spawnFrustumCullRadius;
		unsigned int flags;
		ParticleGravityOptions gravityOptions;
		int groupIDs[4];
		float unk1;
		float unk2;
		ParticleFloatRange particleSpawnShapeRange;
		unsigned int pad[1];
	}; assert_sizeof(ParticleEmitterDef, 144);

	enum ParticleScriptedInputNodeType
	{
		PARTICLE_SCRIPTED_INPUT_NODE_INVALID = 0xFFFFFFFF,
		PARTICLE_SCRIPTED_INPUT_NODE_KVP = 0x0,
		PARTICLE_SCRIPTED_INPUT_NODE_EMITTER_DISABLE = 0x1,
		PARTICLE_SCRIPTED_INPUT_NODE_COUNT = 0x2,
	};

	struct ParticleScriptedInputNode
	{
		ParticleScriptedInputNodeType m_type;
		unsigned int m_flags;
		unsigned int m_emitterIndex;
		unsigned int m_pad[5];
	};

	enum ParticleScriptedInputKey
	{
	};

	struct ParticleScriptedInputRecord
	{
		ParticleScriptedInputKey key;
		bool dirty;
		bool value;
		unsigned int pad[2];
	};

	struct ParticleScriptedInputNodeKVP : ParticleScriptedInputNode
	{
		ParticleScriptedInputRecord m_inputRecord;
	};

	struct ParticleScriptedInputNodeEmitterDisable : ParticleScriptedInputNode
	{
	};

	union ParticleScriptedInputNodeDef // maybe wrong
	{
		ParticleScriptedInputNode nodeBase;
		ParticleScriptedInputNodeKVP KVP;
		ParticleScriptedInputNodeEmitterDisable emitterDisable;
	}; assert_sizeof(ParticleScriptedInputNodeDef, 48);

	enum ParticlePhaseOptions
	{
		PARTICLE_PHASE_OPTION_PHASE_NEVER = 0x0,
		PARTICLE_PHASE_OPTION_PHASE_ONLY = 0x1,
		PARTICLE_PHASE_OPTION_ALWAYS = 0x2,
		PARTICLE_PHASE_OPTION_COUNT = 0x3,
	};

	struct ParticleSystemDef
	{
		const char* name;
		ParticleEmitterDef* emitterDefs;
		ParticleScriptedInputNodeDef* scriptedInputNodeDefs;
		int version;
		int numEmitters;
		int numScriptedInputNodes;
		unsigned int flags;
		int occlusionOverrideEmitterIndex;
		ParticlePhaseOptions phaseOptions;
		float drawFrustumCullRadius;
		float updateFrustumCullRadius;
		float sunDistance;
		int preRollMSec;
		float4 editorPosition;
		float4 editorRotation;
		float4 gameTweakPosition;
		float4 gameTweakRotation;
	}; assert_sizeof(ParticleSystemDef, 0x80);

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	enum FXElemAtlasBehavior : std::uint8_t
	{
		FX_ATLAS_START_FIXED = 0x0,
		FX_ATLAS_START_RANDOM = 0x1,
		FX_ATLAS_START_INDEXED = 0x2,
		FX_ATLAS_START_MASK = 0x3,
		FX_ATLAS_PLAY_OVER_LIFE = 0x4,
		FX_ATLAS_LOOP_ONLY_N_TIMES = 0x8,
	};

	struct FxElemAtlas
	{
		unsigned char behavior;
		unsigned char index;
		unsigned char fps;
		unsigned char unk;
		unsigned char loopCount;
		unsigned char colIndexBits;
		unsigned char rowIndexBits;
		short entryCount;
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		float color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[3];
	};

	union FxEffectDefRef
	{
		FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
		const char* vectorFieldName;
		GfxLightDef* lightDef;
		FxParticleSimAnimation* particleSimAnimation;
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		vec2_t pos;
		vec2_t normal;
		vec2_t texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float invSplitDist;
		float invSplitArcDist;
		float invSplitTime;
		int vertCount;
		FxTrailVertex* verts;
		int indCount;
		unsigned short* inds;
	};

	struct FxSparkFountainDef
	{
		float gravity;
		float bounceFrac;
		float bounceRand;
		float sparkSpacing;
		float sparkLength;
		int sparkCount;
		float loopTime;
		float velMin;
		float velMax;
		float velConeFrac;
		float restSpeed;
		float boostTime;
		float boostFactor;
	};

	struct FxSpotLightDef
	{
		float fovInnerFraction;
		float startRadius;
		float endRadius;
		float brightness;
		float maxLength;
		int exponent;
	};

	struct FxFlareDef
	{
		float position;
		int angularRotCount;
		int flags;
		FxFloatRange depthScaleRange;
		FxFloatRange depthScaleValue;
		FxFloatRange radialRot;
		FxFloatRange radialScaleX;
		FxFloatRange radialScaleY;
		vec3_t dir;
		int intensityXIntervalCount;
		int intensityYIntervalCount;
		int srcCosIntensityIntervalCount;
		int srcCosScaleIntervalCount;
		float* intensityX;
		float* intensityY;
		float* srcCosIntensity;
		float* srcCosScale;
	};

	union FxElemExtendedDefPtr
	{
		FxTrailDef* trailDef;
		FxSparkFountainDef* sparkFountainDef;
		FxSpotLightDef* spotLightDef;
		FxFlareDef* flareDef;
		char* unknownDef;
	};

	enum FxElemType : std::uint8_t
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 1,
		FX_ELEM_TYPE_TAIL = 2,
		FX_ELEM_TYPE_TRAIL = 3,
		FX_ELEM_TYPE_FLARE = 4,
		FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION = 5,
		FX_ELEM_TYPE_CLOUD = 6,
		FX_ELEM_TYPE_SPARK_CLOUD = 7,
		FX_ELEM_TYPE_SPARK_FOUNTAIN = 8,
		FX_ELEM_TYPE_MODEL = 9,
		FX_ELEM_TYPE_OMNI_LIGHT = 10,
		FX_ELEM_TYPE_SPOT_LIGHT = 11,
		FX_ELEM_TYPE_SOUND = 12,
		FX_ELEM_TYPE_DECAL = 13,
		FX_ELEM_TYPE_RUNNER = 14,
		FX_ELEM_TYPE_VECTORFIELD = 15,
		FX_ELEM_TYPE_COUNT = 16,
		FX_ELEM_TYPE_LAST_SPRITE = FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION,
		FX_ELEM_TYPE_LAST_DRAWN = FX_ELEM_TYPE_SPOT_LIGHT,
	};

	enum FxElemDefFlags : std::uint32_t
	{
		FX_ELEM_SPAWN_RELATIVE_TO_EFFECT = 0x2,
		FX_ELEM_SPAWN_FRUSTUM_CULL = 0x4,
		FX_ELEM_RUNNER_USES_RAND_ROT = 0x8,
		FX_ELEM_SPAWN_OFFSET_NONE = 0x0,
		FX_ELEM_SPAWN_OFFSET_SPHERE = 0x10,
		FX_ELEM_SPAWN_OFFSET_CYLINDER = 0x20,
		FX_ELEM_SPAWN_OFFSET_MASK = 0x30,
		FX_ELEM_RUN_RELATIVE_TO_WORLD = 0x0,
		FX_ELEM_RUN_RELATIVE_TO_SPAWN = 0x40,
		FX_ELEM_RUN_RELATIVE_TO_EFFECT = 0x80,
		FX_ELEM_RUN_RELATIVE_TO_OFFSET = 0xC0,
		FX_ELEM_RUN_RELATIVE_TO_CAMERA = 0x100,
		FX_ELEM_RUN_MASK = 0x1C0,
		FX_ELEM_DIE_ON_TOUCH = 0x200,
		FX_ELEM_DRAW_PAST_FOG = 0x400,
		FX_ELEM_DRAW_WITH_VIEWMODEL = 0x800,
		FX_ELEM_BLOCK_SIGHT = 0x1000,
		FX_ELEM_DRAW_IN_THERMAL_VIEW_ONLY = 0x2000,
		FX_ELEM_TRAIL_ORIENT_BY_VELOCITY = 0x4000,
		FX_ELEM_EMIT_BOLT = 0x80000000,
		FX_ELEM_EMIT_ORIENT_BY_ELEM = 0x8000,
		FX_ELEM_USE_OCCLUSION_QUERY = 0x10000,
		FX_ELEM_NODRAW_IN_THERMAL_VIEW = 0x20000,
		FX_ELEM_THERMAL_MASK = 0x22000,
		FX_ELEM_SPAWN_IMPACT_FX_WITH_SURFACE_NAME = 0x40000,
		FX_ELEM_DEPRECATED = 0x80000,
		FX_ELEM_VOLUMETRIC_TRAIL = 0x100000,
		FX_ELEM_USE_COLLISION = 0x200000,
		FX_ELEM_USE_VECTORFIELDS = 0x400000,
		FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL = 0x1000000,
		FX_ELEM_HAS_VELOCITY_GRAPH_WORLD = 0x2000000,
		FX_ELEM_HAS_GRAVITY = 0x4000000,
		FX_ELEM_USE_MODEL_PHYSICS = 0x8000000,
		FX_ELEM_NONUNIFORM_SCALE = 0x10000000,
		FX_ELEM_FOUNTAIN_DISABLE_COLLISION = 0x80000000,
	};

	enum FxElemDefExtraFlags : std::uint32_t
	{
		FX_ELEM2_BILLBOARD_FACING_CAMERA_PERPENDICULAR = 0x1,
		FX_ELEM2_BILLBOARD_FACING_PLAYER = 0x2,
		FX_ELEM2_BILLBOARD_FACING_MASK = 0x3,
		FX_ELEM2_EMIT_TRAILS = 0x4,
		FX_ELEM2_REACTIVE_TURBULANCE = 0x8,
		FX_ELEM2_GPU_LIGHTING = 0x10,
		FX_ELEM2_GPU_LIGHTING_SH = 0x20,
		FX_ELEM2_COLOR_OR_ALPHA_CURVE = 0x40,
	};

	struct FxElemDef
	{
		int flags;
		int flags2;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		FxElemType elemType;
		unsigned char visualCount;
		unsigned char velIntervalCount;
		unsigned char visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		Bounds collBounds;
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxElemExtendedDefPtr extended;
		unsigned char sortOrder;
		unsigned char lightingFrac;
		unsigned char useItemClip;
		unsigned char fadeInfo;
		int randomSeed;
		float litUnlitBlendFactor; // FX_EvaluateVisualState_DoLighting
		float pad;
	}; assert_sizeof(FxElemDef, 304);
	assert_offsetof(FxElemDef, elemType, 182);
	assert_offsetof(FxElemDef, visualCount, 183);
	assert_offsetof(FxElemDef, visuals, 208);
	assert_offsetof(FxElemDef, extended, 280);

	enum FxEffectDefFlags : std::uint32_t
	{
		FX_EFFECT_NEEDS_LIGHTING_AT_SPAWN = 0x1,
		FX_EFFECT_NEEDS_LIGHTING_PER_FRAME_AT_ORIGIN = 0x2,
		FX_EFFECT_RECEIVES_DYNAMIC_LIGHTING_PER_FRAME_AT_ORIGIN = 0x4,
		FX_EFFECT_USE_VECTORFIELDS = 0x8,
		FX_EFFECT_UNK = 0x10,
		FX_EFFECT_NEEDS_LIGHTING_AT_SPAWN_2 = 0x20,
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		float elemMaxRadius;
		float occlusionQueryDepthBias;
		int occlusionQueryFadeIn;
		int occlusionQueryFadeOut;
		FxFloatRange occlusionQueryScaleRange;
		FxElemDef* elemDefs;
	}; assert_sizeof(FxEffectDef, 0x40);

	struct FxImpactEntry
	{
		FxCombinedDef nonflesh[64];
		FxCombinedDef flesh[23];
	};

	struct FxImpactTable
	{
		const char* name;
		FxImpactEntry* table; // size: 49
	};

	struct SurfaceFxEntry
	{
		FxCombinedDef surfaceEffect[64];
	};

	struct SurfaceFxTable
	{
		const char* name;
		SurfaceFxEntry* table; // size: 6
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

	enum LbColType
	{
		LBCOL_TYPE_NUMBER = 0x0,
		LBCOL_TYPE_TIME = 0x1,
		LBCOL_TYPE_LEVELXP = 0x2,
		LBCOL_TYPE_PRESTIGE = 0x3,
		LBCOL_TYPE_BIGNUMBER = 0x4,
		LBCOL_TYPE_PERCENT = 0x5,
		LBCOL_TYPE_TIME_FULL = 0x6,
		LBCOL_TYPE_COUNT = 0x7,
	};

	enum LbAggType
	{
		LBAGG_TYPE_MIN = 0x0,
		LBAGG_TYPE_MAX = 0x1,
		LBAGG_TYPE_SUM = 0x2,
		LBAGG_TYPE_LAST = 0x3,
		LBAGG_TYPE_COUNT = 0x4,
	};

	struct LbColumnDef
	{
		const char* name;
		int id;
		int propertyId;
		bool hidden;
		int statsGroup;
		const char* statName;
		LbColType type;
		int precision;
		LbAggType agg;
		int uiCalColX;
		int uiCalColY;
	};

	enum LbUpdateType
	{
		LBUPDATE_TYPE_NORMAL = 0x0,
		LBUPDATE_TYPE_RANK = 0x1,
		LBUPDATE_TYPE_COMBINE = 0x2,
		LBUPDATE_TYPE_VIRTUAL = 0x3,
		LBUPDATE_TYPE_COUNT = 0x4,
	};

	struct LeaderboardDef
	{
		const char* name;
		int id;
		int sourceLbId;
		int sourceLbWidth;
		int columnCount;
		int xpColId;
		int prestigeColId;
		LbColumnDef* columns;
		LbUpdateType updateType;
		int trackTypes;
		int rankColIdX;
		int rankColIdY;
	};

	struct LbVrColumnDef
	{
		const char* name;
		int id;
		int uiCalColX;
		int uiCalColY;
	};

	struct VirtualLeaderboardDef
	{
		const char* name;
		const char* sourceName;
		int id;
		int sourceId;
		LbVrColumnDef* columns;
		int columnCount;
		int rankColIdX;
		int rankColIdY;
	};

	struct StructuredDataEnumEntry
	{
		scr_string_t string;
		unsigned short index;
	};

	struct StructuredDataEnum
	{
		int entryCount;
		int reservedEntryCount;
		StructuredDataEnumEntry* entries;
	};

	enum StructuredDataTypeCategory
	{
		DATA_INT = 0x0,
		DATA_BYTE = 0x1,
		DATA_BOOL = 0x2,
		DATA_STRING = 0x3,
		DATA_ENUM = 0x4,
		DATA_STRUCT = 0x5,
		DATA_INDEXED_ARRAY = 0x6,
		DATA_ENUM_ARRAY = 0x7,
		DATA_FLOAT = 0x8,
		DATA_SHORT = 0x9,
		DATA_COUNT = 0xA,
	};

	union StructuredDataTypeUnion
	{
		unsigned int stringDataLength;
		int enumIndex;
		int structIndex;
		int indexedArrayIndex;
		int enumedArrayIndex;
		int index;
	};

	struct StructuredDataType
	{
		StructuredDataTypeCategory type;
		StructuredDataTypeUnion u;
	};

	enum StructuredDataValidationType
	{
		VALIDATION_NONE = 0x0,
	};

	struct StructuredDataStructProperty
	{
		scr_string_t name;
		StructuredDataType type;
		unsigned int offset;
		StructuredDataValidationType validation;
	};

	struct StructuredDataStruct
	{
		int propertyCount;
		StructuredDataStructProperty* properties;
		int size;
		unsigned int bitOffset;
	};

	struct StructuredDataIndexedArray
	{
		int arraySize;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataEnumedArray
	{
		int enumIndex;
		StructuredDataType elementType;
		unsigned int elementSize;
	};

	struct StructuredDataDef
	{
		int version;
		unsigned int formatChecksum;
		int enumCount;
		StructuredDataEnum* enums;
		int structCount;
		StructuredDataStruct* structs;
		int indexedArrayCount;
		StructuredDataIndexedArray* indexedArrays;
		int enumedArrayCount;
		StructuredDataEnumedArray* enumedArrays;
		StructuredDataType rootType;
		unsigned int size;
	};

	struct StructuredDataDefSet
	{
		const char* name;
		unsigned int defCount;
		StructuredDataDef* defs;
	};

	enum DDLFLags : std::uint8_t
	{
		DDL_FLAG_DIRTY = 0x0,
		DDL_FLAG_CHECKSUM = 0x1,
		DDL_FLAG_CODE_VERSION = 0x2,
		DDL_FLAG_USER_FLAGS = 0x4,
		DDL_FLAG_NO_PADDING = 0x8,
		DDL_FLAG_RESERVE = 0x10,
		DDL_FLAG_DDL_CHECKSUM = 0x20,
	};

	enum DDLType : std::int32_t
	{
		DDL_BYTE_TYPE = 0x0,
		DDL_SHORT_TYPE = 0x1,
		DDL_BOOL_TYPE = 0x2,
		DDL_INT_TYPE = 0x3,
		DDL_UINT64_TYPE = 0x4,
		DDL_FLOAT_TYPE = 0x5,
		DDL_FIXEDPOINT_TYPE = 0x6,
		DDL_STRING_TYPE = 0x7,
		DDL_STRUCT_TYPE = 0x8,
		DDL_ENUM_TYPE = 0x9,
		DDL_PAD_TYPE = 0xA,
	};

	struct DDLMember
	{
		const char* name;
		int index;
		void* parent;
		int bitSize;
		int limitSize;
		int offset;
		int type;
		int externalIndex;
		unsigned int rangeLimit;
		unsigned int serverDelta;
		unsigned int clientDelta;
		int arraySize;
		int enumIndex;
		int permission;
	};

	struct DDLHash
	{
		unsigned int hash;
		int index;
	};

	struct DDLHashTable
	{
		DDLHash* list;
		int count;
		int max;
	};

	struct DDLStruct
	{
		const char* name;
		int bitSize;
		int memberCount;
		DDLMember* members;
		DDLHashTable hashTableUpper;
		DDLHashTable hashTableLower;
	};

	struct DDLEnum
	{
		const char* name;
		int memberCount;
		const char** members;
		DDLHashTable hashTable;
	};

	struct DDLDef
	{
		char* name;
		unsigned short version;
		unsigned int checksum;
		unsigned char flags;
		int bitSize;
		int byteSize;
		DDLStruct* structList;
		int structCount;
		DDLEnum* enumList;
		int enumCount;
		DDLDef* next;
		int headerBitSize;
		int headerByteSize;
		int reserveSize;
		int userFlagsSize;
		bool paddingUsed;
	};

	struct DDLFile
	{
		char* name;
		DDLDef* ddlDef;
	};

	struct TracerDef
	{
		const char* name;
		Material* material;
		unsigned int drawInterval;
		bool fadeOverTime;
		float fadeTime;
		float speed;
		float beamLength;
		float beamWidth;
		float screwRadius;
		float screwDist;
		vec4_t colors[5];
	}; assert_sizeof(TracerDef, 0x80);

	struct SpaceshipAnimParts
	{
		XAnimParts* anim[2];
	};

	enum VehCamZOffsetMode
	{
		VEHCAM_ZMODE_WORLD = 0x0,
		VEHCAM_ZMODE_VEHICLE = 0x1,
		VEHCAM_ZMODE_VIEW = 0x2,
		VEHCAM_ZMODE_COUNT = 0x3,
	};

	enum VehicleTurretFireType
	{
		VEH_TURRET_SINGLE_FIRE = 0x0,
		VEH_TURRET_DUAL_FIRE = 0x1,
		VEH_TURRET_ALT_FIRE = 0x2,
		VEH_TURRET_FIRE_TYPE_COUNT = 0x3,
	};

	struct VehiclePhysicsDef
	{
		int physicsEnabled;
		float unk_1;
		float unk_2;
		int unk_3;
		int unk_4;
		int unk_5;
		float unk_6;
		float unk_7;
		float unk_8;
		float unk_9;
		float unk_10;
		float unk_11;
		float unk_12;
		float unk_13;
		float unk_14;
		float unk_15;
		float unk_16;
		float unk_17;
		float unk_18;
		float unk_19;
		float unk_20;
		float unk_21;
		float unk_22;
		float unk_23;
		float unk_24;
		float unk_25;
		float unk_26;
		float unk_27;
		float unk_28;
		float unk_29;
		float unk_30;
		float unk_31;
		float unk_32;
		float unk_33;
		float unk_34;
		float unk_35;
		float unk_36;
		float unk_37;
		float unk_38;
		float unk_39;
		float unk_40;
		float unk_41;
		float unk_42;
		float unk_43;
		float unk_44;
		float unk_45;
		float unk_46;
		float unk_47;
		float unk_48;
		float unk_49;
		int unk_50;
		float unk_51;
		float unk_52;
		float unk_53;
		float unk_54;
		float unk_55;
		float unk_56;
		float unk_57;
		float unk_58;
		float unk_59;
		float unk_60;
		float unk_61;
		float unk_62;
		float unk_63;
		float unk_64;
		float unk_65;
		float unk_66;
		float unk_67;
		float unk_68;
		float unk_69;
		float unk_70;
		float unk_71;
		float unk_72;
		float unk_73;
		float unk_74;
		float unk_75;
		float unk_76;
		float unk_77;
		float unk_78;
		float unk_79;
		float unk_80;
		float unk_81;
		float unk_82;
		float unk_83;
		float unk_84;
		float unk_85;
		float unk_86;
		float unk_87;
		float unk_88;
		float unk_89;
		float unk_90;
		float unk_91;
		float unk_92;
		float unk_93;
		float unk_94;
		float unk_95;
		float unk_96;
		float unk_97;
		float unk_98;
		float unk_99;
		float unk_100;
		float unk_101;
		float unk_102;
		float unk_103;
		float unk_104;
		float unk_105;
		float unk_106;
		float unk_107;
		float unk_108;
		float unk_109;
		float unk_110;
		float unk_111;
	}; assert_sizeof(VehiclePhysicsDef, 448);

	struct VehicleDef
	{
		const char* name;
		char type;
		const char* useHintString;
		int health;
		int hitClientScriptables;
		float texScrollScale;
		float topSpeed;
		float accel;
		float rotRate;
		float rotAccel;
		float maxBodyPitch;
		float maxBodyRoll;
		float fakeBodyAccelPitch;
		float fakeBodyAccelRoll;
		float fakeBodyVelPitch;
		float fakeBodyVelRoll;
		float fakeBodySideVelPitch;
		float fakeBodyPitchStrength;
		float fakeBodyRollStrength;
		float fakeBodyPitchDampening;
		float fakeBodyRollDampening;
		float fakeBodyBoatRockingAmplitude;
		float fakeBodyBoatRockingPeriod;
		float fakeBodyBoatRockingRotationPeriod;
		float fakeBodyBoatRockingFadeoutSpeed;
		float boatBouncingMinForce;
		float boatBouncingMaxForce;
		float boatBouncingRate;
		float boatBouncingFadeinSpeed;
		float boatBouncingFadeoutSteeringAngle;
		float collisionDamage;
		float collisionSpeed;
		vec3_t killcamOffset;
		int playerProtected;
		int bulletDamage;
		int armorPiercingDamage;
		int grenadeDamage;
		int projectileDamage;
		int projectileSplashDamage;
		int heavyExplosiveDamage;
		VehiclePhysicsDef vehPhysDef;
		int vehHelicopterLockAltitude;
		int vehHelicopterOffsetFromMesh;
		float vehHelicopterAltitudeOffset;
		float vehHelicopterPitchOffset;
		float vehHelicopterBoundsRadius;
		float vehHelicopterMaxSpeed;
		float vehHelicopterMaxSpeedVertical;
		float vehHelicopterMaxAccel;
		float vehHelicopterMaxAccelVertical;
		float vehHelicopterDecelerationFwd;
		float vehHelicopterDecelerationSide;
		float vehHelicopterDecelerationUp;
		float vehHelicopterMaxYawRate;
		float vehHelicopterMaxYawAccel;
		float vehHelicopterTiltFromVelocity;
		float vehHelicopterTiltFromControllerAxes;
		float vehHelicopterTiltFromAcceleration;
		float vehHelicopterTiltFromDeceleration;
		float vehHelicopterTiltFromFwdAndYaw_VelAtMaxTilt;
		float vehHelicopterTiltFromFwdAndYaw;
		float vehHelicopterTiltMomentum;
		float vehHelicopterTiltSpeed;
		float vehHelicopterMaxPitch;
		float vehHelicopterMaxRoll;
		float vehHelicopterHoverSpeedThreshold;
		float vehHelicopterJitterJerkyness;
		float vehHelicopterLookaheadTime;
		int vehHelicopterSoftCollisions;
		int vehHelicopterUseGroundFX;
		FxCombinedDef vehHelicopterGroundFx;
		FxCombinedDef vehHelicopterGroundWaterFx;
		float vehHelicopterGroundFxDefaultRepeatRate;
		float vehHelicopterGroundFxSlowestRepeatRate;
		float vehHelicopterGroundFxFastestRepeatRate;
		float vehHelicopterGroundFxMinGroundDist;
		float vehHelicopterGroundFxMaxGroundDist;
		float ssWeaponSwitchDelaySeconds;
		float ssWeaponRaiseDelaySeconds;
		vec3_t ssFlyMaxSpeedMph;
		vec3_t ssFlyMaxAccelerationMphps;
		vec3_t ssFlyMaxCounterAccelerationMphps;
		vec3_t ssFlyFrictionMphps;
		vec3_t ssFlyJukeSpeedMph;
		vec3_t ssHoverMaxSpeedMph;
		vec3_t ssHoverMaxAccelerationMphps;
		vec3_t ssHoverMaxCounterAccelerationMphps;
		vec3_t ssHoverFrictionMphps;
		vec3_t ssHoverJukeSpeedMph;
		float ssFlyJukeRampTime;
		float ssHoverJukeRampTime;
		float ssBrakingFrictionMphps;
		float ssFlyRedirectVelocityScale;
		float ssFlySlowRedirectVelocityScale;
		float ssHoverRedirectVelocityScale;
		float ssFlyMinSpeedRatio;
		bool ssProjectVelocityDesiresToXY;
		bool ssEnforceMinGroundClearance;
		float ssMinGroundClearanceHeight;
		float ssMinGroundClearanceLiftForce;
		bool ssTerrainModeMovement;
		float ssTerrainModeGravity;
		float ssTerrainModeFlyTime;
		vec2_t ssAimOffsetClampDeg;
		vec2_t ssPitchUpSoftLimitDeg;
		vec2_t ssPitchDownSoftLimitDeg;
		float ssClutchPitchSpeedScale;
		vec2_t ssFlyAimSpeedDps;
		vec2_t ssFlySlowAimSpeedDps;
		vec2_t ssHoverAimSpeedDps;
		vec2_t ssFlyAimDriftScale;
		vec2_t ssFlySlowAimDriftScale;
		vec2_t ssHoverAimDriftScale;
		float ssMinSpringOscillation;
		float ssMaxSpringOscillation;
		float ssMinSpringStrength;
		float ssMaxSpringStrength;
		float ssMinSpringAngSpeed;
		float ssMaxSpringAngSpeed;
		float ssSpringControllerContribution;
		float ssTiltSpringOscillation;
		float ssTiltSpringStrength;
		float ssTiltSpringAngleContribution;
		int ssADSToLockOn;
		float ssLockOnMinAngle;
		float ssLockOnMaxAngle;
		float ssPilotAssistNoLockAimScale;
		float ssPilotAssistNoLockMinAngle;
		float ssPilotAssistNoLockMaxAngle;
		float ssPilotAssistAimScale;
		float ssPilotAssistMinAngle;
		float ssPilotAssistMaxAngle;
		vec2_t ssAutoLevelStrengthHover;
		vec2_t ssAutoLevelStrengthFly;
		bool ssIsRollRightStick;
		float ssRollMaxSpeedDps;
		float ssBoostSpeedScale;
		float ssBoostThrustScale;
		bool ssEnableBoostToFly;
		float ssBoostTime;
		float ssBoostRegenTime;
		float ssBoostRegenStallTime;
		float ssBoostMinThresholdRatio;
		float ssCollisionRadiusI;
		float ssCylinderHeightI;
		bool ssUseCylinder;
		bool ssUsePlayerClip;
		float ssCollisionOffsetI;
		float ssCollisionHardThresholdMph;
		float ssFOVFlyOffsetDeg;
		float ssFOVFlyBoostOffsetDeg;
		float ssFOVHoverOffsetDeg;
		float ssZoomBlendSpeedDps;
		float ssZoomMaxAngle;
		float ssZoomMinDistI;
		float ssZoomIdealDistI;
		float ssZoomMaxDistI;
		bool ssShowPilotViewmodel;
		float ssCameraAnimScale;
		vec2_t ssFlyTiltShipAngles;
		float ssFlyTiltCameraScale;
		vec2_t ssHoverTiltShipAngles;
		float ssHoverTiltCameraScale;
		float ssMotionIdleRateMinSpeed;
		float ssMotionIdleRateMaxSpeed;
		float ssMotionIdleMinRate;
		float ssMotionIdleMaxRate;
		scr_string_t ssAnimTree;
		SpaceshipAnimParts ssAnimParts[33];
		FxCombinedDef ssThrustFxLoop;
		FxCombinedDef ssJukeFx;
		RumbleInfo* ssIdleRumble;
		RumbleInfo* ssSmallRumble;
		RumbleInfo* ssMedRumble;
		RumbleInfo* ssLargeRumble;
		SndAliasLookup rattleLoop;
		float rattleLoopMinVolume;
		float rattleLoopMaxVolume;
		SndAliasLookup airLoop;
		float airLoopMinVolume;
		float airLoopMaxVolume;
		SndAliasLookup engineLoop;
		float engineLoopMinVolume;
		float engineLoopMaxVolume;
		float engineLoopMinPitch;
		float engineLoopMaxPitch;
		SndAliasLookup hoverLoop;
		float hoverLoopMinVolume;
		float hoverLoopMaxVolume;
		float hoverLoopMinPitch;
		float hoverLoopMaxPitch;
		SndAliasLookup boostLoop;
		float boostLoopMaxVolume;
		float boostLoopMaxPitch;
		SndAliasLookup ssThrustLoop;
		float ssThrustLoopMaxVolume;
		float ssThrustLoopMaxPitch;
		SndAliasLookup boostStart;
		SndAliasLookup boostStop;
		SndAliasLookup boostDepleted;
		SndAliasLookup boostUnavailable;
		SndAliasLookup jukeLeft;
		SndAliasLookup jukeRight;
		SndAliasLookup jukeUpDown;
		SndAliasLookup jukeBack;
		SndAliasLookup jukeFront;
		SndAliasLookup flightOn;
		SndAliasLookup flightOff;
		float flightOnTimer;
		float flightOffTimer;
		SndAliasLookup hardCollision;
		SndAliasLookup softCollision;
		int camLookEnabled;
		int camRelativeControl;
		int camRemoteDrive;
		float camLerp;
		float camHeight;
		float camRadius;
		float camPitchInfluence;
		float camYawInfluence;
		float camRollInfluence;
		float camFovIncrease;
		float camFovOffset;
		float camFovSpeed;
		float camReturnSpeed;
		float camReturnLerp;
		float camVehicleAnglePitchRate;
		float camVehicleAngleYawRate;
		float camVehicleAngleRollRate;
		int vehCam_UseGDT;
		float vehCam_anglesPitch;
		float vehCam_anglesYaw;
		float vehCam_anglesRoll;
		float vehCam_offsetX;
		float vehCam_offsetY;
		float vehCam_offsetZ;
		float vehCam_radius;
		float vehCam_speedInfluence;
		float vehCam_pitchTurnRate;
		float vehCam_pitchClamp;
		float vehCam_yawTurnRate;
		float vehCam_yawClamp;
		VehCamZOffsetMode vehCam_zOffsetMode;
		float vehCam_anglesPitch3P;
		float vehCam_anglesYaw3P;
		float vehCam_anglesRoll3P;
		float vehCam_offsetX3P;
		float vehCam_offsetY3P;
		float vehCam_offsetZ3P;
		float vehCam_radius3P;
		float vehCam_speedInfluence3P;
		float vehCam_pitchTurnRate3P;
		float vehCam_pitchClamp3P;
		float vehCam_yawTurnRate3P;
		float vehCam_yawClamp3P;
		VehCamZOffsetMode vehCam_zOffsetMode3P;
		const char* turretWeaponName;
		WeaponCompleteDef* turretWeapon;
		float turretHorizSpanLeft;
		float turretHorizSpanRight;
		float turretVertSpanUp;
		float turretVertSpanDown;
		float turretHorizResistLeft;
		float turretHorizResistRight;
		float turretVertResistUp;
		float turretVertResistDown;
		float turretRotRate;
		VehicleTurretFireType turretFireType;
		SndAliasLookup turretSpinSnd;
		SndAliasLookup turretStopSnd;
		int trophyEnabled;
		float trophyRadius;
		float trophyInactiveRadius;
		int trophyAmmoCount;
		float trophyReloadTime;
		scr_string_t trophyTags[4];
		FxCombinedDef trophyExplodeFx;
		FxCombinedDef trophyFlashFx;
		Material* compassFriendlyIcon;
		Material* compassEnemyIcon;
		Material* compassFriendlyAltIcon;
		Material* compassEnemyAltIcon;
		int compassIconWidth;
		int compassIconHeight;
		SndAliasLookup idleLowSnd;
		SndAliasLookup idleHighSnd;
		SndAliasLookup engineLowSnd;
		SndAliasLookup engineHighSnd;
		float engineSndSpeed;
		scr_string_t audioOriginTag;
		SndAliasLookup engineStartUpSnd;
		int engineStartUpLength;
		SndAliasLookup engineShutdownSnd;
		SndAliasLookup engineIdleSnd;
		SndAliasLookup engineSustainSnd;
		SndAliasLookup engineRampUpSnd;
		int engineRampUpLength;
		SndAliasLookup engineRampDownSnd;
		int engineRampDownLength;
		SndAliasLookup suspensionSoftSnd;
		float suspensionSoftCompression;
		SndAliasLookup suspensionHardSnd;
		float suspensionHardCompression;
		SndAliasLookup collisionSnd;
		float collisionBlendSpeed;
		SndAliasLookup speedSnd;
		float speedSndBlendSpeed;
		SndAliasLookup surfaceSndName;
		float surfaceSndBlendSpeed;
		float slideVolume;
		float slideBlendSpeed;
		float inAirPitch;
		const char* soundTriggerOverrideZone;
		bool soundTriggerOverrideReverb;
		bool soundTriggerOverrideMix;
		bool soundTriggerOverrideFilter;
		bool soundTriggerOverrideOcclusion;
		bool soundTriggerOverrideAmbient;
		bool soundTriggerOverrideAmbientEvents;
		bool soundTriggerOverrideADSR;
		int visionBlendTime;
		const char* globalVisionSettings;
		const char* mapVisionSettings;
	}; assert_sizeof(VehicleDef, 0xA90);

	struct AddonMapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		ClipInfo* info;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		GfxBrushModel* models;
	}; assert_sizeof(AddonMapEnts, 0x88);

	enum NetConstStringType
	{
		NETCONSTSTRINGTYPE_XMODEL = 0, // mdl
		NETCONSTSTRINGTYPE_MATERIAL = 1, // mat
		NETCONSTSTRINGTYPE_RUMBLE = 2, // rmb
		NETCONSTSTRINGTYPE_RUMBLE_GRAPH = 3, // rmg
		NETCONSTSTRINGTYPE_VEHICLES = 4, // veh
		NETCONSTSTRINGTYPE_FX = 5, // efx
		NETCONSTSTRINGTYPE_VFX = 6, // vfx
		NETCONSTSTRINGTYPE_LOCSTRING = 7, //loc
		NETCONSTSTRINGTYPE_SHOCK = 8, // shk
		NETCONSTSTRINGTYPE_CLIENT_TAGS = 9, // tag
		NETCONSTSTRINGTYPE_HEADICON = 10, // hic
		NETCONSTSTRINGTYPE_STATUSICON = 11, // sic
		NETCONSTSTRINGTYPE_NAMEPLATE = 12, // nps
		NETCONSTSTRINGTYPE_MINIMAPICON = 13, // mic
		NETCONSTSTRINGTYPE_WEAPON = 14, // wep
		NETCONSTSTRINGTYPE_HINTSTRING = 15, // hnt
		NETCONSTSTRINGTYPE_ANIM = 16, // anm
		NETCONSTSTRINGTYPE_ANIMCLASS = 17, // acl
		NETCONSTSTRINGTYPE_LUI = 18, // lui
		NETCONSTSTRINGTYPE_SUIT = 19, // sut
		NETCONSTSTRINGTYPE_GESTURE = 20, // ges
		NETCONSTSTRINGTYPE_TGT = 21, // tgt
		NETCONSTSTRINGTYPE_VISION = 22, // vsn
		NETCONSTSTRINGTYPE_COUNT = 25,
		NETCONSTSTRINGTYPE_NONE = 25,
	};

	enum NetConstStringSource
	{
		NETCONSTSTRINGSOURCE_MAP = 0x0,
		NETCONSTSTRINGSOURCE_PRE_MAP = 0x1,
		NETCONSTSTRINGSOURCE_COMMON = 0x2,
		NETCONSTSTRINGSOURCE_COUNT = 0x3,
		NETCONSTSTRINGSOURCE_NONE = 0x3,
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

	struct ScriptableEventDef;
	struct ScriptablePartDef;

	struct ScriptableEventBaseDef
	{
		const char* name;
		int flags;
	}; assert_sizeof(ScriptableEventBaseDef, 16);

	enum Scriptable_EventType
	{
		Scriptable_EventType_StateChange = 0x0,
		Scriptable_EventType_Wait = 0x1,
		Scriptable_EventType_Random = 0x2,
		Scriptable_EventType_Script = 0x3,
		Scriptable_EventType_Model = 0x4,
		Scriptable_EventType_Collision = 0x5,
		Scriptable_EventType_Animation = 0x6,
		Scriptable_EventType_HideShowBone = 0x7,
		Scriptable_EventType_NoteTrack = 0x8,
		Scriptable_EventType_ChunkDynent = 0x9,
		Scriptable_EventType_SpawnDynent = 0xA,
		Scriptable_EventType_PFX = 0xB,
		Scriptable_EventType_Sound = 0xC,
		Scriptable_EventType_Explosion = 0xD,
		Scriptable_EventType_Light = 0xE,
		Scriptable_EventType_Sun = 0xF,
		Scriptable_EventType_Rumble = 0x10,
		Scriptable_EventType_Screenshake = 0x11,
		Scriptable_EventType_PartDamage = 0x12,
		Scriptable_EventType_SetMayhem = 0x13,
		Scriptable_EventType_PlayMayhem = 0x14,
		Scriptable_EventType_ViewmodelShaderParam = 0x15,
		Scriptable_EventType_ViewmodelChangeImage = 0x16,
		Scriptable_EventType_ClientViewSelector = 0x17,
		Scriptable_EventType_TeamSelector = 0x18,
		Scriptable_EventType_AddModel = 0x19,
		Scriptable_EventType_ApplyForce = 0x1A,
		Scriptable_EventType_CompassIcon = 0x1B,
		Scriptable_EventType_MaterialOverride = 0x1C,
		Scriptable_EventType_Count = 0x1D,
	};

	struct ScriptableEventAnonymousDef
	{
		ScriptableEventBaseDef* base;
		char buffer[144];
	};

	struct unk_1453E1B90
	{
		const char* name;
		char __pad0[8];
	}; assert_sizeof(unk_1453E1B90, 0x10);

	struct unk_1453E1BC0
	{
		const char* name;
	};

	struct unk_1453E1C00
	{
		const char* name;
	};

	struct unk_1453E1C20
	{
		const char* name;
		char __pad0[8];
	};

	struct unk_1453E1C70
	{
		const char* name;
		unsigned int count;
		unsigned int* val;
	};

	union unk_1453E1C80
	{
		unk_1453E1BC0 __1;
		unk_1453E1C00 __2;
		unk_1453E1C20 __3;
		unk_1453E1C70 __4;
	};

	enum ScriptablePartReferenceType
	{
		type_1453E1BC0,
		type_1453E1C00,
		type_1453E1C20,
		type_1453E1C70,
		unk_1453E1CC8_type_count,
	};

	struct ScriptablePartReference
	{
		unk_1453E1B90 unk01;
		int type;
		unk_1453E1C80 u;
	}; assert_sizeof(ScriptablePartReference, 0x30);

	struct ScriptableEventStateChangeDef
	{
		ScriptableEventBaseDef* base;
		ScriptablePartReference partReference;
		ScriptablePartDef* part;
	}; assert_sizeof(ScriptableEventStateChangeDef, 64);

	struct ScriptableEventWaitDef
	{
		ScriptableEventBaseDef* base;
		float delayMin;
		float delayMax;
		unsigned short eventStreamBufferOffsetServer;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventWaitDef, 24);

	struct ScriptableEventRandomDef
	{
		ScriptableEventBaseDef* base;
		float threshold;
		unsigned int eventACount;
		ScriptableEventDef* eventsA;
		unsigned int eventBCount;
		ScriptableEventDef* eventsB;
		unsigned short eventStreamBufferOffsetServer;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventRandomDef, 48);

	struct ScriptableEventScriptDef
	{
		ScriptableEventBaseDef* base;
		const char* notification;
		scr_string_t scrNotification;
		int param;
		//bool doNotifyCallback;
	}; assert_sizeof(ScriptableEventScriptDef, 24);

	struct ScriptableEventModelDef
	{
		ScriptableEventBaseDef* base;
		XModel* model;
		char hudOutlineColor;
		bool hudOutlineActive;
		bool hudOutlineFill;
		bool neverMoves;
		bool dynamicSimulation;
		bool activatePhysics;
	}; assert_sizeof(ScriptableEventModelDef, 24);

	struct ScriptableEventCollisionDef
	{
		ScriptableEventBaseDef* base;
		const char* collmapName;
		bool aiObstacle;
		int clipmapCModelIndex;
		bool neverMoves;
		bool canPush;
		bool canTouch;
		bool destroyOnExit;
	}; assert_sizeof(ScriptableEventCollisionDef, 32);

	struct $6CB7272563F4458FB40A4A5E123C4ABA
	{
		unsigned short index;
		unsigned short tree;
	};

	union $754892263A8270AD7855E320C3BF2A51
	{
		$6CB7272563F4458FB40A4A5E123C4ABA __s0;
		unsigned __int64 linkPointer;
	};

	struct scr_anim_t
	{
		$754892263A8270AD7855E320C3BF2A51 ___u0;
	};

	struct ScriptableEventAnimationDef
	{
		ScriptableEventBaseDef* base;
		XAnimParts* animation;
		bool stateful;
		bool overrideAnimation;
		bool pauseOnExit;
		bool clientOnlyInMP;
		float startTimeMin;
		float startTimeMax;
		bool startTimeNormalized;
		float playbackRateMin;
		float playbackRateMax;
		float blendTime;
		unsigned int eventAtEndCount;
		ScriptableEventDef* eventsAtEnd;
		unsigned short eventStreamBufferOffsetClient;
		unsigned short eventStreamBufferOffsetServer;
		scr_anim_t animationIndex[2];
	}; assert_sizeof(ScriptableEventAnimationDef, 80);

	struct ScriptableEventHideShowBoneDef
	{
		ScriptableEventBaseDef* base;
		const char* tagName;
		scr_string_t scrTagName;
		bool hide;
		bool allowMissingTag;
		bool hideShowChildren;
	}; assert_sizeof(ScriptableEventHideShowBoneDef, 24);

	struct ScriptableNoteTrackDef
	{
		int flags;
		const char* noteTrackName;
		scr_string_t scrNoteTrackName;
		unsigned int numEvents;
		ScriptableEventDef* events;
	}; assert_sizeof(ScriptableNoteTrackDef, 32);

	struct ScriptableEventNoteTrackDef
	{
		ScriptableEventBaseDef* base;
		unsigned int noteTrackCount;
		ScriptableNoteTrackDef* noteTracks;
	}; assert_sizeof(ScriptableEventNoteTrackDef, 24);

	struct ScriptableEventChunkDynentDef
	{
		ScriptableEventBaseDef* base;
		ScriptablePartReference partReference;
		vec3_t launchLinVel;
		vec3_t launchAngVel;
		ScriptablePartDef* part;
	}; assert_sizeof(ScriptableEventChunkDynentDef, 88);

	struct ScriptableEventSpawnDynentDef
	{
		ScriptableEventBaseDef* base;
		bool stateful;
		XModel* model;
		const char* tagName;
		scr_string_t scrTagName;
		bool allowCauseVelocity;
		bool allowNearestVehicleVelocity;
		bool worldSpaceLinVel;
		bool worldSpaceAngVel;
		bool randomRangeLinVel;
		bool randomRangeAngVel;
		vec3_t launchLinVel;
		vec3_t launchAngVel;
		unsigned short eventStreamBufferOffsetClient;
		bool allowMissingTag;
		bool useRootOnMissingTag;
	}; assert_sizeof(ScriptableEventSpawnDynentDef, 72);

	struct ScriptableEventPFXDef
	{
		ScriptableEventBaseDef* base;
		bool stateful;
		scr_string_t* scrTagNames;
		unsigned int scrTagCount;
		scr_string_t* scrEndTagNames;
		unsigned int scrEndTagCount;
		bool useTagAngles;
		const char* effectAlias;
		FxCombinedDef effectDef;
		float loopRate;
		bool oneshotLooping;
		unsigned short eventStreamBufferOffsetClient;
		bool allowMissingTag;
		bool useRootOnMissingTag;
		bool allowNearestVehicleVelocity;
		bool isViewmodel;
		bool killOnExit;
		bool useDynamicScriptedBeamLength;
		int beamBoneAxis;
		float beamLength;
		char __pad0[8];
	}; assert_sizeof(ScriptableEventPFXDef, 104);
	assert_offsetof(ScriptableEventPFXDef, effectDef, 56);

	struct ScriptableEventSoundDef
	{
		ScriptableEventBaseDef* base;
		char __pad0[8];
		const char* tagName;
		scr_string_t scrTagName;
		const char* soundAlias;
		const char* soundAliasCache;
		char __pad1[8];
	}; assert_sizeof(ScriptableEventSoundDef, 56);
	assert_offsetof(ScriptableEventSoundDef, tagName, 16);
	assert_offsetof(ScriptableEventSoundDef, soundAlias, 32);
	assert_offsetof(ScriptableEventSoundDef, soundAliasCache, 40);

	struct ScriptableEventExplosionDef
	{
		ScriptableEventBaseDef* base;
		const char* tagName;
		scr_string_t scrTagName;
		float radius;
		float dmgOuter;
		float dmgInner;
		bool stateful;
		bool allowMissingTag;
		bool useRootOnMissingTag;
		char __pad0[8];
	}; assert_sizeof(ScriptableEventExplosionDef, 48);

	struct ScriptableEventLightDef
	{
		ScriptableEventBaseDef* base;
		const char* name;
		scr_string_t scrName;
		float intensityScaleMin;
		float intensityScaleMax;
		float radiusMin;
		float radiusMax;
		bool useColor;
		vec4_t colorLinearSrgb;
		bool useStateTransitionTime;
		float transitionMin;
		float transitionMax;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventLightDef, 72);

	struct ScriptableEventSunDef
	{
		ScriptableEventBaseDef* base;
		bool useIntensity;
		float intensityOverrideMin;
		float intensityOverrideMax;
		bool useColor;
		vec4_t colorLinearSrgb;
		bool useStateTransitionTime;
		float transitionMin;
		float transitionMax;
		float pitchMin;
		float pitchMax;
		float headingMin;
		float headingMax;
		bool useDirection;
	}; assert_sizeof(ScriptableEventSunDef, 72);

	struct ScriptableEventRumbleDef
	{
		ScriptableEventBaseDef* base;
		bool stateful;
		const char* tagName;
		scr_string_t scrTagName;
		const char* rumble;
		RumbleInfo* rumbleAsset;
		bool allowMissingTag;
		bool useRootOnMissingTag;
	}; assert_sizeof(ScriptableEventRumbleDef, 56);

	struct ScriptableEventScreenshakeDef
	{
		ScriptableEventBaseDef* base;
		bool stateful;
		bool isEarthquake;
		bool isScreenShake;
		const char* tagName;
		scr_string_t scrTagName;
		float duration;
		float durationFadeUp;
		float durationFadeDown;
		int radius;
		float scaleEarthquake;
		int scalePitch;
		int scaleYaw;
		int scaleRoll;
		int frequencyPitch;
		int frequencyYaw;
		int frequencyRoll;
		bool allowMissingTag;
		bool useRootOnMissingTag;
	}; assert_sizeof(ScriptableEventScreenshakeDef, 80);

	struct ScriptableEventPartDamageDef
	{
		ScriptableEventBaseDef* base;
		ScriptablePartReference partReference;
		float amount;
		//bool destroy;
		float amountDoT;
		float intervalDoT;
		unsigned short eventStreamBufferOffsetServer;
		unsigned short eventStreamBufferOffsetClient;
		ScriptablePartDef* part;
	}; assert_sizeof(ScriptableEventPartDamageDef, 80);

	struct ScriptableEventSetMayhemDef
	{
		ScriptableEventBaseDef* base;
		MayhemData* mayhem;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventSetMayhemDef, 24);

	enum Scriptable_MayhemAction
	{
		Scriptable_MayhemAction_Play = 0x0,
		Scriptable_MayhemAction_Pause = 0x1,
		Scriptable_MayhemAction_Count = 0x2,
	};

	struct ScriptableEventPlayMayhemDef
	{
		ScriptableEventBaseDef* base;
		Scriptable_MayhemAction action;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventPlayMayhemDef, 16);

	enum Scriptable_ShaderParamType
	{
		Scriptable_ShaderParamType_Unknown = 0x0,
		Scriptable_ShaderParamType_ScrollRate_X = 0x1,
		Scriptable_ShaderParamType_ScrollRate_Y = 0x2,
		Scriptable_ShaderParamType_ScrollRate_R = 0x3,
		Scriptable_ShaderParamType_Rotation = 0x4,
		Scriptable_ShaderParamType_Tiling_X = 0x5,
		Scriptable_ShaderParamType_Tiling_Y = 0x6,
		Scriptable_ShaderParamType_Alpha = 0x7,
		Scriptable_ShaderParamType_Emissive = 0x8,
		Scriptable_ShaderParamType_AtlasFrame = 0x9,
		Scriptable_ShaderParamType_Placeholder1 = 0xA,
		Scriptable_ShaderParamType_Placeholder2 = 0xB,
		Scriptable_ShaderParamType_Placeholder3 = 0xC,
		Scriptable_ShaderParamType_Placeholder4 = 0xD,
		Scriptable_ShaderParamType_Count = 0xE,
	};

	struct ScriptableEventViewmodelShaderParamDef
	{
		ScriptableEventBaseDef* base;
		Scriptable_ShaderParamType shaderParam;
		float floatValue;
		float placeHolderValue1;
		float placeHolderValue2;
		float placeHolderValue3;
		float placeHolderValue4;
		float transitionTime;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventViewmodelShaderParamDef, 40);

	enum Scriptable_ImageId
	{
		Scriptable_ImageId_Unknown = 0x0,
		Scriptable_ImageId_Count = 0x1,
	};

	struct ScriptableEventViewmodelChangeImageDef
	{
		ScriptableEventBaseDef* base;
		Scriptable_ImageId imageId;
		int imageValue;
	}; assert_sizeof(ScriptableEventViewmodelChangeImageDef, 16);

	struct ScriptableEventClientViewSelectorDef
	{
		ScriptableEventBaseDef* base;
		unsigned int event1pCount;
		ScriptableEventDef* events1p;
		unsigned int event3pCount;
		ScriptableEventDef* events3p;
		unsigned short eventStreamBufferOffsetClient;
	}; assert_sizeof(ScriptableEventClientViewSelectorDef, 48);

	enum Scriptable_TeamFilter
	{
		Scriptable_Team_Filter_All = 0x0,
		Scriptable_Team_Filter_Bad = 0x1,
		Scriptable_Team_Filter_Axis = 0x2,
		Scriptable_Team_Filter_Allies = 0x3,
		Scriptable_Team_Filter_Three = 0x4,
		Scriptable_Team_Filter_Neutral = 0x5,
		Scriptable_Team_Filter_Dead = 0x6,
		Scriptable_Team_Filter_My_Team = 0x7,
		Scriptable_Team_Filter_Enemy_Team = 0x8,
		Scriptable_Team_Filter_All_But_My_Team = 0x9,
		Scriptable_Team_Filter_All_But_Enemy_Team = 0xA,
		Scriptable_Team_Filter_Enemy_Team_And_Me = 0xB,
		Scriptable_Team_Filter_Just_Me = 0xC,
		Scriptable_Team_Filter_Count = 0xD,
	};

	struct ScriptableEventTeamSelectorDef
	{
		ScriptableEventBaseDef* base;
		Scriptable_TeamFilter teamFilter;
		bool testLocalPlayer;
		unsigned int eventPassCount;
		ScriptableEventDef* eventsPass;
		unsigned int eventFailCount;
		ScriptableEventDef* eventsFail;
		unsigned short eventStreamBufferOffsetClient;
		unsigned short eventStreamBufferOffsetServer;
	}; assert_sizeof(ScriptableEventTeamSelectorDef, 56);

	struct ScriptableEventAddModelDef
	{
		ScriptableEventBaseDef* base;
		const char* tagName;
		scr_string_t scrTagName;
		XModel* model;
	}; assert_sizeof(ScriptableEventAddModelDef, 32);

	struct ScriptableEventApplyForceDef
	{
		ScriptableEventBaseDef* base;
		vec3_t forcePos;
		vec3_t forceVector;
	}; assert_sizeof(ScriptableEventApplyForceDef, 32);

	struct ScriptableEventCompassIconDef
	{
		ScriptableEventBaseDef* base;
		bool stateful;
		bool friendlyCompassIconAlwaysVisible;
		bool enemyCompassIconAlwaysVisible;
		Material* friendlyArrow;
		Material* friendlyFiring;
		Material* friendlyChatting;
		Material* friendlyYelling;
		Material* partyArrow;
		Material* partyFiring;
		Material* partyChatting;
		Material* partyYelling;
		Material* squadArrow;
		Material* squadFiring;
		Material* enemyCompassIconQuiet[3];
		Material* enemyCompassIconFiring[3];
		Material* enemyCompassIconDirectional;
	}; assert_sizeof(ScriptableEventCompassIconDef, 152);

	enum Scriptable_MaterialOverrideType
	{
		Scriptable_MaterialOverrideType_Off = 0x0,
		Scriptable_MaterialOverrideType_Refraction = 0x1,
		Scriptable_MaterialOverrideType_Dissolve = 0x2,
		Scriptable_MaterialOverrideType_FlagAmplitudeScale = 0x3,
		Scriptable_MaterialOverrideType_Count = 0x4,
	};

	struct ScriptableEventMaterialOverrideDef
	{
		ScriptableEventBaseDef* base;
		Scriptable_MaterialOverrideType materialOverrideType;
		char __pad0[4];
		Material* material;
		int transitionTime;
		char finalValue;
		unsigned short eventStreamBufferOffsetClient;
		char __pad1[8];
	}; assert_sizeof(ScriptableEventMaterialOverrideDef, 40);

	union ScriptableEventDefUnion
	{
		ScriptableEventAnonymousDef anonymous;
		ScriptableEventStateChangeDef stateChange; //
		ScriptableEventWaitDef wait;
		ScriptableEventRandomDef random;
		ScriptableEventScriptDef script; //
		ScriptableEventModelDef model;
		ScriptableEventCollisionDef collision;
		ScriptableEventAnimationDef animation;
		ScriptableEventHideShowBoneDef hideShowBone;
		ScriptableEventNoteTrackDef noteTrack;
		ScriptableEventChunkDynentDef chunkDynent;
		ScriptableEventSpawnDynentDef spawnDynent; //
		ScriptableEventPFXDef particleFX; //
		ScriptableEventSoundDef sound; //
		ScriptableEventExplosionDef explosion; //
		ScriptableEventLightDef light; //
		ScriptableEventSunDef sun;
		ScriptableEventRumbleDef rumble;
		ScriptableEventScreenshakeDef screenshake;
		ScriptableEventPartDamageDef partDamage; //
		ScriptableEventSetMayhemDef setMayhem;
		ScriptableEventPlayMayhemDef playMayhem;
		ScriptableEventViewmodelShaderParamDef viewmodelShaderParam;
		ScriptableEventViewmodelChangeImageDef viewmodelChangeImage;
		ScriptableEventClientViewSelectorDef clientViewSelector;
		ScriptableEventTeamSelectorDef teamSelector;
		ScriptableEventAddModelDef addModel;
		ScriptableEventApplyForceDef applyForce; //
		ScriptableEventCompassIconDef compassIcon;
		ScriptableEventMaterialOverrideDef materialOverride; //
	};

	struct ScriptableEventDef
	{
		ScriptableEventBaseDef base;
		Scriptable_EventType type;
		ScriptableEventDefUnion data;
	}; assert_sizeof(ScriptableEventDef, 176);

	struct ScriptableStateBaseDef
	{
		const char* name;
		int flags;
		unsigned int numEvents;
		ScriptableEventDef* events;
	}; assert_sizeof(ScriptableStateBaseDef, 24);

	enum Scriptable_StateType
	{
		Scriptable_StateType_Simple = 0x0,
		Scriptable_StateType_Health = 0x1,
		Scriptable_StateType_Scripted = 0x2,
		Scriptable_StateType_Count = 0x3,
	};

	struct ScriptableStateSimpleDef
	{
		ScriptableStateBaseDef* base;
	};

	struct ScriptableStateHealthDef
	{
		ScriptableStateBaseDef* base;
		int health;
		int validDamageCause;
		//int validAttackers;
		//int validAttackerTeams;
		int minimumDamage;
		float damagePropagationToParent;
		float damagePropagationToChild;
		float damagePropagationFromParent;
		float damagePropagationFromChild;
		const char* script_id;
		scr_string_t scrScript_id;
		bool localizeSplashDamage;
	}; assert_sizeof(ScriptableStateHealthDef, 56);
	assert_offsetof(ScriptableStateHealthDef, scrScript_id, 48);

	struct ScriptableStateScriptedDef
	{
		ScriptableStateBaseDef* base;
		const char* script_id;
		scr_string_t scrScript_id;
	};

	union ScriptableStateDefUnion
	{
		ScriptableStateSimpleDef simple;
		ScriptableStateHealthDef health; //
		ScriptableStateScriptedDef scripted;
	};

	struct ScriptableStateDef
	{
		ScriptableStateBaseDef base;
		Scriptable_StateType type;
		ScriptableStateDefUnion data;
	}; assert_sizeof(ScriptableStateDef, 88);
	assert_offsetof(ScriptableStateDef, data, 32);

	struct ScriptableDamageTagOverride
	{
		const char* tag;
		scr_string_t scrTag;
	};

	struct ScriptablePartDef // unchecked
	{
		const char* name;
		scr_string_t scrName;
		int flags;
		unsigned int flatId;
		unsigned int serverInstanceFlatId;
		unsigned int serverControlledFlatId;
		const char* tagName;
		scr_string_t scrTagName;
		unsigned int numStates;
		ScriptableStateDef* states;
		ScriptablePartDef* parentPart;
		unsigned int numChildParts;
		ScriptablePartDef* childParts;
		unsigned int numDamageTagOverrides;
		ScriptableDamageTagOverride* damageTagOverrides;
		unsigned short eventStreamBufferOffsetServer;
		unsigned short eventStreamBufferOffsetClient;
		unsigned short eventStreamSize;
	}; assert_sizeof(ScriptablePartDef, 104);

	struct ScriptableDef // unchecked
	{
		const char* name;
		int flags;
		int type;
		ScriptableDef* nextScriptableDef;
		unsigned int numParts;
		ScriptablePartDef* parts;
		unsigned int maxNumDynEntsRequired;
		unsigned int partCount;
		unsigned int serverInstancedPartCount;
		unsigned int serverControlledPartCount;
		unsigned int maxNumDynEntPartsBase;
		unsigned int maxNumDynEntPartsForSpawning;
		unsigned short eventStreamSizeRequiredServer;
		unsigned short eventStreamSizeRequiredClient;
		unsigned short eventStreamSize;
		unsigned int ffMemCost;
		scr_string_t animationTreeName;
		void* animationTreeDef[2];
		unsigned int numXModels;
		XModel** models;
	}; assert_sizeof(ScriptableDef, 0x70);

	struct EquipmentClothData
	{
		char* foleyName;
		char* footstepName;
	};

	struct EquipmentWeaponRattleData
	{
		char* szName;
		float priority;
	};

	struct EquipmentSndChance
	{
		float rattleChance;
		float accentChance;
		float silentChance;
	};

	struct EquipmentChanceMoveTypes
	{
		EquipmentSndChance* chances;
	};

	struct EquipmentChanceRattleTypes
	{
		EquipmentChanceMoveTypes* chances;
	};

	struct EquipmentSoundSet
	{
		SndAliasList* soundPLR;
		SndAliasList* soundNPC;
	};

	struct EquipSoundSetMoveTypes
	{
		EquipmentSoundSet* soundSets;
	};

	struct EquipSoundSetMantleTypes
	{
		EquipmentSoundSet soundSets[12];
	}; assert_sizeof(EquipSoundSetMantleTypes, 192);

	struct EquipSoundSetStanceTypes
	{
		EquipmentSoundSet soundSets[4];
	};

	struct EquipSoundSetMeleeVM
	{
		EquipmentSoundSet soundSets[56];
	}; assert_sizeof(EquipSoundSetMeleeVM, 896);

	struct EquipmentSoundTable
	{
		const char* szName;
		unsigned int numClothTypes;
		unsigned int numWeaponRattleTypes;
		unsigned int numMoveTypes;
		unsigned int numStanceTypes;
		EquipmentClothData* clothTypes;
		EquipmentWeaponRattleData* weaponRattleTypes;
		EquipmentChanceRattleTypes* chancesPLR;
		EquipmentChanceRattleTypes* chancesNPC;
		EquipSoundSetMoveTypes* mvmtClothFootstepCeilingSoundSets;
		EquipSoundSetMoveTypes* mvmtClothFoleySoundSets;
		EquipSoundSetMoveTypes* mvmtRattleSoundSets;
		EquipSoundSetMoveTypes mvmtAccentSoundSets;
		EquipSoundSetMantleTypes* mvmtMantleSoundSets;
		EquipSoundSetStanceTypes* mvmtStanceSoundSets;
		EquipSoundSetMeleeVM* meleeAttackVMSoundSets;
	}; assert_sizeof(EquipmentSoundTable, 0x70);
	assert_offsetof(EquipmentSoundTable, mvmtStanceSoundSets, 96);

	enum VectorFieldType : std::int32_t
	{
		VECTOR_FIELD_TYPE_NONE = 0x0,
		VECTOR_FIELD_TYPE_FORCE = 0x1,
		VECTOR_FIELD_TYPE_VELOCITY = 0x2,
		VECTOR_FIELD_NUM_TYPES = 0x3,
	};

	struct VectorSubField
	{
		vec4_t* linearData;
		ExtentBounds worldBounds;
		vec3_t ds;
		unsigned int numEntries[3];
		unsigned int flags;
		VectorFieldType type;
		unsigned int pad[4];
	}; assert_sizeof(VectorSubField, 80);

	struct VectorField
	{
		const char* name;
		VectorSubField* subFields;
		ExtentBounds worldBounds;
		unsigned int numSubFields;
		unsigned int pad;
	}; assert_sizeof(VectorField, 48);

	struct FxParticleSimAnimationHeader
	{
		float playbackRate;
		float duration;
		unsigned int frameCount;
		float minX;
		float minY;
		float minZ;
		float boundsXDelta;
		float boundsYDelta;
		float boundsZDelta;
		float maxWidth;
		float maxHeight;
		unsigned int colorTableSize;
		unsigned int particleDataCount;
		unsigned int maxActiveParticles;
		bool evalVisStatePerParticle;
		bool sortParticlesAtRuntime;
		bool hasOrientation3D;
		bool hasIndex;
	};

	struct FxParticleSimAnimationParticleData
	{
		unsigned short xNormalizedPos;
		unsigned short yNormalizedPos;
		unsigned short zNormalizedPos;
		unsigned short xNormalizedWidth;
		unsigned short yNormalizedHeight;
		unsigned short orientation;
		unsigned short lifetime;
		unsigned short particleID;
		unsigned short xNormalizedPosNextFrame;
		unsigned short yNormalizedPosNextFrame;
		unsigned short zNormalizedPosNextFrame;
		unsigned short xNormalizedWidthNextFrame;
		unsigned short yNormalizedHeightNextFrame;
		short orientationDelta;
		unsigned short colorTableIndex;
		unsigned short nextColorTableIndex;
	};

	struct FxParticleSimAnimationParticleDataOrientation3D
	{
		unsigned short yOrientation;
		unsigned short zOrientation;
		short yOrientationDelta;
		short zOrientationDelta;
	};

	struct FxParticleSimAnimationParticleDataIndex
	{
		char index;
		char nextIndex;
	};

	struct FxParticleSimAnimationFrame
	{
		unsigned int particleDataOffset;
		unsigned int numActiveParticles;
	};

	struct FxParticleSimAnimation
	{
		const char* name;
		Material* material;
		FxParticleSimAnimationHeader header;
		FxParticleSimAnimationParticleData* particleData;
		FxParticleSimAnimationParticleDataOrientation3D* particleDataOrientation3D;
		FxParticleSimAnimationParticleDataIndex* particleDataIndex;
		FxParticleSimAnimationFrame* frames;
		vec4_t* colorTable;
	};

	struct AlwaysloadedFlags
	{
		unsigned int imageCount;
		GfxImage** images;
		unsigned int* imageFlags;
		unsigned int stringCount;
		const char** strings;
		unsigned int* stringFlags;
		unsigned int materialCount;
		Material** materials;
		unsigned int* materialFlags;
	}; assert_sizeof(AlwaysloadedFlags, 72);

	struct AlwaysloadedFlagsPerModel
	{
		XModel* model;
		AlwaysloadedFlags flags;
	}; assert_sizeof(AlwaysloadedFlagsPerModel, 80);

	struct ForcedStubAssetReference
	{
		const char* name;
		char type;
	};

	struct TransientFileReference
	{
		const char* name;
		char type;
	};

	struct TransientAssetReference
	{
		const char* name;
		char type;
		unsigned int mpParentIndex;
	};

	struct TransientSets
	{
		unsigned int count;
		const char** names;
		unsigned int* masks;
	};

	struct StreamingInfo
	{
		const char* name;
		int unused[2];
		AlwaysloadedFlags flags;
		int transientInfoFlags;
		unsigned int modelCount;
		AlwaysloadedFlagsPerModel* perModelFlags;
		unsigned int forcedStubAssetCount;
		ForcedStubAssetReference* forcedStubAssets;
		unsigned int transientFileCount;
		TransientFileReference* transientFiles;
		unsigned int transientAssetCount;
		TransientAssetReference* transientAssets;
		unsigned int stringCount;
		const char** strings;
		TransientSets transientSets;
	}; assert_sizeof(StreamingInfo, 0xC0);
	assert_offsetof(StreamingInfo, modelCount, 92);

	struct LaserDef
	{
		const char* name;
		Material* laserMaterial;
		Material* laserLightMaterial;
		bool ownerOnly;
		char pad1[3];
		float range;
		float radius;
		float endOffsetViewmodel;
		float flarePct;
		FxCombinedDef beamEffect;
		FxCombinedDef laserEndEffect;
		vec4_t color;
		vec4_t hdrColorScale;
		bool laserLight;
		char pad2[3];
		float laserLightRadius;
		float laserLightBeginOffset;
		float laserLightEndOffset;
		float laserLightBodyTweak;
		vec4_t laserLightColor;
		vec4_t laserLightHdrColorScale;
		float range_alt;
		float radius_alt;
		float laserLightRadius_alt;
		float flarePct_alt;
	}; assert_sizeof(LaserDef, 0xB8);

	struct TTFDef
	{
		const char* name;
		int fileLen;
		const char* file;
		void* ftFace;
	}; assert_sizeof(TTFDef, 0x20);

	enum SuitBodyAnimType
	{
		SUITBODYANIMTYPE_HUMAN = 0x0,
		SUITBODYANIMTYPE_JUGGERNAUT = 0x1,
		SUITBODYANIMTYPE_COUNT = 0x2,
	};

	struct SuitDef
	{
		const char* name;
		float cam_minVelocityForFovIncrease;
		float cam_maxVelocityForFovIncrease;
		float cam_velocityToDecreaseFov;
		float cam_fovIncreaseAtMaxVelocity;
		float cam_oneOverFovEaseInTimeMs;
		float cam_oneOverFovEaseOutTimeMs;
		bool enableIKOverride;
		float player_globalAccelScale;
		float player_sprintSpeedScale;
		bool player_sprintUnlimited;
		float player_viewBobScale;
		bool jump_slowdownEnable;
		float jump_height;
		float sprintLeap_height;
		float sprintLeap_forwardVelocityScale;
		int sprintLeap_minSprintTimeMs;
		float doubleJump_accel;
		float doubleJump_speed;
		float doubleJump_speedNoBoost;
		float doubleJump_frictionMin;
		float doubleJump_frictionMax;
		float doubleJump_initialUpBoostAccel;
		float doubleJump_standardUpBoostAccel;
		float doubleJump_energyNeededForInitialUpBoost;
		float doubleJump_energyNeededForStandardUpBoost;
		float doubleJump_maxUpwardsVelocity;
		bool doubleJump_enableMinZVelocity;
		float doubleJump_minZVelocity;
		float doubleJump_energyInitialCost;
		float doubleJump_boostUpEnergyUseRate;
		float doubleJump_energyUsePerButtonPress;
		bool doubleJump_hoverOnly;
		float doubleJump_maxViewPitchDip;
		float doubleJump_maxViewBackwardsPitchDip;
		float doubleJump_maxViewRoll;
		float doubleJump_oneOverEaseInTime;
		float doubleJump_oneOverEaseOutTimeMs;
		bool doubleJump_alwaysDipView;
		SndAliasLookup doubleJump_sound;
		SndAliasLookup doubleJump_soundPlayer;
		SndAliasLookup doubleJump_releaseSound;
		SndAliasLookup doubleJump_releaseSoundPlayer;
		int slide_sprint_penalty_ms;
		bool slide_allow_firing;
		bool slide_allow_ads;
		bool slide_allow_weapon_switch;
		float slide_jump_speed_scale;
		float slide_energy_cost_ratio;
		float slide_energy_unknown01;
		float slide_energy_unknown02;
		float slide_energy_unknown03;
		int slide_max_time_ms;
		int slide_max_time_reduced_ms;
		int slide_max_time_base_ms;
		float wallRun_minZVel;
		float wallRun_minTriggerSpeed;
		float wallRun_minMaintainSpeed;
		int wallRun_delayPeriodMs;
		float wallRun_minJumpHeight;
		int wallRun_maxTimeMs;
		int wallRun_fallStageTimeMs;
		float wallRun_maxHeight;
		float wallRun_jumpHeight;
		float wallRun_jumpVelocity;
		float wallRun_frictionScale;
		float wallRun_speedScale;
		float wallRun_speedScaleADS;
		float wallRun_energyInitialCost;
		float wallRun_energyChangePerSecond;
		float wallRun_energyUnknown01;
		float wallRun_energyUnknown02;
		SuitAnimType suitAnimType;
		SuitAnimPackage* animPackage;
		SuitAnimPackage* animPackageL;
		SuitAnimPackage* animPackageR;
		SuitAnimPackage* animPackageRelaxed;
		SuitAnimPackage* animPackageSafe;
		char facialAnimType;
		SuitBodyAnimType bodyAnimType;
		ScriptableDef* scriptableDef;
		int viewheight_stand;
		int viewheight_crouch;
		int viewheight_prone;
		int viewheight_laststand;
		int viewheight_dead;
		int viewheight_swim;
		int viewheight_slide;
		int bounds_radius;
		int bounds_height_stand;
		int bounds_height_crouch;
		int bounds_height_prone;
		Bounds bounds_stand;
		Bounds bounds_crouch;
		Bounds bounds_prone;
		int radialMotionBlur_interpTimeIn;
		int radialMotionBlur_interpTimeOut;
		float radialMotionBlur_sprintMinRadius;
		float radialMotionBlur_sprintMaxRadius;
		float radialMotionBlur_sprintMinStrength;
		float radialMotionBlur_sprintMaxStrength;
		float radialMotionBlur_slideMinRadius;
		float radialMotionBlur_slideMaxRadius;
		float radialMotionBlur_slideMinStrength;
		float radialMotionBlur_slideMaxStrength;
		float radialMotionBlur_doubleJumpMinRadius;
		float radialMotionBlur_doubleJumpMaxRadius;
		float radialMotionBlur_doubleJumpMinStrength;
		float radialMotionBlur_doubleJumpMaxStrength;
		float radialMotionBlur_wallRunMinRadius;
		float radialMotionBlur_wallRunMaxRadius;
		float radialMotionBlur_wallRunMinStrength;
		float radialMotionBlur_wallRunMaxStrength;
		float radialMotionBlur_groundPoundMinRadius;
		float radialMotionBlur_groundPoundMaxRadius;
		float radialMotionBlur_groundPoundMinStrength;
		float radialMotionBlur_groundPoundMaxStrength;
		float radialMotionBlur_rewindMinRadius;
		float radialMotionBlur_rewindMaxRadius;
		float radialMotionBlur_rewindMinStrength;
		float radialMotionBlur_rewindMaxStrength;
		float radialMotionBlur_dodgeMinRadius;
		float radialMotionBlur_dodgeMaxRadius;
		float radialMotionBlur_dodgeMinStrength;
		float radialMotionBlur_dodgeMaxStrength;
		RumbleInfo* groundPound_activationRumble;
		SndAliasLookup groundPound_activationSound;
		SndAliasLookup groundPound_activationSoundPlayer;
		RumbleInfo* groundPound_landingRumble;
		SndAliasLookup groundPound_landingSound;
		SndAliasLookup groundPound_landingSoundPlayer;
		RumbleInfo* landing_rumbleLowHeight;
		RumbleInfo* landing_rumbleMediumHeight;
		RumbleInfo* landing_rumbleHighHeight;
		RumbleInfo* landing_rumbleExtremeHeight;
		float landing_speedScale;
		float footstep_shakeBroadcastRadiusInches;
		int footstep_shakeDurationMs;
		float footstep_shakeAmplitude;
		RumbleInfo* footstep_rumble;
	}; assert_sizeof(SuitDef, 0x2C0);

	struct SuitAnimEntry
	{
		XAnimParts* anim;
		int time;
		unsigned int animType;
	}; assert_sizeof(SuitAnimEntry, 16);

	enum gestureAnimType_t
	{
		GESTUREANIMTYPE_NONE = 0x0,
		GESTUREANIMTYPE_SLIDE = 0x1,
		GESTUREANIMTYPE_SLIDE_AKIMBO = 0x2,
		GESTUREANIMTYPE_MANTLE_ON_F_LOW = 0x3,
		GESTUREANIMTYPE_MANTLE_ON_F_MIDDLE = 0x4,
		GESTUREANIMTYPE_MANTLE_ON_F_HIGH = 0x5,
		GESTUREANIMTYPE_MANTLE_ON_F_EXTREME = 0x6,
		GESTUREANIMTYPE_MANTLE_ON_B_LOW = 0x7,
		GESTUREANIMTYPE_MANTLE_ON_B_MIDDLE = 0x8,
		GESTUREANIMTYPE_MANTLE_ON_B_HIGH = 0x9,
		GESTUREANIMTYPE_MANTLE_ON_B_EXTREME = 0xA,
		GESTUREANIMTYPE_MANTLE_ON_L_LOW = 0xB,
		GESTUREANIMTYPE_MANTLE_ON_L_MIDDLE = 0xC,
		GESTUREANIMTYPE_MANTLE_ON_L_HIGH = 0xD,
		GESTUREANIMTYPE_MANTLE_ON_L_EXTREME = 0xE,
		GESTUREANIMTYPE_MANTLE_ON_R_LOW = 0xF,
		GESTUREANIMTYPE_MANTLE_ON_R_MIDDLE = 0x10,
		GESTUREANIMTYPE_MANTLE_ON_R_HIGH = 0x11,
		GESTUREANIMTYPE_MANTLE_ON_R_EXTREME = 0x12,
		GESTUREANIMTYPE_MANTLE_OVER_F_LOW = 0x13,
		GESTUREANIMTYPE_MANTLE_OVER_F_MIDDLE = 0x14,
		GESTUREANIMTYPE_MANTLE_OVER_F_HIGH = 0x15,
		GESTUREANIMTYPE_MANTLE_OVER_F_EXTREME = 0x16,
		GESTUREANIMTYPE_MANTLE_OVER_B_LOW = 0x17,
		GESTUREANIMTYPE_MANTLE_OVER_B_MIDDLE = 0x18,
		GESTUREANIMTYPE_MANTLE_OVER_B_HIGH = 0x19,
		GESTUREANIMTYPE_MANTLE_OVER_B_EXTREME = 0x1A,
		GESTUREANIMTYPE_MANTLE_OVER_L_LOW = 0x1B,
		GESTUREANIMTYPE_MANTLE_OVER_L_MIDDLE = 0x1C,
		GESTUREANIMTYPE_MANTLE_OVER_L_HIGH = 0x1D,
		GESTUREANIMTYPE_MANTLE_OVER_L_EXTREME = 0x1E,
		GESTUREANIMTYPE_MANTLE_OVER_R_LOW = 0x1F,
		GESTUREANIMTYPE_MANTLE_OVER_R_MIDDLE = 0x20,
		GESTUREANIMTYPE_MANTLE_OVER_R_HIGH = 0x21,
		GESTUREANIMTYPE_MANTLE_OVER_R_EXTREME = 0x22,
		GESTUREANIMTYPE_MANTLE_LADDER_LOW = 0x23,
		GESTUREANIMTYPE_MANTLE_LADDER_MIDDLE = 0x24,
		GESTUREANIMTYPE_MANTLE_LADDER_HIGH = 0x25,
		GESTUREANIMTYPE_WALLRUN_LEFT = 0x26,
		GESTUREANIMTYPE_WALLRUN_RIGHT = 0x27,
		GESTUREANIMTYPE_WALLRUN_LEFT_AKIMBO = 0x28,
		GESTUREANIMTYPE_WALLRUN_RIGHT_AKIMBO = 0x29,
		GESTUREANIMTYPE_MOUNT_REACH = 0x2A,
		GESTUREANIMTYPE_MOUNT_TOP = 0x2B,
		GESTUREANIMTYPE_MOUNT_LEFT = 0x2C,
		GESTUREANIMTYPE_MOUNT_RIGHT = 0x2D,
		GESTUREANIMTYPE_GROUND_POUND = 0x2E,
		GESTUREANIMTYPE_NVG_ON = 0x2F,
		GESTUREANIMTYPE_NVG_OFF = 0x30,
		GESTUREANIMTYPE_NUM = 0x36,
	};

	struct SuitGestureEntry
	{
		Gesture* gesture;
		gestureAnimType_t gestureAnimType;
	};

	struct SuitAnimPerWeapClass
	{
		SuitAnimEntry* anims;
		unsigned int numAnims;
		int weapClasses;
		SuitGestureEntry* gestures;
		int numGestures;
		FootstepAnim footstep[9];
	}; assert_sizeof(SuitAnimPerWeapClass, 640);

	struct SuitAnimPackage
	{
		const char* name;
		SuitAnimPerWeapClass animOverrides[6];
	}; assert_sizeof(SuitAnimPackage, 0xF08);

	struct SpaceshipTargetDef
	{
		const char* name;
		scr_string_t targetTag;
		float annotationOffsetX;
		float annotationOffsetY;
		float annotationOffsetZ;
		bool disableLeading;
		float lock_minDistance;
		float lock_maxDistance;
		float lock_cutoffDistance;
		float lock_minRate;
		float lock_maxRate;
		float lock_maxZoomFOV;
		float lock_followDistance;
		float lockCooldown_rate;
		float aa_closeDistance;
		float aa_farDistance;
		float aa_closeMinAngle;
		float aa_closeMaxAngle;
		float aa_closeBlendMax;
		float aa_farMinAngle;
		float aa_farMaxAngle;
		float aa_farBlendMax;
		float aaLocked_closeDistance;
		float aaLocked_farDistance;
		float aaLocked_closeMinAngle;
		float aaLocked_closeMaxAngle;
		float aaLocked_closeBlendMax;
		float aaLocked_farMinAngle;
		float aaLocked_farMaxAngle;
		float aaLocked_farBlendMax;
		float pilotAssistScale;
		float callout_maxDistance;
		float callout_maxAngle;
	};

	enum RumbleGraphs
	{
		RUMBLEGRAPH_HIGH = 0x0,
		RUMBLEGRAPH_LOW = 0x1,
		RUMBLEGRAPH_COUNT = 0x2,
	};

	struct RumbleInfo
	{
		const char* name;
		int duration;
		float range;
		RumbleGraph* highRumbleGraph;
		RumbleGraph* lowRumbleGraph;
		bool fadeWithDistance;
		bool broadcast;
	};

	struct RumbleGraph
	{
		const char* name;
		vec2_t knots[16];
		unsigned short knotCount;
	};

	struct WeaponAnimPackageStateTimers
	{
		unsigned short iFireDelay;
		unsigned short iMeleeDelay;
		unsigned short iDetonateDelay;
		unsigned short iFirstRaiseTime;
		unsigned short iAltRaiseTime;
		unsigned short iAltRaiseADSTime;
		unsigned short iAltRaiseAkimboTime;
		unsigned short iRechamberTime;
		unsigned short rechamberTimeOneHanded;
		unsigned short iRechamberBoltTime;
		unsigned short iHoldFireTime;
		unsigned short iHoldPrimeTime;
		unsigned short iDetonateTime;
		unsigned short iMeleeTime;
		unsigned short meleeChargeTime;
		unsigned short meleeChargeDelay;
		unsigned short altMeleeTime;
		unsigned short altMeleeChargeTime;
		unsigned short altMeleeChargeDelay;
		unsigned short iReloadTime;
		unsigned short iFastReloadTime;
		unsigned short reloadShowRocketTime;
		unsigned short iReloadEmptyTime;
		unsigned short iFastReloadEmptyTime;
		unsigned short iReloadAddTime;
		unsigned short iFastReloadAddTime;
		unsigned short iReloadEmptyAddTime;
		unsigned short iFastReloadEmptyAddTime;
		unsigned short iReloadStartTime;
		unsigned short iAltOverride3PInTime;
		unsigned short iAltOverride3POutTime;
		unsigned short iReloadStartAddTime;
		unsigned short iFastReloadStartAddTime;
		unsigned short iReloadEndTime;
		unsigned short iFastReloadEndTime;
		unsigned short iDropTime;
		unsigned short iRaiseTime;
		unsigned short iAltDropTime;
		unsigned short iAltDropADSTime;
		unsigned short iAltDropAkimboTime;
		unsigned short quickDropTime;
		unsigned short quickRaiseTime;
		unsigned short iBreachRaiseTime;
		unsigned short iEmptyRaiseTime;
		unsigned short iEmptyDropTime;
		unsigned short sprintInTime;
		unsigned short sprintLoopTime;
		unsigned short sprintOutTime;
		unsigned short walkTime;
		unsigned short gungHoTime;
		unsigned short crawlTime;
		unsigned short stunnedTimeBegin;
		unsigned short stunnedTimeLoop;
		unsigned short stunnedTimeEnd;
		unsigned short nightVisionWearTime;
		unsigned short nightVisionWearTimeFadeOutEnd;
		unsigned short nightVisionWearTimePowerUp;
		unsigned short nightVisionRemoveTime;
		unsigned short nightVisionRemoveTimePowerDown;
		unsigned short nightVisionRemoveTimeFadeInStart;
		unsigned short primeTime;
		bool bHoldFullPrime;
		bool bAltOverride3PADSOnly;
		unsigned short blastFrontTime;
		unsigned short blastRightTime;
		unsigned short blastBackTime;
		unsigned short blastLeftTime;
		unsigned short slideInTime;
		unsigned short slideLoopTime;
		unsigned short slideOutTime;
		unsigned short dodgeTime;
		unsigned short wallRunInTime;
		unsigned short wallRunOutTime;
		unsigned short leapInTime;
		unsigned short leapLoopTime;
		unsigned short leapOutTime;
		unsigned short leapCancelTime;
		unsigned short chargeInTime;
		unsigned short chargeLoopTime;
		unsigned short chargeOutTime;
		unsigned short iRegenerationTime;
		unsigned short iRegenerationAddTime;
		unsigned short iReloadInterruptTime;
		unsigned short iReloadEmptyInterruptTime;
		unsigned short iReloadEndInterruptTime;
		unsigned short iReloadMultiple1InterruptTime;
		unsigned short iReloadMultiple2InterruptTime;
		unsigned short iReloadMultiple3InterruptTime;
		unsigned short iReloadMultiple4InterruptTime;
		unsigned short iReloadMultiple5InterruptTime;
		unsigned short iReloadMultiple6InterruptTime;
		unsigned short iReloadMultiple7InterruptTime;
		unsigned short iReloadMultiple8InterruptTime;
		unsigned short iFastReloadInterruptTime;
		unsigned short iFastReloadEmptyInterruptTime;
		unsigned short iFastReloadEndInterruptTime;
		unsigned short iFastReloadMultiple1InterruptTime;
		unsigned short iFastReloadMultiple2InterruptTime;
		unsigned short iFastReloadMultiple3InterruptTime;
		unsigned short iFastReloadMultiple4InterruptTime;
		unsigned short iFastReloadMultiple5InterruptTime;
		unsigned short iFastReloadMultiple6InterruptTime;
		unsigned short iFastReloadMultiple7InterruptTime;
		unsigned short iFastReloadMultiple8InterruptTime;
	}; assert_sizeof(WeaponAnimPackageStateTimers, 206);

	struct FootstepAnims
	{
		FootstepAnim footstep[9];
	}; assert_sizeof(FootstepAnims, 612);

	struct WeaponAnimPackage
	{
		const char* name;
		XAnimParts** anims; // array: 260
		WeaponAnimPackageStateTimers* timers;
		int meleeAnimType;
		int meleeAnimPrimaryCount;
		int meleeAnimAltCount;
		int fireAnimTimesMs[4];
		int pad;
		FootstepAnims* footstep; // always same values
	}; assert_sizeof(WeaponAnimPackage, 0x40);
	assert_offsetof(WeaponAnimPackage, footstep, 56);

	struct WeaponSFXPackageSounds
	{
		const char* name;
		SndAliasLookup pickupSound;
		SndAliasLookup pickupSoundPlayer;
		SndAliasLookup ammoPickupSound;
		SndAliasLookup ammoPickupSoundPlayer;
		SndAliasLookup projectileSound;
		SndAliasLookup pullbackSound;
		SndAliasLookup pullbackSoundPlayer;
		SndAliasLookup pullbackSoundQuick;
		SndAliasLookup pullbackSoundQuickPlayer;
		SndAliasLookup fireSound;
		SndAliasLookup fireSoundPlayer;
		SndAliasLookup fireSoundPlayerAkimbo;
		SndAliasLookup fireSoundPlayerAkimboRight;
		SndAliasLookup fireStartSound;
		SndAliasLookup fireStartSoundPlayer;
		SndAliasLookup fireLoopSound;
		SndAliasLookup fireLoopSoundPlayer;
		SndAliasLookup fireStopSound;
		SndAliasLookup fireStopSoundPlayer;
		SndAliasLookup fireLastSound;
		SndAliasLookup fireLastSoundPlayer;
		SndAliasLookup fireLastSoundPlayerAkimbo;
		SndAliasLookup fireLastSoundPlayerAkimboRight;
		SndAliasLookup regenerateSound;
		SndAliasLookup regenerateSoundPlayer;
		SndAliasLookup emptyFireSound;
		SndAliasLookup emptyFireSoundPlayer;
		SndAliasLookup emptyRegenFireSound;
		SndAliasLookup emptyRegenFireSoundPlayer;
		SndAliasLookup rechamberSound;
		SndAliasLookup rechamberSoundPlayer;
		SndAliasLookup reloadSound;
		SndAliasLookup reloadSoundPlayer;
		SndAliasLookup reloadEmptySound;
		SndAliasLookup reloadEmptySoundPlayer;
		SndAliasLookup reloadStartSound;
		SndAliasLookup reloadStartSoundPlayer;
		SndAliasLookup reloadEndSound;
		SndAliasLookup reloadEndSoundPlayer;
		SndAliasLookup detonateSound;
		SndAliasLookup detonateSoundPlayer;
		SndAliasLookup nightVisionWearSound;
		SndAliasLookup nightVisionWearSoundPlayer;
		SndAliasLookup nightVisionRemoveSound;
		SndAliasLookup nightVisionRemoveSoundPlayer;
		SndAliasLookup altSwitchSound;
		SndAliasLookup altSwitchSoundPlayer;
		SndAliasLookup raiseSound;
		SndAliasLookup raiseSoundPlayer;
		SndAliasLookup firstRaiseSound;
		SndAliasLookup firstRaiseSoundPlayer;
		SndAliasLookup putawaySound;
		SndAliasLookup putawaySoundPlayer;
		SndAliasLookup enterAdsSoundPlayer;
		SndAliasLookup leaveAdsSoundPlayer;
		SndAliasLookup bulletRicochetImpact;
		SndAliasLookup projExplosionSound;
		SndAliasLookup projDudSound;
		SndAliasLookup projIgnitionSound;
		SndAliasLookup bounceSound;
		SndAliasLookup rollingSound;
	}; assert_sizeof(WeaponSFXPackageSounds, 0x1F0);

	struct WeaponSFXPackage
	{
		const char* name;
		WeaponSFXPackageSounds* sounds;
		char rattleSoundType;
		const char* szAdsrBaseSetting;
		const char* szWeapSndReflectionClass;
		float weapSndFireVolumeShot1;
		float weapSndFireVolumeShot2;
		float weapSndFireVolumeShot3;
		float weapSndProneFireLFEVolume;
	}; assert_sizeof(WeaponSFXPackage, 0x38);

	struct WeaponVFXPackageDefs
	{
		FxCombinedDef viewFlashEffect;
		FxCombinedDef worldFlashEffect;
		FxCombinedDef viewFlashADSEffect;
		FxCombinedDef viewFlashOverchargeEffect;
		FxCombinedDef viewFlashOverchargeADSEffect;
		FxCombinedDef worldFlashOverchargeEffect;
		FxCombinedDef viewShellEjectEffect;
		FxCombinedDef worldShellEjectEffect;
		FxCombinedDef viewLastShotEjectEffect;
		FxCombinedDef worldLastShotEjectEffect;
		FxCombinedDef viewMagEjectEffect;
	}; assert_sizeof(WeaponVFXPackageDefs, 0xB0);

	struct WeaponVFXPackage
	{
		const char* name;
		WeaponVFXPackageDefs* vfx;
	};

	struct BehaviorTreeNode
	{
		const char* name;
		char __pad0[16];
	}; assert_sizeof(BehaviorTreeNode, 24);

	struct BehaviorTree
	{
		const char* name;
		unsigned short nodeCount;
		BehaviorTreeNode* nodes;
	}; assert_sizeof(BehaviorTree, 0x18);

	struct XAnimArcheType;

	enum XAnimUpVectorType
	{
		XANIM_UP_VECTOR_TYPE_WORLD_SPACE = 0x0,
		XANIM_UP_VECTOR_TYPE_BONE_RELATIVE = 0x1,
		XANIM_UP_VECTOR_TYPE_AIM_AT_BONE = 0x2,
	};

	struct XAnimAimConstraint
	{
		vec3_t aimTargetLocalOffset;
		vec3_t localAimVector;
		vec3_t localUpVector;
		vec3_t axisMask;
		vec3_t upVector;
		XAnimUpVectorType upVectorType;
	};

	struct XAnimRotationConstraint
	{
		vec4_t rotationOffsetQuat;
		vec3_t axisMask;
		float blendFraction;
	};

	struct XAnimPositionConstraint
	{
		float blendFraction;
		vec3_t minLocalTranslationLimit;
		vec3_t maxLocalTranslationLimit;
		vec3_t offset;
		vec3_t sourceBoneOffsets[2];
		vec3_t axisMask;
	};

	struct XAnimDistanceDrivenOffsetConstraint
	{
		vec4_t minLocalOffset;
		vec4_t maxLocalOffset;
		vec3_t sourceBoneOffsets[2];
		float distanceScale;
		float distanceBias;
		char curveType;
	};

	union XAnimConstraintUnion
	{
		XAnimPositionConstraint positionConstraint;
		XAnimRotationConstraint rotationConstraint;
		XAnimAimConstraint aimConstraint;
		XAnimDistanceDrivenOffsetConstraint distanceDrivenOffsetConstraint;
	};

	struct XAnimConstraint
	{
		char type;
		char localTargetBoneIndex;
		scr_string_t sourceBoneNames[2];
		XAnimConstraintUnion u;
	}; assert_sizeof(XAnimConstraint, 88);

	struct unk_1453E1E68
	{
		scr_string_t unk01;
		char __pad0[92];
		scr_string_t unk02;
		char __pad1[4];
		scr_string_t unk03;
		char __pad2[20];
	}; assert_sizeof(unk_1453E1E68, 0x80);

	struct unk_1453E1E30
	{
		scr_string_t unk01;
		char __pad0[8];
	}; assert_sizeof(unk_1453E1E30, 12);

	struct unk_1453E1EA8
	{
		char __pad0[32];
	}; assert_sizeof(unk_1453E1EA8, 32);

	struct XAnimProceduralBones
	{
		const char* name;
		XAnimConstraint* constraints;
		unsigned int numConstraints;
		unsigned int numTargetBones;
		scr_string_t* targetBoneNames;
		unsigned int unk01_count;
		unk_1453E1E68* unk01;
		unsigned int unk02_count;
		unk_1453E1E30* unk02;
		int unk_04;
		unsigned int unk03_count;
		unk_1453E1EA8* unk03;
	}; assert_sizeof(XAnimProceduralBones, 0x50);

	struct ReticleDef
	{
		const char* name;
		const char* widgetName;
		bool lockReticleToTag;
		bool primaryAffectedByEMP;
		bool secondaryAffectedByEMP;
		bool scaleWithFOV;
		float naturalDistance;
		float minReticleScale;
		float maxReticleScale;
	}; assert_sizeof(ReticleDef, 0x20);

	struct GfxLightMap
	{
		const char* name;
		GfxImage* textures[3];
	};

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
		ComputeShader* computeShader;
		MaterialVertexShader* vertexShader;
		MaterialHullShader* hullShader;
		MaterialDomainShader* domainShader;
		MaterialPixelShader* pixelShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		SndGlobals* soundGlobals;
		SndBank* soundBank;
		SndBankTransient* soundBankTransient;
		clipMap_t* clipMap;
		ComWorld* comWorld;
		GlassWorld* glassWorld;
		PathData* pathData;
		NavMeshData* navMeshData;
		MapEnts* mapEnts;
		FxWorld* fxWorld;
		GfxWorld* gfxWorld;
		GfxWorldTransientZone* gfxWorldTransientZone;
		//void* iesProfile;
		GfxLightDef* lightDef;
		//void* uiMap;
		AnimationClass* animClass;
		PlayerAnimScript* playerAnim;
		Gesture* gesture;
		LocalizeEntry* localize;
		WeaponAttachment* attachment;
		WeaponCompleteDef* weapon;
		ParticleSystemDef* vfx;
		FxEffectDef* fx;
		FxImpactTable* impactFx;
		SurfaceFxTable* surfaceFx;
		//void* aiType;
		//void* mpType;
		//void* character;
		//void* modelAlias;
		RawFile* rawfile;
		ScriptFile* scriptfile;
		StringTable* stringTable;
		LeaderboardDef* leaderboardDef;
		VirtualLeaderboardDef* virtualLeaderboardDef;
		StructuredDataDefSet* structuredDataDefSet;
		DDLFile* ddlFile;
		TracerDef* tracerDef;
		VehicleDef* vehDef;
		AddonMapEnts* addonMapEnts;
		NetConstStrings* netConstStrings;
		LuaFile* luaFile;
		ScriptableDef* scriptable;
		EquipmentSoundTable* equipSndTable;
		VectorField* vectorField;
		FxParticleSimAnimation* particleSimAnimation;
		StreamingInfo* streamingInfo;
		LaserDef* laserDef;
		TTFDef* ttfDef;
		SuitDef* suitDef;
		SuitAnimPackage* suitAnimPackage;
		SpaceshipTargetDef* spaceshipTargetDef;
		RumbleInfo* rumble;
		RumbleGraph* rumbleGraph;
		WeaponAnimPackage* weaponAnimPackage;
		WeaponSFXPackage* weaponSFXPackage;
		WeaponVFXPackage* weaponVFXPackage;
		BehaviorTree* behaviorTree;
		//XAnimArcheType* archeType;
		XAnimProceduralBones* proceduralBones;
		ReticleDef* reticleDef;
		GfxLightMap* lightMap;
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

	typedef std::uint32_t GfxBlendStateBits[4];

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
		DB_LOAD_SYNC = 0x1,
		DB_LOAD_ASYNC = 0x2,
	};

	enum DBAllocFlags : std::int32_t
	{
		DB_ZONE_NONE = 0x0,
		DB_ZONE_PERMANENT = 0x1,
		DB_ZONE_GLOBAL_TIER1 = 0x2,
		DB_ZONE_GLOBAL_TIER2 = 0x4,
		DB_ZONE_GAMEMODE_TIER1 = 0x8,
		DB_ZONE_GAMEMODE_TIER2 = 0x10,
		DB_ZONE_UI = 0x20,
		DB_ZONE_UI_SCENE = 0x40,
		DB_ZONE_GAME = 0x80,
		DB_ZONE_LOAD = 0x100,
		DB_ZONE_PRELOAD_LEVEL_SP = 0x20000,
		DB_ZONE_PRELOAD_TRANSIENT_SP = 0x40000,
		DB_ZONE_CUSTOM = 0x1000000, // added for custom zone loading
	};

	struct XZoneInfo
	{
		const char* name;
		int allocFlags;
		int freeFlags;
	};

	struct DBFile
	{
		char name[64];
		void* handle;
		unsigned __int64 memoryBufferSize;
		unsigned char* memoryBuffer;
		void* handle_dcache;
		char __pad1[22];
		bool isSecure;
		char __pad2[1];
		int some_check;
		char __pad3[44];
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
		unsigned char* (__fastcall* zalloc)(unsigned char*, unsigned int, unsigned int);
		void(__fastcall* zfree)(unsigned char*, unsigned char*);
		unsigned char* opaque;
		int data_type;
		unsigned long adler;
		unsigned long reserved;
	};

	enum DB_CompressorType : std::uint8_t
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
		void* unk1;
		unsigned char* fileBuffer;
		unsigned __int64 readSize;
		unsigned __int64 completedReadSize;
		unsigned __int64 offset;
		unsigned char* start_in;
		unsigned int readError;
		int readingResident;
		DB_ReadStream stream;
		__int64 read_size_;
		__int64 offset_;
		int unk4;
		int unk5;
		int unk6;
		int unk7;
		int unk8;
		int unk9;
		int unk10;
		int unk11;
	}; assert_sizeof(DB_LoadData, 0xA8);

	struct DB_Zone
	{
		char name[64];
		char __pad0[1080];
	}; assert_sizeof(DB_Zone, 1144);

	enum XFileBlock
	{
		XFILE_BLOCK_TEMP = 0x0,
		XFILE_BLOCK_UNK1 = 0x1,
		XFILE_BLOCK_UNK2 = 0x2,
		XFILE_BLOCK_IMAGE_STREAM = 0x3,
		XFILE_BLOCK_SHARED_STREAM = 0x4,
		XFILE_BLOCK_CALLBACK = 0x5,
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

	struct XStreamFile
	{
		std::uint64_t offset;
		std::uint64_t offsetEnd;
		std::uint16_t fileIndex;
		bool isLocalized;
	};

	struct XZoneMemory
	{
		XBlock blocks[MAX_XFILE_COUNT];
		unsigned __int64 callbackPos;
		char __pad0[104];
		XStreamFile* shared_ff_data;
		unsigned int shared_ff_count;
		unsigned int shared_ff_count2;
		void* unknown; // always 0
		XStreamFile* streamed_images;
		unsigned int streamed_image_count;
		int streamed_image_index;
		char __pad1[0x100]; // unk size
	};

	struct DB_AuthHash
	{
		unsigned char bytes[32];
	};

	struct DB_AuthSignature
	{
		unsigned char bytes[256];
	};

	struct DB_MasterBlock
	{
		DB_AuthHash chunkHashes[512];
	}; assert_sizeof(DB_MasterBlock, 0x4000);

	struct DB_AuthSubHeader
	{
		char fastfileName[32];
		unsigned int reserved;
		DB_AuthHash masterBlockHashes[192];
	};

	struct DB_AuthHeader // sub_1409E6100
	{
		char magic[8]; // IWffs100
		unsigned int reserved;
		DB_AuthHash subheaderHash;
		DB_AuthSignature signedSubheaderHash;
		DB_AuthSubHeader subheader;
		char padding[9904]; // not used
	}; assert_sizeof(DB_AuthHeader, 0x4000);

	struct XPakHeader
	{
		char magic[8];
		std::uint32_t version;
		char unknown[20]; // unused
		unsigned char iv[16];
		DB_AuthHash hash;
		DB_AuthSignature signedhash;
		// iv for each image
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
		char magic[8];
		std::uint32_t version;
		std::uint8_t unused; // (unused)
		std::uint8_t has_no_image_fastfile; // .ffi
		std::uint8_t has_no_shared_fastfile; // .ffs
		std::uint8_t unk1;
		std::uint32_t fileTimeHigh; // (unused)
		std::uint32_t fileTimeLow; // (unused)
		XFileStreamData stream_data;
		std::uint32_t shared_ff_hash; // some check
		std::uint32_t shared_ff_count; // streamed shared_asset count
		std::uint32_t image_ff_hash; // some check
		std::uint32_t image_ff_count; // streamed image count
		// image streams 24 * sharedcount
		// image streams 24 * imagecount
		std::uint64_t fileLen;
		std::uint64_t fileLenUnk1;
		std::uint64_t fileLenUnk2;
		// if signed: DB_AuthHeader info;
		// XFileCompressorHeader
	};

	struct XFileCompressorHeader
	{
		DB_CompressorType compressor;
		char magic[3];
	};

#pragma pack(push, 1)
	struct XBlockCompressionBlockHeader
	{
		unsigned int compressedSize;
		std::uint64_t uncompressedSize;
	};

	struct XBlockCompressionBlockSizeAndCompressionType
	{
		unsigned int blockSize : 24;
		unsigned int compressionType : 8;
	};

	struct XBlockCompressionDataHeader
	{
		std::uint64_t uncompressedSize;
		XBlockCompressionBlockSizeAndCompressionType blockSizeAndType;
	};
#pragma pack(pop)

	enum XBlockCompressionType
	{
		XBLOCK_COMPRESSION_INVALID = 0x0,
		XBLOCK_COMPRESSION_ZLIB_SIZE = 0x1,
		XBLOCK_COMPRESSION_ZLIB_SPEED = 0x2,
		XBLOCK_COMPRESSION_LZ4HC = 0x3,
		XBLOCK_COMPRESSION_LZ4 = 0x4,
		XBLOCK_COMPRESSION_NONE = 0x5,
	};

	struct XFileReadData
	{
		bool header_parsed;
		std::uint8_t has_no_image_fastfile;
		std::uint8_t has_no_shared_fastfile;
		std::uint8_t unk1;
		char __pad0[4];
		XFileStreamData stream_data;
		std::uint32_t version;
		std::uint32_t shared_ff_hash;
		std::uint32_t shared_ff_count;
		std::uint64_t fileLenUnk1;
		std::uint32_t image_ff_hash;
		std::uint32_t image_ff_count;
		std::uint64_t fileLen;
		std::uint64_t fileLenUnk2;
	}; assert_sizeof(XFileReadData, 224);

	// SoundAssetBank

	enum snd_asset_format : std::int32_t
	{
		SND_ASSET_FORMAT_PCMS16 = 0x0,
		SND_ASSET_FORMAT_PCMS24 = 0x1,
		SND_ASSET_FORMAT_PCMS32 = 0x2,
		SND_ASSET_FORMAT_IEEE = 0x3,
		SND_ASSET_FORMAT_XMA4 = 0x4,
		SND_ASSET_FORMAT_MP3 = 0x5,
		SND_ASSET_FORMAT_MSADPCM = 0x6,
		SND_ASSET_FORMAT_WMA = 0x7,
		SND_ASSET_FORMAT_FLAC = 0x8,
		SND_ASSET_FORMAT_WIIUADPCM = 0x9,
		SND_ASSET_FORMAT_MPC = 0xA,
		SND_ASSET_FORMAT_COUNT = 0xB,
	};

#pragma pack(push, 1)
	struct SndAssetBankHeader
	{
		unsigned int magic;
		unsigned int version;
		unsigned int entrySize;
		unsigned int checksumSize;
		unsigned int dependencySize;
		unsigned int entryCount;
		unsigned int dependencyCount;
		unsigned int buildVersion;
		std::uint64_t fileSize;
		std::uint64_t entryOffset;
		std::uint64_t checksumOffset;
		char checksumChecksum[16];
		char dependencies[512];
		std::uint64_t SourceChecksumOffset;
		std::uint64_t AssetNameOffset;
		char zoneName[64];
		char platform[8];
		char language[3];
		unsigned int convertedAssetVersion;
		unsigned int assetSectionSize;
		char padding[5];
	};

	struct SndAssetBankEntry
	{
		unsigned int id;
		unsigned int size;
		unsigned int seekTableSize;
		unsigned int frameCount;
		unsigned int hybridPcmSize;
		unsigned __int64 offset;
		int frameRate;
		char channelCount;
		char looping;
		char format;
		unsigned short EnvelopeTime1;
		unsigned short EnvelopeTime2;
		char EnvelopeLoudness0;
		char EnvelopeLoudness1;
		char EnvelopeLoudness2;
		char EnvelopeLoudness3;
		char BlocksizeKB;
	}; static_assert(sizeof(SndAssetBankEntry) == 44);
#pragma pack(pop)
}