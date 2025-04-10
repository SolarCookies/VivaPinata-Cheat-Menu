# VivaPinata Cheat Menu
---

# Current Features:
- Renders Custom ImGui UI
- Coins
- Level
- Experience
- Overhead Camera Height
- Unlimited Garden Space

---

# How to Install and Use:
- Extract the latest release into the root of the "Viva Pinata.exe" Directory
- Launch like normal using the startup.exe
- Once in the game you can press the Insert key to toggle the menu

# Building from source
- You will need to run the "Setup-Project.bat" to generate the sln file for Visual Studio 2022
- Drag the asl hook dlls from a release into the root of the "Viva Pinata.exe" Directory (bink232.dll and bink232Hooked.dll)
- drag the "plugins/global.ini" from a release into "VivaPinata/plugins/" so you have the correct asl loader settings
- Rename the dll that visual studio built for the mod menu to a .asl
- drag that asl into the plugins folder

Keep in mind you can also set the output dir to be the plugins folder with a .asl extension and set the command to run the exe automatically when pressing play or debug in visual studio
