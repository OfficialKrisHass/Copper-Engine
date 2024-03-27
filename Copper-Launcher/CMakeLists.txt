message(STATUS "")
message(STATUS "BUILDING Copper-Launcher")

# Sources

list(APPEND sources Base.h Dialogs.cpp Dialogs.h Entry.cpp LauncherApp.cpp PersistentData.cpp PersistentData.h ProjectEntry.cpp ProjectEntry.h ThemeEditor.cpp
                    ThemeEditor.h UI.cpp UI.h Window.cpp Window.h)
list(TRANSFORM sources PREPEND ${PROJECT_SOURCE_DIR}/Copper-Launcher/src/)

# Include dirs

list(APPEND includes GLFW/include GLAD/include yaml-cpp/include ImGui pfd)
list(TRANSFORM includes PREPEND ${PROJECT_SOURCE_DIR}/Copper-Engine/lib/)

# Defines

list(APPEND defines YAML_CPP_STATIC_DEFINE)
list(APPEND defines CU_LINUX)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    list(APPEND defines CU_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    list(APPEND defines CU_RELEASE)
endif()

# Core

add_executable(Copper-Launcher ${sources})

set_target_properties(Copper-Launcher PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Build/${OUTPUT_DIR}/Copper-Launcher")

target_include_directories(Copper-Launcher PRIVATE ${includes})
target_compile_definitions(Copper-Launcher PRIVATE ${defines})
target_link_libraries(Copper-Launcher glfw GLAD ImGui yaml-cpp)
