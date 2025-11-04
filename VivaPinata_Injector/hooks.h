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

	//int __cdecl GardenSpaceThreshold_7785D0(int Context, int a2, int a3)
	using GardenSpaceThresholdFn_007785D0 = int(__cdecl*)(int Context, int a2, int a3) noexcept;
	inline GardenSpaceThresholdFn_007785D0 GardenSpaceThreshold_007785D0_Original = nullptr;
	int __cdecl GardenSpaceThreshold_007785D0(int Context, int a2, int a3) noexcept;
	
	
}
