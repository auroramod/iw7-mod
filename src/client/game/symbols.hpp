#pragma once

#define WEAK __declspec(selectany)

namespace game
{
	/***************************************************************
	 * Functions
	 **************************************************************/

	WEAK symbol<void(int localClientNum, const char* text)> Cbuf_AddText{ 0xB7C290 };
}
