project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}
	
	includedirs
	{
		"include"
	}
	
	filter "system:windows"
		systemversion "10.0.22621.0"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
