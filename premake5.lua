workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

outputDir = "windows-%{cfg.architecture}-%{cfg.buildcfg}"

group "Libraries"
    include "Copper-Engine/lib/GLFW"
    include "Copper-Engine/lib/GLAD"
    include "Copper-Engine/lib/ImGui"
    include "Copper-Engine/lib/yaml-cpp"
    include "Copper-Engine/lib/assimp"

    include "Copper-Engine/lib/Copper-Math/CMath"
group ""

project "Copper-Engine"
    location "Copper-Engine"
    kind "StaticLib"
    language "C++"  

    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

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

        "%{prj.name}/src/Platform/Windows/**.cpp",
        "%{prj.name}/src/Platform/OpenGL/**.cpp",

        "%{prj.name}/lib/stb/stb/stb_image.cpp",

        "%{prj.name}/lib/ImGuizmo/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/lib/ImGuizmo/ImGuizmo/ImGuizmo.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/GLFW/include",
        "%{prj.name}/lib/GLAD/include",
        "%{prj.name}/lib/GLM/include",
        "%{prj.name}/lib/yaml-cpp/include",
        "%{prj.name}/lib/assimp/include",
        "%{prj.name}/lib/mono/include",
        "%{prj.name}/lib/PhysX/include",
        "%{prj.name}/lib/spdlog",
        "%{prj.name}/lib/ImGui",
        "%{prj.name}/lib/ImGuizmo",
        "%{prj.name}/lib/stb",
        "%{prj.name}/lib/Copper-Math",
        "%{prj.name}/lib/pfd",
        "%{prj.name}/lib/uuid",

    }

    links {

        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "assimp",
        "opengl32.lib",

    }

    defines {

        "CU_ENGINE",
        "CU_EDITOR",
        "CU_WINDOWS",

        "YAML_CPP_STATIC_DEFINE",

        "INCLUDE_GLM",
        "GLM_ENABLE_EXPERIMENTAL",

        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        
        "VERSION_MAJOR=0",
        "VERSION_MINOR=3",
        "VERSION_PATCH=0",
        "VERSION_TWEAK=85",
        
        "SCENE_VERSION=0",

    }

    filter "files:Copper-Engine/lib/ImGuizmo/ImGuizmo/**.cpp"
        flags { "NoPCH" }

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

    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

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
        "Copper-Engine/lib/ImGuizmo",
        "Copper-Engine/lib/Copper-Math",
        "Copper-Engine/lib/uuid",
        
        "Copper-Engine/lib/GLM/include",
        "Copper-Engine/lib/yaml-cpp/include",
        "Copper-Engine/lib/assimp/include",

        "%{prj.name}/lib/FileWatch",

    }

    links {
        
        "Copper-Engine",
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "assimp",

        "Copper-Engine/lib/mono/lib/%{cfg.buildcfg}/mono-2.0-sgen.lib",
        "Copper-Engine/lib//PhysX/lib/%{cfg.buildcfg}/PhysX_64.lib",
        "Copper-Engine/lib//PhysX/lib/%{cfg.buildcfg}/PhysXCommon_64.lib",
        "Copper-Engine/lib//PhysX/lib/%{cfg.buildcfg}/PhysXFoundation_64.lib",
        "Copper-Engine/lib//PhysX/lib/%{cfg.buildcfg}/PhysXExtensions_static_64.lib",

    }

    defines {

        "CU_EDITOR",
        "CU_WINDOWS",
        "INCLUDE_GLM",

        "YAML_CPP_STATIC_DEFINE",
        
        "GLM_ENABLE_EXPERIMENTAL",
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

    }

    postbuildcommands {

        "{COPYDIR} assets ../Build/" .. outputDir .. "/Copper-Editor/assets",

        "{COPYDIR} lib/mono/lib ../Build/" .. outputDir .. "/Copper-Editor/lib/mono/lib",

        "{COPYDIR} lib/PhysX/windows/%{cfg.buildcfg} ../Build/" .. outputDir .. "/Copper-Editor",
        "{COPYFILE} lib/mono/bin/windows/%{cfg.buildcfg}/mono-2.0-sgen.dll ../Build/" .. outputDir .. "/Copper-Editor",

    }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

project "Copper-ScriptingAPI"
    location "Copper-ScriptingAPI"
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir("Copper-Editor/assets/ScriptAPI")
    objdir("Copper-Editor/assets/ScriptAPI/Int")

    files {

        "%{prj.name}/Source/**.cs"

    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

project "Copper-CppTesting"
    location "Copper-CppTesting"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("BuildInt/" .. outputDir .. "/%{prj.name}")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "Copper-Engine/src",

        "Copper-Engine/lib/spdlog",
        "Copper-Engine/lib/ImGui",
        "Copper-Engine/lib/ImGuizmo",
        "Copper-Engine/lib/Copper-Math",
        
        "Copper-Engine/lib/GLM/include",
        "Copper-Engine/lib/yaml-cpp/include",
        "Copper-Engine/lib/assimp/include",

    }

    links {

        "Copper-Engine",
        "Copper-Engine/lib/mono/lib/%{cfg.buildcfg}/mono-2.0-sgen.lib",

    }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

        linkoptions {

            '/NODEFAULTLIB:"libcmt.lib"',
            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

        linkoptions {

            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"libcmtd.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

project "Copper-Launcher"
    location "Copper-Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("Build/" .. outputDir .. "/Copper-Editor")
    objdir("BuildInt/" .. outputDir .. "/Copper-Editor")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "Copper-Engine/lib/GLFW/include",
        "Copper-Engine/lib/GLAD/include",
        "Copper-Engine/lib/yaml-cpp/include",
        "Copper-Engine/lib/ImGui",

    }

    links {

        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",

    }

    defines { "Launcher=Copper::Launcher" }
    
    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

        linkoptions {

            '/NODEFAULTLIB:"libcmt.lib"',
            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

        linkoptions {

            '/NODEFAULTLIB:"msvcrt.lib"',
            '/NODEFAULTLIB:"libcmtd.lib"',
            '/NODEFAULTLIB:"msvcrtd.lib"'

        }