#pragma once
#include "../Hook.h"

class PrintHooks : public Hook
{
	public:

		void Setup() override;

}; 

namespace Hooks {
	// Binds a print function at offset 0x0073FB80 
	using PrintLineFn_00405740 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00405740 PrintFunc_00405740_Original = nullptr;
	void __cdecl PrintFunc_00405740(const char* format, ...) noexcept;

	// Binds a print function at offset 0x0073FB80 
	using PrintLineFn_00405790 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00405790 PrintFunc_00405790_Original = nullptr;
	void __cdecl PrintFunc_00405790(const char* format, ...) noexcept;

	// Binds a print function at offset 0x004055e0
	using PrintLineFn_004055e0 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_004055e0 PrintFunc_004055e0_Original = nullptr;
	void __cdecl PrintFunc_004055e0(const char* format, ...) noexcept;

	// Binds a print function at offset 0x00402bf0
	using PrintLineFn_00402bf0 = void(__cdecl*)(const char*, ...) noexcept;
	inline PrintLineFn_00402bf0 PrintFunc_00402bf0_Original = nullptr;
	void __cdecl PrintFunc_00402bf0(const char* format, ...) noexcept;

	// Binds a print function at offset 0x00851720
	using PrintLineFn_00851720 = void(__cdecl*)(wchar_t*, rsize_t, const wchar_t*, ...) noexcept;
	inline PrintLineFn_00851720 PrintFunc_00851720_Original = nullptr;
	// Not actually a function for printing, it's used for parsing file strings.
	void __cdecl PrintFunc_00851720(wchar_t* buffer, rsize_t bufsz, const wchar_t* format, ...) noexcept;

	//void Print_490810(char a1, char *Format, ...)
	using PrintLineFn_490810 = void(__cdecl*)(char, char*, ...) noexcept;
	inline PrintLineFn_490810 PrintFunc_490810_Original = nullptr;
	void __cdecl PrintFunc_490810(char a1, char* Format, ...) noexcept;
};
