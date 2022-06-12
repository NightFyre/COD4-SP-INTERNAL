#pragma once
#include "helper.h"
#include "Console.hpp"

namespace COD4_SP {
	
	//struct EntityObject
	//{
	//	char pad_0000[24]; //0x0000
	//	Vector3 Position; //0x0018
	//	char pad_0024[288]; //0x0024
	//	int32_t Health; //0x0144
	//	int32_t MaxHealth; //0x0148
	//}; //Size: 0x014C

	struct EntityObject
	{
		char pad_0000[24]; //0x0000
		Vector3 FeetPosition; //0x0018
		char pad_0024[176]; //0x0024
		Vector3 HeadPosition; //0x00D4
		char pad_00E0[100]; //0x00E0
		int32_t Health; //0x0144
		int32_t MaxHealth; //0x0148
	}; //Size: 0x014C

	struct Offsets {
		int EntArrayPTR = 0x009CF7A8;

		//	ESP
		int ScreenX = 0x002E17FC;
		int ScreenY = 0x002E1800;
		int ViewMatrix = 0x002E1804;

		//	FIXES
		int mouse = 0xFE06F5;			//	bool	::	  01
		int FOV = 0x3F77CC;				//	float	:: 00 00
		int NOCLIP = 0xA18D54;			//	bool	::	  00
		int fxDRAW = 0x7CFA50;			//	bool	::    00

		//	CHEATS
		int GOD = 0x881548;				//	bool	::    00
		int ammoINSTRUCT = 0x1BFA67;	//	2bytes	:: 29 D0
		int RECOIL = 0x34661;			//	2Bytes	:: 74 22
		int rSHAKE = 0x1C160D;			//	6Bytes	:: 89 91 20 06 00 00
		int RAPIDFIRE = 0x1C13CC;		//	3Bytes	:: 89 4E 3C
		int SPREAD = 0x0;
	};

	class GameData
	{
	public:
		EntityObject* EntObject{};
		Offsets offsets{};
		uintptr_t FOV_ADDR{};

		//	CHEATS
		bool bINFINITEAMMO = FALSE;
		bool bINFINITEHEALTH = FALSE;
		bool bRAPIDFIRE = FALSE;
		bool bRECOIL = FALSE;
		bool bSPREAD = FALSE;
		bool bNOCLIP = FALSE;
		bool bDRAWFX = FALSE;
		bool bESP = FALSE;

		explicit GameData();
		~GameData() noexcept = default;
		GameData(GameData const&) = delete;
		GameData(GameData&&) = delete;
		GameData& operator=(GameData const&) = delete;
		GameData& operator=(GameData&&) = delete;
	};

	class GameVariables
	{
	public:
		//	Dx & ImGui
		int g_GamePid{};
		HANDLE g_GameHandle{};
		HWND g_GameWindow{};
		int g_GameWidth;
		int g_GameHeight;
		ImVec2 g_WindowSize{};
		LPCSTR g_GameTitle;
		LPCSTR g_ClassName;
		LPCSTR g_GamePath;

		//	MENU RELATED
		bool m_ShowMenu = FALSE;
		bool m_ShowConsole = FALSE;
		bool m_ShowHud{};
		bool m_ShowDemo{};

		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables) = delete;
	
		void Initialize();
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameData> g_GameData;
}