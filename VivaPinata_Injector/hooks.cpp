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
	
	// DirectX9 hooks

	// Add SetViewport hook
	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 48),
		&SetViewport,
		reinterpret_cast<void**>(&SetViewportOriginal)
	)) throw std::runtime_error("Unable to hook SetViewport()");


	//Viva Pinata.exe hooks
	
	// Add SetPlayerCoins hook
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0073FB80),
		&SetPlayerCoins,
		reinterpret_cast<void**>(&SetPlayerCoinsOriginal)
	)) throw std::runtime_error("Unable to hook SetPlayerCoins()");

	// Add UpdateCameraByMode hook
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0057B1F0),
		&UpdateCameraByMode,
		reinterpret_cast<void**>(&UpdateCameraByModeOriginal)
	)) throw std::runtime_error("Unable to hook UpdateCameraByMode()");


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

// DirectX9 hooks

// Binds to Set Viewport, This is the only function that i could find that would display the menu on the screen
//  - More research is needed to find a better function to bind to that renders ontop of the UI
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

	ImGui::SetNextWindowBgAlpha(0.1f); // Ensure full opacity
	//ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always); // Top-left corner

	gui::RenderImGUI();

	device->SetRenderState(D3DRS_ZENABLE, TRUE); // Re-enable Z-buffer
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Re-enable Z writes


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return result;
}

//Viva Pinata.exe hooks

// Binds SetPlayerCoins at offset 0x0073FB80 
//  - We can use this funtion to get the player data offset 
//  - Runs when the save is loaded along with when the coins change
char __cdecl hooks::SetPlayerCoins(PlayerData* Player, uint32_t NewCoinsValue) noexcept
{
	const auto result = SetPlayerCoinsOriginal(Player, NewCoinsValue);

	std::cout << "SetPlayerCoins called with PlayerID: " << Player->PlayerID << " to Value: " << NewCoinsValue << std::endl;
	g_PlayerDataPtr = Player;

	return result;
}

// Binds UpdateCameraByMode at offset 0x0057B1F0
//  - We can use this funtion to get the camera data? offset
//  - Runs every game tick while the player is in the normal camera mode
//  - This is really only useful for ajusting the camera height offset in the top down view mode (At least for now)
int __cdecl hooks::UpdateCameraByMode(int a1, CameraData* a2) noexcept
{
	const auto result = UpdateCameraByModeOriginal(a1,a2);

	g_CameraDataPtr = a2;

	return result;
}

