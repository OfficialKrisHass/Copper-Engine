project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("Build/" .. outputDir)
	objdir ("BuildInt/" .. outputDir)

	files {

		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"

	}

	includedirs {

		"include"

	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
