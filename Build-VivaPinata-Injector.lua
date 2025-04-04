-- premake5.lua
workspace "VivaPinata_Injector"
   architecture "x32" -- 32-bit architecture (Viva Pinata is a 32-bit game)
   configurations { "Debug", "Release"}
   startproject "VivaPinata_Injector"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
      cppdialect "C++20" -- Set C++20 standard
      systemversion "latest" -- Use the latest Windows SDK

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VivaPinata_Injector"
   kind "SharedLib" -- Build as a DLL
   language "C++"
   targetdir ("$(SolutionDir)build/")
   objdir ("$(SolutionDir)build/intermediates/")

   -- Enable Multi-Byte Character Set
   characterset "MBCS"

   -- Include all files 
   files { "VivaPinata_Injector/**.h", "VivaPinata_Injector/**.cpp", "venders/**.h", "venders/**.cpp", "venders/**.c" }


   -- Organize files under a filter called "ImGui"
   vpaths {
      ["ImGui/*"] = { "venders/imgui/**.h", "venders/imgui/**.cpp" },
      ["Minhook/*"] = { "venders/minhook/**.h", "venders/minhook/**.c" }
   }
   
   -- Includes directories for ImGui and Minhook
   includedirs { "./venders" }
