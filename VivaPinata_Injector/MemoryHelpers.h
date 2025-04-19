#pragma once  
#include <cstdint>  
#include <libloaderapi.h>  
#include <cstdlib>
#include <vector>


//Gets the address of the viva pinata.exe module and add the address offset to it
inline static uintptr_t GetVivaAddressPtr(uintptr_t address) noexcept {

	HMODULE moduleHandle = GetModuleHandleA("Viva Pinata.exe");

	//Make sure the viva pinata module handle is valid before using it
	if (!moduleHandle) {
		throw std::runtime_error("Failed to get Viva Pinata.exe' module handle");
	}

	//Convert the module handle to a pointer
	uintptr_t VivaPinataBaseAddress = reinterpret_cast<uintptr_t>(moduleHandle);

	//Add the address offset to the base address of viva pinata
	uintptr_t addressValue = VivaPinataBaseAddress + address;

	return addressValue;
}

//Simple Memory Patches are used to patch only one address at a time, They store both the enabled and disabled bytes that way we can restore the original bytes if toggled
static struct SimpleMemoryPatch {
	uintptr_t address;
	std::vector<byte> disabledbytes;
	std::vector<byte> enabledbytes;
};

//Complex Memory Patches are used to patch multiple addresses at a time
static struct ComplexMemoryPatch {
	std::vector<SimpleMemoryPatch> patches;
};


namespace MemHelp {

	//Checks if the patch is enabled by checking if the bytes at the address are equal to the enabled bytes
	inline static bool IsPatchEnabled(const SimpleMemoryPatch& patch) noexcept {

		//Gets the address of the patch relative to the module handle of viva pinata
		uintptr_t addressValue = GetVivaAddressPtr(patch.address);

		//For each bytes in the enabled patch, check to see if its equal to the bytes at the address if not we know the patch is not enabled
		for (size_t i = 0; i < patch.enabledbytes.size(); ++i) {

			if (*reinterpret_cast<byte*>(addressValue + i) != patch.enabledbytes[i]) {
				return false;
			}

		}
	}

	//Checks if the patch is enabled by checking if the bytes at the address are equal to the enabled bytes but this time it has a for loop :O
	inline static bool IsPatchEnabled(const ComplexMemoryPatch& patch) noexcept {
		//check to see if bytes at address are equal to the enabled bytes
		for (const auto& p : patch.patches) {
			if (!IsPatchEnabled(p)) {
				return false;
			}
		}
		return true;
	}

	//Sets bytes at the patch address based on if it should be enabled or disabled
	inline static void SetPatch(SimpleMemoryPatch& patch, bool enable) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(patch.address);
		if (enable) {
			for (size_t i = 0; i < patch.enabledbytes.size(); ++i) {

				//make address writable
				DWORD oldProtect;
				VirtualProtect(reinterpret_cast<LPVOID>(addressValue + i), sizeof(byte), PAGE_EXECUTE_READWRITE, &oldProtect);

				//set the value
				*reinterpret_cast<byte*>(addressValue + i) = patch.enabledbytes[i];

				//restore the old protection
				VirtualProtect(reinterpret_cast<LPVOID>(addressValue + i), sizeof(byte), oldProtect, &oldProtect);
			}
		}
		else {
			for (size_t i = 0; i < patch.disabledbytes.size(); ++i) {

				//make address writable
				DWORD oldProtect;
				VirtualProtect(reinterpret_cast<LPVOID>(addressValue + i), sizeof(byte), PAGE_EXECUTE_READWRITE, &oldProtect);

				//set the value
				*reinterpret_cast<byte*>(addressValue + i) = patch.disabledbytes[i];

				//restore the old protection
				VirtualProtect(reinterpret_cast<LPVOID>(addressValue + i), sizeof(byte), oldProtect, &oldProtect);
			}
		}
	}

	//Sets bytes at the patch address based on if it should be enabled or disabled but this time it has a for loop :O
	inline static void SetPatch(ComplexMemoryPatch& patch, bool enable) noexcept {
		for (auto& p : patch.patches) {
			SetPatch(p, enable);
		}
	}

	//Gets Double at the address relative to the module handle of viva pinata
	inline static double GetDouble(uintptr_t address) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		return *reinterpret_cast<double*>(addressValue);
	}

	//Sets Double at the address relative to the module handle of viva pinata
	inline static void SetDouble(uintptr_t address, double value) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		//make address writable
		DWORD oldProtect;
		VirtualProtect(reinterpret_cast<LPVOID>(addressValue), sizeof(double), PAGE_EXECUTE_READWRITE, &oldProtect);
		//set the value
		*reinterpret_cast<double*>(addressValue) = value;
		//restore the old protection
		VirtualProtect(reinterpret_cast<LPVOID>(addressValue), sizeof(double), oldProtect, &oldProtect);
	}

	//Gets Int at the address relative to the module handle of viva pinata
	inline static int GetInt(uintptr_t address) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		return *reinterpret_cast<int*>(addressValue);
	}

	//Sets Int at the address relative to the module handle of viva pinata
	inline static void SetInt(uintptr_t address, int value) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		//make address writable
		DWORD oldProtect;
		VirtualProtect(reinterpret_cast<LPVOID>(addressValue), sizeof(int), PAGE_EXECUTE_READWRITE, &oldProtect);
		*reinterpret_cast<int*>(addressValue) = value;
		//restore the old protection
		VirtualProtect(reinterpret_cast<LPVOID>(addressValue), sizeof(int), oldProtect, &oldProtect);
	}


}