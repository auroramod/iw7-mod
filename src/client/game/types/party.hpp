#pragma once

namespace game::party
{
	enum PartyPreloadMapStage : std::uint32_t
	{
		PRELOAD_MAP_IDLE = 0x0,
		PRELOAD_MAP_INITIATED = 0x1,
		PRELOAD_MAP_STARTED = 0x2,
		PRELOAD_MAP_COUNT = 0x3,
	};

	struct PartyData
	{
		char __pad0[11444];
		PartyPreloadMapStage preloadingMapStage;
		char __pad1[101];
		bool m_gameStartSkipCountdown;
		char __pad2[110];
		int lobbyFlags;
		bool gameStartRequested;
	};
	static_assert(offsetof(PartyData, preloadingMapStage) == 11444);
	static_assert(offsetof(PartyData, m_gameStartSkipCountdown) == 11549);
	static_assert(offsetof(PartyData, lobbyFlags) == 11660);
	static_assert(offsetof(PartyData, gameStartRequested) == 11664);
}