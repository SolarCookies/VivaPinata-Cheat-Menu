#include "gui.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	std::cout << "Creating temp window class" << std::endl;
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	std::cout << "Destroying temp window class" << std::endl;
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	std::cout << "Creating temp window" << std::endl;
	// create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;

	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window) {
		std::cout << "Destroying temp window" << std::endl;
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX(std::string* ErrorMessageHandle) noexcept
{
	std::cout << "Setting up DirectX device" << std::endl;

	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle) {
		std::cout << "Failed to get d3d9.dll handle" << std::endl;
		ErrorMessageHandle->append("Failed to get d3d9.dll handle");
		return false;
	}
	else {
		std::cout << "Got d3d9.dll handle successfully" << std::endl;
	}
		

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create) {
		std::cout << "Failed to get Direct3DCreate9 address" << std::endl;
		ErrorMessageHandle->append("Failed to get Direct3DCreate9 address");
		return false;
	}
	else {
		std::cout << "Got Direct3DCreate9 address successfully" << std::endl;
	}

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9) {
		std::cout << "Failed to create d3d9" << std::endl;
		ErrorMessageHandle->append("Failed to create d3d9");
		return false;
	}
	else {
		std::cout << "Created d3d9 successfully" << std::endl;
	}

	std::cout << "Creating DirectX device" << std::endl;
	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) {
		std::cout << "Failed to create DirectX device" << std::endl;
		ErrorMessageHandle->append("Failed to create device");
		return false;
	}
	else {
		std::cout << "Created DirectX device successfully" << std::endl;
	}

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		std::cout << "Destroying DirectX device" << std::endl;
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		std::cout << "Destroying d3d9" << std::endl;
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	
	if (!SetupWindowClass("hackClass001"))
		std::cout << "Failed to create window class" << std::endl;

	if (!SetupWindow("Hack Window"))
		std::cout << "Failed to create window" << std::endl;

	std::string errorMessage;
	if (!SetupDirectX(&errorMessage)) {
		std::cout << "Failed to setup DirectX: ";
		std::cout << errorMessage << std::endl;
	}


	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept  
{  
	std::cout << "Setting up ImGUI menu" << std::endl;
	std::cout << "With Device: " << device << std::endl;

   if (!device) {  
	   std::cout << "Invalid device passed to SetupMenu" << std::endl;
       return;  
   }  

   auto params = D3DDEVICE_CREATION_PARAMETERS{ };  
   if (FAILED(device->GetCreationParameters(&params))) { 
	   std::cout << "Failed to get device creation parameters" << std::endl;
       return;  
   }  

   window = params.hFocusWindow;  
   std::cout << "Window: " << window << std::endl;

   originalWindowProcess = reinterpret_cast<WNDPROC>(  
       SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))  
   );  

   if (!originalWindowProcess) {  
	   std::cout << "Failed to set window long pointer" << std::endl; 
       return;  
   }  

   ImGui::CreateContext();  
   ImGui::StyleColorsDark();  

   ImGuiIO& io = ImGui::GetIO(); 

   ImGui_ImplWin32_Init(window);
   
   ImGui_ImplDX9_Init(device);

   setup = true;  
   std::cout << "ImGUI menu setup complete" << std::endl;
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// retore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::RenderImGUI() noexcept
{

	ImGui::Begin("cool menu", &open);
	ImGui::Text("Hello world!");
	ImGui::End();

	ImGui::ShowDemoWindow();
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toogle menu
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		gui::open = !gui::open;
		if (gui::open) {
			std::cout << "ImGUI Menu opened" << std::endl;
		}
		else {
			std::cout << "ImGUI Menu closed" << std::endl;
		}
	}

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}
