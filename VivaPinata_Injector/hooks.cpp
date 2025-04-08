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
		VirtualFunction(gui::Ourdevice, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 17),
		&Present,
		reinterpret_cast<void**>(&PresentOriginal)
	)) throw std::runtime_error("Unable to hook Present()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 82),
		&DrawIndexedPrimitive,
		reinterpret_cast<void**>(&DrawIndexedPrimitiveOriginal)
	)) throw std::runtime_error("Unable to hook DrawIndexedPrimitive()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 41),
		&BeginScene,
		reinterpret_cast<void**>(&BeginSceneOriginal)
	)) throw std::runtime_error("Unable to hook BeginScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 49),
		&GetViewport,
		reinterpret_cast<void**>(&GetViewportOriginal)
	)) throw std::runtime_error("Unable to hook GetViewport()");

	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 48),
		&SetViewport,
		reinterpret_cast<void**>(&SetViewportOriginal)
	)) throw std::runtime_error("Unable to hook SetViewport()");


	//add setcoins hook
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0073FB80),
		&SetPlayerCoins,
		reinterpret_cast<void**>(&SetPlayerCoinsOriginal)
	)) throw std::runtime_error("Unable to hook SetPlayerCoins()");

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
	const auto result = EndSceneOriginal(device, device);

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept{
	const auto result = ResetOriginal(device, device, params);

	return result;
}

HRESULT __stdcall hooks::Present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND hwnd, const RGNDATA* region) noexcept
{

	const auto result = PresentOriginal(device, src, dest, hwnd, region);

	return result;
}


HRESULT __stdcall hooks::DrawIndexedPrimitive(IDirect3DDevice9* device, D3DPRIMITIVETYPE type, INT baseVertexIndex, UINT minVertexIndex, UINT numVertices, UINT startIndex, UINT primCount) noexcept {
	

	return DrawIndexedPrimitiveOriginal(device, type, baseVertexIndex, minVertexIndex, numVertices, startIndex, primCount);
}

HRESULT __stdcall hooks::BeginScene(IDirect3DDevice9* device) noexcept
{

	return BeginSceneOriginal(device);
}

HRESULT __stdcall hooks::SetViewport(IDirect3DDevice9* device, const D3DVIEWPORT9* viewport) noexcept
{
	const auto result = SetViewportOriginal(device, viewport);

	if (!gui::setup) {
		gui::SetupMenu(device);
	}

	if (!gui::open) {
		return result;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE); // Disable Z-buffer
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Disable Z writes

	ImGui::SetNextWindowBgAlpha(1.0f); // Ensure full opacity
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // Top-left corner

	gui::RenderImGUI();

	device->SetRenderState(D3DRS_ZENABLE, TRUE); // Re-enable Z-buffer
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Re-enable Z writes


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return result;
}

HRESULT __stdcall hooks::GetViewport(IDirect3DDevice9* device, D3DVIEWPORT9* viewport) noexcept
{
	const auto result = GetViewportOriginal(device, viewport);

	return result;
}


//Viva Pinata.exe hooks

char __cdecl hooks::SetPlayerCoins(PlayerData* Player, uint32_t NewCoinsValue) noexcept
{
	const auto result = SetPlayerCoinsOriginal(Player, NewCoinsValue);

	std::cout << "SetPlayerCoins called with PlayerID: " << Player->PlayerID << " to Value: " << NewCoinsValue << std::endl;
	PlayerDataPtr = Player;

	return result;
}

