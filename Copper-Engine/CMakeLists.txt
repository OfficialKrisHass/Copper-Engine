message(STATUS "")
message(STATUS "BUILDING Copper-Engine")

# Source Files
list(APPEND sources src/Copper.h src/cupch.h src/cupch.cpp
                    lib/stb/stb/stb_image.h lib/stb/stb/stb_image.cpp
                    lib/ImGuizmo/ImGuizmo/ImGuizmo.h lib/ImGuizmo/ImGuizmo/ImGuizmo.cpp)

include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Core/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Events/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Renderer/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/UI/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Input/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Scene/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Components/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Scripting/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Physics/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Utilities/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Filesystem/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/YAMLOverloads/CMakeLists.txt)

include(${PROJECT_SOURCE_DIR}/Copper-Engine/lib/Copper-Math/CMath/CMakeLists.txt)
list(TRANSFORM CMathSources PREPEND lib/Copper-Math/)

list(APPEND sources ${CMathSources})

# Include dirs
list(APPEND includeDirs lib/GLFW/include lib/GLAD/include lib/GLM/include lib/yaml-cpp/include lib/assimp/include lib/mono/include lib/PhysX/include
                        lib/spdlog lib/ImGui lib/ImGuizmo lib/stb lib/Copper-Math lib/pfd)

# Defines
list(APPEND defines CU_ENGINE CU_EDITOR CU_CMAKE _CRT_SECURE_NO_WARNINGS INCLUDE_GLM)

# OS Specific
# TODO: Add options to control what os we are building for
list(APPEND defines CU_LINUX)

include(src/Platform/Linux/CMakeLists.txt)
include(src/Platform/OpenGL/CMakeLists.txt)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    list(APPEND defines CU_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    list(APPEND defines CU_RELEASE)
endif()

list(TRANSFORM sources PREPEND ${PROJECT_SOURCE_DIR}/Copper-Engine/)

configure_file(src/Config.h.in src/Config.h)

# Core
add_library(Copper-Engine STATIC ${sources})

# Compiler setup
set_target_properties(Copper-Engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Build/${OUTPUT_DIR}/Copper-Engine")
target_precompile_headers(Copper-Engine PRIVATE src/cupch.h)
target_compile_definitions(Copper-Engine PRIVATE ${defines})
target_compile_options(Copper-Engine PRIVATE -I/usr/lib/pkgconfig/../../include/mono-2.0 -D_REENTRANT)
target_link_options(Copper-Engine PRIVATE -L/usr/lib/pkgconfig/../../lib -lmono-2.0 -lm -lpthread)

target_include_directories(Copper-Engine PRIVATE src ${includeDirs} ${PROJECT_BINARY_DIR}/Copper-Engine/src)

# Libraries
add_subdirectory(lib/GLFW)
add_subdirectory(lib/GLAD)
add_subdirectory(lib/ImGui)
add_subdirectory(lib/yaml-cpp)
add_subdirectory(lib/assimp)

target_link_libraries(Copper-Engine PRIVATE glfw GLAD ImGui yaml-cpp assimp)