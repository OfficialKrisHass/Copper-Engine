workspace "Copper-Engine"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Copper-Editor"

buildDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
print(buildDir)

group "Libraries"
    include "Copper-Engine/lib/GLFW"
    include "Copper-Engine/lib/GLAD"
    include "Copper-Engine/lib/ImGui"
    include "Copper-Engine/lib/yaml-cpp"
    include "Copper-Engine/lib/assimp"

    include "Copper-Engine/lib/Copper-Math/CMath"
group ""

include "Copper-ScriptingAPI"

local versionString = os.getenv("COPPER_VERSION")
local version = {}
versionString:gsub("[^%.]+", function (str) table.insert(version, tonumber(str)) end)

project "Copper-Engine"
    location "Copper-Engine"
    kind "StaticLib"
    language "C++"  

    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

    targetdir("Build/" .. buildDir .. "/%{prj.name}")
    objdir("Build/" .. buildDir .. "/%{prj.name}/Intermediate")

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
        "%{prj.name}/lib/spdlog",
        "%{prj.name}/lib/ImGui",
        "%{prj.name}/lib/ImGuizmo",
        "%{prj.name}/lib/stb",
        "%{prj.name}/lib/Copper-Math",
        "%{prj.name}/lib/portable-file-dialogs",
        "%{prj.name}/lib/uuid",

        "%{prj.name}/lib/mono/msvc/include",
        "%{prj.name}/lib/PhysX/physx/include",

        -- Stupid premake doesn't support include dirs for a single file so the entire project
        -- needs this, just for ImGuizmo.cpp to use it :)))))
        "%{prj.name}/lib/ImGui/ImGui",

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

        "CU_PREMAKE",

        "YAML_CPP_STATIC_DEFINE",

        "INCLUDE_GLM",
        "GLM_ENABLE_EXPERIMENTAL",

        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

        "VERSION_MAJOR=" .. version[1],
        "VERSION_MINOR=" .. version[2],
        "VERSION_PATCH=" .. version[3],
        "VERSION_TWEAK=" .. version[4],

        "SCENE_VERSION=0",

    }

    filter "action:vs*"
        buildoptions { "/utf-8" }

    filter "files:Copper-Engine/lib/ImGuizmo/ImGuizmo/**.cpp"
        flags { "NoPCH" }
        includedirs { "%{prj.name}/lib/ImGui/ImGui" }

    filter "configurations:Debug"
        defines {

            "CU_DEBUG",
            "CU_LOG_STATUS",

            "_DEBUG",

        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines {

            "CU_RELEASE",

            "NDEBUG",

        }
        runtime "Release"
        optimize "on"

project "Copper-Editor"
    location "Copper-Editor"
    kind "ConsoleApp"
    language "C++"

    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

    targetdir("Build/" .. buildDir .. "/%{prj.name}")
    objdir("Build/" .. buildDir .. "/%{prj.name}/Intermediate")

    debugdir "%{cfg.targetdir}"

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/FileWatch",

        "Copper-Engine/src",
        "Copper-Engine/lib/spdlog",
        "Copper-Engine/lib/ImGui",
        "Copper-Engine/lib/ImGuizmo",
        "Copper-Engine/lib/Copper-Math",
        "Copper-Engine/lib/uuid",
        
        "Copper-Engine/lib/GLM/include",
        "Copper-Engine/lib/yaml-cpp/include",
        "Copper-Engine/lib/assimp/include",

    }

    links {
        
        "Copper-Engine",
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "assimp",

    }


    defines {

        "CU_EDITOR",
        "CU_WINDOWS",
        "INCLUDE_GLM",

        "YAML_CPP_STATIC_DEFINE",
        
        "GLM_ENABLE_EXPERIMENTAL",
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",

        "VERSION_MAJOR=" .. version[1],
        "VERSION_MINOR=" .. version[2],
        "VERSION_PATCH=" .. version[3],
        "VERSION_TWEAK=" .. version[4],

    }

    filter "configurations:Debug"
        defines {

            "CU_DEBUG",
            "CU_LOG_STATUS",

            "_DEBUG"

        }
        runtime "Debug"
        symbols "on"

        links {

            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysX_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysXCommon_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysXFoundation_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysXExtensions_static_64.lib",

        }

        postbuildcommands {
            
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py libraries Debug %{cfg.system} " .. _MAIN_SCRIPT_DIR,
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py editor Debug %{cfg.system} " .. _MAIN_SCRIPT_DIR,

        }

        debugargs { "-e", os.getcwd() .. "/%{prj.name}/" }

    filter "configurations:Release"
        defines {

            "CU_RELEASE",

            "NDEBUG",

        }
        runtime "Release"
        optimize "on"

        links {

            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/release/PhysX_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/release/PhysXCommon_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/release/PhysXFoundation_64.lib",
            "Copper-Engine/lib/PhysX/physx/bin/win.x86_64.vc143.mt/release/PhysXExtensions_static_64.lib",

        }

        postbuildcommands {
            
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py libraries Release %{cfg.system} " .. _MAIN_SCRIPT_DIR,
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py editor Release %{cfg.system} " .. _MAIN_SCRIPT_DIR,

        }

    filter "system:windows"
        links {

            "Copper-Engine/lib/mono/msvc/build/sgen/x64/lib/%{cfg.buildcfg}/mono-2.0-sgen.lib",

        }

    filter "action:vs*"
        buildoptions { "/utf-8" }

project "Copper-APIBinder"
    location "Copper-APIBinder"
    kind "ConsoleApp"
    language "C++"

    cppdialect "C++20"
    staticruntime "on"

    targetdir("Build/" .. buildDir .. "/%{prj.name}")
    objdir("Build/" .. buildDir .. "/%{prj.name}/Intermediate")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "Copper-Engine/lib/mono/msvc/include",

    }

    links {

        "Copper-Engine/lib/mono/lib/%{cfg.buildcfg}/mono-2.0-sgen.lib",

    }

    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

project "Copper-Launcher"
    location "Copper-Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("Build/" .. buildDir .. "/Copper-Launcher")
    objdir("Build/" .. buildDir .. "/Copper-Launcher/Intermediate")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "Copper-Engine/lib/GLFW/include",
        "Copper-Engine/lib/GLAD/include",
        "Copper-Engine/lib/yaml-cpp/include",
        "Copper-Engine/lib/ImGui",
        "Copper-Engine/lib/portable-file-dialogs",

        -- Stupid premake doesn't support include dirs for a single file so the entire project
        -- needs this, just for ImGuizmo.cpp to use it :)))))
        "Copper-Engine/lib/ImGui/ImGui",

    }

    defines {

        "YAML_CPP_STATIC_DEFINE",
        "_CRT_SECURE_NO_WARNINGS",
        
        "CU_WINDOWS",

    }

    links {

        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",

    }
    
    filter "configurations:Debug"
        defines "CU_DEBUG"
        runtime "Debug"
        symbols "on"

        postbuildcommands {
            
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py launcher Debug %{cfg.system} "  .. _MAIN_SCRIPT_DIR,

        }

    filter "configurations:Release"
        defines "CU_RELEASE"
        runtime "Release"
        optimize "on"

        postbuildcommands {
            
            "python " .. _MAIN_SCRIPT_DIR .. "/scripts/post_build.py launcher Release %{cfg.system} "  .. _MAIN_SCRIPT_DIR,

        }