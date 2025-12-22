#include "AssetHooks.h"
#include "../../Patches.h"
#include <windows.h>

// Helper function for safe memory reading with SEH
static bool SafeReadInt(uintptr_t address, int* outValue) {
	__try {
		*outValue = *(int*)address;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

static int ScanAndReplaceMemoryOffsets(int oldOffset, int newOffset) {
	// Scan entire user-mode address space (0x00010000 to 0x7FFFFFFF)
	uintptr_t scanStart = 0x00010000;
	uintptr_t scanEnd = 0x7FFFFFFF;

	MEMORY_BASIC_INFORMATION mbi;
	uintptr_t address = scanStart;
	int replacementCount = 0;
	int foundCount = 0;

	std::cout << "Scanning for old offset: 0x" << std::hex << oldOffset << std::dec << std::endl;

	while (address < scanEnd) {
		if (VirtualQuery((void*)address, &mbi, sizeof(mbi)) == 0) {
			address += 0x1000;
			continue;
		}

		// Only scan committed, readable memory
		if (mbi.State == MEM_COMMIT &&
			(mbi.Protect == PAGE_READWRITE ||
				mbi.Protect == PAGE_EXECUTE_READWRITE ||
				mbi.Protect == PAGE_READONLY ||
				mbi.Protect == PAGE_EXECUTE_READ) &&
			!(mbi.Protect & PAGE_GUARD) &&
			!(mbi.Protect & PAGE_NOACCESS)) {

			uintptr_t regionStart = (uintptr_t)mbi.BaseAddress;
			size_t regionSize = mbi.RegionSize;

			// Scan this region for the old offset value
			for (uintptr_t scanAddr = regionStart;
				scanAddr < regionStart + regionSize - sizeof(int);
				scanAddr += 1) {

				int value;
				if (SafeReadInt(scanAddr, &value) && value == oldOffset) {
					foundCount++;
					std::cout << "Found at address: 0x" << std::hex << scanAddr << std::dec << std::endl;

					// Try to make it writable and replace
					DWORD oldProtect;
					if (VirtualProtect((void*)scanAddr, sizeof(int), PAGE_READWRITE, &oldProtect)) {
						*(int*)scanAddr = newOffset;
						VirtualProtect((void*)scanAddr, sizeof(int), oldProtect, &oldProtect);
						replacementCount++;
						std::cout << "  -> Successfully replaced!" << std::endl;
					}
					else {
						std::cout << "  -> VirtualProtect failed, error: " << GetLastError() << std::endl;
					}
				}
			}
		}

		address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
	}

	std::cout << "Found " << foundCount << " occurrences, successfully replaced "
		<< replacementCount << " of them" << std::endl;
	std::cout << "Old offset: 0x" << std::hex << oldOffset
		<< " -> New offset: 0x" << newOffset << std::dec << std::endl;

	return replacementCount;
}

void AssetHooks::Setup()
{
	std::cout << "Setting up Asset hooks..." << std::endl;

	// Hook the various Asset management functions.

	//FindAssetVDATptr_008BD640
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008BD640),
			&Hooks::FindAssetVDATptr_008BD640,
			reinterpret_cast<void**>(&Hooks::FindAssetVDATptr_008BD640_Original)
		)) throw std::runtime_error("Unable to hook FindAssetVDATptr_008BD640()");
	}

	//FindAssetWithTag_008BC290
	if (1) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008BC290),
			&Hooks::FindAssetWithTag_008BC290,
			reinterpret_cast<void**>(&Hooks::FindAssetWithTag_008BC290_Original)
		)) throw std::runtime_error("Unable to hook FindAssetWithTag_008BC290()");
	}

	//sub_8B8750
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008B8750),
			&Hooks::sub_8B8750,
			reinterpret_cast<void**>(&Hooks::sub_8B8750_Original)
		)) throw std::runtime_error("Unable to hook sub_8B8750()");
	}

	//FindPackageTableEntry
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x0086C760),
			&Hooks::FindPackageTableEntry_0086C760,
			reinterpret_cast<void**>(&Hooks::FindPackageTableEntry_Original)
		)) throw std::runtime_error("Unable to hook FindPackageTableEntry_86C760()");
	}

	//BeginPackageLoad_008B8170
	if (0) { //crashes when enabled, only works for first few packages good for researching
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008B8170),
			&Hooks::BeginPackageLoad_008B8170,
			reinterpret_cast<void**>(&Hooks::BeginPackageLoad_008B8170_Original)
		)) throw std::runtime_error("Unable to hook BeginPackageLoad_008B8170()");
	}

	//Hash_8BC9E0
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008BC9E0),
			&Hooks::Hash_8BC9E0,
			reinterpret_cast<void**>(&Hooks::Hash_8BC9E0_Original)
		)) throw std::runtime_error("Unable to hook Hash_8BC9E0()");
	}

	//PackageLoad_8B6D00
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x008B6D00),
			&Hooks::PackageLoad_8B6D00,
			reinterpret_cast<void**>(&Hooks::PackageLoad_008B6D00_Original)
		)) throw std::runtime_error("Unable to hook PackageLoad_8B6D00()");
	}

	// sub_93AE80
	if (1) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x0093AE80),
			&Hooks::sub_93AE80,
			reinterpret_cast<void**>(&Hooks::sub_93AE80_Original)
		)) throw std::runtime_error("Unable to hook sub_93AE80()");
	}
}

/// <summary>
/// These next 2 functions have something to do with loading assets, specifically i believe they find/load the VDAT files with the tag name.
/// </summary>
int __cdecl Hooks::FindAssetVDATptr_008BD640(char* FileName, int** a2, DWORD* a3, int a4, char* a5, int a6) noexcept
{
	int result = FindAssetVDATptr_008BD640_Original(FileName, a2, a3, a4, a5, a6);
	return result;
}

int __cdecl Hooks::FindAssetWithTag_008BC290(char* Source) noexcept
{

	int result = FindAssetWithTag_008BC290_Original(Source);

	if (Dumpvdata || Loadvdata) {
		// Use the internal function at 0x008BBFC0 to get the asset reference
		using sub_8BBFC0_t = unsigned int(__fastcall*)(int, char*, char*, char**);
		auto sub_8BBFC0 = reinterpret_cast<sub_8BBFC0_t>(0x008BBFC0);
		char* assetRef = nullptr;
		unsigned int result2 = sub_8BBFC0(0, 0, Source, &assetRef);

		//assetRef is the location of the asset data, it contains the tag at the start but more crucially at +136 there is a pointer to a map that contains the data start and size

		if (assetRef) {
			int mapPtr = *(int*)(assetRef + 136);

			//At offset 0 of the map is the data start offset
			int datastart = *(int*)(mapPtr + 0);
			//at offset 4 of the map is the data size
			int datasize = *(int*)(mapPtr + 4);

			std::vector<char> assetData1;
			assetData1.resize(datasize);
			//copy data from datastart to assetData
			std::memcpy(assetData1.data(), (char*)datastart, datasize);


			//dump data to file for analysis
			// /dumped_assets/<Source>_data.vdat
			std::string outDir = "dumped_assets";
			std::filesystem::create_directories(outDir);
			std::string outFile = outDir + "/" + std::string(Source) + ".vdat";

			if (Dumpvdata) {
				// if file exists ignore, this prevents overwriting modified files and redundant dumps
				if (std::filesystem::exists(outFile)) {
					//std::cerr << "File already exists, skipping dump: " << outFile << std::endl;
					goto ELSE;
				}
				std::ofstream ofs(outFile, std::ios::binary);
				if (ofs.is_open()) {
					//write
					ofs.write(assetData1.data(), datasize);
					ofs.close();

				}
			}
		ELSE:
			if (Loadvdata) {
				//check to see if asset is already loaded
				uint32_t namehash = CRC32Hash(Source);
				//check assetMap
				if (assetMap.find(namehash) != assetMap.end()) {
					//already loaded
					return result;
				}
				else {
					//std::cout << "Loading asset from file: " << outFile << std::endl;
					// 
					// if file dosnt exist ignore
					if (!std::filesystem::exists(outFile)) {
						return result;
					}

					//load data from file
					std::ifstream ifs(outFile, std::ios::binary);
					if (ifs.is_open()) {
						ifs.seekg(0, std::ios::end);              // Seek to end
						std::streamsize fileSize = ifs.tellg();   // Get file size
						ifs.seekg(0, std::ios::beg);

						uint32_t offsetBefore = assetData.size();
						assetData.resize(offsetBefore + fileSize); // Resize before reading!
						ifs.read(reinterpret_cast<char*>(assetData.data() + offsetBefore), fileSize);
						ifs.close();

						AssetVDAT dataEntry;

						dataEntry.Size = static_cast<int>(assetData.size()) - offsetBefore;
						dataEntry.Offset = reinterpret_cast<uint32_t>(assetData.data() + offsetBefore);
						dataEntry.Name = Source;
						dataEntry.NameHash = namehash;
						assetMap[namehash] = dataEntry;
						std::cout << "Old size: " << *(int*)(mapPtr + 4) << " New size: " << assetData.size() << std::endl;
						std::cout << "Old Offset: " << *(int*)(mapPtr + 0) << " New Offset: " << dataEntry.Offset << std::endl;

						// 00 out old data block at offset *(int*)(mapPtr + 0) with the size of *(int*)(mapPtr + 4)
						//for (int i = 0; i < *(int*)(mapPtr + 4); ++i) {
						//	*((char*)(*(int*)(mapPtr + 0) + i)) = 0;
						//}


						//int replacementCount = ScanAndReplaceMemoryOffsets(*(int*)(mapPtr + 0), dataEntry.Offset);

						ReplaceOffsetsMap[*(int*)(mapPtr + 0)] = dataEntry.Offset;
						*(int*)(mapPtr + 0) = dataEntry.Offset;
						*(int*)(mapPtr + 4) = dataEntry.Size;

					}
				}
			}


			/*
			int unk1 = *(int*)(mapPtr + 8);
			int unk2 = *(int*)(mapPtr + 12);
			int unk3 = *(int*)(mapPtr + 16);
			int unk4 = *(int*)(mapPtr + 20);
			int unk5 = *(int*)(mapPtr + 24);
			std::cout << "--------------" << Source << "--------------" << std::endl;
			std::cout << "Data Start: " << std::hex << datastart << std::dec << std::endl;
			std::cout << "Data Size: " << datasize << std::endl;
			std::cout << "unk1: " << unk1 << std::endl;
			std::cout << "unk2: " << unk2 << std::endl;
			std::cout << "unk3: " << unk3 << std::endl;
			std::cout << "unk4: " << unk4 << std::endl;
			std::cout << "unk5: " << unk5 << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			*/
		}
	}



	//std::cout << "FindAssetWithTag_008BC290 called with Source: " << (Source ? Source : "nullptr") << " DataStart: " << datastart << " DataSize: " << datasize << std::endl;

	return result;
}

//unknown function related to asset loading more debugging needed
int __cdecl Hooks::sub_8B8750(int a1, DWORD* a2, int a3) noexcept
{
	int result = sub_8B8750_Original(a1, a2, a3);
	return result;
}

unsigned int __cdecl Hooks::FindPackageTableEntry_0086C760(int tableInfo, int first, int last, int(__cdecl* CompareFn)(unsigned int, int), int pkgNumber) noexcept
{
	//pkgNumber is a address to the actual package number in memory so its actually an int*
	//std::cout << " pkgNumber: " << *(int*)pkgNumber << std::endl;
	return FindPackageTableEntry_Original(tableInfo, first, last, CompareFn, pkgNumber);
}

int __cdecl Hooks::BeginPackageLoad_008B8170(int PKG_Number, int a2, int a3, int a4)
{
	//std::cout << "Loading Package: " << PKG_Number << std::endl;
	return BeginPackageLoad_008B8170_Original(PKG_Number, a2, a3, a4);
}

int __cdecl Hooks::Hash_8BC9E0(int String) noexcept
{
	int hash = Hash_8BC9E0_Original(String);
	std::cout << (char*)String << "," << std::hex << hash << std::dec << std::endl;
	return hash;
}

DWORD* __cdecl Hooks::PackageLoad_8B6D00(int bundleMgr, DWORD* ctx, int entry) noexcept
{
	//print first 100 bytes of ctx as hex
	std::cout << "PackageLoad_8B6D00 called with bundleMgr: " << bundleMgr << " ctx: " << ctx << " entry: " << entry << std::endl;
	std::cout << "ctx data (first 100 bytes): ";
	for (size_t i = 0; i < 100; ++i) {
		unsigned char b = *((unsigned char*)ctx + i);
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
	}
	DWORD* Asset = PackageLoad_008B6D00_Original(bundleMgr, ctx, entry);
	
	return Asset;
}

int __cdecl Hooks::sub_93AE80(DWORD* a1, DWORD* a2) noexcept
{
	//v8 is the offset to the requirements data
	int v7 = a1[752];
	int* v8 = *(int**)(v7 + 12);

	//check to see if v8 is in the ReplaceOffsetsMap as a key
	if (ReplaceOffsetsMap.find((int)*(int**)(v7 + 12)) != ReplaceOffsetsMap.end()) {
		//we want to set the original a1[752] + 12 to the new offset
		std::cout << "Replacing requirements offset from " << std::hex << (int)*(int**)(v7 + 12) << " to " << ReplaceOffsetsMap[(int)*(int**)(v7 + 12)] << std::dec << std::endl;
		*(int**)(v7 + 12) = (int*)ReplaceOffsetsMap[(int)*(int**)(v7 + 12)];

	}
	
	return sub_93AE80_Original(a1, a2);
}


