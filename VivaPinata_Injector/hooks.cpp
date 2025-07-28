#include "hooks.h"

#include <wchar.h>
#include <stdexcept>
#include <intrin.h>
#include <fstream>
#include <algorithm>


#include "minhook/minhook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <shlobj.h> // Add this include to define CSIDL_DESKTOP


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
	
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00433B80),
		&AllocateStarMenu_00433B80,
		reinterpret_cast<void**>(&AllocateStarMenu_00433B80_Original)
	)) throw std::runtime_error("Unable to hook AllocateStarMenu_00433B80()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x008B73E0),
		&AddOrUpdatePackageContext_008B73E0,
		reinterpret_cast<void**>(&AddOrUpdatePackageContext_008B73E0_Original)
	)) throw std::runtime_error("Unable to hook AddOrUpdatePackageContext_008B73E0()");
	
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00865A40),
		&OpenWadFile,
		reinterpret_cast<void**>(&OpenWadFile_00865A40_Original)
	)) throw std::runtime_error("Unable to hook OpenWadFile()");

	
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004A1BE0),
		&ReadQualitySettings,
		reinterpret_cast<void**>(&ReadQualitySettings_004A1BE0_Original)
	)) throw std::runtime_error("Unable to hook ReadQualitySettings()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0096E7B0),
		&UnknownTest_0096E7B0,
		reinterpret_cast<void**>(&UnknownTest_0096E7B0_Original)
	)) throw std::runtime_error("Unable to hook UnknownTest_0096E7B0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x006B6860),
		&PinataInfo_006B6860,
		reinterpret_cast<void**>(&PinataInfo_006B6860_Original)
	)) throw std::runtime_error("Unable to hook PinataInfo_006B6860()");

	
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


int __stdcall hooks::AllocateStarMenu_00433B80(int a1, DWORD* a2) noexcept
{
	int result = AllocateStarMenu_00433B80_Original(a1, a2);
	std::cout << "AllocateStarMenu_00433B80 called with a1: " << a1 << " a2: " << a2 << " and result: " << result << std::endl;
	std::cout << "result: " << result << std::endl;
	return result;
}

int __cdecl hooks::AddOrUpdatePackageContext_008B73E0(int* a1) noexcept
{
	std::cout << "AddOrUpdatePackageContext_008B73E0 called with a1: " << a1 << std::endl;
	return AddOrUpdatePackageContext_008B73E0_Original(a1);
}

bool __fastcall hooks::OpenWadFile(void* thisptr, void* /*unused*/, const char* FileEntryBuffer, DWORD* OpenFileBuffer, size_t* ShaderLength, char UnknownFlag) noexcept
{
	bool result = OpenWadFile_00865A40_Original(thisptr, nullptr, FileEntryBuffer, OpenFileBuffer, ShaderLength, UnknownFlag);
	std::cout << "Loading Shader: " << (FileEntryBuffer ? FileEntryBuffer : "nullptr") << std::endl;

	for(size_t i = 0; i < 20000; ++i) {
		Tempbuffer[i] = 0; // Initialize Tempbuffer to zero
	}

	if (FileEntryBuffer && OpenFileBuffer && ShaderLength && *ShaderLength > 0) {
		char desktopPath[MAX_PATH];
		std::string cwd = std::filesystem::current_path().string();
		strncpy(desktopPath, cwd.c_str(), MAX_PATH - 1);
		desktopPath[MAX_PATH - 1] = '\0';
		
		std::filesystem::path outDir = std::filesystem::path(desktopPath);
		std::string sanitizedFileEntry(FileEntryBuffer);
		std::replace(sanitizedFileEntry.begin(), sanitizedFileEntry.end(), ' ', '_');
		std::filesystem::path outFile = outDir / sanitizedFileEntry;

		if (std::filesystem::exists(outFile)) {
			std::ifstream ifs(outFile, std::ios::binary | std::ios::ate);
			if (ifs.is_open()) {
				std::streamsize fileSize = ifs.tellg();
				ifs.seekg(0, std::ios::beg);

				TempbytesToRead = std::filesystem::file_size(outFile);
				if(TempbytesToRead == 0) {
					goto ELSE; // Skip reading if the file is empty
				}
				for(size_t i = 0; i < TempbytesToRead; ++i) {
					Tempbuffer[i] = ifs.get();
				}
				ifs.close();
			}
			else {
				//std::cerr << "Failed to open file: " << outFile << std::endl;
			}
		}
		else {
			ELSE:
			std::cerr << "File does not exist: " << outFile << std::endl;
			Temppath = outFile.string(); // Store the path for later use
			//create file with subdirectory
			std::filesystem::create_directories(outFile.parent_path());
			std::filesystem::create_directories(outDir);
			std::ofstream ofs(outFile, std::ios::binary);
			if (ofs.is_open()) {
				std::cout << "Creating file: " << outFile << std::endl;
				ofs.close();
			} else {
				std::cerr << "Failed to create file: " << outFile << std::endl;
			}
		}
	}

	return result;
}

char __fastcall hooks::ReadQualitySettings(void* thisptr, void* /*unused*/, char* Str, int a3, int a4) noexcept
{
	//a3 length, Str is the file


	char result = ReadQualitySettings_004A1BE0_Original(thisptr, nullptr, Str, a3, a4);
	std::cout << "Load Quality Settings called" << std::endl;

	// Only write if the file is empty
	if (std::filesystem::exists(Temppath) && std::filesystem::file_size(Temppath) == 0) {
		std::ofstream ofs(Temppath, std::ios::binary);
		if (ofs.is_open()) {
			ofs.write(Str, a3);
			ofs.close();
			//std::cout << "Wrote " << a3 << " bytes to " << Temppath << std::endl;
		}
		else {
			//std::cerr << "Failed to open file for writing: " << Temppath << std::endl;
		}
	}
	else {
		
		memccpy(Str, Tempbuffer, 0, a3); // Copy the contents of Str to Tempbuffer
		a3 = static_cast<int>(TempbytesToRead);
	}


	return result;
}

//Has something to do with loading cutscenes or something similar, not sure yet.
char* __cdecl hooks::UnknownTest_0096E7B0(char* Source, int a2) noexcept {
	char* result = UnknownTest_0096E7B0_Original(Source, a2);
	std::cout << "UnknownTest_0096E7B0 called with Source: " << (Source ? Source : "nullptr") << " and a2: " << a2 << std::endl;
	return result;
}

int __fastcall  hooks::PinataInfo_006B6860(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept {
	int result = PinataInfo_006B6860_Original(thisptr, nullptr, thisr, a2);
	std::cout << "PinataInfo_006B6860 called with thisptr: " << thisptr << " thisr: " << thisr << " and a2: " << a2 << std::endl;
	return result;
}

