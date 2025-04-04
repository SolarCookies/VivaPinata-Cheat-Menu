#pragma once
#include <d3d9.h>
#include <string>
#include <iostream>

namespace gui
{
	// show menu?
	inline bool open = true;

	// is menu setup?
	inline bool setup = false;

	// winapi related
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC originalWindowProcess = nullptr;

	// dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX(std::string* ErrorMessageHandle) noexcept;
	void DestroyDirectX() noexcept;

	// setup device
	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void RenderImGUI() noexcept;
}
