#pragma once
#define WIN32_LEAN_AND_MEAN

// standard includes
#include <Windows.h>
#include <iostream>
#include <xstring>
#include <string>
#include <chrono>
#include <thread>
#include <cstdint>

using namespace std::chrono_literals;

// directx
#include <d3d9.h>
#include <d3dx9.h>

/* imgui */
// @credits: https://github.com/ocornut/imgui
#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_freetype.h"

/* imgui implementation */
#include "../ext/imgui/imgui_internal.h"
#include "../ext/imgui/dx9/imgui_impl_dx9.h"
#include "../ext/imgui/win32/imgui_impl_win32.h"

// gui includes
#include "gui/gui.h"

// memory
#include "memory/memory.h"
#include "memory/protection/xorstr.h"

// utilities
#include "utilities/logging.h"

// other
#include "global.h"