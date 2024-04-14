project "Copper-ScriptingAPI"
    location "."
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir("../Copper-Editor/assets/ScriptingAPI")
    objdir("../Copper-Editor/assets/ScriptingAPI/Int")

    files {

        "Source/**.cs"

    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"


