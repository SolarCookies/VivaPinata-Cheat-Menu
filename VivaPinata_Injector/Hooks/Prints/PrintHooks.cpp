#pragma once
#include "PrintHooks.h"

void PrintHooks::Setup() {

	// Hook the various print functions.
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00405740),
		&Hooks::PrintFunc_00405740,
		reinterpret_cast<void**>(&Hooks::PrintFunc_00405740_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00405740()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00405790),
		&Hooks::PrintFunc_00405790,
		reinterpret_cast<void**>(&Hooks::PrintFunc_00405790_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00405790()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x004055e0),
		&Hooks::PrintFunc_004055e0,
		reinterpret_cast<void**>(&Hooks::PrintFunc_004055e0_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_004055e0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00402bf0),
		&Hooks::PrintFunc_00402bf0,
		reinterpret_cast<void**>(&Hooks::PrintFunc_00402bf0_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_00402bf0()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00490810),
		&Hooks::PrintFunc_490810,
		reinterpret_cast<void**>(&Hooks::PrintFunc_490810_Original)
	)) throw std::runtime_error("Unable to hook PrintFunc_490810()");

	//std::cout << "Print functions hooked successfully." << std::endl;


}

void __cdecl Hooks::PrintFunc_00405740(const char* format, ...) noexcept
{
	PrintFunc_00405740_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	//std::cout << strToPrint << std::endl;
}

void __cdecl Hooks::PrintFunc_00405790(const char* format, ...) noexcept
{
	PrintFunc_00405790_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	//std::cout << strToPrint << std::endl;
}

void __cdecl Hooks::PrintFunc_004055e0(const char* format, ...) noexcept
{
	PrintFunc_004055e0_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	//std::cout << strToPrint << std::endl;
}

void __cdecl Hooks::PrintFunc_00402bf0(const char* format, ...) noexcept
{
	PrintFunc_00402bf0_Original(format);

	char strToPrint[512];
	va_list argptr;
	va_start(argptr, format);
	vsprintf(strToPrint, format, argptr);
	va_end(argptr);

	//std::cout << strToPrint << std::endl;
}

// Not actually a function for printing, it's used for parsing file strings.
void __cdecl Hooks::PrintFunc_00851720(wchar_t* buffer, rsize_t bufsz, const wchar_t* format, ...) noexcept
{
	va_list argptr;
	va_start(argptr, format);
	int output = _vsnwprintf_s(buffer, bufsz, bufsz - 1, format, argptr);
	va_end(argptr);

	if (output == -1) {
		buffer[bufsz - 1] = '\0';
	}

	//std::wcout << buffer << std::endl;

	//PrintFunc_00851720_Original(buffer, bufsz, buffer); Don't need to call the original function, hook does the same job.

}

void __cdecl Hooks::PrintFunc_490810(char a1, char* Format, ...) noexcept {
	if (a1 != 0) return;

	char Buffer[512];
	va_list args;
	va_start(args, Format);
	vsprintf_s(Buffer, sizeof(Buffer), Format, args);
	va_end(args);

	//std::cout << Buffer << std::endl;

	PrintFunc_490810_Original(a1, Format);
}
