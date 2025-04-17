#pragma once
#include "gui.h"


namespace hooks
{

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

	//Viva Pinata.exe hooks

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
}
