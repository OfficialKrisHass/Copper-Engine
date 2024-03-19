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

	defines {

		"YAML_CPP_STATIC_DEFINE"

	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"