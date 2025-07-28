#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>

#include "MemoryHelpers.h"
#include "Patches.h"
#include "hooks.h"

#include <iostream>

namespace menu {

	enum class MenuType
	{
		Main, //Shows a menu to select the type of menu
		Player, //Shows Player Options/Mods
		Garden, //Shows Garden Options/Mods
		Items, //Shows Item Options/Mods
		Enities, //Shows Piñata/NPC Options/Mods
		MenuSettings, //Shows Menu Options, Hotkeys, theme, etc
		About //Shows the crdits and info about the mod
	};

	inline MenuType menuType = MenuType::Main;

	static inline ImVec4 ColorTextBasedOnType(ActorType type) {
		ImVec4 color;
		switch (type) {
		case ActorType::Pinata:
			color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
			break;
		case ActorType::Decor:
			color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
			break;
		case ActorType::Shopkeeper:
			color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
			break;
		case ActorType::Produse:
			color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
			break;
		case ActorType::LifeCandy:
			color = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
			break;
		case ActorType::Tool:
			color = ImVec4(0.5f, 0.0f, 0.5f, 1.0f); // Purple
			break;
		case ActorType::Seed:
			color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
			break;
		case ActorType::Fruit:
			color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
			break;
		case ActorType::Veggie:
			color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f); // Light Green
			break;
		case ActorType::Egg:
			color = ImVec4(0.5f, 0.5f, 1.0f, 1.0f); // Light Blue
			break;
		case ActorType::Crate:
			color = ImVec4(0.5f, 0.5f, 0.0f, 1.0f); // Olive
			break;
		case ActorType::Shed:
			color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
			break;
		case ActorType::Home:
			color = ImVec4(1.0f, 0.5f, 0.5f, 1.0f); // Light Red
			break;
		case ActorType::SeedPacket:
			color = ImVec4(0.5f, 1.0f, 1.0f, 1.0f); // Light Cyan
			break;
		case ActorType::Flower:
			color = ImVec4(1.0f, 0.5f, 1.0f, 1.0f); // Light Magenta
			break;
		case ActorType::Tree:
			color = ImVec4(0.5f, 0.5f, 1.0f, 1.0f); // Light Purple
			break;
		case ActorType::LandType:
			color = ImVec4(1.0f, 0.5f, 0.5f, 1.0f); // Light Red
			break;
		case ActorType::Assessory:
			color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f); // Light Green
			break;
		case ActorType::Ruffian:
			color = ImVec4(1.0f, 0.5f, 0.5f, 1.0f); // Light Red
			break;
		case ActorType::Helper:
			color = ImVec4(0.5f, 1.0f, 0.5f, 1.0f); // Light Green
			break;
		case ActorType::Unknown:
			color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
			break;
		case ActorType::Coin:
			color = ImVec4(1.0f, 1.0f, 0.5f, 1.0f); // Light Yellow
			break;
		case ActorType::Weed:
			color = ImVec4(0.5f, 0.2f, 0.2f, 1.0f); // Dark Red
			break;
		case ActorType::Totem:
			color = ImVec4(0.2f, 0.5f, 0.2f, 1.0f); // Dark Green
			break;
		case ActorType::Contract:
			color = ImVec4(0.2f, 0.2f, 0.5f, 1.0f); // Dark Blue
			break;
		case ActorType::Paving:
			color = ImVec4(0.5f, 0.2f, 0.5f, 1.0f); // Dark Purple
			break;
		default:
			color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
			break;
		}
		return color;
	}

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

		// Easy Break Sick Piñata
		if (ImGui::Checkbox("Instant Break Sick Pinata", &g_EasyBreakSickPinata)) {
			if (g_EasyBreakSickPinata) {
				std::cout << "Instant Break Sick Pinata enabled" << std::endl;
			}
			else {
				std::cout << "Instant Break Sick Pinata disabled" << std::endl;
			}
		}

		
		//Get current hunted Piñata from EE8510 
		uint32_t huntedPinata = MemHelp::GetInt(0xAE8510);
		const char* hunted = std::to_string(huntedPinata).c_str();

		for (PinataIDs Piñata : g_PinataIDs) {
			if (Piñata.ID == huntedPinata) {
				hunted = Piñata.Name;
				break;
			}
		}


		if (g_Debug) {
			if (ImGui::InputInt("Hunted Pinata ID", reinterpret_cast<int*>(&huntedPinata))) {
				if (hooks::Unknown_00727E10(huntedPinata) == 0) {
					std::cout << "Invalid ID" << std::endl;
				}
				else {
					MemHelp::SetInt(0xAE8510, huntedPinata);
					MemHelp::SetInt(0xAE8514, huntedPinata);
					MemHelp::SetInt(0xAE8518, huntedPinata);
				}
			}
		}



		//Add dropdown for hunted Piñata
		if (ImGui::BeginCombo("Hunted Pinata", hunted)) {
			//for each Piñata in the hunted Piñata 
			for (PinataIDs Pinata : g_PinataIDs) {
				//set color based on the type of actor
				ImGui::PushStyleColor(ImGuiCol_Text, ColorTextBasedOnType(Pinata.Type));
				if (ImGui::Selectable(Pinata.Name)) {
					//set the hunted Piñata
					std::cout << "Hunted Piñata: " << Pinata.Name << std::endl;
					MemHelp::SetInt(0xAE8510, Pinata.ID);
					MemHelp::SetInt(0xAE8514, Pinata.ID);
					MemHelp::SetInt(0xAE8518, Pinata.ID);
				}
				ImGui::PopStyleColor();
			}
			ImGui::EndCombo();
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
		ImGui::Text("Shop Item ID List");
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

		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("u/Little-Business-9508 ");
		ImGui::SetWindowFontScale(0.6f);
		ImGui::BeginChild("ScrollableRegion4", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		ImGui::Text("Gretchen Hunted Pinata Offsets");
		ImGui::EndChild();
		


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