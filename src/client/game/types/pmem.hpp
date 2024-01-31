#pragma once

namespace game::pmem
{
	enum PMem_Stack : __int32
	{
		PMEM_STACK_GAME = 0x0,
		PMEM_STACK_RENDER_TARGETS = 0x1,
		PMEM_STACK_MEM_VIRTUAL = 0x2,
		PMEM_STACK_MEMCARD_LARGE_BUFFER = 0x3,
		PMEM_STACK_SOUND = 0x4,
		PMEM_STACK_STASHED_MEMORY = 0x5,
		PMEM_STACK_CINEMATIC = 0x6,
		PMEM_STACK_COUNT = 0x7,
	};

	enum PMem_Source
	{
		PMEM_SOURCE_EXTERNAL = 0x0,
		PMEM_SOURCE_SCRIPT = 0x1,
	};

	enum PMem_Direction : __int32
	{
		PHYS_ALLOC_LOW = 0x0,
		PHYS_ALLOC_HIGH = 0x1,
		PHYS_ALLOC_COUNT = 0x2,
	};

	enum Mem_PageID
	{
	};

	struct Mem_PageRange
	{
		Mem_PageID firstPageID;
		Mem_PageID lastPageID;
	};

	struct PhysicalMemoryAllocation
	{
		const char* name;
		char* prev_buffer;
		char* next_buffer;
		unsigned __int64 pos;
		Mem_PageRange pageRange;
	};

	struct PhysicalMemoryPrim
	{
		const char* name;
		unsigned int allocListCount;
		char __pad0[4];
		unsigned __int8* buf;
		unsigned __int64 unk_pos;
		int unk1;
		char __pad2[4];
		unsigned __int64 pos;
		PhysicalMemoryAllocation allocList[32];
	};

	struct PhysicalMemory
	{
		PhysicalMemoryPrim prim[2];
	};
}