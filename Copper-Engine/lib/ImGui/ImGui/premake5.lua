project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("Build/" .. outputDir)
    objdir ("BuildInt/" .. outputDir)

    includedirs {

        ""

    }

    files {

        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",

        "misc/cpp/imgui_stdlib.h",
        "misc/cpp/imgui_stdlib.cpp"

    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"