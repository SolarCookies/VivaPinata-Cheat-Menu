#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "minhook/minhook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

void hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 17),
		&Present,
		reinterpret_cast<void**>(&PresentOriginal)
	)) throw std::runtime_error("Unable to hook Present()");

	// enable hooks
	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");


	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept{
	

	
	//if (frameCount > 60) {
	//	MessageBeep(MB_ICONERROR); // plays a beep sound every 5 seconds while ticking
	//	frameCount = 0;
	//}

	
	const auto result = EndSceneOriginal(device, device);

	frameCount++;
	

	if (!gui::setup) {
		gui::SetupMenu(device);
	}

	// Comment out ImGui rendering for testing
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	gui::RenderImGUI();

	ImGui::ShowDemoWindow();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	MessageBeep(MB_ICONERROR);

	//ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	//ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

HRESULT __stdcall hooks::Present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND hwnd, const RGNDATA* region) noexcept
{

	const auto result = PresentOriginal(device, src, dest, hwnd, region);

	
	return result;

}
