#include "Menu.hpp"
float vMatrix[16];

namespace COD4_SP {
	void Draw3DBox(Vector3 Pos, float* Matrix, int Width, int Height, ImColor color, int Thickness);
	namespace TABS {
		void Main() {
			ImGui::Text("PLAYER");
			ImGui::Spacing();

			if (ImGui::Checkbox("INFINITE HEALTH", &g_GameData->bINFINITEHEALTH))
				*(bool*)g_Hooks->ResolveOffset(g_GameData->offsets.GOD) = g_GameData->bINFINITEHEALTH;

			if (ImGui::Checkbox("NO CLIP", &g_GameData->bNOCLIP))
				*(bool*)g_Hooks->ResolveOffset(g_GameData->offsets.NOCLIP) = g_GameData->bNOCLIP;

			if (*(float*)g_Hooks->ResolvePTRS(g_GameData->offsets.FOV, { 0xC }) != NULL) {
				ImGui::Text("FOV");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				ImGui::SliderFloat("##FOV", &*(float*)g_Hooks->ResolvePTRS(g_GameData->offsets.FOV, { 0xC }), 65, 120, "%.0f");
				ImGui::SameLine();
				if (ImGui::Button("RESET")) *(float*)g_Hooks->ResolvePTRS(g_GameData->offsets.FOV, { 0xC }) = 65;
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("WEAPON");
			ImGui::Spacing();
			if (ImGui::Checkbox("INFINITE AMMO", &g_GameData->bINFINITEAMMO)) {
				switch (g_GameData->bINFINITEAMMO) {
				case(TRUE):
					g_Hooks->Nop((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.ammoINSTRUCT), 2);
					break;
				case(FALSE):
					g_Hooks->Patch((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.ammoINSTRUCT), (BYTE*)"\x29\xD0", 2);
					break;
				}
			}
			if (ImGui::Checkbox("NO RECOIL", &g_GameData->bRECOIL)) {
				switch (g_GameData->bRECOIL) {
				case(TRUE):
					g_Hooks->Patch((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.RECOIL), (BYTE*)"\xEB\x22", 2);
					g_Hooks->Nop((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.rSHAKE), 6);
					break;
				case(FALSE):
					g_Hooks->Patch((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.RECOIL), (BYTE*)"\x74\x22", 2);
					g_Hooks->Patch((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.rSHAKE), (BYTE*)"\x89\x91\x20\x06\x00\x00", 6);
					break;
				}
			}
			if (ImGui::Checkbox("NO SPREAD", &g_GameData->bSPREAD)) {

			}
			if (ImGui::Checkbox("RAPID FIRE", &g_GameData->bRAPIDFIRE)) {
				switch (g_GameData->bRAPIDFIRE) {
				case(TRUE):
					g_Hooks->Nop((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.RAPIDFIRE), 3);
					break;
				case(FALSE):
					g_Hooks->Patch((BYTE*)g_Hooks->ResolveOffset(g_GameData->offsets.RAPIDFIRE), (BYTE*)"\x89\x4E\x3C", 3);
					break;
				}
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("ENVIRONMENT");
			ImGui::Spacing();

			if (ImGui::Checkbox("DRAW FX", &g_GameData->bDRAWFX))
				*(bool*)g_Hooks->ResolvePTRS(g_GameData->offsets.fxDRAW, { 0xC }) = !g_GameData->bDRAWFX;

			ImGui::Checkbox("ESP", &g_GameData->bESP);
			if (g_GameData->bESP){
				//ImGui::BeginCombo("POSITION", std::to_string(g_Menu->m_dbg_drawpos).c_str());
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				ImGui::Combo("POSITION", &g_Menu->m_dbg_drawpos, "Heaven\0Hell\0Corner\0Center\0");
				ImGui::Checkbox("HEALTH BARS", &g_Menu->m_dbg_healthbars);

				ImGui::SliderFloat("[X-] TOP RIGHT", &g_Menu->tRight, 0, 200);
				ImGui::SliderFloat("[X+] TOP LEFT", &g_Menu->tLeft, 0, 200);
				ImGui::SliderFloat("[X+] BOTTOM RIGHT", &g_Menu->bRight, 0, 200);
				ImGui::SliderFloat("[Y+] BOTTOM LEFT", &g_Menu->bLeft, 0, 200);
			}
		}

		void About() {
			ImGui::Text("[DX9] COD4:SP ImGui INTERNAL MENU");
			ImGui::Text("BUILD VERSION: v1.0");
			ImGui::Text("BUILD DATE: 6/4/2022");
			ImGui::Checkbox("RGB THEME", &g_Menu->dbg_RAINBOW_THEME);
			ImGui::Checkbox("SHOW IMGUI DEMO", &g_GameVariables->m_ShowDemo);
#if DEBUG
			if (ImGui::Checkbox("SHOW DEBUG CONSOLE", &g_GameVariables->m_ShowConsole)) {
				switch (g_GameVariables->m_ShowConsole) {
				case(TRUE):
					ShowWindow(g_Console->g_hWnd, SW_SHOW);
					break;
				case(FALSE):
					ShowWindow(g_Console->g_hWnd, SW_HIDE);
					break;
				}
			}
#endif

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("UNHOOK DLL", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20))) g_Killswitch = TRUE;
		}
	}

	void Menu::Draw()
	{
		if (g_GameVariables->m_ShowMenu)
			MainMenu();

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();

		if (g_GameData->bESP)
			ESP(m_dbg_drawpos);
	}

	void Menu::MainMenu()
	{
		if (dbg_RAINBOW_THEME) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
			ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
		}
		if (!ImGui::Begin("COD4:SP INTERNAL", &g_GameVariables->m_ShowMenu, 96))
		{
			ImGui::End();
			return;
		}
		if (dbg_RAINBOW_THEME) {
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("MAIN")) 
			{
				TABS::Main();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("ABOUT"))
			{
				TABS::About();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	
	void Menu::HUD(bool* p_open)
	{
		//	Here you can draw a secondary window to show the user what menu items they have active
		// This makes it easier on the user , include a method to show and hide the HUD

	}
	
	void Menu::Loops()
	{
		///	Code inside this function body will execute every 1ms
		// Best used for constant writes and for anything that you need written while the menu is not shown

		if (g_GameData->bINFINITEHEALTH) {

		}

		if (g_GameData->bINFINITEAMMO) {
			
		}
	}

	void Menu::ESP(byte Pos)
	{
		/// <summary>
		/// Establish Colors
		ImColor cLines{};
		ImColor cText = { 255, 255, 255, 255 };
		if (dbg_RAINBOW_THEME)
			cLines = dbg_RAINBOW;
		else
			cLines = { 255, 0, 0, 255 };

		int dist = 0x74;
		float X = *(float*)g_Hooks->ResolveOffset(g_GameData->offsets.ScreenX);
		float Y = *(float*)g_Hooks->ResolveOffset(g_GameData->offsets.ScreenY);
		Vector2 pos = { ImGui::GetMainViewport()->GetCenter().x, ImGui::GetMainViewport()->GetCenter().y };


		/// <summary>
		/// Establish Position to draw from
		ImVec2 drawPosition{};
		switch (Pos) {

			//	Heaven
		case(0):
			drawPosition = {pos.x, 0};
			break;

			//	Hell
		case(1):
			drawPosition = {pos.x, Y};
			break;

			// Corner
		case(2):
			drawPosition = {0, 0};
			break;

			// Center
		case(3):
			drawPosition = {pos.x, pos.y};
			break;
		}
		uintptr_t ViewMatrix = g_Hooks->ResolveOffset(g_GameData->offsets.ViewMatrix);
		memcpy(&vMatrix, (BYTE*)ViewMatrix, sizeof(vMatrix));
		for (int i = 0; i < 35; i++) {
			if (i == NULL) continue;

			//	GET ENTITY
			g_GameData->EntObject = (EntityObject*)*(uintptr_t*)g_Hooks->ResolveOffset(g_GameData->offsets.EntArrayPTR + i * dist);
			if (g_GameData->EntObject == NULL)
				continue;

			//	GET HEALTH
			if (g_GameData->EntObject->Health > 150 || g_GameData->EntObject->Health <= NULL)
				continue;

			std::string HP = std::to_string(g_GameData->EntObject->Health);
			Vector2 screen;
			if (!g_D3D9Window->WorldToScreen(g_GameData->EntObject->FeetPosition, screen, vMatrix, X, Y))
				continue;

			Vector2 head;
			if (!g_D3D9Window->WorldToScreen(g_GameData->EntObject->HeadPosition, head, vMatrix, X, Y))
				continue;


			float vhead = head.y - screen.y;
			float width = vhead / 2;
			float center = width / -2;

			ImGui::GetBackgroundDrawList()->AddLine(drawPosition, ImVec2(screen.x, screen.y), cLines, 1);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen.x, screen.y), cText, HP.c_str());
			Draw3DBox(g_GameData->EntObject->FeetPosition, vMatrix, X, Y, cLines, 1);
			//ImGui::GetBackgroundDrawList()->AddRect(ImVec2(screen.x + center, screen.y), ImVec2(screen.x - center, head.y), cLines);
			if (m_dbg_healthbars) {
				//float Width = 100;
				//float Height = 10;
				//ImVec2 clipMin{ 0.0f, (150 - (float)g_GameData->EntObject->Health) };
				//ImVec2 clipMax{Width + 1.0f, Height + 1.0f};
				//ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(screen.x + clipMin.x, screen.y + clipMin.y), 
				//	ImVec2(screen.x + clipMax.x, screen.y + clipMax.y));
				//ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(screen.x, screen.y), ImVec2(screen.x + 100, screen.y + 100), ImColor(255, 0, 0, 255));
				//ImGui::GetBackgroundDrawList()->AddRect(ImVec2(screen.x, screen.y), ImVec2(screen.x + 100, screen.y + 100), ImColor(255, 0, 0, 255));
				//ImGui::PopClipRect();
				//ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(screen.x - (tRight - 1), screen.y - (tLeft - 1)), ImVec2(screen.x + (bRight - 1), screen.y + (bLeft - 1)));
				ImGui::GetBackgroundDrawList()->AddRect(ImVec2(screen.x - tRight, screen.y - tLeft), ImVec2(screen.x + bRight, screen.y + bLeft), ImColor(255, 0, 0, 255));
				//ImGui::GetBackgroundDrawList()->PopClipRect();
			}
		}
	}

	void Draw3DBox(Vector3 Pos, float* Matrix, int Width, int Height, ImColor color, int Thickness) {

		//	BOX TOP DIMENSIONS
		Vector3 top_front_left = Vector3{ Pos.x + 10, Pos.y - 10, Pos.z + 55 };
		ImVec2 top_front_left_2D;
		Vector3 top_front_right = Vector3{ Pos.x + 10, Pos.y + 10, Pos.z + 55 };
		ImVec2 top_front_right_2D;
		Vector3 top_bottom_left = Vector3{ Pos.x - 10, Pos.y + 10, Pos.z + 55 };
		ImVec2 top_bottom_left_2D;
		Vector3 top_bottom_right = Vector3{ Pos.x - 10, Pos.y - 10, Pos.z + 55 };
		ImVec2 top_bottom_right_2D;
		
		//	Get World To Screen Position for Top Half of Box
		g_D3D9Window->WorldToScreen(top_front_left, (Vector2&)top_front_left_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(top_front_right, (Vector2&)top_front_right_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(top_bottom_left, (Vector2&)top_bottom_left_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(top_bottom_right, (Vector2&)top_bottom_right_2D, Matrix, Width, Height);

		//	BOX BOTTOM DIMENSIONS
		Vector3 bottom_front_left = Vector3{ Pos.x + 10, Pos.y - 10, Pos.z };
		ImVec2 bottom_front_left_2D;
		Vector3 bottom_front_right = Vector3{ Pos.x + 10, Pos.y + 10, Pos.z };
		ImVec2 bottom_front_right_2D;
		Vector3 bottom_rear_left = Vector3{ Pos.x - 10, Pos.y + 10, Pos.z };
		ImVec2 bottom_rear_left_2D;
		Vector3 bottom_rear_right = Vector3{ Pos.x - 10, Pos.y - 10, Pos.z };
		ImVec2 bottom_rear_right_2D;

		//	Get World To Screen Position for Lower Half of Box
		g_D3D9Window->WorldToScreen(bottom_front_left, (Vector2&)bottom_front_left_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(bottom_front_right, (Vector2&)bottom_front_right_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(bottom_rear_left, (Vector2&)bottom_rear_left_2D, Matrix, Width, Height);
		g_D3D9Window->WorldToScreen(bottom_rear_right, (Vector2&)bottom_rear_right_2D, Matrix, Width, Height);


		//	DRAW 3D BOXES
		ImGui::GetBackgroundDrawList()->AddLine(top_bottom_left_2D, top_front_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(top_front_left_2D, top_front_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(top_front_left_2D, top_bottom_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(top_bottom_left_2D, top_bottom_right_2D, color, Thickness);

		ImGui::GetBackgroundDrawList()->AddLine(bottom_rear_left_2D, bottom_front_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_front_right_2D, bottom_front_left_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_front_left_2D, bottom_rear_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_rear_right_2D, bottom_rear_left_2D, color, Thickness);

		ImGui::GetBackgroundDrawList()->AddLine(bottom_rear_right_2D, top_bottom_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_rear_left_2D, top_bottom_left_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_front_right_2D, top_front_right_2D, color, Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(bottom_front_left_2D, top_front_left_2D, color, Thickness);
	}
}
