#pragma once

namespace game::ddl
{
	enum DDLType
	{
		DDL_INVALID_TYPE = 0xFFFFFFFF,
		DDL_BYTE_TYPE = 0x0,
		DDL_SHORT_TYPE = 0x1,
		DDL_UINT_TYPE = 0x2,
		DDL_INT_TYPE = 0x3,
		DDL_UINT64_TYPE = 0x4,
		DDL_FLOAT_TYPE = 0x5,
		DDL_FIXEDPOINT_TYPE = 0x6,
		DDL_STRING_TYPE = 0x7,
		DDL_STRUCT_TYPE = 0x8,
		DDL_ENUM_TYPE = 0x9,
	};

	union DDLValue
	{
		int intValue;
		unsigned int uintValue;
		unsigned __int64 uint64Value;
		float floatValue;
		float fixedPointValue;
		const char* stringPtr;
	};

	struct DDLMember
	{
		const char* name;
		int index;
		int bitSize;
		int limitSize;
		int offset;
		int type;
		int externalIndex;
		unsigned int rangeLimit;
		bool isArray;
		int arraySize;
		int enumIndex;
	};

	struct DDLState
	{
		bool isValid;
		int offset;
		int arrayIndex;
		DDLMember* member;
		//const DDLDef* ddlDef;
	};

	struct DDLContext
	{

	};
}