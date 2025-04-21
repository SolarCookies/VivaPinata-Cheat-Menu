#include "hooks.h"

#include <wchar.h>
#include <stdexcept>
#include <intrin.h>
#include <fstream>

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


	//Viva Piñata.exe hooks
	
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

	// Hook the various print functions.
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00405740),
		&PrintFunc_00405740,
		reinterpret_cast<void**>(&PrintFunc_00405740_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00405740()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00405790),
		&PrintFunc_00405790,
		reinterpret_cast<void**>(&PrintFunc_00405790_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00405790()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004055e0),
		&PrintFunc_004055e0,
		reinterpret_cast<void**>(&PrintFunc_004055e0_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_004055e0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00402bf0),
		&PrintFunc_00402bf0,
		reinterpret_cast<void**>(&PrintFunc_00402bf0_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00402bf0()");

	// Not an actual print function.
	//if (MH_CreateHook(
	//	reinterpret_cast<void*>(0x00851720),
	//	&PrintFunc_00851720,
	//	reinterpret_cast<void**>(&PrintFunc_00851720_Original)
	//)) throw std::runtime_error("Unable to hook PrintFunc_00851720()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00551640),
		&PinataDamage_00551640,
		reinterpret_cast<void**>(&PinataDamage_00551640_Original)
	)) throw std::runtime_error("Unable to hook PinataDamage_00551640()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00751B30),
		&ItemDamage_00751B30,
		reinterpret_cast<void**>(&ItemDamage_00751B30_Original)
	)) throw std::runtime_error("Unable to hook ItemDamage_00751B30()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00745AD0),
		&GetItemName_00745AD0,
		reinterpret_cast<void**>(&GetItemName_00745AD0_Original)
	)) throw std::runtime_error("Unable to hook GetItemName_00745AD0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00745BE0),
		&GetItemNameParent_00745BE0,
		reinterpret_cast<void**>(&GetItemNameParent_00745BE0_Original)
	)) throw std::runtime_error("Unable to hook GetItemNameParent_00745BE0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00727E10),
		&Unknown_00727E10,
		reinterpret_cast<void**>(&Unknown_00727E10_Original)
	)) throw std::runtime_error("Unable to hook Unknown_00727E10()");

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

//Viva Piñata.exe hooks

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

void __cdecl hooks::PrintFunc_00405790(const char* format, ...) noexcept
{
	PrintFunc_00405790_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	std::cout << strToPrint << std::endl;
}

void __cdecl hooks::PrintFunc_004055e0(const char* format, ...) noexcept
{
	PrintFunc_004055e0_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	std::cout << strToPrint << std::endl;
}

void __cdecl hooks::PrintFunc_00402bf0(const char* format, ...) noexcept
{
	PrintFunc_00402bf0_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	std::cout << strToPrint << std::endl;
}

void __cdecl hooks::PrintFunc_00405740(const char* format, ...) noexcept
{
	PrintFunc_00405740_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	std::cout << strToPrint << std::endl;
}

// Not actually a function for printing, it's used for parsing file strings.
void __cdecl hooks::PrintFunc_00851720(wchar_t* buffer, rsize_t bufsz, const wchar_t* format, ...) noexcept
{
	va_list argptr;
	va_start(argptr, format);
	int output = _vsnwprintf_s(buffer, bufsz, bufsz - 1, format, argptr);
	va_end(argptr);

	if (output == -1) {
		buffer[bufsz - 1] = '\0';
	}

	std::wcout << buffer << std::endl;

	//PrintFunc_00851720_Original(buffer, bufsz, buffer); Don't need to call the original function, hook does the same job.
}

//Calls when the Piñata takes damage by the player (Maybe also other things?)
int __cdecl hooks::PinataDamage_00551640(int a1, int Damage) noexcept
{
	int result;
	if (g_EasyBreakSickPinata) {
		std::cout << "PinataDamage_00551640 called with a1: " << a1 << " and Damage: " << Damage << ".. Overriding to 1000.." << std::endl;
		result = PinataDamage_00551640_Original(a1, 1000);
	}
	else {
		result = PinataDamage_00551640_Original(a1, Damage);
		std::cout << "PinataDamage_00551640 called with a1: " << a1 << " and Damage: " << Damage << std::endl;
	}
	
	return result;
}

//Calls when player damages a item
int __cdecl hooks::ItemDamage_00751B30(int a1, int Damage, int a3) noexcept
{
	int result;
	if (g_EasyBreakItems) {
		std::cout << "ItemDamage_00751B30 called with a1: " << a1 << " and Damage: " << Damage << ".. Overriding to 1000.." << std::endl;
		result = ItemDamage_00751B30_Original(a1, 1000, a3);
	}
	else {
		result = ItemDamage_00751B30_Original(a1, Damage, a3);
		std::cout << "ItemDamage_00751B30 called with a1: " << a1 << " and Damage: " << Damage << std::endl;
	}
	return result;
}

wchar_t* __cdecl hooks::GetItemName_00745AD0(int ID, DWORD* a2, int a3) noexcept
{
	wchar_t* result;

	result = GetItemName_00745AD0_Original(ID, a2, a3);

	if (!result) {
		return result;
	}

	//std::wcout << L"ID: " << ID << L" Name: " << result << std::endl;
	//write to file
	//std::string Line = "{" + std::to_string(ID) + ", " + MemHelp::WCharToString(result) + "}," + "\n";
	//std::ofstream filestream("Log.txt", std::ios::app);
	//MemHelp::print(Line, filestream);
	//filestream.close();


	return result;
}

const char* __cdecl hooks::GetItemNameParent_00745BE0(int ID) noexcept
{
	const char* result;

	std::cout << "GetItemNameParent_00745BE0 called" << std::endl;
	/*
	if(!Update){
		for (int i = 0; i < 5000; i++) {
			if (Unknown_00727E10(i) == 0) {
				//std::cout << "Invalid ID" << std::endl;
			}
			else {
				result = GetItemNameParent_00745BE0_Original(i);
			}
		}
	}
	Update = true;
	*/

	result = GetItemNameParent_00745BE0_Original(ID);
	if (!result) {
		return result;
	}
	std::cout << "GetItemNameParent_00745BE0 called with ID: " << ID << " and result: " << result << std::endl;
	return result;
}

int __cdecl hooks::Unknown_00727E10(int a1) noexcept
{
	int result = Unknown_00727E10_Original(a1);
	//std::cout << "Unknown_00727E10 called with a1: " << a1 << " and result: " << result << std::endl;
	return result;
}