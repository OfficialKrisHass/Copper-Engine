cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SCENE_VERSION 0)

project(Copper-Engine VERSION $ENV{COPPER_VERSION} LANGUAGES CXX C)
message(STATUS "Building Copper Engine with configuration - ${CMAKE_BUILD_TYPE}")
message(STATUS "Version $ENV{COPPER_VERSION}")

set(OUTPUT_DIR linux-x86_64-${CMAKE_BUILD_TYPE})

add_subdirectory(Copper-Engine)
add_subdirectory(Copper-Editor)
add_subdirectory(Copper-Launcher)
add_subdirectory(Copper-APIBinder)

install(TARGETS Copper-Editor Copper-Launcher RUNTIME DESTINATION bin)

install(DIRECTORY Copper-Editor/assets Copper-Editor/lib Copper-Editor/util DESTINATION share/copper-editor)
install(FILES Copper-Editor/imgui.ini DESTINATION share/copper-editor)

install(DIRECTORY Copper-Launcher/assets DESTINATION share/copper-launcher)
install(FILES Copper-Launcher/imgui.ini DESTINATION share/copper-launcher)
