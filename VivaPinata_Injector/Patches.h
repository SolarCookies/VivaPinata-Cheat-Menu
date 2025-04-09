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

//Windowed mode 
// (This cant be set at runtime because its called at the beginning of the game,
// so this is just a example of how to use a complex patch)
//Though if we hook the main function we can set it before the game starts based on a config file
inline static ComplexMemoryPatch WindowedMode = {
	{
		{
			0x7D348D, 
				{0xC7, 0x46, 0x20, 0x00, 0x00, 0x00, 0x00}, 
				{0xC7, 0x46, 0x20, 0x01, 0x00, 0x00, 0x00}
		},

		{
			0x8C1700, 
				{0x68, 0x00, 0x00, 0x00, 0x80}, 
				{0x68, 0x00, 0x00, 0x8E, 0x00}
		}
	//We should add a anti lock mouse patch here
	//as well as a minimize window patch
	//and a resizeable window patch
	}
};
