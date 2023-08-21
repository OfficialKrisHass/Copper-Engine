message(STATUS "")
message(STATUS "BUILDING ImGui")

add_library(ImGui STATIC ImGui/imconfig.h ImGui/imgui.h ImGui/imgui_internal.h ImGui/imstb_rectpack.h ImGui/imstb_textedit.h ImGui/imstb_truetype.h ImGui/misc/cpp/imgui_stdlib.h
                         ImGui/imgui_demo.cpp ImGui/imgui_draw.cpp ImGui/imgui_tables.cpp ImGui/imgui_widgets.cpp ImGui/imgui.cpp ImGui/misc/cpp/imgui_stdlib.cpp)
set_target_properties(ImGui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Copper-Engine/lib/ImGui/Build/linux-x86_64-${CMAKE_BUILD_TYPE}")

target_include_directories(ImGui PRIVATE ImGui)