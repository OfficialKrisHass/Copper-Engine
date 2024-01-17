workspace "Copper-ScriptingAPI"
    architecture "x64"
    configurations { "Debug", "Release", }

project "Copper-ScriptingAPI"
    location "."
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir("../Copper-Editor/assets/ScriptAPI")
    objdir("../Copper-Editor/assets/ScriptAPI/Int")

    files {

        "Source/**.cs"

    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"