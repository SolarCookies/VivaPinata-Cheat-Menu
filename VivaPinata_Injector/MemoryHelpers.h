#pragma once  
#include <cstdint>  
#include <libloaderapi.h>  
#include <cstdlib>
#include <vector>

//This header file is to help set and get variables in program memory ie "get "Viva Pinata.exe"+5F63E8 type double"  


inline static uintptr_t GetVivaAddressPtr(const char* address) noexcept {
	uintptr_t addressValue = reinterpret_cast<uintptr_t>(GetModuleHandleA("Viva Pinata.exe")) + std::strtoul(address, nullptr, 16);
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

inline static bool IsPatchEnabled(const SimpleMemoryPatch& patch) noexcept {
	//check to see if bytes at address are equal to the enabled bytes
	for (size_t i = 0; i < patch.enabledbytes.size(); ++i) {
		if (*(byte*)(patch.address + i) != patch.enabledbytes[i]) {
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
	if (enable) {
		//set bytes at address to enabled bytes
		for (size_t i = 0; i < patch.enabledbytes.size(); ++i) {
			*(byte*)(patch.address + i) = patch.enabledbytes[i];
		}
	}
	else {
		//set bytes at address to disabled bytes
		for (size_t i = 0; i < patch.disabledbytes.size(); ++i) {
			*(byte*)(patch.address + i) = patch.disabledbytes[i];
		}
	}
}
inline static void SetPatch(ComplexMemoryPatch& patch, bool enable) noexcept {
	if (enable) {
		//set bytes at address to enabled bytes
		for (SimpleMemoryPatch& p : patch.patches) {
			SetPatch(p, true);
		}
	}
	else {
		//set bytes at address to disabled bytes
		for (SimpleMemoryPatch& p : patch.patches) {
			SetPatch(p, false);
		}
	}
}

inline static double GetDouble(const char* address) noexcept {
   uintptr_t addressValue = GetVivaAddressPtr(address);
   double value = *reinterpret_cast<double*>(addressValue);  
   return value;  
}

inline static void SetDouble(const char* address, double value) noexcept {
	uintptr_t addressValue = GetVivaAddressPtr(address);
	*reinterpret_cast<double*>(addressValue) = value;
}