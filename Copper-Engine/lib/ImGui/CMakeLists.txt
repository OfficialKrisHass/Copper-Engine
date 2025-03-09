message(STATUS "")
message(STATUS "BUILDING ImGui")

add_library(ImGui STATIC ImGui/imconfig.h ImGui/imgui.h ImGui/imgui_internal.h ImGui/imstb_rectpack.h ImGui/imstb_textedit.h ImGui/imstb_truetype.h ImGui/misc/cpp/imgui_stdlib.h
                         ImGui/imgui_demo.cpp ImGui/imgui_draw.cpp ImGui/imgui_tables.cpp ImGui/imgui_widgets.cpp ImGui/imgui.cpp ImGui/misc/cpp/imgui_stdlib.cpp
                         ImGui/backends/imgui_impl_opengl3.cpp ImGui/backends/imgui_impl_glfw.cpp)
set_target_properties(ImGui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Copper-Engine/lib/ImGui/Build/linux-x86_64-${CMAKE_BUILD_TYPE}")

set_source_files_properties(ImGui/backends/imgui_impl_glfw.cpp PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/Copper-Engine/lib/GLFW/include)

target_include_directories(ImGui PRIVATE ImGui)
target_compile_definitions(ImGui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

