#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include "MemoryHelpers.h"
#include "Patches.h"

#include <iostream>

namespace menu {

	enum class MenuType
	{
		Main, //Shows a menu to select the type of menu
		Player, //Shows Player Options/Mods
		Garden, //Shows Garden Options/Mods
		Items, //Shows Item Options/Mods
		Enities, //Shows Pinata/NPC Options/Mods
		MenuSettings, //Shows Menu Options, Hotkeys, theme, etc
		About //Shows the crdits and info about the mod
	};

	inline MenuType menuType = MenuType::Main; //Default menu type

	static inline void RenderPlayer() {

		float overheadCameraHeight = MemHelp::GetDouble(0x005F63E8);
		if (ImGui::SliderFloat("Overhead Camera Height", &overheadCameraHeight, -5000.0f, 5000.0f)) {
			MemHelp::SetDouble(0x005F63E8, overheadCameraHeight);
		}

		// Player Coin Slider
		if (g_PlayerDataPtr) {
			uint32_t coins = g_PlayerDataPtr->Coins;
			if (ImGui::SliderInt("Player Coins", reinterpret_cast<int*>(&coins), 0, 1000000000)) {
				//SetPlayerCoins(coins);
				g_PlayerDataPtr->Coins = coins;
			}
		}

		// Player Level Slider
		if (g_PlayerDataPtr) {
			uint32_t level = g_PlayerDataPtr->Level;
			if (ImGui::SliderInt("Player Level", reinterpret_cast<int*>(&level), 0, 512)) {
				g_PlayerDataPtr->Level = level;
			}
		}

		//int input for player experiance
		if (g_PlayerDataPtr) {
			int experience = g_PlayerDataPtr->ExperiencePoints;
			//add warning popup
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Recommended to do this in the main menu to skip exp tick up");
			}
			if (ImGui::InputInt("Player Experience", &experience)) {
				g_PlayerDataPtr->ExperiencePoints = experience;
			}
		}

	}

	static inline void RenderGarden() {

		bool bUnlimitedGardenSpace = MemHelp::IsPatchEnabled(g_UnlimitedGardenSpace);
		if (ImGui::Checkbox("Unlimited Garden Space", &bUnlimitedGardenSpace)) {
			MemHelp::SetPatch(g_UnlimitedGardenSpace, bUnlimitedGardenSpace);
			if (bUnlimitedGardenSpace) {
				std::cout << "Unlimited Garden Space enabled" << std::endl;
			}
			else {
				std::cout << "Unlimited Garden Space disabled" << std::endl;
			}
		}

	}

	static inline void RenderItems() {

		// Easy Break Items
		if (ImGui::Checkbox("Instant Break Items", &g_EasyBreakItems)) {
			if (g_EasyBreakItems) {
				std::cout << "Instant Break Items enabled" << std::endl;
			}
			else {
				std::cout << "Instant Break Items disabled" << std::endl;
			}
		}

	}

	static inline void RenderEnities() {

		// Easy Break Sick Pinata
		if (ImGui::Checkbox("Instant Break Sick Pinata", &g_EasyBreakSickPinata)) {
			if (g_EasyBreakSickPinata) {
				std::cout << "Instant Break Sick Pinata enabled" << std::endl;
			}
			else {
				std::cout << "Instant Break Sick Pinata disabled" << std::endl;
			}
		}

	}

	static inline void RenderMenuSettings() {

		// debug checkbox
		if (ImGui::Checkbox("Debug Mode", &g_Debug)) {
			if (g_Debug) {
				std::cout << "Debug mode enabled" << std::endl;
			}
			else {
				std::cout << "Debug mode disabled" << std::endl;
			}
		}

		//Show debug values if enabled
		if (g_Debug) {
			//Debug values (Once set they will display)
			ImGui::Separator();
			if (g_PlayerDataPtr) {
				ImGui::Text("PlayerData offset: %p", g_PlayerDataPtr);
				ImGui::Text("PlayerID?: %d", g_PlayerDataPtr->PlayerID);
				ImGui::Text("Coins: %d", g_PlayerDataPtr->Coins);
				ImGui::Text("Player Level: %d", g_PlayerDataPtr->Level);
			}
			//camera data text
			if (g_CameraDataPtr) {
				ImGui::Text("Camera Mode: %d", g_CameraDataPtr->CameraMode);
			}
			if (g_i1 != 0)
			{
				ImGui::Text("i1: %d", g_i1);
			}
			if (g_i2 != 0)
			{
				ImGui::Text("i2: %d", g_i2);
			}
			if (g_i3 != 0)
			{
				ImGui::Text("i3: %d", g_i3);
			}
			if (g_i4 != 0)
			{
				ImGui::Text("i4: %d", g_i4);
			}
			if (g_f1 != 0.0f)
			{
				ImGui::Text("f1: %f", g_f1);
			}
			if (g_f2 != 0.0f)
			{
				ImGui::Text("f2: %f", g_f2);
			}
			if (g_f3 != 0.0f)
			{
				ImGui::Text("f3: %f", g_f3);
			}
			if (g_f4 != 0.0f)
			{
				ImGui::Text("f4: %f", g_f4);
			}
			if (g_c1 != "null")
			{
				ImGui::Text("c1: %s", g_c1);
			}
			if (g_c2 != "null")
			{
				ImGui::Text("c2: %s", g_c2);
			}
			if (g_c3 != "null")
			{
				ImGui::Text("c3: %s", g_c3);
			}
			if (g_c4 != "null")
			{
				ImGui::Text("c4: %s", g_c4);
			}
		}

	}

	static inline void RenderAbout() {
		//Render Credits
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Credits: ");
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Separator();

		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("SolarCookies ");
		ImGui::SetWindowFontScale(0.6f);
        ImGui::BeginChild("ScrollableRegion1", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);  
        ImGui::Text("ImGui Menu Injection");  
        ImGui::Text("Player Level");  
        ImGui::Text("Player Experience");  
        ImGui::Text("Player Coins");  
        ImGui::Text("Overhead Camera Height");  
        ImGui::Text("Instant Break Items");  
        ImGui::Text("Instant Break Sick Pinata");  
        ImGui::EndChild();

		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("OlieGamerTV ");
		ImGui::SetWindowFontScale(0.6f);
		ImGui::BeginChild("ScrollableRegion2", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::Text("Print Function hooks");
		ImGui::EndChild();
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("Jacob ");
		ImGui::SetWindowFontScale(0.6f);
		ImGui::BeginChild("ScrollableRegion3", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::Text("Unlimited Garden Space");
		ImGui::EndChild();

		/* //in order to add these patches we will need a way to insert assembly by allocating memory and jumping to it then returning to the original code
		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("u/Little-Business-9508 ");
		ImGui::SetWindowFontScale(0.6f);
		ImGui::BeginChild("ScrollableRegion4", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::Text("Willy Quick Build");
		ImGui::Text("Instant Pinata Hunts/Mission");
		ImGui::Text("Gretchen Always Has Pinata");
		ImGui::EndChild();
		*/


		ImGui::SetWindowFontScale(1.0f);
	}

	static inline void RenderMain() noexcept
	{
		if (ImGui::Button("Player Options")) {
			menuType = MenuType::Player;
		}
		if (ImGui::Button("Garden Options")) {
			menuType = MenuType::Garden;
		}
		if (ImGui::Button("Item Options")) {
			menuType = MenuType::Items;
		}
		if (ImGui::Button("Enity Options")) {
			menuType = MenuType::Enities;
		}
		if (ImGui::Button("Menu Settings")) {
			menuType = MenuType::MenuSettings;
		}
		if (ImGui::Button("About")) {
			menuType = MenuType::About;
		}
	}

	static inline void RenderMenu() noexcept
	{
		if (menuType != MenuType::Main) {
			if (ImGui::Button("Back")) {
				menuType = MenuType::Main;
			}
		}
		ImGui::Separator();

		switch (menuType) {
		case MenuType::Main:
			RenderMain();
			break;
		case MenuType::Player:
			RenderPlayer();
			break;
		case MenuType::Garden:
			RenderGarden();
			break;
		case MenuType::Items:
			RenderItems();
			break;
		case MenuType::Enities:
			RenderEnities();
			break;
		case MenuType::MenuSettings:
			RenderMenuSettings();
			break;
		case MenuType::About:
			RenderAbout();
			break;
		default:
			break;
		}

	}

}