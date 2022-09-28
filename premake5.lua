workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

outputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Copper-Engine/lib/GLFW"
include "Copper-Engine/lib/GLAD"
include "Copper-Engine/lib/ImGui/ImGui"
include "Copper-Engine/lib/yaml-cpp"

project "Copper-Engine"
    location "Copper-Engine"
    kind "StaticLib"
    language "C++"  
    cppdialect "C++17"
    staticruntime "on"

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("BuildInt/" .. outputDir .. "/%{prj.name}")

    pchheader "cupch.h"
    pchsource "Copper-Engine/src/cupch.cpp"

    files {

        "%{prj.name}/src/Copper.h",
        "%{prj.name}/src/cupch.h",
        "%{prj.name}/src/cupch.cpp",

        "%{prj.name}/src/Engine/**.h",
        "%{prj.name}/src/Engine/**.cpp",
        
        "%{prj.name}/lib/stb/stb/stb_image.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/spdlog",
        "%{prj.name}/lib/GLFW/include",
        "%{prj.name}/lib/GLAD/include",
        "%{prj.name}/lib/GLM/include",
        "%{prj.name}/lib/yaml-cpp/include",
        "%{prj.name}/lib/ImGui",
        "%{prj.name}/lib/stb",

    }

    links {

        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"

    }

    defines {

        "CU_ENGINE",

    }

    postbuildcommands {
    
        "{COPYDIR} assets ../Build/" .. outputDir .. "/Copper-Editor/assets",
        

    }

    filter "system:windows"
        systemversion "latest"

        defines {

            "CU_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"

        }

        files {

            "%{prj.name}/src/Platform/Windows/**.cpp",
            "%{prj.name}/src/Platform/OpenGL/**.cpp"

        }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

project "Copper-Editor"
    location "Copper-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("BuildInt/" .. outputDir .. "/%{prj.name}")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "Copper-Engine/src",
        "Copper-Engine/lib/spdlog",
        "Copper-Engine/lib/ImGui",
        "Copper-Engine/lib/GLM/include",

    }

    links {

        "Copper-Engine"

    }

    defines {

        "CU_EDITOR"

    }
    
    postbuildcommands {
        
        "{COPYDIR} assets ../Build/" .. outputDir .. "/Copper-Editor/assets",
        
    }

    filter "system:windows"
        staticruntime "on"
        systemversion "latest"

        defines {

            "CU_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"

        }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"