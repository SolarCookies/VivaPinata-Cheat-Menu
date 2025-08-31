#pragma once
#include "../../gui.h"
#include "DXHooks.h"

void DXHooks::Setup() {

	// Add SetViewport hook
	if (MH_CreateHook(
		VirtualFunction(gui::Ourdevice, 48),
		&Hooks::SetViewport,
		reinterpret_cast<void**>(&Hooks::SetViewportOriginal)
	)) throw std::runtime_error("Unable to hook SetViewport()");

	//Assemble shader
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004604C2),
		&Hooks::D3DXAssembleShader_004604C2,
		reinterpret_cast<void**>(&Hooks::D3DXAssembleShader_004604C2_Original)
	)) throw std::runtime_error("Unable to hook D3DXAssembleShader()");

	// Compile Shader
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004604C8),
		&Hooks::D3DXCompileShader_004604C8,
		reinterpret_cast<void**>(&Hooks::D3DXCompileShader_004604C8_Original)
	)) throw std::runtime_error("Unable to hook D3DXCompileShader()");

}


// DirectX9 hooks

// Binds to Set Viewport, This is the only function that i could find that would display the menu on the screen
//  - More research is needed to find a better function to bind to that renders ontop of the UI
HRESULT __stdcall Hooks::SetViewport(IDirect3DDevice9* device, const D3DVIEWPORT9* viewport) noexcept
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

int __stdcall Hooks::D3DXAssembleShader_004604C2(int a1, int a2, int a3, int a4, int a5, int a6, int a7) noexcept {
	int result = D3DXAssembleShader_004604C2_Original(a1, a2, a3, a4, a5, a6, a7);
	//std::cout << "D3DXAssembleShader() called" << std::endl;

	char desktopPath[MAX_PATH];
	std::string cwd = std::filesystem::current_path().string();
	strncpy(desktopPath, cwd.c_str(), MAX_PATH - 1);
	desktopPath[MAX_PATH - 1] = '\0';

	std::string filename = "Shader_" + std::to_string(a1) + ".txt";

	std::filesystem::path outDir = std::filesystem::path(desktopPath) / "Temp";

	return result;
}

int __stdcall Hooks::D3DXCompileShader_004604C8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) noexcept {
	int result = D3DXCompileShader_004604C8_Original(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	//std::cout << "D3DXCompileShader() called" << std::endl;
	return result;
}
