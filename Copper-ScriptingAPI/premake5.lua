buildDir = "../Build/%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

project "Copper-ScriptingAPI"
    location "."
    language "C#"
    kind "SharedLib"
    dotnetframework "4.8"

    targetdir(buildDir .. "/%{prj.name}")
    objdir(buildDir .. "/%{prj.name}/Intermediate")

    files {

        "Source/**.cs"

    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"


