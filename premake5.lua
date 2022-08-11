workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

outputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Copper-Engine/lib/GLFW"
include "Copper-Engine/lib/GLAD"

project "Copper-Engine"
    location "Copper-Engine"
    kind "SharedLib"
    language "C++"  

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

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/spdlog",
        "%{prj.name}/lib/GLFW/include",
        "%{prj.name}/lib/GLAD/include",
        "%{prj.name}/lib/GLM/include",

    }

    links {

        "GLFW",
        "GLAD",
        "opengl32.lib"

    }

    defines {

        "CU_ENGINE",
        "CU_DLL"

    }

    postbuildcommands {

        "{COPYDIR} assets ../Build/" .. outputDir .. "/Copper-Editor/assets"

    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {

            "CU_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"

        }

        files {

            "%{prj.name}/src/Platform/Windows/**.cpp",
            "%{prj.name}/src/Platform/OpenGL/**.cpp"

        }

        postbuildcommands {

            ("{COPY} %{cfg.buildtarget.relpath} ../Build/" .. outputDir .. "/Copper-Editor")

        }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        symbols "On"

        buildoptions "/MTd"

    filter "configurations:Release"
        defines "CU_RELEASE"
        optimize "On"

        buildoptions "/MT"

project "Copper-Editor"
    location "Copper-Editor"
    kind "ConsoleApp"
    language "C++"

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

    }

    links {

        "Copper-Engine"

    }

    defines {

        "CU_EDITOR"

    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {

            "CU_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"

        }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        symbols "On"

        buildoptions "/MTd"

    filter "configurations:Release"
        defines "CU_RELEASE"
        optimize "On"

        buildoptions "/MT"