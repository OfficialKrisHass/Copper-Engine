workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

outputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

project "Copper-Engine"
    location "Copper-Engine"
    kind "SharedLib"
    language "C++"  

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("BuildInt/" .. outputDir .. "/%{prj.name}")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

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

        linkoptions {

            '/NODEFAULTLIB:"libcmt.lib"',
            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

    filter "configurations:Release"
        defines "CU_RELEASE"
        optimize "On"

        buildoptions "/MT"

        linkoptions {

            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"libcmtd.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

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

        "Copper-Engine/src"

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

        linkoptions {

            '/NODEFAULTLIB:"libcmt.lib"',
            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

    filter "configurations:Release"
        defines "CU_RELEASE"
        optimize "On"

        buildoptions "/MT"

        linkoptions {

            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"libcmtd.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }