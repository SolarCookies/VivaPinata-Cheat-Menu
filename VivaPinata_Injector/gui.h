#pragma once
#include <d3d9.h>
#include <string>
#include <iostream>

#include "MemoryHelpers.h"
#include "Patches.h"

namespace gui
{
	// show menu?
	inline bool open = false;

	// is menu setup?
	inline bool setup = false;

	// winapi related
	inline HWND Ourwindow = nullptr;
	inline WNDCLASSEX OurwindowClass = { };
	inline WNDPROC originalWindowProcess = nullptr;

	// dx stuff
	inline LPDIRECT3DDEVICE9 Ourdevice = nullptr;
	inline LPDIRECT3D9 Ourd3d9 = nullptr;

	// setup temp window class
	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	// setup temp window
	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	// setup temp directx
	bool SetupDirectX(std::string* ErrorMessageHandle) noexcept;
	void DestroyDirectX() noexcept;

	// setup device
	void Setup();

	// setup ImGUI
	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;

	// destroy ImGUI
	void Destroy() noexcept;

	// render ImGUI
	void RenderImGUI() noexcept;
}
