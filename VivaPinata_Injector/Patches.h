#pragma once
#include "MemoryHelpers.h"

// PlayerData struct
struct PlayerData
{
	int PlayerID;
	uint32_t Coins;
};

inline PlayerData* PlayerDataPtr;

// Function to set player coins
typedef char(__cdecl* SetPlayerCoinsF)(struct PlayerData*, uint32_t) noexcept;
inline SetPlayerCoinsF SetPlayerCoinsPtr = (SetPlayerCoinsF)GetVivaAddressPtr("0073FB80");
inline static void SetPlayerCoins(uint32_t Value) {
	if (PlayerDataPtr)
	{
		SetPlayerCoinsPtr(PlayerDataPtr, Value);
	}
}

//Unlimited Garden Space
inline static SimpleMemoryPatch UnlimitedGardenSpace = {
	0x7785D0,
	{0x53, 0x56, 0x8B, 0x74, 0x24, 0x10},
	{0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3}
};
