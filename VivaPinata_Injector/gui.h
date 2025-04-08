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
