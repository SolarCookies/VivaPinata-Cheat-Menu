#pragma once

#include "minhook/minhook.h"

//Common includes
#include <wchar.h>
#include <stdexcept>
#include <intrin.h>
#include <fstream>
#include <algorithm>
#include <shlobj.h>
#include <iostream>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

class Hook
{
public:
	virtual void Setup() {};
};