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
	
	if (MH_CreateHook(
		reinterpret_cast<void*>(0x007785D0),
		&GardenSpaceThreshold_007785D0,
		reinterpret_cast<void**>(&GardenSpaceThreshold_007785D0_Original)
	)) throw std::runtime_error("Unable to GardenSpaceThreshold_007785D0()");

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

//Unlimited Garden Space
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














