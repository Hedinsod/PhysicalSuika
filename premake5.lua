-- premake5.lua
workspace "PhysicalSuika"
	architecture "x64"
	configurations { "Debug", "Development", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PhysicalSuika"
	location "PhysicalSuika"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/Source/pch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/Source",
	}
	
	links
	{
		"winmm.lib"
	}

	filter "system:windows"
		systemversion "10.0.22621.0"

		defines
		{
		--	"MR_PLATFORM_WINDOWS",
		--	"GLFW_INCLUDE_NONE",
		}

	filter "configurations:Debug"
		defines "BUILD_DEBUG"
		symbols "on"

	filter "configurations:Development"
		defines "BUILD_DEVELOPMENT"
		optimize "on"

	filter "configurations:Release"
		defines "BUILD_RELEASE"
		optimize "on"
		
--	filter "system:windows"
--		buildoptions "/utf-8"
	