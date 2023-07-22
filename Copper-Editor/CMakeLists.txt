message(STATUS "")
message(STATUS "BUILDING Copper-Editor")

# Source Files
include(${PROJECT_SOURCE_DIR}/Copper-Editor/src/Core/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Editor/src/Panels/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Editor/src/Projects/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/Copper-Editor/src/Viewport/CMakeLists.txt)

list(TRANSFORM sources PREPEND ${PROJECT_SOURCE_DIR}/Copper-Editor/)

# Include dirs
list(APPEND includeDirs lib/spdlog lib/ImGui lib/ImGuizmo lib/Copper-Math lib/GLM/include lib/yaml-cpp/include lib/assimp/include)
list(TRANSFORM includeDirs PREPEND ../Copper-Engine/)

list(APPEND includeDirs ${PROJECT_BINARY_DIR}/Copper-Engine/src ../Copper-Engine/src lib/FileWatch src)

# Defines
list(APPEND defines CU_EDITOR INCLUDE_GLM _CRT_SECURE_NO_WARNING)

# OS Specific
list(APPEND defines CU_LINUX)

# Config Specific
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    list(APPEND defines CU_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    list(APPEND defines CU_RELEASE)
endif()

# Core
add_executable(Copper-Editor ${sources})

# Compiler setup
set_target_properties(Copper-Editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Build/${OUTPUT_DIR}/Copper-Editor")
target_compile_definitions(Copper-Editor PRIVATE ${defines})
target_compile_options(Copper-Engine PRIVATE -I/usr/include/mono-2.0 -D_REENTRANT)
target_link_options(Copper-Engine PRIVATE -lPhysXExtensions_static_64 -lPhysX_static_64 -lPhysXPvdSDK_static_64 -lPhysXCooking_static_64 -lPhysXCommon_static_64 -lPhysXFoundation_static_64)

target_include_directories(Copper-Editor PRIVATE ${includeDirs})

# Libaries
target_link_libraries(Copper-Editor PRIVATE Copper-Engine glfw GLAD ImGui yaml-cpp assimp
                                            monosgen-2.0 /usr/lib/libpthread.a
                                            -lX11 -lstdc++fs)