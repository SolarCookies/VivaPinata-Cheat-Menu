#pragma once
#include "../Hook.h"

class DXHooks : public Hook
{
	public:

		void Setup() override;

		constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
		{
			return (*static_cast<void***>(thisptr))[index];
		}
};

namespace Hooks {
	
	// DirectX9 hooks

	// Binds to Set Viewport, This is the only function that i could find that would display the menu on the screen
	//  - More research is needed to find a better function to bind to that renders ontop of the UI
	using SetViewportFn = HRESULT(__stdcall*)(IDirect3DDevice9*, const D3DVIEWPORT9*) noexcept;
	inline SetViewportFn SetViewportOriginal = nullptr;
	HRESULT __stdcall SetViewport(IDirect3DDevice9* device, const D3DVIEWPORT9* viewport) noexcept;

	//int __stdcall D3DXAssembleShader(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
	using D3DXAssembleShaderFn_004604C2 = int(__stdcall*)(int a1, int a2, int a3, int a4, int a5, int a6, int a7) noexcept;
	inline D3DXAssembleShaderFn_004604C2 D3DXAssembleShader_004604C2_Original = nullptr;
	int __stdcall D3DXAssembleShader_004604C2(int a1, int a2, int a3, int a4, int a5, int a6, int a7) noexcept;

	//int __stdcall D3DXCompileShader(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
	using D3DXCompileShaderFn_004604C8 = int(__stdcall*)(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) noexcept;
	inline D3DXCompileShaderFn_004604C8 D3DXCompileShader_004604C8_Original = nullptr;
	int __stdcall D3DXCompileShader_004604C8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) noexcept;
};
