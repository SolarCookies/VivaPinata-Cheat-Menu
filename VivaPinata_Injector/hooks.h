#pragma once
#include "gui.h"


#include <filesystem>
#include <windows.h>
#include <iostream>

namespace hooks
{

	inline std::filesystem::path Temppath = std::filesystem::temp_directory_path() / "VivaPinata_Injector";
	inline size_t TempbytesToRead = 0;
	inline char Tempbuffer[20000];

	void Setup();
	void Destroy() noexcept;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
	{
		return (*static_cast<void***>(thisptr))[index];
	}

	// DirectX9 hooks

	// Binds to Set Viewport, This is the only function that i could find that would display the menu on the screen
	//  - More research is needed to find a better function to bind to that renders ontop of the UI
	using SetViewportFn = HRESULT(__stdcall*)(IDirect3DDevice9*, const D3DVIEWPORT9*) noexcept;
	inline SetViewportFn SetViewportOriginal = nullptr;
	HRESULT __stdcall SetViewport(IDirect3DDevice9* device, const D3DVIEWPORT9* viewport) noexcept;

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

	// Binds a print function at offset 0x0073FB80 
	using PrintLineFn_00405740 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00405740 PrintFunc_00405740_Original = nullptr;
	void __cdecl PrintFunc_00405740(const char* format, ...) noexcept;

	// Binds a print function at offset 0x0073FB80 
	using PrintLineFn_00405790 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00405790 PrintFunc_00405790_Original = nullptr;
	void __cdecl PrintFunc_00405790(const char* format, ...) noexcept;

	// Binds a print function at offset 0x004055e0
	using PrintLineFn_004055e0 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_004055e0 PrintFunc_004055e0_Original = nullptr;
	void __cdecl PrintFunc_004055e0(const char* format, ...) noexcept;

	// Binds a print function at offset 0x00402bf0
	using PrintLineFn_00402bf0 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00402bf0 PrintFunc_00402bf0_Original = nullptr;
	void __cdecl PrintFunc_00402bf0(const char* format, ...) noexcept;

	// Binds a print function at offset 0x00851720
	using PrintLineFn_00851720 = void(__cdecl*)(wchar_t*, rsize_t, const wchar_t*, ...) noexcept;
	inline PrintLineFn_00851720 PrintFunc_00851720_Original = nullptr;
	void __cdecl PrintFunc_00851720(wchar_t* buffer, rsize_t bufsz, const wchar_t* format, ...) noexcept;

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
}
