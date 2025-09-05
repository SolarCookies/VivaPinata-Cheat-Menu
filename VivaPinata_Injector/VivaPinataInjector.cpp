#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <cstdint>
#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "hooks.h"


// Custom stream buffer that writes to two buffers (console + file)
class TeeBuf : public std::streambuf {
	std::streambuf* sb1;
	std::streambuf* sb2;

public:
	TeeBuf(std::streambuf* buf1, std::streambuf* buf2) : sb1(buf1), sb2(buf2) {}

protected:
	int overflow(int c) override {
		if (c == EOF) return !EOF;
		if (sb1->sputc(c) == EOF) return EOF;
		if (sb2->sputc(c) == EOF) return EOF;
		return c;
	}

	int sync() override {
		return (sb1->pubsync() == 0 && sb2->pubsync() == 0) ? 0 : -1;
	}
};

std::ofstream logfile;
TeeBuf* teeBuf;
std::ostream* out;

void SetupConsole()
{
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONOUT$", "w", stderr);
	freopen_s(&file, "CONIN$", "r", stdin);

	// Set console to UTF-8
	SetConsoleOutputCP(CP_UTF8);
	std::ios::sync_with_stdio();

	// Open log file
	logfile.open("plugins/VivaPinata_ModMenu.log", std::ios::out | std::ios::trunc);

	// Create tee stream (console + file)
	teeBuf = new TeeBuf(std::cout.rdbuf(), logfile.rdbuf());
	out = new std::ostream(teeBuf);

	// Redirect std::cout and std::cerr to our tee stream
	std::cout.rdbuf(teeBuf);
	std::cerr.rdbuf(teeBuf);
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