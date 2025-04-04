#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <cstdint>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "hooks.h"


// Function to set up the console
void SetupConsole()
{
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONOUT$", "w", stderr);
	freopen_s(&file, "CONIN$", "r", stdin);

	// Set the console code page to UTF-8 so that we can use Unicode characters
	SetConsoleOutputCP(CP_UTF8);
	std::ios::sync_with_stdio();
}


// setup function
void Setup(const HMODULE instance)
{
	SetupConsole(); // Allows us to log messages to a console

	try
	{
		std::cout << "Starting GUI setup..." << std::endl;
		gui::Setup();
		std::cout << "GUI setup complete" << std::endl;
		std::cout << "Starting hooks setup..." << std::endl;
		hooks::Setup();
		std::cout << "Hooks setup complete" << std::endl;
	}
	catch (const std::exception& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"hack error",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

UNLOAD:
	std::cout << "Unloading hooks..." << std::endl;
	hooks::Destroy();
	std::cout << "Destroying GUI..." << std::endl;
	gui::Destroy();

	std::cout << "Freeing Library and unloading DLL..." << std::endl;
	FreeLibraryAndExitThread(instance, 0);
}

// entry point
BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), // Runs the setup function on a new thread
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return TRUE;
}