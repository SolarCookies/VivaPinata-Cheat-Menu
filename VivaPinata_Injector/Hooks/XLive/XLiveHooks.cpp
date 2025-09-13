#pragma once
#include "XLiveHooks.h"
#include <iostream>

void XLiveHooks::Setup() {
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x00686746),
		&Hooks::XShowGamerCardUI_5252,
		reinterpret_cast<void**>(&Hooks::XShowGamerCardUI_5252_Original)
	)) throw std::runtime_error("Unable to hook XShowGamerCardUI_5252()");

	if (MH_CreateHook(
		reinterpret_cast<void*>(0x008499E0),
		&Hooks::sub_8499E0,
		reinterpret_cast<void**>(&Hooks::sub_8499E0_Original)
	)) throw std::runtime_error("Unable to hook sub_8499E0()");

    if (MH_CreateHook(
        reinterpret_cast<void*>(0x006B3490),
        &Hooks::sub_6B3490,
        reinterpret_cast<void**>(&Hooks::sub_6B3490_Original)
	)) throw std::runtime_error("Unable to hook sub_6B3490()");
}

int __stdcall Hooks::XShowGamerCardUI_5252(int a1, int a2, int a3)
{
	std::cout << "XShowGamerCardUI_5252 Called: " << a1 << ", " << a2 << ", " << a3 << std::endl;
	//int result = Hooks::XShowGamerCardUI_5252_Original(a1, a2, a3);
	int result = 0; // force success
	return result;
}

void __cdecl Hooks::sub_8499E0(uint32_t* a1 /*base pointer*/, int new_state)
{
	new_state = 2; // force state to 2 This makes the game only have the players username rather then try to fetch the friends list from live
	Hooks::sub_8499E0_Original(a1, new_state); // call original first (optional)
    return;
    
}

int __fastcall Hooks::sub_6B3490(void* _this, void* unused, int a2)
{
	return Hooks::sub_6B3490_Original(_this, unused, a2);
}
