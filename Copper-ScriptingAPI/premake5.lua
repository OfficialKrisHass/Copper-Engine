buildDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

project "Copper-ScriptingAPI"
    location "."
    language "C#"
    kind "SharedLib"
    dotnetframework "net8.0"

    targetdir("../Build/" .. buildDir .. "/%{prj.name}")
    objdir("../Build/" .. buildDir .. "/%{prj.name}/Intermediate")

    filter "action:gmake*"
        files {

            "Source/**.cs"

        }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"
