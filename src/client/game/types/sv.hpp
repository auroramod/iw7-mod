#pragma once

namespace game::sv
{
	struct SvServerInitSettings
	{
		char mapName[64];
		char gameType[64];
		char serverHostName[64];
		bool hardcoreMode;
		unsigned int maxClientCount;
		unsigned int maxAgentCount;
		bool isMapPreloaded;
		bool isSaveGame;
		bool isRestart;
		bool isFrontEnd;
		char __pad0[2];
		bool serverThreadStartup;
	}; //static_assert(sizeof(SvServerInitSettings) == 212);
	static_assert(offsetof(SvServerInitSettings, maxClientCount) == 196);
	static_assert(offsetof(SvServerInitSettings, isMapPreloaded) == 204);
	static_assert(offsetof(SvServerInitSettings, isFrontEnd) == 207);
	static_assert(offsetof(SvServerInitSettings, serverThreadStartup) == 210);
}