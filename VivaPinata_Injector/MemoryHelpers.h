#pragma once  
#include <cstdint>  
#include <libloaderapi.h>  
#include <cstdlib>
#include <vector>

//This header file is to help set and get variables in program memory ie "get "Viva Pinata.exe"+5F63E8 type double"  


inline static uintptr_t GetVivaAddressPtr(uintptr_t address) noexcept {
	//Get offset of the viva pinata.exe + the address offset
	//module handle
	HMODULE moduleHandle = GetModuleHandleA("Viva Pinata.exe");
	if (!moduleHandle) {
		throw std::runtime_error("Failed to get Viva Pinata.exe' module handle");
	}
	uintptr_t VivaPinataBaseAddress = reinterpret_cast<uintptr_t>(moduleHandle);
	uintptr_t addressValue = VivaPinataBaseAddress + address;
	return addressValue;
}



static struct SimpleMemoryPatch {
	uintptr_t address;
	std::vector<byte> disabledbytes;
	std::vector<byte> enabledbytes;
};

static struct ComplexMemoryPatch {
	std::vector<SimpleMemoryPatch> patches;
};

namespace MemHelp {


	inline static bool IsPatchEnabled(const SimpleMemoryPatch& patch) noexcept {
		//check to see if bytes at address are equal to the enabled bytes
		uintptr_t addressValue = GetVivaAddressPtr(patch.address);
		for (size_t i = 0; i < patch.enabledbytes.size(); ++i) {
			if (*reinterpret_cast<byte*>(addressValue + i) != patch.enabledbytes[i]) {
				return false;
			}
		}
	}
	inline static bool IsPatchEnabled(const ComplexMemoryPatch& patch) noexcept {
		//check to see if bytes at address are equal to the enabled bytes
		for (const auto& p : patch.patches) {
			if (!IsPatchEnabled(p)) {
				return false;
			}
		}
		return true;
	}

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
	inline static void SetPatch(ComplexMemoryPatch& patch, bool enable) noexcept {
		for (auto& p : patch.patches) {
			SetPatch(p, enable);
		}
	}

	inline static double GetDouble(uintptr_t address) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		return *reinterpret_cast<double*>(addressValue);
	}

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

	inline static int GetInt(uintptr_t address) noexcept {
		uintptr_t addressValue = GetVivaAddressPtr(address);
		return *reinterpret_cast<int*>(addressValue);
	}

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