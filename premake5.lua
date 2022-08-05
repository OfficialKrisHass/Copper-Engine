workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

outputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Copper-Engine/lib/GLFW"

project "Copper-Engine"
    location "Copper-Engine"
    kind "SharedLib"
    language "C++"  

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("BuildInt/" .. outputDir .. "/%{prj.name}")

    pchheader "cupch.h"
    pchsource "Copper-Engine/src/cupch.cpp"

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/spdlog",
        "%{prj.name}/lib/GLFW/include",

    }

    links {

        "GLFW",
        "opengl32.lib"

    }

    defines {

        "CU_ENGINE",
        "CU_DLL"

    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {

            "CU_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"

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