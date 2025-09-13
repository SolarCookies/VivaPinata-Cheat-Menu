#include "hooks.h"

//Hooks
#include "Hooks/Prints/PrintHooks.h"
#include "Hooks/DirectX/DXHooks.h"
#include "Hooks/XLive/XLiveHooks.h"
#include "Patches.h"




void hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	// Register Hook Classes
	{
		//HookList.push_back(std::make_unique<PrintHooks>());
		HookList.push_back(std::make_unique<DXHooks>());
		HookList.push_back(std::make_unique<XLiveHooks>());
	}

	OnStartupPatch();

	// Setup All Hook Classes
	for (auto& HookClass : HookList) {
		HookClass->Setup();
	}
	

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
		reinterpret_cast<void*>(0x0096E7B0),
		&UnknownTest_0096E7B0,
		reinterpret_cast<void**>(&UnknownTest_0096E7B0_Original)
	)) throw std::runtime_error("Unable to hook UnknownTest_0096E7B0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x006B6860),
		&PinataInfo_006B6860,
		reinterpret_cast<void**>(&PinataInfo_006B6860_Original)
	)) throw std::runtime_error("Unable to hook PinataInfo_006B6860()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00433590),
		&SlotMachineBalance_00433590,
		reinterpret_cast<void**>(&SlotMachineBalance_00433590_Original)
	)) throw std::runtime_error("Unable to hook SlotMachineBalance_00433590()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00907950),
		&PinataAvatar_00907950,
		reinterpret_cast<void**>(&PinataAvatar_00907950_Original)
	)) throw std::runtime_error("Unable to hook PinataAvatar_00907950()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x005EEF9F),
		&TextDisplay_005EEF9F,
		reinterpret_cast<void**>(&TextDisplay_005EEF9F_Original)
	)) throw std::runtime_error("Unable to hook TextDisplay_005EEF9F()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007740F0),
		&LoadMap_007740F0,
		reinterpret_cast<void**>(&LoadMap_007740F0_Original)
	)) throw std::runtime_error("Unable to hook LoadMap_007740F0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007DEA60),
		&GetUITexture_007DEA60,
		reinterpret_cast<void**>(&GetUITexture_007DEA60_Original)
	)) throw std::runtime_error("Unable to GetUITexture_007DEA60()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0070D680),
		&GetPinataIDs_0070D680,
		reinterpret_cast<void**>(&GetPinataIDs_0070D680_Original)
	)) throw std::runtime_error("Unable to GetPinataIDs_0070D680()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004B5B90),
		&sub_4B5B90,
		reinterpret_cast<void**>(&sub_4B5B90_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004B5430),
		&sub_4B5430,
		reinterpret_cast<void**>(&sub_4B5430_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004B54A0),
		&sub_4B54A0,
		reinterpret_cast<void**>(&sub_4B54A0_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007D0A20),
		&sub_7D0A20,
		reinterpret_cast<void**>(&sub_7D0A20_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x0080E820),
		&sub_80E820,
		reinterpret_cast<void**>(&sub_80E820_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00957750),
		&sub_957750,
		reinterpret_cast<void**>(&sub_957750_Original)
	)) throw std::runtime_error("Unable to ");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007F0E60),
		&Tags_7F0E60,
		reinterpret_cast<void**>(&Tags_7F0E60_Original)
	)) throw std::runtime_error("Unable to Tags_7F0E60()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007F0D10),
		&CreateTaggedFloat,
		reinterpret_cast<void**>(&CreateTaggedFloat_Original)
	)) throw std::runtime_error("Unable to CreateTaggedFloat_007F0D10()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00798220),
		&sub_798220,
		reinterpret_cast<void**>(&sub_798220_Original)
	)) throw std::runtime_error("Unable to sub_798220()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007785D0),
		&GardenSpaceThreshold_007785D0,
		reinterpret_cast<void**>(&GardenSpaceThreshold_007785D0_Original)
	)) throw std::runtime_error("Unable to GardenSpaceThreshold_007785D0()");

	/*
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007F8FB0),
		&SetGlobalSaveStruct_007F8FB0,
		reinterpret_cast<void**>(&SetGlobalSaveStruct_007F8FB0_Original)
	)) throw std::runtime_error("Unable toSetGlobalSaveStruct_007F8FB0()");
	
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007F0EB0),
		&sub_7F0EB0,
		reinterpret_cast<void**>(&sub_7F0EB0_Original)
	)) throw std::runtime_error("Unable to hook to sub_7F0EB0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007F2170),
		&sub_7F2170,
		reinterpret_cast<void**>(&sub_7F2170_Original)
	)) throw std::runtime_error("Unable to hook to sub_7F2170()");
	*/

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


//Viva Piñata.exe hooks

// Binds SetPlayerCoins at offset 0x0073FB80 
//  - We can use this funtion to get the player data offset 
//  - Runs when the save is loaded along with when the coins change
char __cdecl hooks::SetPlayerCoins(PlayerData* Player, uint32_t NewCoinsValue) noexcept
{
	const auto result = SetPlayerCoinsOriginal(Player, NewCoinsValue);

	//std::cout << "SetPlayerCoins called with PlayerID: " << Player->PlayerID << " to Value: " << NewCoinsValue << std::endl;
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

//Calls when the Piñata takes damage by the player (Maybe also other things?)
int __cdecl hooks::PinataDamage_00551640(int a1, int Damage) noexcept
{
	int result;
	if (g_EasyBreakSickPinata) {
		//std::cout << "PinataDamage_00551640 called with a1: " << a1 << " and Damage: " << Damage << ".. Overriding to 1000.." << std::endl;
		result = PinataDamage_00551640_Original(a1, 1000);
	}
	else {
		result = PinataDamage_00551640_Original(a1, Damage);
		//std::cout << "PinataDamage_00551640 called with a1: " << a1 << " and Damage: " << Damage << std::endl;
	}

	return result;
}

//Calls when player damages a item
int __cdecl hooks::ItemDamage_00751B30(int a1, int Damage, int a3) noexcept
{
	int result;
	if (g_EasyBreakItems) {
		//std::cout << "ItemDamage_00751B30 called with a1: " << a1 << " and Damage: " << Damage << ".. Overriding to 1000.." << std::endl;
		result = ItemDamage_00751B30_Original(a1, 1000, a3);
	}
	else {
		result = ItemDamage_00751B30_Original(a1, Damage, a3);
		//std::cout << "ItemDamage_00751B30 called with a1: " << a1 << " and Damage: " << Damage << std::endl;
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

	//std::cout << "GetItemNameParent_00745BE0 called" << std::endl;
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
	//std::cout << "GetItemNameParent_00745BE0 called with ID: " << ID << " and result: " << result << std::endl;
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
	//std::cout << "AllocateStarMenu_00433B80 called with a1: " << a1 << " a2: " << a2 << " and result: " << result << std::endl;
	//std::cout << "result: " << result << std::endl;
	return result;
}

int __cdecl hooks::AddOrUpdatePackageContext_008B73E0(int* a1) noexcept
{
	//std::cout << "AddOrUpdatePackageContext_008B73E0 called with a1: " << a1 << std::endl;
	return AddOrUpdatePackageContext_008B73E0_Original(a1);
}

bool __fastcall hooks::OpenWadFile(void* thisptr, void* /*unused*/, const char* FileEntryBuffer, DWORD* OpenFileBuffer, size_t* ShaderLength, char UnknownFlag) noexcept
{
	bool result = OpenWadFile_00865A40_Original(thisptr, nullptr, FileEntryBuffer, OpenFileBuffer, ShaderLength, UnknownFlag);
	//std::cout << "Loading Shader: " << (FileEntryBuffer ? FileEntryBuffer : "nullptr") << std::endl;

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
			//std::cerr << "File does not exist: " << outFile << std::endl;
			Temppath = outFile.string(); // Store the path for later use
			//create file with subdirectory
			std::filesystem::create_directories(outFile.parent_path());
			std::filesystem::create_directories(outDir);
			std::ofstream ofs(outFile, std::ios::binary);
			if (ofs.is_open()) {
				//std::cout << "Creating file: " << outFile << std::endl;
				ofs.close();
			} else {
				//std::cerr << "Failed to create file: " << outFile << std::endl;
			}
		}
	}

	return result;
}

char __fastcall hooks::ReadQualitySettings(void* thisptr, void* /*unused*/, char* Str, int a3, int a4) noexcept
{
	//a3 length, Str is the file


	char result = ReadQualitySettings_004A1BE0_Original(thisptr, nullptr, Str, a3, a4);
	//std::cout << "Load Quality Settings called" << std::endl;

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
	//std::cout << "UnknownTest_0096E7B0 called with Source: " << (Source ? Source : "nullptr") << " and a2: " << a2 << std::endl;
	return result;
}

int __fastcall  hooks::PinataInfo_006B6860(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept {
	int result = PinataInfo_006B6860_Original(thisptr, nullptr, thisr, a2);
	//std::cout << "PinataInfo_006B6860 called with thisptr: " << thisptr << " thisr: " << thisr << " and a2: " << a2 << std::endl;
	return result;
}

int __stdcall hooks::SlotMachineBalance_00433590(int a1, DWORD* a2) noexcept {
	int result = SlotMachineBalance_00433590_Original(a1, a2);
	//std::cout << "SlotMachineBalance_00433590 called with a1: " << a1 << " and a2: " << a2 << std::endl;
	return result;
}

int __fastcall hooks::PinataAvatar_00907950(int* thisptr, void* /*unused*/, int* thisr, int a2) noexcept {
	int result = PinataAvatar_00907950_Original(thisptr, nullptr, thisr, a2);
	//std::cout << "PinataAvatar_00907950 called with thisptr: " << thisptr << " thisr: " << thisr << " and a2: " << a2 << std::endl;
	return result;
}

int __stdcall hooks::TextDisplay_005EEF9F(int a1, int a2) noexcept {

	//wchar_t Destination[24] = {0};
	//wcsncpy(Destination, L"Solar Was Here", 24);

	//int result = TextDisplay_005EEF9F_Original(a1, (int)Destination);

	int result = TextDisplay_005EEF9F_Original(a1, a2);

	return result;
}

int __cdecl hooks::LoadMap_007740F0(struct tm* Tm, int a2) noexcept {
	int result = LoadMap_007740F0_Original(Tm, a2);
	//std::cout << "LoadMap_007740F0 called with Tm: " << Tm << " and a2: " << a2 << std::endl;
	//std::cout << "Loaded Map: " << "Year: " << Tm->tm_year << " Month: " << Tm->tm_mon + 1 << " Day: " << Tm->tm_mday  << " Hour: " << Tm->tm_hour << " Min: " << Tm->tm_min << " Sec: " << Tm->tm_sec << std::endl;

	//Print int at a2 + 4
	if (a2) {
		int* intPtr = reinterpret_cast<int*>(a2 + 4);
		//std::cout << "Int value at a2 + 4 'gardenWidth': " << *intPtr << std::endl;
		//std::cout << "Float value at a2 + 4 'gardenWidth': " << *reinterpret_cast<float*>(a2 + 4) << std::endl;
		intPtr = reinterpret_cast<int*>(a2 + 8);
		//std::cout << "Int value at a2 + 8 'gardenHalfWidth': " << *intPtr << std::endl;
		//std::cout << "Float value at a2 + 8 'gardenHalfWidth': " << *reinterpret_cast<float*>(a2 + 8) << std::endl;

	}
	return result;
}

int __stdcall hooks::GetUITexture_007DEA60(int a1, wchar_t* Str, DWORD* a3, DWORD* a4) noexcept {
	int result = GetUITexture_007DEA60_Original(a1, Str, a3, a4);
	//std::wcout << L"Texture Loading: " << (Str ? Str : L"nullptr") << std::endl;
	return result;
}

int __cdecl hooks::GetPinataIDs_0070D680(int a1) noexcept{
	int result = GetPinataIDs_0070D680_Original(a1);
	//std::cout << "Get ID: " << result << " Aka: " << g_IDNames[result] << std::endl;
	return result;
}

DWORD* __fastcall hooks::SetGlobalSaveStruct_007F8FB0(DWORD* thisptr) noexcept {

	DWORD* result = SetGlobalSaveStruct_007F8FB0_Original(thisptr);
	return result;

	/* thisptr 
		XMLSave[0] = sub_7F30E0;
		XMLSave[1] = sub_7F3680;
		XMLSave[2] = sub_7F3E70;
		XMLSave[3] = nullsub_44;
		XMLSave[4] = 0;
		CompressedSave[0] = sub_7F42C0;
		CompressedSave[1] = sub_7F4560;
		CompressedSave[2] = sub_7F48C0;
		CompressedSave[3] = nullsub_45;
		CompressedSave[4] = 1;
	*/
	//Force thisptr to be XMLSave
	//thisptr[0] = 0x007F30E0; // sub_7F30E0
	//thisptr[1] = 0x007F3680; // sub_7F3680
	//thisptr[2] = 0x007F3E70; // sub_7F3E70
	//thisptr[3] = 0x0044B1A0; // nullsub_44 (This is a null function, so we can use any valid address)
	//thisptr[4] = 0; // 0 for XMLSave
	//DWORD* result = SetGlobalSaveStruct_007F8FB0_Original(thisptr);
	//return result;
}

int __cdecl hooks::sub_7F0EB0(int a1, int a2, int a3) noexcept
{
	int result = sub_7F0EB0_Original(a1, a2, a3);

	char* str = reinterpret_cast<char*>(a3);

	//std::cout << "Tag: " << str << "a1: " << a1 << "a2: " << a2 << std::endl;

	return result;
}

BOOL __cdecl hooks::sub_7F2170(DWORD* a1, DWORD* a2, BYTE* a3) noexcept
{
	BOOL result = sub_7F2170_Original(a1, a2, a3);
	//int ValueAtA1_Int = (a2 && a2[0]) ? static_cast<int>(a2[0]) : 0;
	//std::cout << "Loading Tag: " << reinterpret_cast<char*>(a3) << std::endl;
	//std::cout << "Value at a1 as Int: " << ValueAtA1_Int << std::endl;
	return result;
}

void __cdecl hooks::sub_4B5B90(int a1) noexcept
{
	//g_time 
	*(float*)(a1 + 176) = g_Time;
	*(float*)(a1 + 180) = g_Time;
	//*(float*)(a1 + 4) = 0;
	sub_4B5B90_Original(a1);
	//std::cout << "Time is: " << *(int*)(a1 + 4) << std::endl;
	return;
}

int __cdecl hooks::PseudoRandom_00418FF0(int* a1, int a2, int a3) noexcept
{
	int result = PseudoRandom_00418FF0_Original(a1, a2, a3);
	return result;
}

int __cdecl hooks::sub_80E820(int a1) noexcept
{
	return *(DWORD*)(a1 + 120);
}

int __cdecl hooks::sub_957750(int a1) noexcept
{
	//std::cout << "sub_957750 called with a1: " << a1 << std::endl;
	return sub_957750_Original(a1);
}

void __cdecl hooks::sub_7D0A20(int* a1, float a2) noexcept
{
	sub_7D0A20_Original(a1, a2);
	return;
}

int __cdecl hooks::sub_4B54A0(DWORD* a1, int a2) noexcept
{
	int result = sub_4B54A0_Original(a1, a2);

	return result;
}

//These 2 functions have something to do with the wildcard random chance
int __cdecl hooks::sub_4B5430(int a1) noexcept
{
	int v2 = sub_80E820(a1);
	int v4 = sub_957750(v2);
	int TimeContext = *(DWORD*)(v4 + 13120);

	//print mem location of a1
	//std::cout << "Memory location of a1: " << std::hex << a1 << std::dec << std::endl;
	//store that offset in int
	RESULT_4B5B90 = a1;
	
	if (g_UpdateTime) {
		*(float*)(TimeContext + 4) = g_Time;
		g_UpdateTime = false;
	}

	g_Time = *(float*)(TimeContext + 4);

	int result = sub_4B5430_Original(a1);
	return result;
}

int __cdecl hooks::Tags_7F0E60(int a1, int a2, int a3) noexcept
{
	
	if (a3) {
		char* str = reinterpret_cast<char*>(a3);
		if (strcmp(str, "isWildcard") == 0) {
			//a2 = 1; //Doesn't set the pinatas to wildcard
		}
	}

	int result = Tags_7F0E60_Original(a1, a2, a3);
	return result;
}

inline bool doonce = false;
inline int indexx = 0;
int __cdecl hooks::CreateTaggedFloat(int a1, float a2, int a3) noexcept
{
	
	int result = CreateTaggedFloat_Original(a1, a2, a3);
	return result;

	
	if (a3) {
		char* str = reinterpret_cast<char*>(a3);
		if (strcmp(str, "x") == 0) {
			a2 = 0.0f; //Set x position to 0
		}
		else if (strcmp(str, "y") == 0) {
			a2 = 0.0f; //Set y position to 0
		}
		else if (strcmp(str, "z") == 0) {
			a2 = 0.0f; //Set z position to 0
		}
		else if (strcmp(str, "r") == 0) {
			a2 = 0.0f; //Set rotation to 0
		}
		else if (strcmp(str, "p") == 0) {
			a2 = 0.0f; //Set rotation to 0
		}
		else if (strcmp(str, "y") == 0) {
			a2 = 0.0f; //Set rotation to 0
		}
		else if (strcmp(str, "scale") == 0) {
			a2 = 5.0f; //Set scale to 1
		}
	}
}

int __cdecl hooks::sub_798220(int* Player, int a2, void* a3) noexcept
{
	//*(char*)(*(DWORD*)Player + 184) = 2; //Sets the player to be player 3 (multiplayer) blocks tools from being used.
	int result = sub_798220_Original(Player, a2, a3);
	return result;
}

int __cdecl hooks::GardenSpaceThreshold_007785D0(int Context, int a2, int a3) noexcept
{
	//Force all budgets to be 0
	*reinterpret_cast<int*>(Context + 4804) = 0;
	*reinterpret_cast<int*>(Context + 4808) = 0;
	*reinterpret_cast<int*>(Context + 4812) = 0;
	*reinterpret_cast<int*>(Context + 4816) = 0;
	*reinterpret_cast<int*>(Context + 4820) = 0;
	*reinterpret_cast<int*>(Context + 4824) = 0;

	//Increase all caps to 999999
	*reinterpret_cast<int*>(Context + 4932) = 999999;
	*reinterpret_cast<int*>(Context + 4936) = 999999;
	*reinterpret_cast<int*>(Context + 4940) = 999999;
	*reinterpret_cast<int*>(Context + 4944) = 999999;
	*reinterpret_cast<int*>(Context + 4948) = 999999;
	*reinterpret_cast<int*>(Context + 4952) = 999999;
	return 1;
}













