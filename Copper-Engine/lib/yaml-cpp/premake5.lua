project "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("Build/" .. outputDir)
	objdir ("BuildInt/" .. outputDir)

	files {
	    
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
		
	}

	includedirs {
	    
		"include"
		
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
