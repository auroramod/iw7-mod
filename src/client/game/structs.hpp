#pragma once
#include <d3d11.h>

#define PROTOCOL 1

namespace game
{
	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];

	enum scr_string_t : std::int32_t
	{
	};

	struct CmdArgs
	{
		int nesting;
		int localClientNum[8];
		int controllerIndex[8];
		int argc[8];
		const char** argv[8];
	};

	struct cmd_function_s
	{
		cmd_function_s* next;
		const char* name;
		void(__cdecl* function)();
	};

	enum DvarSetSource : std::uint32_t
	{
		DVAR_SOURCE_INTERNAL = 0x0,
		DVAR_SOURCE_EXTERNAL = 0x1,
		DVAR_SOURCE_SCRIPT = 0x2,
		DVAR_SOURCE_UISCRIPT = 0x3,
		DVAR_SOURCE_SERVERCMD = 0x4,
		DVAR_SOURCE_NUM = 0x5,
	};

	enum DvarFlags : std::uint32_t
	{
		DVAR_FLAG_NONE = 0,
		DVAR_FLAG_SAVED = 0x1,
		DVAR_FLAG_LATCHED = 0x2,
		DVAR_FLAG_CHEAT = 0x4,
		DVAR_FLAG_REPLICATED = 0x8,
		DVAR_FLAG_WRITE = 0x800,
		DVAR_FLAG_READ = 0x2000,
	};

	enum dvar_type : std::int8_t
	{
		boolean = 0,
		value = 1,
		vec2 = 2,
		vec3 = 3,
		vec4 = 4,
		integer = 5,
		enumeration = 6,
		string = 7,
		color = 8,
		rgb = 9 // Color without alpha
	};

	union dvar_value
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		float vector[4];
		const char* string;
		char color[4];
	};

	struct $A37BA207B3DDD6345C554D4661813EDD
	{
		int stringCount;
		const char* const* strings;
	};

	struct $9CA192F9DB66A3CB7E01DE78A0DEA53D
	{
		int min;
		int max;
	};

	struct $251C2428A496074035CACA7AAF3D55BD
	{
		float min;
		float max;
	};

	union dvar_limits
	{
		$A37BA207B3DDD6345C554D4661813EDD enumeration;
		$9CA192F9DB66A3CB7E01DE78A0DEA53D integer;
		$251C2428A496074035CACA7AAF3D55BD value;
		$251C2428A496074035CACA7AAF3D55BD vector;
	};

	struct dvar_t
	{
		int hash; // 0
		unsigned int flags; // 4
		bool unk; // 8 tls value?
		dvar_type type; // 9
		bool modified; // 10
		dvar_value current; // 16
		dvar_value latched; // 32
		dvar_value reset; // 48
		dvar_limits domain; // 64
		char unk2; // 80 always 0?
		void* unk3; // 88 some pointer related to hash?
	}; static_assert(sizeof(dvar_t) == 96);

	enum svscmd_type
	{
		SV_CMD_CAN_IGNORE = 0x0,
		SV_CMD_RELIABLE = 0x1,
	};

	enum keyNum_t
	{
		K_NONE = 0x0,
		K_FIRSTGAMEPADBUTTON_RANGE_1 = 0x1,
		K_BUTTON_A = 0x1,
		K_BUTTON_B = 0x2,
		K_BUTTON_X = 0x3,
		K_BUTTON_Y = 0x4,
		K_BUTTON_LSHLDR = 0x5,
		K_BUTTON_RSHLDR = 0x6,
		K_LASTGAMEPADBUTTON_RANGE_1 = 0x6,
		K_BS = 0x8,
		K_TAB = 0x9,
		K_ENTER = 0xD,
		K_FIRSTGAMEPADBUTTON_RANGE_2 = 0xE,
		K_BUTTON_START = 0xE,
		K_BUTTON_BACK = 0xF,
		K_BUTTON_LSTICK = 0x10,
		K_BUTTON_RSTICK = 0x11,
		K_BUTTON_LTRIG = 0x12,
		K_BUTTON_RTRIG = 0x13,
		K_DPAD_UP = 0x14,
		K_FIRSTDPAD = 0x14,
		K_DPAD_DOWN = 0x15,
		K_DPAD_LEFT = 0x16,
		K_DPAD_RIGHT = 0x17,
		K_BUTTON_LSTICK_ALTIMAGE2 = 0x10,
		K_BUTTON_RSTICK_ALTIMAGE2 = 0x11,
		K_BUTTON_LSTICK_ALTIMAGE = 0xBC,
		K_BUTTON_RSTICK_ALTIMAGE = 0xBD,
		K_LASTDPAD = 0x17,
		K_LASTGAMEPADBUTTON_RANGE_2 = 0x17,
		K_ESCAPE = 0x1B,
		K_FIRSTGAMEPADBUTTON_RANGE_3 = 0x1C,
		K_APAD_UP = 0x1C,
		K_FIRSTAPAD = 0x1C,
		K_APAD_DOWN = 0x1D,
		K_APAD_LEFT = 0x1E,
		K_APAD_RIGHT = 0x1F,
		K_LASTAPAD = 0x1F,
		K_LASTGAMEPADBUTTON_RANGE_3 = 0x1F,
		K_SPACE = 0x20,
		K_GRAVE = 0x60,
		K_TILDE = 0x7E,
		K_BACKSPACE = 0x7F,
		K_ASCII_FIRST = 0x80,
		K_ASCII_181 = 0x80,
		K_ASCII_191 = 0x81,
		K_ASCII_223 = 0x82,
		K_ASCII_224 = 0x83,
		K_ASCII_225 = 0x84,
		K_ASCII_228 = 0x85,
		K_ASCII_229 = 0x86,
		K_ASCII_230 = 0x87,
		K_ASCII_231 = 0x88,
		K_ASCII_232 = 0x89,
		K_ASCII_233 = 0x8A,
		K_ASCII_236 = 0x8B,
		K_ASCII_241 = 0x8C,
		K_ASCII_242 = 0x8D,
		K_ASCII_243 = 0x8E,
		K_ASCII_246 = 0x8F,
		K_ASCII_248 = 0x90,
		K_ASCII_249 = 0x91,
		K_ASCII_250 = 0x92,
		K_ASCII_252 = 0x93,
		K_END_ASCII_CHARS = 0x94,
		K_COMMAND = 0x96,
		K_CAPSLOCK = 0x97,
		K_POWER = 0x98,
		K_PAUSE = 0x99,
		K_UPARROW = 0x9A,
		K_DOWNARROW = 0x9B,
		K_LEFTARROW = 0x9C,
		K_RIGHTARROW = 0x9D,
		K_ALT = 0x9E,
		K_CTRL = 0x9F,
		K_SHIFT = 0xA0,
		K_INS = 0xA1,
		K_DEL = 0xA2,
		K_PGDN = 0xA3,
		K_PGUP = 0xA4,
		K_HOME = 0xA5,
		K_END = 0xA6,
		K_F1 = 0xA7,
		K_F2 = 0xA8,
		K_F3 = 0xA9,
		K_F4 = 0xAA,
		K_F5 = 0xAB,
		K_F6 = 0xAC,
		K_F7 = 0xAD,
		K_F8 = 0xAE,
		K_F9 = 0xAF,
		K_F10 = 0xB0,
		K_F11 = 0xB1,
		K_F12 = 0xB2,
		K_F13 = 0xB3,
		K_F14 = 0xB4,
		K_F15 = 0xB5,
		K_KP_HOME = 0xB6,
		K_KP_UPARROW = 0xB7,
		K_KP_PGUP = 0xB8,
		K_KP_LEFTARROW = 0xB9,
		K_KP_5 = 0xBA,
		K_KP_RIGHTARROW = 0xBB,
		K_KP_END = 0xBC,
		K_KP_DOWNARROW = 0xBD,
		K_KP_PGDN = 0xBE,
		K_KP_ENTER = 0xBF,
		K_KP_INS = 0xC0,
		K_KP_DEL = 0xC1,
		K_KP_SLASH = 0xC2,
		K_KP_MINUS = 0xC3,
		K_KP_PLUS = 0xC4,
		K_KP_NUMLOCK = 0xC5,
		K_KP_STAR = 0xC6,
		K_KP_EQUALS = 0xC7,
		K_MOUSE1 = 0xC8,
		K_MOUSE2 = 0xC9,
		K_MOUSE3 = 0xCA,
		K_MOUSE4 = 0xCB,
		K_MOUSE5 = 0xCC,
		K_MWHEELDOWN = 0xCD,
		K_MWHEELUP = 0xCE,
		K_AUX1 = 0xCF,
		K_AUX2 = 0xD0,
		K_AUX3 = 0xD1,
		K_AUX4 = 0xD2,
		K_AUX5 = 0xD3,
		K_AUX6 = 0xD4,
		K_AUX7 = 0xD5,
		K_AUX8 = 0xD6,
		K_AUX9 = 0xD7,
		K_AUX10 = 0xD8,
		K_AUX11 = 0xD9,
		K_AUX12 = 0xDA,
		K_AUX13 = 0xDB,
		K_AUX14 = 0xDC,
		K_AUX15 = 0xDD,
		K_AUX16 = 0xDE,
		K_LAST_KEY = 0xDF
	};

	struct KeyState
	{
		int down;
		int unk;
		int repeats;
		int unk2;
	};

	struct field_t
	{
		int cursor;
		int scroll;
		int drawWidth;
		int widthInPixels;
		float charHeight;
		int fixedSize;
		char buffer[256];
	};

	struct PlayerKeyState
	{
		int overstrikeMode;
		int anyKeyDown;
		KeyState keys[256];
		field_t test;
		char __pad0[8];
	}; static_assert(sizeof(PlayerKeyState) == 0x1128);

	struct ScreenPlacement
	{
		vec2_t scaleVirtualToReal;
		vec2_t scaleVirtualToFull;
		vec2_t scaleRealToVirtual;
		vec2_t realViewportPosition;
		vec2_t realViewportSize;
		vec2_t virtualViewableMin;
		vec2_t virtualViewableMax;
		vec2_t realViewableMin;
		vec2_t realViewableMax;
		vec2_t virtualAdjustableMin;
		vec2_t virtualAdjustableMax;
		vec2_t realAdjustableMin;
		vec2_t realAdjustableMax;
		vec2_t subScreenLeft;
	};

	enum netadrtype_t
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_BROADCAST = 0x3,
		NA_IP = 0x4,
	};

	enum netsrc_t
	{
		NS_CLIENT1 = 0x0,
		NS_MAXCLIENTS = 0x1,
		NS_SERVER = 0x2,
		NS_PACKET = 0x3,
		NS_INVALID_NETSRC = 0x4,
	};

	struct netadr_s
	{
		netadrtype_t type;
		unsigned char ip[4];
		unsigned __int16 port;
		netsrc_t localNetID;
		unsigned int addrHandleIndex;
	};

	namespace entity
	{
		struct entityState_t
		{
			__int16 number; // 0
		}; // sizeof = ?

		struct gclient_s
		{
			char __pad0[19376];
			char name[32]; // 19376
			char __pad1[1516];
			int flags; // 20924
		}; // sizeof = 29208?

		static_assert(offsetof(gclient_s, name) == 19376);
		static_assert(offsetof(gclient_s, flags) == 20924);

#pragma pack(push, 1)
		struct gentity_s
		{
			entityState_t s; // 0
			char __pad0[368 - sizeof(entityState_t)];
			gclient_s* client; // 368
			char __pad1[80];
			int flags; // 456
			char __pad3[556];
		}; static_assert(sizeof(gentity_s) == 1016);
#pragma pack(pop)

		static_assert(offsetof(gentity_s, client) == 368);
		static_assert(offsetof(gentity_s, flags) == 456);

		struct clientHeader_t
		{
			char __pad0[8];
			int state; // 8
		}; // sizeof = ?

		struct client_t
		{
			clientHeader_t header; // 0
			char __pad0[124];
			gentity_s* gentity; // 136
			char __pad1[1044];
			char name[32]; // 1188
			char __pad2[714196];
		}; static_assert(sizeof(client_t) == 715416);

		static_assert(offsetof(client_t, gentity) == 136);
		static_assert(offsetof(client_t, name) == 1188);
	}
	using namespace entity;

	namespace assets
	{
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
			ASSET_TYPE_IMPACT_FX = 41, // not confirmed
			ASSET_TYPE_SURFACE_FX = 42, // not confirmed
			ASSET_TYPE_AITYPE = 43, // not confirmed + unused
			ASSET_TYPE_MPTYPE = 44, // not confirmed + unused
			ASSET_TYPE_CHARACTER = 45, // not confirmed + unused
			ASSET_TYPE_XMODELALIAS = 46, // not confirmed + unused
			ASSET_TYPE_UNKNOWN = 47, // not confirmed + unused
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

		struct PhysicsLibrary
		{
			const char* name;
			bool isMaterialList;
			bool isBodyQualityList;
			bool isMotionPropertiesList;
			bool isGlobalTypeCompendium;
			unsigned int havokDataSize;
			char* havokData;
		}; static_assert(sizeof(PhysicsLibrary) == 0x18);

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
		}; static_assert(sizeof(PhysicsSFXEventAsset) == 0x20);

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
		}; static_assert(sizeof(PhysicsVFXEventAsset) == 0x20);

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
		}; static_assert(sizeof(PhysicsAsset) == 0x50);

		struct PhysicsFXPipeline
		{
			const char* name;
			char __pad0[24];
		}; static_assert(sizeof(PhysicsFXPipeline) == 0x20);

		struct PhysicsFXShape
		{
			const char* name;
			unsigned int size;
			vec3_t* vecs;
		}; static_assert(sizeof(PhysicsFXShape) == 0x18);

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
		}; static_assert(sizeof(XAnimParts) == 0x88);

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

		struct XSurface // similar to iw6
		{
			unsigned short flags; // 0
			unsigned short vertCount; // 2
			unsigned short triCount; // 4
			char rigidVertListCount; // 6
			char subdivLevelCount; // 7
			short blendVertCounts[8]; // 8
			char __pad0[8];
			GfxVertexUnion0 verts0; // 32
			Face* triIndices; // 40
			char __pad1[24];
			XRigidVertList* rigidVertLists; // 72
			char __pad2[48];
			void* subdiv; // XSurfaceSubdivInfo* subdiv // 128
			unsigned int* blendVerts; // 136
			char __pad3[80];
			int partBits[8]; // 224
		}; static_assert(sizeof(XSurface) == 0x100);

		struct XModelSurfs
		{
			const char* name;
			XSurface* surfs;
			unsigned short numsurfs;
			int partBits[8];
		}; static_assert(sizeof(XModelSurfs) == 0x38);

		struct XModel
		{
			const char* name;
			char __pad0[728];
		}; static_assert(sizeof(XModel) == 0x2E0);

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
			void* buffer; //ID3D12Resource* buffer;
			//GfxShaderBufferView view;
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
		}; static_assert(sizeof(MayhemAnimFramesSplineCompressed) == 0x58);

		struct MayhemAnimFramesUncompressed
		{
			char* diskQuat;
			char* diskPos;
			MayhemTransBounds* transBounds;
			GfxWrappedBaseBuffer quat;
			GfxWrappedBaseBuffer pos;
			unsigned int quatStride;
			unsigned int posStride;
		}; static_assert(sizeof(MayhemAnimFramesUncompressed) == 0x30);

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
		}; static_assert(sizeof(MayhemDataKeysSplineCompressed) == 0x28);

		struct MayhemDataKeysUncompressed
		{
			char* keys;
			unsigned int numStreams;
			unsigned int keyStride;
		}; static_assert(sizeof(MayhemDataKeysUncompressed) == 0x10);

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
		}; static_assert(sizeof(MayhemAnim) == 56);

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
		}; static_assert(sizeof(MayhemData) == 0x50);

		struct Material
		{
			const char* name;
			char __pad0[0x110];
		}; static_assert(sizeof(Material) == 0x118);

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
		}; static_assert(sizeof(GfxImage) == 0x70);

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
		}; static_assert(sizeof(SndGlobals) == 0xD0);

		struct SndBank
		{
			const char* name;
			const char* zone;
			const char* gameLanguage;
			const char* soundLanguage;
			char __pad0[320];
		};

		struct SndBankResident
		{
			SndBank bank;
		}; static_assert(sizeof(SndBankResident) == 0x160);

		struct SndBankTransient
		{
			const char* name;
			unsigned int elementCount;
			unsigned int* elements;
		}; static_assert(sizeof(SndBankTransient) == 0x18);

		struct LocalizeEntry
		{
			const char* name;
			const char* value;
		}; static_assert(sizeof(LocalizeEntry) == 0x10);

		struct RawFile
		{
			const char* name;
			int compressedLen;
			int len;
			const char* buffer;
		}; static_assert(sizeof(RawFile) == 0x18);

		struct ScriptFile
		{
			const char* name;
			int compressedLen;
			int len;
			int bytecodeLen;
			const char* buffer;
			char* bytecode;
		}; static_assert(sizeof(ScriptFile) == 0x28);

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
		}; static_assert(sizeof(StringTable) == 0x18);

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
		}; static_assert(sizeof(NetConstStrings) == 0x20);

		struct LuaFile
		{
			const char* name;
			int len;
			char strippingType;
			const char* buffer;
		}; static_assert(sizeof(LuaFile) == 0x18);

		struct TTFDef
		{
			const char* name;
			int fileLen;
			const char* file;
			void* ftFace;
		}; static_assert(sizeof(TTFDef) == 0x20);

		union XAssetHeader
		{
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
			void* data;
		};

		struct XAsset
		{
			XAssetType type;
			XAssetHeader header;
		};

		struct DBFile
		{
			char name[64];
			void* handle;
			char __pad[0x60];
		};
		static_assert(sizeof(DBFile) == 0xA8);
	}
	using namespace assets;

	struct GfxFont
	{
		const char* fontName;
		int pixelHeight;
		TTFDef* ttfDef;
	};

	namespace demonware
	{
		enum DWOnlineStatus
		{
			DW_LIVE_DISCONNECTED = 0x0,
			DW_LIVE_UNKNOWN = 0x1,
			DW_LIVE_CONNECTING = 0x2,
			DW_LIVE_CONNECTED = 0x3,
		};

		enum DWNetStatus
		{
			DW_NET_ERROR_START_FAILED = 0x0,
			DW_NET_ERROR_NO_LOCAL_IP = 0x1,
			DW_NET_NOT_STARTED = 0x2,
			DW_NET_STARTING_LAN = 0x3,
			DW_NET_STARTED_LAN = 0x4,
			DW_NET_STARTING_ONLINE = 0x5,
			DW_NET_STARTED_ONLINE = 0x6,
		};

		enum bdLobbyErrorCode : uint32_t
		{
			BD_NO_ERROR = 0x0,
			BD_TOO_MANY_TASKS = 0x1,
			BD_NOT_CONNECTED = 0x2,
			BD_SEND_FAILED = 0x3,
			BD_HANDLE_TASK_FAILED = 0x4,
			BD_START_TASK_FAILED = 0x5,
			BD_RESULT_EXCEEDS_BUFFER_SIZE = 0x64,
			BD_ACCESS_DENIED = 0x65,
			BD_EXCEPTION_IN_DB = 0x66,
			BD_MALFORMED_TASK_HEADER = 0x67,
			BD_INVALID_ROW = 0x68,
			BD_EMPTY_ARG_LIST = 0x69,
			BD_PARAM_PARSE_ERROR = 0x6A,
			BD_PARAM_MISMATCHED_TYPE = 0x6B,
			BD_SERVICE_NOT_AVAILABLE = 0x6C,
			BD_CONNECTION_RESET = 0x6D,
			BD_INVALID_USER_ID = 0x6E,
			BD_LOBBY_PROTOCOL_VERSION_FAILURE = 0x6F,
			BD_LOBBY_INTERNAL_FAILURE = 0x70,
			BD_LOBBY_PROTOCOL_ERROR = 0x71,
			BD_LOBBY_FAILED_TO_DECODE_UTF8 = 0x72,
			BD_LOBBY_ASCII_EXPECTED = 0x73,
			BD_INVALID_CONTEXT = 0x74,
			BD_LOBBY_METHOD_DISABLED = 0x75,
			BD_LOBBY_MIGRATION_FAILURE = 0x76,
			BD_ASYNCHRONOUS_ERROR = 0xC8,
			BD_STREAMING_COMPLETE = 0xC9,
			BD_MEMBER_NO_PROPOSAL = 0x12C,
			BD_TEAMNAME_ALREADY_EXISTS = 0x12D,
			BD_MAX_TEAM_MEMBERSHIPS_LIMITED = 0x12E,
			BD_MAX_TEAM_OWNERSHIPS_LIMITED = 0x12F,
			BD_NOT_A_TEAM_MEMBER = 0x130,
			BD_INVALID_TEAM_ID = 0x131,
			BD_INVALID_TEAM_NAME = 0x132,
			BD_NOT_A_TEAM_OWNER = 0x133,
			BD_NOT_AN_ADMIN_OR_OWNER = 0x134,
			BD_MEMBER_PROPOSAL_EXISTS = 0x135,
			BD_MEMBER_EXISTS = 0x136,
			BD_TEAM_FULL = 0x137,
			BD_VULGAR_TEAM_NAME = 0x138,
			BD_TEAM_USERID_BANNED = 0x139,
			BD_TEAM_EMPTY = 0x13A,
			BD_INVALID_TEAM_PROFILE_QUERY_ID = 0x13B,
			BD_TEAMNAME_TOO_SHORT = 0x13C,
			BD_UNIQUE_PROFILE_DATA_EXISTS_ALREADY = 0x13D,
			BD_APPLICATION_EXISTS = 0x13E,
			BD_APPLICATIONS_MAX_EXCEEDED = 0x13F,
			BD_PAGE_SIZE_LIMIT_EXCEEDED = 0x140,
			BD_APPLICATION_REJECTED_EXISTS = 0x141,
			BD_APPLICATION_WITHDRAWN_EXISTS = 0x142,
			BD_APPLICATION_DOES_NOT_EXIST = 0x143,
			BD_APPLICATION_INVALID_STATE = 0x144,
			BD_MEMBER_BAN_EXISTS = 0x145,
			BD_MEMBER_BAN_DOES_NOT_EXIST = 0x146,
			BD_OWNER_BAN_FORBIDDEN = 0x147,
			BD_INVALID_ACCOUNT_TYPE = 0x148,
			BD_CONFIGURED_ACCOUNT_TYPE_NOT_FOUND = 0x149,
			BD_OWNER_OF_NON_EMPTY_TEAM = 0x14A,
			BD_CANNOT_APPLY_TO_PIRIVATE_TEAM = 0x14B,
			BD_MEMBER_IS_OWNER = 0x15E,
			BD_AUTO_JOINING_DISABLED = 0x15F,
			BD_TEAM_SHOWCASE_DISABLED = 0x160,
			BD_INVALID_FILTER = 0x162,
			BD_INVALID_TEAM_LEADERBOARD = 0x163,
			BD_TEAM_LEADERBOARD_LOADING = 0x164,
			BD_TEAM_SHOWCASE_COUNT_EXCEEDED = 0x165,
			BD_USER_ALREADY_TEAM_OWNER = 0x16A,
			BD_UPPER_TEAM_MEMBER_LIMIT_EXCEEDED = 0x16C,
			BD_TEAM_MEMBER_LIMIT_NOT_SUPPORTED = 0x16D,
			BD_MISMATCHED_TEAM_PROFILE_COLLISION_FIELD = 0x16E,
			BD_INVALID_LEADERBOARD_ID = 0x190,
			BD_INVALID_STATS_SET = 0x191,
			BD_EMPTY_STATS_SET_IGNORED = 0x193,
			BD_NO_DIRECT_ACCESS_TO_ARBITRATED_LBS = 0x194,
			BD_STATS_WRITE_PERMISSION_DENIED = 0x195,
			BD_STATS_WRITE_TYPE_DATA_TYPE_MISMATCH = 0x196,
			BD_NO_STATS_FOR_USER = 0x197,
			BD_INVALID_ACCESS_TO_UNRANKED_LB = 0x198,
			BD_INVALID_EXTERNAL_TITLE_ID = 0x199,
			BD_DIFFERENT_LEADERBOARD_SCHEMAS = 0x19A,
			BD_TOO_MANY_LEADERBOARDS_REQUESTED = 0x19B,
			BD_ENTITLEMENTS_ERROR = 0x19C,
			BD_ENTITLEMENTS_INVALID_TITLEID = 0x19D,
			BD_ENTITLEMENTS_INVALID_LEADERBOARDID = 0x19E,
			BD_ENTITLEMENTS_INVALID_GET_MODE_FOR_TITLE = 0x19F,
			BD_ENTITLEMENTS_URL_CONNECTION_ERROR = 0x1A0,
			BD_ENTITLEMENTS_CONFIG_ERROR = 0x1A1,
			BD_ENTITLEMENTS_NAMED_PARENT_ERROR = 0x1A2,
			BD_ENTITLEMENTS_NAMED_KEY_ERROR = 0x1A3,
			BD_TOO_MANY_ENTITY_IDS_REQUESTED = 0x1A4,
			BD_STATS_READ_FAILED = 0x1A5,
			BD_STATS_INVALID_WRITE_TO_VIRTUAL_LEADERBOARD = 0x1A6,
			BD_STATS_INVALID_WRITE_TYPE_TO_MULTIRANK_LB = 0x1A7,
			BD_STATS_INVALID_EXCLUDED_COLUMN = 0x1A8,
			BD_STATS_INVALID_INCLUDED_COLUMN = 0x1A9,
			BD_STATS_WRITE_NO_SCRIPT_MODULE = 0x1AA,
			BD_STATS_WRITE_SCRIPT_MODULE_ERROR = 0x1AB,
			BD_NON_READ_ON_READ_ONLY_LEADERBOARD = 0x1AC,
			BD_INVALID_TITLE_ID = 0x1F4,
			BD_DOWN_FOR_MAINTENANCE = 0x1F5,
			BD_MESSAGING_INVALID_MAIL_ID = 0x258,
			BD_SELF_BLOCK_NOT_ALLOWED = 0x259,
			BD_GLOBAL_MESSAGE_ACCESS_DENIED = 0x25A,
			BD_GLOBAL_MESSAGES_USER_LIMIT_EXCEEDED = 0x25B,
			BD_MESSAGING_SENDER_DOES_NOT_EXIST = 0x25C,
			BD_AUTH_NO_ERROR = 0x2BC,
			BD_AUTH_BAD_REQUEST = 0x2BD,
			BD_AUTH_SERVER_CONFIG_ERROR = 0x2BE,
			BD_AUTH_BAD_TITLE_ID = 0x2BF,
			BD_AUTH_BAD_ACCOUNT = 0x2C0,
			BD_AUTH_ILLEGAL_OPERATION = 0x2C1,
			BD_AUTH_INCORRECT_LICENSE_CODE = 0x2C2,
			BD_AUTH_CREATE_USERNAME_EXISTS = 0x2C3,
			BD_AUTH_CREATE_USERNAME_ILLEGAL = 0x2C4,
			BD_AUTH_CREATE_USERNAME_VULGAR = 0x2C5,
			BD_AUTH_CREATE_MAX_ACC_EXCEEDED = 0x2C6,
			BD_AUTH_MIGRATE_NOT_SUPPORTED = 0x2C7,
			BD_AUTH_TITLE_DISABLED = 0x2C8,
			BD_AUTH_ACCOUNT_EXPIRED = 0x2C9,
			BD_AUTH_ACCOUNT_LOCKED = 0x2CA,
			BD_AUTH_UNKNOWN_ERROR = 0x2CB,
			BD_AUTH_INCORRECT_PASSWORD = 0x2CC,
			BD_AUTH_IP_NOT_IN_ALLOWED_RANGE = 0x2CD,
			BD_AUTH_WII_TOKEN_VERIFICATION_FAILED = 0x2CE,
			BD_AUTH_WII_AUTHENTICATION_FAILED = 0x2CF,
			BD_AUTH_IP_KEY_LIMIT_REACHED = 0x2D0,
			BD_AUTH_INVALID_GSPID = 0x2D1,
			BD_AUTH_INVALID_IP_RANGE_ID = 0x2D2,
			BD_AUTH_3DS_TOKEN_VERIFICATION_FAILED = 0x2D1,
			BD_AUTH_3DS_AUTHENTICATION_FAILED = 0x2D2,
			BD_AUTH_STEAM_APP_ID_MISMATCH = 0x2D3,
			BD_AUTH_ABACCOUNTS_APP_ID_MISMATCH = 0x2D4,
			BD_AUTH_CODO_USERNAME_NOT_SET = 0x2D5,
			BD_AUTH_WIIU_TOKEN_VERIFICATION_FAILED = 0x2D6,
			BD_AUTH_WIIU_AUTHENTICATION_FAILED = 0x2D7,
			BD_AUTH_CODO_USERNAME_NOT_BASE64 = 0x2D8,
			BD_AUTH_CODO_USERNAME_NOT_UTF8 = 0x2D9,
			BD_AUTH_TENCENT_TICKET_EXPIRED = 0x2DA,
			BD_AUTH_PS3_SERVICE_ID_MISMATCH = 0x2DB,
			BD_AUTH_CODOID_NOT_WHITELISTED = 0x2DC,
			BD_AUTH_PLATFORM_TOKEN_RETRIEVAL_ERROR = 0x2DD,
			BD_AUTH_JSON_FORMAT_ERROR = 0x2DE,
			BD_AUTH_REPLY_CONTENT_ERROR = 0x2DF,
			BD_AUTH_PLATFORM_TOKEN_EXPIRED = 0x2E0,
			BD_AUTH_CONTINUING = 0x2E1,
			BD_AUTH_PLATFORM_TOKEN_DECRYPTION_ERROR = 0x2E2,
			BD_AUTH_PLATFORM_TOKEN_SIGNATURE_ERROR = 0x2E3,
			BD_AUTH_DNS_RESOLUTION_ERROR = 0x2E4,
			BD_AUTH_SSL_CERTIFICATE_ERROR = 0x2E5,
			BD_AUTH_SERVER_UNAVAILABLE_ERROR = 0x2E6,
			BD_AUTH_ENVIRONMENT_ERROR = 0x2E7,
			BD_AUTH_PLATFORM_DEVICE_ID_ERROR = 0x2E8,
			BD_AUTH_UNO_APP_ID_MISMATCH = 0x2E9,
			BD_AUTH_UNICODE_DECODE_ERROR = 0x2EA,
			BD_AUTH_STEAM_PUBLISHER_BAN = 0x2EB,
			BD_AUTH_TICKET_DECRYPTION_ERROR = 0x2EC,
			BD_AUTH_SIGNATURE_ERROR = 0x2ED,
			BD_AUTH_REQUEST_TIMEOUT_ERROR = 0x2EE,
			BD_AUTH_REQUEST_ABORTED_ERROR = 0x2EF,
			BD_AUTH_SINGLE_IDENTITY_FLOW_DISABLED_ERROR = 0x2F0,
			BD_AUTH_SINGLE_IDENTITY_TOKEN_MISSING_ERROR = 0x2F1,
			BD_AUTH_SINGLE_IDENTITY_TOKEN_EXPIRED_ERROR = 0x2F2,
			BD_AUTH_SINGLE_IDENTITY_TOKEN_INVALID_ERROR = 0x2F3,
			BD_AUTH_SWITCH_TOKEN_VERIFICATION_FAILED = 0x2F4,
			BD_AUTH_SWITCH_AUTHENTICATION_FAILED = 0x2F5,
			BD_AUTH_BAD_DEVICE_KEY = 0x2F6,
			BD_AUTH_BAD_DEVICE_TITLE_KEY = 0x2F7,
			BD_AUTH_TSIG_EXPIRED = 0x2F8,
			BD_AUTH_TOO_MANY_REQUESTS = 0x2F9,
			BD_AUTH_COUNTRY_BLOCKED_ERROR = 0x30D,
			BD_NO_PROFILE_INFO_EXISTS = 0x320,
			BD_PROFILE_COLLISION_FIELD_MISMATCH = 0x321,
			BD_FRIENDSHIP_NOT_REQUSTED = 0x384,
			BD_NOT_A_FRIEND = 0x385,
			BD_SELF_FRIENDSHIP_NOT_ALLOWED = 0x387,
			BD_FRIENDSHIP_EXISTS = 0x388,
			BD_PENDING_FRIENDSHIP_EXISTS = 0x389,
			BD_USERID_BANNED = 0x38A,
			BD_FRIENDS_FULL = 0x38C,
			BD_FRIENDS_NO_RICH_PRESENCE = 0x38D,
			BD_RICH_PRESENCE_TOO_LARGE = 0x38E,
			BD_FRIENDS_REMOTE_FULL = 0x38F,
			BD_NO_FILE = 0x3E8,
			BD_PERMISSION_DENIED = 0x3E9,
			BD_FILESIZE_LIMIT_EXCEEDED = 0x3EA,
			BD_FILENAME_MAX_LENGTH_EXCEEDED = 0x3EB,
			BD_EXTERNAL_STORAGE_SERVICE_ERROR = 0x3EC,
			BD_VALIDATION_ERROR = 0x3ED,
			BD_VALIDATION_TOKEN_ERROR = 0x3EE,
			BD_VALIDATION_TOKEN_GENERATION_ERROR = 0x3EF,
			BD_VALIDATION_TOKEN_VERIFICATION_ERROR = 0x3F0,
			BD_STORAGE_SERVER_UNREACHABLE = 0x3F1,
			BD_CHANNEL_DOES_NOT_EXIST = 0x44D,
			BD_CHANNEL_ALREADY_SUBSCRIBED = 0x44E,
			BD_CHANNEL_NOT_SUBSCRIBED = 0x44F,
			BD_CHANNEL_FULL = 0x450,
			BD_CHANNEL_SUBSCRIPTIONS_FULL = 0x451,
			BD_CHANNEL_NO_SELF_WHISPERING = 0x452,
			BD_CHANNEL_ADMIN_REQUIRED = 0x453,
			BD_CHANNEL_TARGET_NOT_SUBSCRIBED = 0x454,
			BD_CHANNEL_REQUIRES_PASSWORD = 0x455,
			BD_CHANNEL_TARGET_IS_SELF = 0x456,
			BD_CHANNEL_PUBLIC_BAN_NOT_ALLOWED = 0x457,
			BD_CHANNEL_USER_BANNED = 0x458,
			BD_CHANNEL_PUBLIC_PASSWORD_NOT_ALLOWED = 0x459,
			BD_CHANNEL_PUBLIC_KICK_NOT_ALLOWED = 0x45A,
			BD_CHANNEL_MUTED = 0x45B,
			BD_CONTENT_UNLOCK_UNKNOWN_ERROR = 0x514,
			BD_UNLOCK_KEY_INVALID = 0x515,
			BD_UNLOCK_KEY_ALREADY_USED_UP = 0x516,
			BD_CONTENT_UNLOCK_LIMIT_REACHED = 0x517,
			BD_DIFFERENT_HARDWARE_ID = 0x518,
			BD_INVALID_CONTENT_OWNER = 0x519,
			BD_CONTENT_UNLOCK_INVALID_USER = 0x51A,
			BD_CONTENT_UNLOCK_INVALID_CATEGORY = 0x51B,
			BD_KEY_ARCHIVE_INVALID_WRITE_TYPE = 0x5DC,
			BD_KEY_ARCHIVE_EXCEEDED_MAX_IDS_PER_REQUEST = 0x5DD,
			BD_BANDWIDTH_TEST_TRY_AGAIN = 0x712,
			BD_BANDWIDTH_TEST_STILL_IN_PROGRESS = 0x713,
			BD_BANDWIDTH_TEST_NOT_PROGRESS = 0x714,
			BD_BANDWIDTH_TEST_SOCKET_ERROR = 0x715,
			BD_INVALID_SESSION_NONCE = 0x76D,
			BD_ARBITRATION_FAILURE = 0x76F,
			BD_ARBITRATION_USER_NOT_REGISTERED = 0x771,
			BD_ARBITRATION_NOT_CONFIGURED = 0x772,
			BD_CONTENTSTREAMING_FILE_NOT_AVAILABLE = 0x7D0,
			BD_CONTENTSTREAMING_STORAGE_SPACE_EXCEEDED = 0x7D1,
			BD_CONTENTSTREAMING_NUM_FILES_EXCEEDED = 0x7D2,
			BD_CONTENTSTREAMING_UPLOAD_BANDWIDTH_EXCEEDED = 0x7D3,
			BD_CONTENTSTREAMING_FILENAME_MAX_LENGTH_EXCEEDED = 0x7D4,
			BD_CONTENTSTREAMING_MAX_THUMB_DATA_SIZE_EXCEEDED = 0x7D5,
			BD_CONTENTSTREAMING_DOWNLOAD_BANDWIDTH_EXCEEDED = 0x7D6,
			BD_CONTENTSTREAMING_NOT_ENOUGH_DOWNLOAD_BUFFER_SPACE = 0x7D7,
			BD_CONTENTSTREAMING_SERVER_NOT_CONFIGURED = 0x7D8,
			BD_CONTENTSTREAMING_INVALID_APPLE_RECEIPT = 0x7DA,
			BD_CONTENTSTREAMING_APPLE_STORE_NOT_AVAILABLE = 0x7DB,
			BD_CONTENTSTREAMING_APPLE_RECEIPT_FILENAME_MISMATCH = 0x7DC,
			BD_CONTENTSTREAMING_BATCH_DOWNLOAD_PARTIAL_FAILURE = 0x7DD,
			BD_CONTENTSTREAMING_HTTP_ERROR = 0x7E4,
			BD_CONTENTSTREAMING_FAILED_TO_START_HTTP = 0x7E5,
			BD_CONTENTSTREAMING_LOCALE_INVALID = 0x7E6,
			BD_CONTENTSTREAMING_LOCALE_MISSING = 0x7E7,
			BD_VOTERANK_ERROR_EMPTY_RATING_SUBMISSION = 0x7EE,
			BD_VOTERANK_ERROR_MAX_VOTES_EXCEEDED = 0x7EF,
			BD_VOTERANK_ERROR_INVALID_RATING = 0x7F0,
			BD_MAX_NUM_TAGS_EXCEEDED = 0x82A,
			BD_TAGGED_COLLECTION_DOES_NOT_EXIST = 0x82B,
			BD_EMPTY_TAG_ARRAY = 0x82C,
			BD_SEARCH_SERVER_UNREACHABLE = 0x82F,
			BD_ENTITY_ID_NOT_SPECIFIED = 0x830,
			BD_INVALID_QUERY_ID = 0x834,
			BD_NO_ENTRY_TO_UPDATE = 0x835,
			BD_SESSION_INVITE_EXISTS = 0x836,
			BD_INVALID_SESSION_ID = 0x837,
			BD_ATTACHMENT_TOO_LARGE = 0x838,
			BD_INVALID_FIELD_VALUE = 0x839,
			BD_REQUEST_SESSION_NOT_SUPPORTED = 0x83A,
			BD_INVALID_GROUP_ID = 0xAF0,
			BD_MAIL_INVALID_MAIL_ID_ERROR = 0xB55,
			BD_MAIL_PERMISSION_DENIED_ERROR = 0xB56,
			BD_TWITCH_SERVICE_ERROR = 0xC1D,
			BD_TWITCH_ACCOUNT_ALREADY_LINKED = 0xC1E,
			BD_TWITCH_NO_LINKED_ACCOUNT = 0xC1F,
			BD_TWITTER_AUTH_ATTEMPT_FAILED = 0xDAD,
			BD_TWITTER_AUTH_TOKEN_INVALID = 0xDAE,
			BD_TWITTER_UPDATE_LIMIT_REACHED = 0xDAF,
			BD_TWITTER_UNAVAILABLE = 0xDB0,
			BD_TWITTER_ERROR = 0xDB1,
			BD_TWITTER_TIMED_OUT = 0xDB2,
			BD_TWITTER_ACCOUNT_AMBIGUOUS = 0xDB4,
			BD_TWITTER_MAXIMUM_ACCOUNTS_REACHED = 0xDB5,
			BD_TWITTER_ACCOUNT_NOT_REGISTERED = 0xDB6,
			BD_TWITTER_DUPLICATE_STATUS = 0xDB7,
			BD_TWITTER_ACCOUNT_ALREADY_REGISTERED = 0xE1C,
			BD_FACEBOOK_AUTH_ATTEMPT_FAILED = 0xE11,
			BD_FACEBOOK_AUTH_TOKEN_INVALID = 0xE12,
			BD_FACEBOOK_PHOTO_DOES_NOT_EXIST = 0xE13,
			BD_FACEBOOK_PHOTO_INVALID = 0xE14,
			BD_FACEBOOK_PHOTO_ALBUM_FULL = 0xE15,
			BD_FACEBOOK_UNAVAILABLE = 0xE16,
			BD_FACEBOOK_ERROR = 0xE17,
			BD_FACEBOOK_TIMED_OUT = 0xE18,
			BD_FACEBOOK_DISABLED_FOR_USER = 0xE19,
			BD_FACEBOOK_ACCOUNT_AMBIGUOUS = 0xE1A,
			BD_FACEBOOK_MAXIMUM_ACCOUNTS_REACHED = 0xE1B,
			BD_FACEBOOK_INVALID_NUM_PICTURES_REQUESTED = 0xE1C,
			BD_FACEBOOK_VIDEO_DOES_NOT_EXIST = 0xE1D,
			BD_FACEBOOK_ACCOUNT_ALREADY_REGISTERED = 0xE1E,
			BD_FACEBOOK_TARGET_OBJECT_ID_INVALID = 0xE1F,
			BD_FACEBOOK_NO_SUCH_ACCOUNT = 0xE20,
			BD_APNS_INVALID_PAYLOAD = 0xE74,
			BD_APNS_INVALID_TOKEN_LENGTH_ERROR = 0xE76,
			BD_MAX_CONSOLEID_LENGTH_EXCEEDED = 0xEE1,
			BD_MAX_WHITELIST_LENGTH_EXCEEDED = 0xEE2,
			BD_TOTP_CHALLENGE_FAILED = 0xEE3,
			BD_NO_TOTP_ACCOUNT = 0xEE4,
			BD_EXTENDED_AUTH_INFO_ERROR = 0xEE5,
			BD_EXTENDED_AUTH_INFO_LENGTH_EXCEEDED = 0xEE6,
			BD_EXTENDED_AUTH_INFO_EXPIRED = 0xEE7,
			BD_WHITELIST_TRAILING_WHITE_SPACE = 0xEE8,
			BD_WHITELIST_MESSAGE_LENGTH_LIMIT_EXCEEDED = 0xEE9,
			BD_BNET_SESSION_TOKEN_ERROR = 0xEEA,
			BD_BNET_SESSION_CLAIMS_ERROR = 0xEEB,
			BD_TRIAL_STATUS_ERROR = 0xEEC,
			BD_TRIAL_STATUS_INVALID_DATA_ERROR = 0xEED,
			BD_TRIAL_STATUS_UNAVAILABLE_ERROR = 0xEEE,
			BD_LOGON_SCHEDULE_ERROR = 0xEF6,
			BD_LOGON_SCHEDULE_INVALID_TIME_RANGE_ERROR = 0xEF7,
			BD_LOGON_SCHEDULE_INVALID_DAY_ERROR = 0xEF8,
			BD_LOGON_SCHEDULE_INVALID_UPDATE_TYPE_ERROR = 0xEF9,
			BD_LOGON_SCHEDULE_UPDATE_FAILED_ERROR = 0xEFA,
			BD_USERGROUP_NAME_ALREADY_EXISTS = 0x1770,
			BD_INVALID_USERGROUP_ID = 0x1771,
			BD_USER_ALREADY_IN_USERGROUP = 0x1772,
			BD_USER_NOT_IN_USERGROUP = 0x1773,
			BD_INVALID_USERGROUP_MEMBER_TYPE = 0x1774,
			BD_TOO_MANY_MEMBERS_REQUESTED = 0x1775,
			BD_USERGROUP_NAME_TOO_SHORT = 0x1776,
			BD_RICH_PRESENCE_DATA_TOO_LARGE = 0x1A90,
			BD_RICH_PRESENCE_TOO_MANY_USERS = 0x1A91,
			BD_RICH_PRESENCE_ERROR = 0x1A92,
			BD_RICH_PRESENCE_FEATURE_DISABLED_ERROR = 0x1A93,
			BD_RICH_PRESENCE_TOO_MANY_UPDATES = 0x1A94,
			BD_RICH_PRESENCE_SUBSCRIPTIONS_ERROR = 0x1A9A,
			BD_RICH_PRESENCE_TOO_MANY_SUBSCRIPTIONS_ERROR = 0x1A9B,
			BD_PRESENCE_DATA_TOO_LARGE = 0x283C,
			BD_PRESENCE_TOO_MANY_USERS = 0x283D,
			BD_USER_LOGGED_IN_OTHER_TITLE = 0x283E,
			BD_USER_NOT_LOGGED_IN = 0x283F,
			BD_PRESENCE_INVALID_PLATFORM = 0x2840,
			BD_SUBSCRIPTION_TOO_MANY_USERS = 0x1B58,
			BD_SUBSCRIPTION_TICKET_PARSE_ERROR = 0x1B59,
			BD_CODO_ID_INVALID_DATA = 0x1BBC,
			BD_INVALID_MESSAGE_FORMAT = 0x1BBD,
			BD_TLOG_TOO_MANY_MESSAGES = 0x1BBE,
			BD_CODO_ID_NOT_IN_WHITELIST = 0x1BBF,
			BD_TLOG_MESSAGE_TRANSFORMATION_ERROR = 0x1BC0,
			BD_REWARDS_NOT_ENABLED = 0x1BC1,
			BD_REWARDS_INVALID_RULE = 0x1BC2,
			BD_REDEEM_NAME_CHANGE_INTERNAL_ERROR = 0x1BC3,
			BD_REDEEM_NAME_CHANGE_INVALID_ITEM = 0x1BC4,
			BD_REDEEM_NAME_CHANGE_UNAVAILABLE_ITEM = 0x1BC5,
			BD_REDEEM_NAME_CHANGE_IN_PROGRESS = 0x1BC6,
			BD_REDEEM_NAME_CHANGE_INACTIVE_REQUEST = 0x1BC7,
			BD_REDEEM_NAME_CHANGE_INVALID_NAME = 0x1BC8,
			BD_REDEEM_NAME_CHANGE_SAME_NAME = 0x1BC9,
			BD_REDEEM_NAME_CHANGE_DUPLICATE_NAME = 0x1BCA,
			BD_TENCENT_NO_APPLICABLE_REWARDS = 0x1BCB,
			BD_TLOG_MESSAGE_DECOMPRESSION_ERROR = 0x1BCC,
			BD_REDEEM_TEAM_NAME_CHANGE_INTERNAL_ERROR = 0x1BCD,
			BD_REDEEM_TEAM_NAME_CHANGE_INVALID_ITEM = 0x1BCE,
			BD_REDEEM_TEAM_NAME_CHANGE_UNAVAILABLE_ITEM = 0x1BCF,
			BD_TENCENT_REWARD_NOT_FOUND = 0x1BD0,
			BD_MARKETPLACE_ERROR = 0x1F40,
			BD_MARKETPLACE_RESOURCE_NOT_FOUND = 0x1F41,
			BD_MARKETPLACE_INVALID_CURRENCY = 0x1F42,
			BD_MARKETPLACE_INVALID_PARAMETER = 0x1F43,
			BD_MARKETPLACE_RESOURCE_CONFLICT = 0x1F44,
			BD_MARKETPLACE_STORAGE_ERROR = 0x1F45,
			BD_MARKETPLACE_INTEGRITY_ERROR = 0x1F46,
			BD_MARKETPLACE_INSUFFICIENT_FUNDS_ERROR = 0x1F47,
			BD_MARKETPLACE_MMP_SERVICE_ERROR = 0x1F48,
			BD_MARKETPLACE_PRECONDITION_REQUIRED = 0x1F49,
			BD_MARKETPLACE_ITEM_MULTIPLE_PURCHASE_ERROR = 0x1F4A,
			BD_MARKETPLACE_MISSING_REQUIRED_ENTITLEMENT = 0x1F4B,
			BD_MARKETPLACE_VALIDATION_ERROR = 0x1F4C,
			BD_MARKETPLACE_TENCENT_PAYMENT_ERROR = 0x1F4D,
			BD_MARKETPLACE_SKU_NOT_COUPON_ENABLED_ERROR = 0x1F4E,
			BD_MARKETPLACE_TRANSACTION_ERROR = 0x1F4F,
			BD_MARKETPLACE_RECEIPT_USED = 0x1F50,
			BD_MARKETPLACE_INVALID_RECEIPT = 0x1F51,
			BD_MARKETPLACE_STEAM_REQUEST_FAILED = 0x1F52,
			BD_MARKETPLACE_STEAM_NOT_APPROVED = 0x1F53,
			BD_MARKETPLACE_PRODUCT_NOT_FOUND_IN_FP = 0x1F54,
			BD_MARKETPLACE_STEAM_ALREADY_FINALIZED = 0x1F54,
			BD_MARKETPLACE_STEAM_BAD_ORDER_ID = 0x1F55,
			BD_MARKETPLACE_STEAM_CURRENCY_ERROR = 0x1F56,
			BD_MARKETPLACE_PSN_INVALID_NP_TITLE_ID = 0x1F65,
			BD_MARKETPLACE_PSN_INVALID_NP_AUTH_CODE = 0x1F57,
			BD_MARKETPLACE_PSN_INVALID_ENTITLEMENT_ID = 0x1F58,
			BD_MARKETPLACE_PSN_ENTITLEMENT_NOT_PRESENT = 0x1F59,
			BD_MARKETPLACE_PSN_INTERNAL_ERROR = 0x1F5A,
			BD_MARKETPLACE_XB1_UNAUTHORISED_ERROR = 0x1F5B,
			BD_MARKETPLACE_XB1_REQUEST_REJECTED_ERROR = 0x1F5C,
			BD_MARKETPLACE_XB1_INSUFFICIENT_QUANTITY_ERROR = 0x1F5D,
			BD_MARKETPLACE_XB1_BAD_DELEGATION_TOKEN_ERROR = 0x1F5E,
			BD_MARKETPLACE_XB1_REQUEST_FAILED_ERROR = 0x1F5F,
			BD_MARKETPLACE_X360_REQUEST_FAILED = 0x1F60,
			BD_MARKETPLACE_X360_ITEM_NOT_PRESENT = 0x1F61,
			BD_MARKETPLACE_X360_CONNECTION_ERROR = 0x1F62,
			BD_MARKETPLACE_USAGE_TIME_ERROR = 0x1F63,
			BD_MARKETPLACE_DURABLE_ALREADY_GRANTED = 0x1F64,
			BD_MARKETPLACE_FIRST_PARTY_DURABLE_EXISTS = 0x1F66,
			BD_MARKETPLACE_ITEM_LIMIT_REACHED = 0x1F67,
			BD_MARKETPLACE_OVER_ITEM_MAX_QUANTITY_ERROR = 0x1F69,
			BD_MARKETPLACE_INSUFFICIENT_ITEM_QUANTITY = 0x1F6A,
			BD_MARKETPLACE_ENTITY_NOT_ENABLED = 0x1F6B,
			BD_MARKETPLACE_MISCONFIGURED = 0x1F6C,
			BD_MARKETPLACE_COUPON_NOT_CLAIMED_OR_FOUND = 0x1F6D,
			BD_MARKETPLACE_INVALID_DISCOUNT = 0x1F6E,
			BD_MARKETPLACE_INVALID_STORE_VERSION = 0x1F6F,
			BD_MARKETPLACE_SKU_SOLD_OUT = 0x1F72,
			BD_MARKETPLACE_WIIU_AUTH_FAILED = 0x1F73,
			BD_MARKETPLACE_WIIU_INVALID_PARAMETER = 0x1F74,
			BD_MARKETPLACE_WIIU_TAX_LOCATION_NOT_SPECIFIED = 0x1F75,
			BD_MARKETPLACE_WIIU_ACCOUNT_ERROR = 0x1F76,
			BD_MARKETPLACE_WIIU_PURCHASING_ERROR = 0x1F77,
			BD_MARKETPLACE_WIIU_BALANCE_ERROR = 0x1F78,
			BD_MARKETPLACE_WIIU_SERVER_ERROR = 0x1F79,
			BD_MARKETPLACE_WIIU_REQUEST_FAILED = 0x1F7A,
			BD_MARKETPLACE_WIIU_SERVER_MAINTENANCE = 0x1F7B,
			BD_MARKETPLACE_WIIU_SERVICE_TERMINATED = 0x1F7C,
			BD_MARKETPLACE_WIIU_ITEM_NOT_PRESENT = 0x1F7D,
			BD_MARKETPLACE_WIIU_TAX_LOCATION_INVALID = 0x1F7E,
			BD_MARKETPLACE_XB1_DURABLE_NOT_PRESENT = 0x1F7F,
			BD_MARKETPLACE_EXPECTED_PRICE_MISMATCH = 0x1F80,
			BD_MARKETPLACE_ITEM_NOT_CONSUMABLE = 0x1F81,
			BD_MARKETPLACE_IDEMPOTENT_REQUEST_COLLISION = 0x1F82,
			BD_MARKETPLACE_COUPON_NOT_STARTED = 0x1F83,
			BD_MARKETPLACE_MULTIPLE_OPERATIONS_ERROR = 0x1F84,
			BD_MARKETPLACE_MISSING_PAYMENT_PROVIDER_CURRENCY_ERROR = 0x1F85,
			BD_MARKETPLACE_WIIU_LANGUAGE_NOT_SUPPORTED = 0x1F86,
			BD_MARKETPLACE_PAWN_CHOICE_EXPECTED = 0x1F87,
			BD_MARKETPLACE_PAWN_CHOICE_UNSUPPORTED = 0x1F88,
			BD_MARKETPLACE_INVALID_REWARD_ERROR = 0x1F89,
			BD_MARKETPLACE_MISSING_REQUIRED_ITEMS_ERROR = 0x1F8A,
			BD_MARKETPLACE_MISSING_REQUIRED_CURRENCY_BALANCES_ERROR = 0x1F8B,
			BD_MARKETPLACE_MISSING_REQUIRED_ENTITLEMENTS_ERROR = 0x1F8C,
			BD_MARKETPLACE_UNREACHABLE_ERROR = 0x1F8D,
			BD_MARKETPLACE_ITEM_ALREADY_PERMANENTLY_OWNED = 0x1F97,
			BD_MARKETPLACE_EXCEEDED_ITEM_MAX_USAGE_TIME = 0x1F98,
			BD_MARKETPLACE_BNET_REQUEST_FAILED = 0x1F99,
			BD_MARKETPLACE_WEGAME_SERVICE_ERROR = 0x1F9A,
			BD_MARKETPLACE_WEGAME_REQUEST_FAILED = 0x1F9B,
			BD_MARKETPLACE_SWITCH_SERVICE_ERROR = 0x1F9C,
			BD_MARKETPLACE_SWITCH_REQUEST_FAILED = 0x1F9D,
			BD_COMMS_SERVICE_NOT_AVAILABLE_ERROR = 0x28A0,
			BD_COMMS_CLIENT_ERROR = 0x28A1,
			BD_LEAGUE_INVALID_TEAM_SIZE = 0x1FA4,
			BD_LEAGUE_INVALID_TEAM = 0x1FA5,
			BD_LEAGUE_INVALID_SUBDIVISION = 0x1FA6,
			BD_LEAGUE_INVALID_LEAGUE = 0x1FA7,
			BD_LEAGUE_TOO_MANY_RESULTS_REQUESTED = 0x1FA8,
			BD_LEAGUE_METADATA_TOO_LARGE = 0x1FA9,
			BD_LEAGUE_TEAM_ICON_TOO_LARGE = 0x1FAA,
			BD_LEAGUE_TEAM_NAME_TOO_LONG = 0x1FAB,
			BD_LEAGUE_ARRAY_SIZE_MISMATCH = 0x1FAC,
			BD_LEAGUE_SUBDIVISION_MISMATCH = 0x2008,
			BD_LEAGUE_INVALID_WRITE_TYPE = 0x2009,
			BD_LEAGUE_INVALID_STATS_DATA = 0x200A,
			BD_LEAGUE_SUBDIVISION_UNRANKED = 0x200B,
			BD_LEAGUE_CROSS_TEAM_STATS_WRITE_PREVENTED = 0x200C,
			BD_LEAGUE_INVALID_STATS_SEASON = 0x200D,
			BD_CONNECTION_COUNTER_ERROR = 0x20D0,
			BD_LINKED_ACCOUNTS_INVALID_CONTEXT = 0x2198,
			BD_LINKED_ACCOUNTS_INVALID_PLATFORM = 0x2199,
			BD_LINKED_ACCOUNTS_LINKED_ACCOUNTS_FETCH_ERROR = 0x219A,
			BD_LINKED_ACCOUNTS_INVALID_ACCOUNT = 0x219B,
			BD_LINKED_ACCOUNTS_INVALID_TOKEN = 0x219C,
			BD_QUEUED_MATCHING_ERROR = 0x2260,
			BD_QUEUED_MATCHING_NOT_FOUND = 0x2261,
			BD_QUEUED_MATCHING_WRONG_QUEUE_TYPE = 0x2262,
			BD_QUEUED_MATCHING_INVALID_QUEUE_ID = 0x2263,
			BD_LOGIN_QUEUE_NO_ERROR = 0x22C4,
			BD_LOGIN_QUEUE_ENVIRONMENT_ERROR = 0x22C5,
			BD_LOGIN_QUEUE_UNKNOWN_ERROR = 0x22C6,
			BD_LOGIN_QUEUE_BAD_REQUEST = 0x22C7,
			BD_LOGIN_QUEUE_SERVER_UNAVAILABLE_ERROR = 0x22C8,
			BD_LOGIN_QUEUE_SSL_CERTIFICATE_ERROR = 0x22C9,
			BD_LOGIN_QUEUE_DNS_RESOLUTION_ERROR = 0x22CA,
			BD_LOGIN_QUEUE_JSON_FORMAT_ERROR = 0x22CB,
			BD_LOGIN_QUEUE_TICKET_PARSE_ERROR = 0x22CC,
			BD_LOGIN_QUEUE_INVALID_TITLE_ID = 0x22CD,
			BD_LOGIN_QUEUE_INTERNAL_ERROR = 0x22CE,
			BD_LOGIN_QUEUE_CLOSED_QUEUE = 0x22CF,
			BD_LOGIN_QUEUE_INVALID_QUEUE_ID = 0x22D0,
			BD_LOGIN_QUEUE_NOT_FOUND = 0x22D1,
			BD_LOGIN_QUEUE_REPLY_CONTENT_ERROR = 0x22D2,
			BD_LOGIN_QUEUE_TOO_MANY_REQUESTS = 0x22D3,
			BD_VOTING_MAX_VALUE_EXCEEDED = 0x2328,
			BD_VOTING_INVALID_GROUP_NAME = 0x2329,
			BD_VOTING_IN_PROGRESS = 0x232A,
			BD_VOTING_NON_EXISTENT_GROUP = 0x232B,
			BD_USER_GENERATED_STATS_ERROR = 0x238C,
			BD_INVALID_ACCESS_TO_USER_GENERATED_LB_ERROR = 0x238D,
			BD_INVALID_ACCESS_TO_NON_USER_GENERATED_LB_ERROR = 0x238E,
			BD_SUB_LEADERBOARD_ID_MISMATCH_ERROR = 0x238D,
			BD_PUBLISHER_VARIABLES_SERVICE_ERROR = 0x251C,
			BD_PUBLISHER_VARIABLES_NOT_FOUND = 0x251D,
			BD_PUBLISHER_VARIABLES_INVALID_NAMESPACE = 0x251E,
			BD_PUBLISHER_VARIABLES_INVALID_GROUP_ID = 0x251F,
			BD_GMSG_INVALID_CATEGORY_ID = 0x27D8,
			BD_GMSG_CATEGORY_MEMBERSHIPS_LIMIT = 0x27D9,
			BD_GMSG_NONMEMBER_POST_DISALLOWED = 0x27DA,
			BD_GMSG_CATEGORY_DISALLOWS_CLIENT_TYPE = 0x27DB,
			BD_GMSG_PAYLOAD_TOO_BIG = 0x27DC,
			BD_GMSG_MEMBER_POST_DISALLOWED = 0x27DD,
			BD_GMSG_OVERLOADED = 0x27DE,
			BD_GMSG_USER_PERCATEGORY_POST_RATE_EXCEEDED = 0x27DF,
			BD_GMSG_USER_GLOBAL_POST_RATE_EXCEEDED = 0x27E0,
			BD_GMSG_GROUP_POST_RATE_EXCEEDED = 0x27E1,
			BD_GMSG_GROUP_LIMIT_REACHED = 0x27E2,
			BD_GMSG_GROUP_MEMBERSHIP_LIMIT_REACHED = 0x27E3,
			BD_SERVER_INVENTORY_SERVER_ERROR = 0x2904,
			BD_SERVER_INVENTORY_SERVER_ALLOCATED = 0x2905,
			BD_SERVER_INVENTORY_ENTRY_DOES_NOT_EXIST = 0x2906,
			BD_SERVER_INVENTORY_DATA_LAYER_ERROR = 0x2907,
			BD_SERVER_INVENTORY_NOT_ENABLED = 0x2908,
			BD_SERVER_INVENTORY_NO_SERVER_ALLOCATED = 0x2909,
			BD_SERVER_INVENTORY_MAX_REGISTRATION_LENGTH_EXCEEDED = 0x290A,
			BD_SERVER_INVENTORY_REGISTRATION_INVALID_CHARACTERS = 0x290B,
			BD_SERVER_INVENTORY_INVALID_PARAMETER = 0x290D,
			BD_CODO_CLAN_NO_TEMP_RESERVATION = 0x2AF8,
			BD_CODO_CLAN_TOO_MANY_MEMBERSHIPS = 0x2AF9,
			BD_CODO_CLAN_UNEXPECTED_FOUNDATION_STATE = 0x2AFA,
			BD_CODO_CLAN_UNABLE_TO_FIND_NAME = 0x2AFB,
			BD_CODO_CLAN_INTERNAL_MARKETPLACE_ERROR = 0x2AFC,
			BD_CODO_CLAN_INSUFFICIENT_FUNDS = 0x2AFD,
			BD_CODO_CLAN_UNSATISFIED_PRECONDITION = 0x2AFE,
			BD_CODO_CLAN_NO_CLAN = 0x2AFF,
			BD_CODO_CLAN_NO_CHANNEL = 0x2B00,
			BD_CODO_CLAN_NOT_A_CLAN_MEMBER = 0x2B01,
			BD_CODO_CLAN_DISBAND_TOO_MANY_MEMBERS = 0x2B04,
			BD_CODO_CLAN_DISBAND_TOO_SOON = 0x2B05,
			BD_NO_SUCH_ROLE = 0x2EE0,
			BD_NO_SUCH_PERMISSION = 0x2EE1,
			BD_BAD_ROLE_SPECIFICATION = 0x2EE2,
			BD_BAD_PERMISSION_SPECIFICATION = 0x2EE3,
			BD_REDEEMABLE_CODE_REGISTRY_SERVICE_GENERIC_ERROR = 0x300D,
			BD_REDEEMABLE_CODE_MARKETPLACE_SERVICE_GENERIC_ERROR = 0x300E,
			BAD_PAYLOAD_ERROR = 0x300F,
			INVALID_PAYLOAD_SIGNATURE_ERROR = 0x3010,
			GENERATE_PAYLOAD_SIGNATURE_ERROR = 0x3011,
			BD_REDEEMABLE_CODE_EXPIRED_CLIENT_TRANSACTION_ID = 0x3012,
			BD_REDEEMABLE_CODE_NOT_FOUND = 0x3071,
			BD_REDEEMABLE_CODE_USE_LIMIT_EXCEEDED = 0x3072,
			BD_REDEEMABLE_CODE_ALREADY_USED_BY_USER = 0x3073,
			BD_REDEEMABLE_CODE_EXPIRED = 0x3074,
			INVALID_CLIENT_TRANSACTION_ID = 0x3075,
			BD_ACHIEVEMENTS_ENGINE_CLIENT_ERROR = 0x30D5,
			BD_ACHIEVEMENTS_ENGINE_SERVER_ERROR = 0x30D6,
			BD_ACHIEVEMENTS_ENGINE_INSUFFICIENT_BALANCE_ERROR = 0x30D7,
			BD_ACHIEVEMENTS_ENGINE_ACTIVATION_NOT_SCHEDULED_ERROR = 0x30D8,
			BD_ACHIEVEMENTS_ENGINE_ACTIVE_LIMIT_EXCEEDED = 0x30D9,
			BD_ACHIEVEMENTS_ENGINE_DUPLICATE_REQUEST_ID = 0x30DA,
			BD_ACHIEVEMENTS_ENGINE_MULTI_STATUS = 0x30DB,
			BD_MW4_BACKEND_SERVICE_NOT_AVAILABLE = 0x4CF5,
			BD_MW4_BACKEND_RESOURCE_NOT_FOUND = 0x4CF6,
			BD_MW4_BACKEND_BAD_REQUEST = 0x4CF7,
			BD_CLANS_PROPOSAL_DOES_NOT_EXIST = 0x4D08,
			BD_CLANS_GROUP_NAME_NOT_UNIQUE = 0x4D09,
			BD_CLANS_MAX_GROUP_MEMBERSHIPS_REACHED = 0x4D0A,
			BD_CLANS_MAX_GROUP_OWNERSHIPS_REACHED = 0x4D0B,
			BD_CLANS_GROUP_DOES_NOT_EXIST = 0x4D0C,
			BD_CLANS_GROUP_PERMISSION_DENIED = 0x4D0D,
			BD_CLANS_VULGAR_GROUP_NAME = 0x4D0E,
			BD_CLANS_GROUP_NAME_TOO_SHORT = 0x4D0F,
			BD_CLANS_GROUP_ATTACHMENT_LIMIT_EXCEEDED = 0x4D10,
			BD_CLANS_GROUP_FULL = 0x4D11,
			BD_CLANS_MAX_OUTGOING_PROPOSALS_REACHED = 0x4D12,
			BD_CLANS_MEMBER_BAN_EXISTS = 0x4D13,
			BD_CLANS_GROUP_KIND_NOT_CONFIGURED = 0x4D14,
			BD_CLANS_INVALID_ROOT_KIND = 0x4D15,
			BD_CLANS_GROUP_FILE_DOES_NOT_EXIST = 0x4D16,
			BD_CLANS_GROUP_FILE_COLLISION_FIELD_MISMATCH = 0x4D17,
			BD_CLANS_BATCH_REQUEST_LIMIT_EXCEEDED = 0x4D18,
			BD_CLANS_INVALID_PAGE_TOKEN = 0x4D19,
			BD_CLANS_INVALID_GROUP_FILTER = 0x4D1A,
			BD_CLANS_GROUP_TAG_NOT_UNIQUE = 0x4D1B,
			BD_CLANS_GROUP_TAG_TOO_SHORT = 0x4D1C,
			BD_CLANS_VULGAR_GROUP_TAG = 0x4D1D,
			BD_REWARD_NO_REWARD_TOKEN_FOUND = 0x364C,
			BD_REWARD_INVALID_ACHIEVEMENT_IDS_SPECIFIED = 0x364D,
			BD_REWARD_CLIENT_ACHIEVEMENTS_NOT_ENABLED = 0x364E,
			BD_REWARD_EVENTS_ERROR = 0x364F,
			BD_REWARD_EVENTS_NOT_ENABLED = 0x3650,
			BD_REWARD_EVENTS_RULES_ERROR = 0x3651,
			BD_REWARD_EVENTS_DATA_ERROR = 0x3652,
			BD_REWARD_EVENTS_TRANSACTION_ERROR = 0x3653,
			BD_REWARD_CONFIGURATION_ERROR = 0x3654,
			BD_REWARD_TOO_MANY_ACTIVE_CHALLENGES = 0x3655,
			BD_REWARD_EVENTS_TRANSACTION_EXPIRED = 0x3656,
			BD_REWARD_CHALLENGE_NOT_SCHEDULED = 0x3657,
			BD_REWARD_CHALLENGE_ALREADY_COMPLETED = 0x3658,
			BD_REWARD_CODO_TOO_MANY_REROLLS = 0x367E,
			BD_REWARD_CODO_BAD_REROLL_PARAMS = 0x367F,
			BD_UMBRELLA_PROVIDER_SERVER_ERROR = 0x36B0,
			BD_UMBRELLA_PROVIDER_UNSUPPORTED_OPERATION = 0x36B1,
			BD_UMBRELLA_ERROR = 0x36B2,
			BD_UMBRELLA_INVALID_TITLE_ID = 0x36B3,
			BD_UMBRELLA_INVALID_QUERY_STRING = 0x36B4,
			BD_UMBRELLA_INVALID_DATA = 0x36B5,
			BD_UMBRELLA_INVALID_CLIENT = 0x36B6,
			BD_UMBRELLA_PROVIDER_NOT_SUPPORTED = 0x36B7,
			BD_UMBRELLA_UNAUTHORIZED_ACCESS = 0x36B8,
			BD_UMBRELLA_INVALID_TOKEN = 0x36B8,
			BD_UMBRELLA_EXPIRED_PROVIDER_REFRESH_TOKEN = 0x36B9,
			BD_UMBRELLA_NO_VALID_PROVIDER_TOKENS = 0x36BA,
			BD_UMBRELLA_INVALID_LSG_TICKET = 0x36BB,
			BD_UMBRELLA_TOKEN_NOT_FOUND = 0x36BC,
			BD_UMBRELLA_USER_NOT_FOUND = 0x36BD,
			BD_UMBRELLA_ACCOUNT_NOT_FOUND = 0x36BE,
			BD_UMBRELLA_MERGE_CONFLICT = 0x36BF,
			BD_UMBRELLA_PROVIDER_INACCESSIBLE = 0x36C0,
			BD_UMBRELLA_MISSING_FIELD = 0x36C1,
			BD_UMBRELLA_FIELD_INVALID = 0x36C2,
			BD_UMBRELLA_PLAYER_BAN = 0x36C3,
			BD_UMBRELLA_EXPIRED_TOKEN = 0x3719,
			BD_UNO_ERROR = 0x3714,
			BD_UNO_INVALID_DATA = 0x3715,
			BD_UNO_INVALID_DATE_OF_BIRTH = 0x3716,
			BD_UNO_UNAUTHORIZED_ACCESS = 0x3717,
			BD_UNO_INVALID_TOKEN = 0x3718,
			BD_UNO_EXPIRED_TOKEN = 0x3719,
			BD_UNO_EMAIL_NOT_VERIFIED = 0x371A,
			BD_UNO_TOS_VERSION_NOT_FOUND = 0x371B,
			BD_UNO_TOS_CONTENT_NOT_FOUND = 0x371C,
			BD_UNO_TOS_ALREADY_ACCEPTED = 0x371D,
			BD_UNO_MISSING_FIELD = 0x371E,
			BD_UNO_DNS_RESOLUTION_FAILED = 0x371F,
			BD_UNO_FIELD_INVALID = 0x3720,
			BD_UNO_INVALID_USERNAME = 0x3721,
			BD_UNO_INVALID_PASSWORD = 0x3722,
			BD_UNO_EMAIL_ALREADY_EXISTS = 0x3723,
			BD_UNO_DESERIALIZATION_FAILURE = 0x3724,
			BD_UMBRELLA_DESERIALIZATION_FAILURE = 0x3725,
			BD_UNO_NO_RENAME_TOKENS = 0x3726,
			BD_UNO_MARKETPLACE_ERROR = 0x3727,
			BD_SI_ERROR = 0x3777,
			BD_MATCH_SCHEDULER_ERROR = 0x3A98,
			BD_MATCH_SCHEDULER_SCHEDULED_MATCH_DOES_NOT_EXIST = 0x3A99,
			BD_MATCH_SCHEDULER_MAXIMUM_EVENTS_PER_MATCH_REACHED = 0x3A9A,
			BD_MLG_ERROR = 0x3AFC,
			BD_MLG_REMOTE_ERROR = 0x3AFD,
			BD_CODO_AUTO_JOIN_LEVEL_NOT_MET = 0x4A9D,
			BD_CODO_NOT_PERMITTED_TO_UPDATE_PROFILE_FIELD = 0x4A9E,
			BD_CODO_INVALID_PROFILE_VALUE = 0x4A9F,
			BD_CODO_PROFILE_COLUMN_DOES_NOT_EXIST = 0x4AA0,
			BD_CODO_LOUDSPEAKER_INVALID_TOKEN = 0x4AA1,
			BD_CODO_LOUDSPEAKER_INVALID_CATEGORY = 0x4AA2,
			BD_CODO_RESETTABLE_STATS_NOT_CONFIGURED = 0x4AA3,
			BD_CODO_REDEEM_INCREASE_TEAM_MEMBER_LIMIT_INVALID_ITEM = 0x4AAB,
			BD_CODO_REDEEM_INCREASE_TEAM_MEMBER_LIMIT_INTERNAL_ERROR = 0x4AAC,
			BD_CODO_REDEEM_INCREASE_TEAM_MEMBER_LIMIT_UNAVAILABLE_ITEM = 0x4AAD,
			BD_PLAYER_VOTE_REJECTED = 0x5EED,
			BD_AMM_NOT_LOBBY_HOST = 0x38A4,
			BD_AMM_NOT_PARTY_HOST = 0x38A5,
			BD_AMM_NOT_LOBBY_MEMBER = 0x38A6,
			BD_AMM_INVALID_MATCHMAKING_ID = 0x38A7,
			BD_AMM_INVALID_LOBBY_ID = 0x38A8,
			BD_AMM_SEARCH_IN_PROGRESS = 0x38A9,
			BD_AMM_USER_ALREADY_MATCHMAKING = 0x38AA,
			BD_AMM_INVALID_TOKEN = 0x38AB,
			BD_AMM_INVALID_DOCUMENT_FORMAT = 0x38AC,
			BD_AMM_PLAYER_INFO_UNAVAILABLE = 0x38AD,
			BD_AMM_REQUEST_DESERIALIZATION_FAILED = 0x38AE,
			BD_AMM_INVALID_QOS_TRANSACTION_ID = 0x38AF,
			BD_AMM_INVALID_USER_ID_IN_GUEST_SLOTS = 0x38B0,
			BD_AMM_NO_BUILD_NAME_SET = 0x38B1,
			BD_AMM_LOBBY_MERGED = 0x38B2,
			BD_AMM_BACKOFF_REQUESTED = 0x38B3,
			BD_AMM_PLAYER_INFO_INCOMPATIBLE_BUILDS = 0x38B4,
			BD_AMM_INVALID_DC_QOS_ADDRESS = 0x38B5,
			BD_AMM_INVALID_PLAYER_INFO_DC_QOS_SETTINGS = 0x38B6,
			BD_AMM_INVALID_PLAYER_INFO_LISTEN_SERVER_SETTINGS = 0x38B7,
			BD_AMM_MIGRATION_NOT_PERMITTED = 0x38B8,
			BD_AMM_INVALID_LOBBY_MEMBER_STATUS_TRANSITION = 0x38B9,
			BD_AMM_LOBBY_MEMBER_STATUS_REPORTED_FOR_UNKNOWN_PLAYER = 0x38BA,
			BD_AMM_DEDI_SHUTDOWN_NOT_PERMITTED = 0x38BB,
			BD_AMM_INVALID_USER_ID_IN_PLAYER_RULESET_PAYLOADS = 0x38BC,
			BD_AMM_INVALID_MAP_PACK = 0x38BD,
			BD_AMM_TOURNAMENT_PLAYER_NOT_IN_TOURNAMENT = 0x38BE,
			BD_AMM_TOURNAMENT_DUPLICATE_ACK_JOIN = 0x38BF,
			BD_AMM_TOURNAMENT_INVALID_TOURNAMENT_STATUS = 0x38C0,
			BD_AMM_TOURNAMENT_INVALID_TOURNAMENT_ID = 0x38C1,
			BD_AMM_TOURNAMENT_LOBBY_NOT_IN_ROUND = 0x38C2,
			BD_AMM_TOURNAMENT_TEAM_NOT_IN_MATCH = 0x38C3,
			BD_AMM_TOURNAMENT_PLAYER_NOT_IN_ROUND = 0x38C4,
			BD_AMM_TOURNAMENT_INVALID_LOBBY_DOC = 0x38C5,
			BD_AMM_TOURNAMENT_ELIMINATED_PLAYER = 0x38C6,
			BD_AMM_TOURNAMENT_LOBBY_ID_NOT_IN_TOURNAMENT = 0x38C7,
			BD_AMM_TOURNAMENT_MATCH_ALREADY_HAS_RESULT = 0x38C8,
			BD_AMM_TOURNAMENT_MATCH_IN_PROGRESS = 0x38C9,
			BD_AMM_TOURNAMENT_FULL = 0x38CA,
			BD_CROSS_PLATFORM_FRIENDS_UNKNOWN_ERROR = 0x3E80,
			BD_CROSS_PLATFORM_FRIENDS_SELF_FRIENDSHIP_NOT_ALLOWED = 0x3E81,
			BD_CROSS_PLATFORM_FRIENDS_CALLER_FRIENDS_LIST_FULL = 0x3E82,
			BD_CROSS_PLATFORM_FRIENDS_OTHER_PLAYER_FRIENDS_LIST_FULL = 0x3E83,
			BD_CROSS_PLATFORM_FRIENDS_CALLER_OUTGOING_FRIEND_REQUESTS_FULL = 0x3E84,
			BD_CROSS_PLATFORM_FRIENDS_OTHER_PLAYER_INCOMING_FRIEND_REQUESTS_FULL = 0x3E85,
			BD_CROSS_PLATFORM_FRIENDS_INVALID_PAGE_TOKEN = 0x3E86,
			BD_CROSS_PLATFORM_FRIENDS_FRIENDSHIP_ALREADY_EXISTS = 0x3E87,
			BD_CROSS_PLATFORM_FRIENDS_INVITE_ALREADY_EXISTS = 0x3E88,
			BD_CROSS_PLATFORM_FRIENDS_BACKEND_UNAVAILABLE_ERROR = 0x3E89,
			BD_CROSS_PLATFORM_FRIENDS_INVITED_USER_IS_BLOCKED = 0x3E8A,
			BD_CROSS_PLATFORM_FRIENDS_CALLER_IS_BLOCKED = 0x3E8B,
			BD_CROSS_PLATFORM_FRIENDS_BLOCKED_USER_LIST_FULL = 0x3E8C,
			BD_CROSS_PLATFORM_FRIENDS_USER_IS_ALREADY_BLOCKED = 0x3E8D,
			BD_CROSS_PLATFORM_USERLISTS_UNKNOWN_ERROR = 0x3F48,
			BD_CROSS_PLATFORM_USERLISTS_LIST_DOES_NOT_EXIST = 0x3F49,
			BD_CROSS_PLATFORM_USERLISTS_LIST_ALREADY_EXISTS = 0x3F4A,
			BD_CROSS_PLATFORM_USERLISTS_INVALID_PAGE_TOKEN = 0x3F4B,
			BD_CROSS_PLATFORM_PRESENCE_UNKNOWN_ERROR = 0x4010,
			BD_CROSS_PLATFORM_PRESENCE_INVALID_PAGE_TOKEN = 0x4011,
			BD_CROSS_PLATFORM_PRESENCE_SUBSCRIPTIONS_LIST_FULL = 0x4012,
			BD_CROSS_PLATFORM_PRESENCE_SUBSCRIBERS_LIST_FULL = 0x4013,
			BD_CROSS_PLATFORM_PRESENCE_INVALID_MAXIMUM_PAGE_SIZE = 0x4014,
			BD_CROSS_PLATFORM_PRESENCE_EXCEEDED_MAX_USERS_IN_REQUEST = 0x4015,
			BD_CROSS_TITLE_LOCALIZED_STRINGS_UNKNOWN_ERROR = 0x4074,
			BD_CROSS_TITLE_LOCALIZED_STRINGS_STRING_SET_DOES_NOT_EXIST_ERROR = 0x4075,
			BD_CROSS_TITLE_LOCALIZED_STRINGS_INVALID_FORMAT_STRING_ERROR = 0x4076,
			BD_CROSS_TITLE_LOCALIZED_STRINGS_INVALID_STRING_REF_ERROR = 0x4077,
			BD_CROSS_TITLE_LOCALIZED_STRINGS_NOT_ENOUGH_STRING_REFS_ERROR = 0x4078,
			BD_OBJECTSTORE_PROXY_OBJECT_NOT_FOUND = 0x4E20,
			BD_OBJECTSTORE_PROXY_INVALID_ACCESS = 0x4E21,
			BD_OBJECTSTORE_PROXY_SERVICE_UNAVAILABLE = 0x4E22,
			BD_OBJECTSTORE_PROXY_OBJECT_TOO_BIG_FOR_REMAINING_SIZE_WINDOW_SPACE_ERROR = 0x4E23,
			BD_OBJECTSTORE_PROXY_OBJECT_TOO_BIG_FOR_SIZE_WINDOW_ERROR = 0x4E24,
			BD_OBJECTSTORE_HTTP_ERROR = 0x4E84,
			BD_OBJECTSTORE_FAILED_TO_START_HTTP = 0x4E85,
			BD_AB_TESTING_OBJECT_NOT_FOUND = 0x4BC8,
			BD_AB_TESTING_INVALID_ACCESS = 0x4BC9,
			BD_AB_TESTING_SERVICE_UNAVAILABLE = 0x4BCA,
			BD_REST_ERROR_TEMPORARY_REDIRECT = 0x639C,
			BD_REST_ERROR_PERMANENT_REDIRECT = 0x639D,
			BD_REST_ERROR_NOT_MODIFIED = 0x639E,
			BD_REST_ERROR_BAD_REQUEST = 0x639F,
			BD_REST_ERROR_UNAUTHORIZED = 0x63A0,
			BD_REST_ERROR_FORBIDDEN = 0x63A1,
			BD_REST_ERROR_NOT_FOUND = 0x63A2,
			BD_REST_ERROR_METHOD_NOT_ALLOWED = 0x63A3,
			BD_REST_ERROR_NOT_ACCEPTABLE = 0x63A4,
			BD_REST_ERROR_REQUEST_TIMEOUT = 0x63A5,
			BD_REST_ERROR_CONFLICT = 0x63A6,
			BD_REST_ERROR_PRECONDITION_FAILED = 0x63A7,
			BD_REST_ERROR_PAYLOAD_TOO_LARGE = 0x63A8,
			BD_REST_ERROR_REQUEST_URI_TOO_LONG = 0x63A9,
			BD_REST_ERROR_REQUESTED_RANGE_NOT_SATISFIABLE = 0x63AA,
			BD_REST_ERROR_INTERNAL_SERVER_ERROR = 0x63AB,
			BD_REST_ERROR_BAD_GATEWAY = 0x63AC,
			BD_REST_ERROR_SERVICE_UNAVAILABLE = 0x63AD,
			BD_REST_ERROR_GATEWAY_TIMEOUT = 0x63AE,
			BD_REST_ERROR_REQUEST_CANCELLED = 0x63AF,
			BD_LOGIN_UNKOWN_ERROR = 0x7724,
			BD_MAX_ERROR_CODE = 0x7725,
		};

		enum bdNATType : uint8_t
		{
			BD_NAT_UNKNOWN = 0x0,
			BD_NAT_OPEN = 0x1,
			BD_NAT_MODERATE = 0x2,
			BD_NAT_STRICT = 0x3,
		};

#pragma pack(push, 1)
		struct bdAuthTicket
		{
			unsigned int m_magicNumber;
			char m_type;
			unsigned int m_titleID;
			unsigned int m_timeIssued;
			unsigned int m_timeExpires;
			unsigned long long m_licenseID;
			unsigned long long m_userID;
			char m_username[64];
			char m_sessionKey[24];
			char m_usingHashMagicNumber[3];
			char m_hash[4];
		};
#pragma pack(pop)
	}
	using namespace demonware;
}
