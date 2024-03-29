workspace "LinuxTesting"
    location ""
    architecture "x64"
    configurations { "Debug", "Release" }

project "LinuxTesting"
    location ""
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir("Binaries")
    objdir("Objs")

    files {

        "Assets/**.cs"

    }

    links {

        "Binaries/Copper-ScriptingAPI.dll"

    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Off"