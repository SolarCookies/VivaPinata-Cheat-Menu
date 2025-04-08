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

	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	// hook Present
	using PresentFn = HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*) noexcept;
	inline PresentFn PresentOriginal = nullptr;
	HRESULT __stdcall Present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND hwnd, const RGNDATA* region) noexcept;

	using DrawIndexedPrimitiveFn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
	inline DrawIndexedPrimitiveFn DrawIndexedPrimitiveOriginal = nullptr;
	HRESULT __stdcall DrawIndexedPrimitive(IDirect3DDevice9* device, D3DPRIMITIVETYPE type, INT baseVertexIndex, UINT minVertexIndex, UINT numVertices, UINT startIndex, UINT primCount) noexcept;

	//bind begin scene
	using BeginSceneFn = HRESULT(__stdcall*)(IDirect3DDevice9*) noexcept;
	inline BeginSceneFn BeginSceneOriginal = nullptr;
	HRESULT __stdcall BeginScene(IDirect3DDevice9* device) noexcept;

	//bind  SetViewport 
	using SetViewportFn = HRESULT(__stdcall*)(IDirect3DDevice9*, const D3DVIEWPORT9*) noexcept;
	inline SetViewportFn SetViewportOriginal = nullptr;
	HRESULT __stdcall SetViewport(IDirect3DDevice9* device, const D3DVIEWPORT9* viewport) noexcept;

	//bind getviewport
	using GetViewportFn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DVIEWPORT9*) noexcept;
	inline GetViewportFn GetViewportOriginal = nullptr;
	HRESULT __stdcall GetViewport(IDirect3DDevice9* device, D3DVIEWPORT9* viewport) noexcept;

	using SetPlayerCoinsFn = char(__cdecl*)(PlayerData*, uint32_t) noexcept;
	inline SetPlayerCoinsFn SetPlayerCoinsOriginal = nullptr;
	char __cdecl SetPlayerCoins(PlayerData* Player, uint32_t NewCoinsValue) noexcept;




}
