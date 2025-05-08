project "Stb-Image"
	kind "StaticLib"
	language "C++"
	staticruntime "On"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"stb_image.cpp",
		"stb_image.h",
		"stb_truetype.cpp",
		"stb_truetype.h",

	}
	
	filter "system:windows"
		systemversion "10.0.22621.0"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
