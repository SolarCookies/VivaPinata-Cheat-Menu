#pragma once
#include "MemoryHelpers.h"

// PlayerData struct
struct PlayerData
{
	int PlayerID;
	uint32_t Coins;
	uint32_t unknown1;
	uint32_t unknown2;
	uint32_t ExperiencePoints;
	uint32_t Level;
	uint32_t unknown4; //Has something to do with towns people
	uint32_t unknown5;
	uint32_t unknown6;
	uint32_t unknown7;
	uint32_t unknown8;
	uint32_t unknown9;
	int32_t unknown10;
	uint32_t unknown11;
	uint32_t unknown12;
	uint32_t unknown13;
	uint32_t unknown14;
	uint32_t unknown15;
	uint32_t unknown16;
	uint32_t unknown17;
	uint32_t unknown18;
	uint32_t unknown19;
	uint32_t unknown20;
};

struct CameraData
{
	float unknown1;
	float unknown2;
	int CameraMode; // the overhead view and the normal view is 2, tower of sour is 7 and the transformation is 4
};

struct Vector3
{
	float x;
	float y;
	float z;
};

struct EnityData
{
	int Entityoffset;
	Vector3 Position;
};

inline bool g_Debug = false; // Debug mode

inline PlayerData* g_PlayerDataPtr;
inline CameraData* g_CameraDataPtr;
inline std::vector<EnityData> g_EntityData;

//temporary variables for testing funtion inputs on tick without flooding the console (Render these in imgui)
inline int g_i1;
inline int g_i2;
inline int g_i3;
inline int g_i4;

inline float g_f1;
inline float g_f2;
inline float g_f3;
inline float g_f4;

inline char* g_c1;
inline char* g_c2;
inline char* g_c3;
inline char* g_c4;

// Function to set player coins
typedef char(__cdecl* SetPlayerCoinsF)(struct PlayerData*, uint32_t) noexcept;
inline SetPlayerCoinsF SetPlayerCoinsPtr = (SetPlayerCoinsF)GetVivaAddressPtr(0x0073FB80);
inline static void SetPlayerCoins(uint32_t Value) {
	if (g_PlayerDataPtr)
	{
		SetPlayerCoinsPtr(g_PlayerDataPtr, Value);
	}
}

//Unlimited Garden Space
inline static SimpleMemoryPatch g_UnlimitedGardenSpace = {
	0x7785D0,
		{0x53, 0x56, 0x8B, 0x74, 0x24, 0x10},
		{0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3}
};

//Windowed mode 
// (This cant be set at runtime because its called at the beginning of the game,
// so this is just a example of how to use a complex patch)
//Though if we hook the main function we can set it before the game starts based on a config file
inline static ComplexMemoryPatch g_WindowedMode = {
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
