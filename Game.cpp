#include "Game.hpp"
namespace COD4_SP {
	GameVariables::GameVariables()
	{
#if DEVELOPER
		g_Console->printdbg("GameVariables::Initialized\n", g_Console->color.pink);
#endif
		return;
	}

	//	Get Process Window Information
	bool SelectedWindow = FALSE;
	void GameVariables::Initialize()
	{
		while (!SelectedWindow) {
			DWORD ForegroundWindowProcessID;
			GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
			if (GetCurrentProcessId() == ForegroundWindowProcessID) {
				g_GamePid = GetCurrentProcessId();
				g_GameHandle = GetCurrentProcess();
				g_GameWindow = GetForegroundWindow();

				RECT tempRECT;
				GetWindowRect(g_GameWindow, &tempRECT);
				g_GameWidth = tempRECT.right - tempRECT.left;
				g_GameHeight = tempRECT.bottom - tempRECT.top;

				char tempTitle[MAX_PATH];
				GetWindowTextA(g_GameWindow, tempTitle, sizeof(tempTitle));
				g_GameTitle = tempTitle;

				char tempClassName[MAX_PATH];
				GetClassNameA(g_GameWindow, tempClassName, sizeof(tempClassName));
				g_ClassName = tempClassName;

				char tempPath[MAX_PATH];
				GetModuleFileNameExA(g_GameHandle, NULL, tempPath, sizeof(tempPath));
				g_GamePath = tempPath;

				//	We declare this so that we can maintain focus on the window we want info for. 
				//	This prevents a few potential errors from being able to take place
				SetFocus(g_GameWindow);
#if DEVELOPER
				g_Console->printdbg("GameData::Init - Process Window Info Established\n", g_Console->color.pink);
#endif

				SelectedWindow = TRUE;
			}
		}
	}

	GameData::GameData()
	{
		dwGameBase = (uintptr_t)GetModuleHandle(g_moduleName);
#if DEVELOPER
		g_Console->printdbg("GameData::Initialized\n", g_Console->color.pink);
#endif
		return;
	}
}
