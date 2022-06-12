#pragma once
#define DEBUG 0		//	used for cutting out functions and instructions at compile time

#include <windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

//	Console & Basic Imports
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

//	DX11
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

//	INTERNET
#include <wininet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")

//	EXTERNAL LIBS (ImGui & MinHook)
#include "../../../libs/INTERNAL/ImGui/imgui.h"
#include "../../../libs/INTERNAL/ImGui/imgui_internal.h"
#include "../../../libs/INTERNAL/ImGui/backends/imgui_Impl_dx9.h"
#include "../../../libs/INTERNAL/ImGui/backends/imgui_Impl_Win32.h"
#include "../../../libs/INTERNAL/MinHook/MinHook.h"

/// <summary>
///	Global Variables accessible by anything in the DX9Base Namespace
namespace COD4_SP {
	using namespace std::chrono_literals;
	inline HMODULE g_hModule{};
	inline LPCWSTR g_moduleName{};
	inline uintptr_t dwGameBase{};
	inline std::atomic_bool g_Running{};
	inline std::atomic_bool g_Killswitch = FALSE;

	
	struct Vector2 {
		float x, y;
	};

	struct Vector3 {
		float x, y, z;
	};

	struct Vector4 {
		float x, y, z, a;
	};

	struct DoubleVector2 {
		float x, y;
	};

	struct DoubleVector3 {
		float x, y, z;
	};

	struct DoubleVector4 {
		float x, y, z, a;
	};

	struct matrix
	{
		float Matrix[16];
	};
}