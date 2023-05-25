project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir("Build/" .. outputDir)
    objdir("BuildInt/" .. outputDir)

    includedirs { "ImGui" }

    files {

        "ImGui/imconfig.h",
        "ImGui/imgui_demo.cpp",
        "ImGui/imgui_draw.cpp",
        "ImGui/imgui_internal.h",
        "ImGui/imgui_tables.cpp",
        "ImGui/imgui_widgets.cpp",
        "ImGui/imgui.cpp",
        "ImGui/imgui.h",
        "ImGui/imstb_rectpack.h",
        "ImGui/imstb_textedit.h",
        "ImGui/imstb_truetype.h",

        "ImGui/misc/cpp/imgui_stdlib.h",
        "ImGui/misc/cpp/imgui_stdlib.cpp"

    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
    
    filter "configurations:Release"
        runtime "Release"

    filter { "system:windoww", "configurations:Release" }
        buildoptions "/MT"