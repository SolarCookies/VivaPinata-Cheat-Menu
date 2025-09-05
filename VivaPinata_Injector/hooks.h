#pragma once
#include "gui.h"


#include <filesystem>
#include <windows.h>
#include <iostream>
#include "Hooks/Hook.h"

namespace hooks
{

	inline std::filesystem::path Temppath = std::filesystem::temp_directory_path() / "VivaPinata_Injector";
	inline size_t TempbytesToRead = 0;
	inline char Tempbuffer[20000];
	inline int TempIndex = 0;

	inline std::vector<std::unique_ptr<Hook>> HookList;

	void Setup();
	void Destroy() noexcept;


	//Viva Piñata.exe hooks

	// Binds SetPlayerCoins at offset 0x0073FB80 
	//  - We can use this funtion to get the player data offset 
	//  - Runs when the save is loaded along with when the coins change
	using SetPlayerCoinsFn = char(__cdecl*)(PlayerData*, uint32_t) noexcept;
	inline SetPlayerCoinsFn SetPlayerCoinsOriginal = nullptr;
	char __cdecl SetPlayerCoins(PlayerData* Player, uint32_t NewCoinsValue) noexcept;

	// Binds UpdateCameraByMode at offset 0x0057B1F0
	//  - We can use this funtion to get the camera data? offset
	//  - Runs every game tick while the player is in the normal camera mode
	//  - This is really only useful for ajusting the camera height offset in the top down view mode (At least for now)
	using UpdateCameraByModeFn = int(__cdecl*)(int, CameraData*) noexcept;
	inline UpdateCameraByModeFn UpdateCameraByModeOriginal = nullptr;
	int __cdecl UpdateCameraByMode(int a1, CameraData* a2) noexcept;

	using PinataDamageFn_00551640 = int(__cdecl*)(int a1, int Damage) noexcept;
	inline PinataDamageFn_00551640 PinataDamage_00551640_Original = nullptr;
	int __cdecl PinataDamage_00551640(int a1, int Damage) noexcept;

	using ItemDamageFn_00751B30 = int(__cdecl*)(int a1, int Damage, int a3) noexcept;
	inline ItemDamageFn_00751B30 ItemDamage_00751B30_Original = nullptr;
	int __cdecl ItemDamage_00751B30(int a1, int Damage, int a3) noexcept;

	using GetItemNameFn_00745AD0 = wchar_t* (__cdecl*)(int ID, DWORD* a2, int a3) noexcept;
	inline GetItemNameFn_00745AD0 GetItemName_00745AD0_Original = nullptr;
	wchar_t* __cdecl GetItemName_00745AD0(int ID, DWORD* a2, int a3) noexcept;

	using GetItemNameParentFn_00745BE0 = const char* (__cdecl*)(int ID) noexcept;
	inline GetItemNameParentFn_00745BE0 GetItemNameParent_00745BE0_Original = nullptr;
	const char* __cdecl GetItemNameParent_00745BE0(int ID) noexcept;

	using UnknownFn_00727E10 = int(__cdecl*)(int a1) noexcept;
	inline UnknownFn_00727E10 Unknown_00727E10_Original = nullptr;
	int __cdecl Unknown_00727E10(int a1) noexcept;

	using AllocateStarMenuFn_00433B80 = int(__stdcall*)(int a1, DWORD* a2) noexcept;
	inline AllocateStarMenuFn_00433B80 AllocateStarMenu_00433B80_Original = nullptr;
	int __stdcall AllocateStarMenu_00433B80(int a1, DWORD* a2) noexcept;

	using AddOrUpdatePackageContextFn_008B73E0 = int(__cdecl*)(int* a1) noexcept;
	inline AddOrUpdatePackageContextFn_008B73E0 AddOrUpdatePackageContext_008B73E0_Original = nullptr;
	int __cdecl  AddOrUpdatePackageContext_008B73E0(int* a1) noexcept;

	// Note: The original function uses __thiscall (member function, 'this' in ECX).
	// MinHook hooks it using __fastcall, so the first parameter is 'this', the second is a dummy (usually nullptr).
	using OpenWadFileFn_00865A40 = bool(__fastcall*)(void* thisptr, void* /*unused*/, const char* FileEntryBuffer, DWORD* OpenFileBuffer, size_t* ShaderLength, char UnknownFlag) noexcept;
	inline OpenWadFileFn_00865A40 OpenWadFile_00865A40_Original = nullptr;
	bool __fastcall OpenWadFile(void* thisptr, void* /*unused*/, const char* FileEntryBuffer, DWORD* OpenFileBuffer, size_t* ShaderLength, char UnknownFlag) noexcept;

	//char __thiscall ReadQualitySettings(_DWORD *this, char *Str, int a3, int a4) (Fn_004A1BE0)
	using ReadQualitySettingsFn_004A1BE0 = char(__fastcall*)(void* thisptr, void* /*unused*/, char* Str, int a3, int a4) noexcept;
	inline ReadQualitySettingsFn_004A1BE0 ReadQualitySettings_004A1BE0_Original = nullptr;
	char __fastcall ReadQualitySettings(void* thisptr, void* /*unused*/, char* Str, int a3, int a4) noexcept;

	// char* __cdecl sub_96E7B0(char* Source, int a2)
	//Has something to do with loading cutscenes or something similar, not sure yet.
	using UnknownTestFn_0096E7B0 = char* (__cdecl*)(char* Source, int a2) noexcept;
	inline UnknownTestFn_0096E7B0 UnknownTest_0096E7B0_Original = nullptr;
	char* __cdecl UnknownTest_0096E7B0(char* Source, int a2) noexcept;

	//int __thiscall sub_6B6860(int *this, int a2)
	// This function is used to get the pinata info when you click on the information button (Y button) while hovering over a pinata.
	using PinataInfoFn_006B6860 = int(__fastcall*)(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept;
	inline  PinataInfoFn_006B6860  PinataInfo_006B6860_Original = nullptr;
	int __fastcall  PinataInfo_006B6860(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept;

	//int __stdcall sub_433590(int a1, _DWORD *a2)
	//Contains a ref to SlotMachineBalance::`vftable', A1 gets plugged into the vftable so maybe its the price of the item?
	using SlotMachineBalanceFn_00433590 = int(__stdcall*)(int a1, DWORD* a2) noexcept;
	inline SlotMachineBalanceFn_00433590 SlotMachineBalance_00433590_Original = nullptr;
	int __stdcall SlotMachineBalance_00433590(int a1, DWORD* a2) noexcept;

	//int __thiscall sub_907950(int *this, int a2)
	//(Responsible for displaying the name of the pinata when you hover over it)
	using PinataAvatarFn_00907950 = int(__fastcall*)(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept;
	inline PinataAvatarFn_00907950 PinataAvatar_00907950_Original = nullptr;
	int __fastcall PinataAvatar_00907950(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept;

	//int __stdcall sub_5EEF9F(int a1, int a2)
	//(Called when the game needs to draw text to the screen) (Only for some things though)
	using TextDisplayFn_005EEF9F = int(__stdcall*)(int a1, int a2) noexcept;
	inline TextDisplayFn_005EEF9F TextDisplay_005EEF9F_Original = nullptr;
	int __stdcall TextDisplay_005EEF9F(int a1, int a2) noexcept;


	//int __thiscall SetClockHandRot_6BF9A0(int *this, float a2)
	using SetClockHandRotFn_006BF9A0 = int(__fastcall*)(int* thisptr, void* /*unused*/, int* thisr, float a2) noexcept;
	inline SetClockHandRotFn_006BF9A0 SetClockHandRot_006BF9A0_Original = nullptr;
	int __fastcall SetClockHandRot_006BF9A0(int* thisptr, void* /*unused*/, int* thisr, float a2) noexcept;

	//int GetGarden_00773D10()
	using GetGardenFn_00773D10 = int(__cdecl*)() noexcept;
	inline GetGardenFn_00773D10 GetGarden_00773D10_Original = nullptr;
	int __cdecl GetGarden_00773D10() noexcept;

	//_DWORD *__usercall FormatTime_70B020@<eax>(int PtrToTime@<eax>, int *Hours, int *Min, _DWORD *a4)
	using FormatTimeFn_0070B020 = DWORD * (__cdecl*)(int PtrToTime, int* Hours, int* Min, DWORD* a4) noexcept;
	inline FormatTimeFn_0070B020 FormatTime_0070B020_Original = nullptr;
	DWORD* __cdecl FormatTime_0070B020(int PtrToTime, int* Hours, int* Min, DWORD* a4) noexcept;

	//int __cdecl sub_7740F0(struct tm *Tm, int a2)
	using LoadMapFn_007740F0 = int(__cdecl*)(struct tm* Tm, int a2) noexcept;
	inline LoadMapFn_007740F0 LoadMap_007740F0_Original = nullptr;
	int __cdecl LoadMap_007740F0(struct tm* Tm, int a2) noexcept;

	//int __stdcall sub_7DEA60(int a1, wchar_t *Str, _DWORD *a3, _DWORD *a4)
	using GetUITextureFn_007DEA60 = int(__stdcall*)(int a1, wchar_t* Str, DWORD* a3, DWORD* a4) noexcept;
	inline GetUITextureFn_007DEA60 GetUITexture_007DEA60_Original = nullptr;
	int __stdcall GetUITexture_007DEA60(int a1, wchar_t* Str, DWORD* a3, DWORD* a4) noexcept;

	//int __cdecl sub_70D680(int a1) PinataIDs
	using GetPinataIDsFn_0070D680 = int(__cdecl*)(int a1) noexcept;
	inline GetPinataIDsFn_0070D680 GetPinataIDs_0070D680_Original = nullptr;
	int __cdecl GetPinataIDs_0070D680(int a1) noexcept;

	//_DWORD *__thiscall sub_7F8FB0(_DWORD *this) SetGlobalSaveStruct
	using SetGlobalSaveStructFn_007F8FB0 = DWORD * (__fastcall*)(DWORD* thisptr) noexcept;
	inline SetGlobalSaveStructFn_007F8FB0 SetGlobalSaveStruct_007F8FB0_Original = nullptr;
	DWORD* __fastcall SetGlobalSaveStruct_007F8FB0(DWORD* thisptr) noexcept;

	//int __cdecl sub_7F0EB0(int a1, int a2, int a3)
	using sub_7F0EB0Fn = int(__cdecl*)(int a1, int a2, int a3) noexcept;
	inline sub_7F0EB0Fn sub_7F0EB0_Original = nullptr;
	int __cdecl sub_7F0EB0(int a1, int a2, int a3) noexcept;

	//BOOL __cdecl sub_7F2170(_DWORD *a1, _DWORD *a2, _BYTE *a3)
	using sub_7F2170Fn = BOOL(__cdecl*)(DWORD* a1, DWORD* a2, BYTE* a3) noexcept;
	inline sub_7F2170Fn sub_7F2170_Original = nullptr;
	BOOL __cdecl sub_7F2170(DWORD* a1, DWORD* a2, BYTE* a3) noexcept;

	//void __cdecl sub_4B5B90(int a1)
	using sub_4B5B90Fn = void(__cdecl*)(int a1) noexcept;
	inline sub_4B5B90Fn sub_4B5B90_Original = nullptr;
	void __cdecl sub_4B5B90(int a1) noexcept;

	//int __cdecl PseudoRandom_418FF0(int *a1, int a2, int a3)
	using PseudoRandomFn_00418FF0 = int(__cdecl*)(int* a1, int a2, int a3) noexcept;
	inline PseudoRandomFn_00418FF0 PseudoRandom_00418FF0_Original = nullptr;
	int __cdecl PseudoRandom_00418FF0(int* a1, int a2, int a3) noexcept;

	//int __usercall sub_4B5430@<eax>(int a1@<esi>)
	using sub_4B5430Fn = int(__cdecl*)(int a1) noexcept;
	inline sub_4B5430Fn sub_4B5430_Original = nullptr;
	int __cdecl sub_4B5430(int a1) noexcept;

	//int __cdecl sub_4B54A0(_DWORD *a1, int a2)
	using sub_4B54A0Fn = int(__cdecl*)(DWORD* a1, int a2) noexcept;
	inline sub_4B54A0Fn sub_4B54A0_Original = nullptr;
	int __cdecl sub_4B54A0(DWORD* a1, int a2) noexcept;


	//void __cdecl sub_7D0A20(int *a1, float a2)
	using sub_7D0A20Fn = void(__cdecl*)(int* a1, float a2) noexcept;
	inline sub_7D0A20Fn sub_7D0A20_Original = nullptr;
	void __cdecl sub_7D0A20(int* a1, float a2) noexcept;

	//int __cdecl sub_80E820(int a1)
	using sub_80E820Fn = int(__cdecl*)(int a1) noexcept;
	inline sub_80E820Fn sub_80E820_Original = nullptr;
	int __cdecl sub_80E820(int a1) noexcept;

	//int __cdecl sub_957750(int a1)
	using sub_957750Fn = int(__cdecl*)(int a1) noexcept;
	inline sub_957750Fn sub_957750_Original = nullptr;
	int __cdecl sub_957750(int a1) noexcept;

	//int __cdecl Tags_7F0E60(int a1, int a2, int a3)
	using TagsFn_7F0E60 = int(__cdecl*)(int a1, int a2, int a3) noexcept;
	inline TagsFn_7F0E60 Tags_7F0E60_Original = nullptr;
	int __cdecl Tags_7F0E60(int a1, int a2, int a3) noexcept;

	//int __cdecl CreateTaggedFloat(int a1, float a2, int a3)
	using CreateTaggedFloatFn = int(__cdecl*)(int a1, float a2, int a3) noexcept;
	inline CreateTaggedFloatFn CreateTaggedFloat_Original = nullptr;
	int __cdecl CreateTaggedFloat(int a1, float a2, int a3) noexcept;

	//  is the default cursor colors for multiplayer)
	//int __cdecl sub_798220(int* Player, int a2, void* a3)
	using sub_798220Fn = int(__cdecl*)(int* Player, int a2, void* a3) noexcept;
	inline sub_798220Fn sub_798220_Original = nullptr;
	int __cdecl sub_798220(int* Player, int a2, void* a3) noexcept;

	//int __cdecl GardenSpaceThreshold_7785D0(int Context, int a2, int a3)
	using GardenSpaceThresholdFn_007785D0 = int(__cdecl*)(int Context, int a2, int a3) noexcept;
	inline GardenSpaceThresholdFn_007785D0 GardenSpaceThreshold_007785D0_Original = nullptr;
	int __cdecl GardenSpaceThreshold_007785D0(int Context, int a2, int a3) noexcept;
	
}
