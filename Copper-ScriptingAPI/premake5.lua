buildDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

project "Copper-ScriptingAPI"
    location "."
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir("../Build/" .. buildDir .. "/%{prj.name}")
    objdir("../Build/" .. buildDir .. "/%{prj.name}/Intermediate")

    files {

        "Source/**.cs"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"
