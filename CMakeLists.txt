cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

set(SCENE_VERSION 0)

project(Copper-Engine VERSION 0.3.0.97 LANGUAGES CXX C)
message(STATUS "Building with configuration - ${CMAKE_BUILD_TYPE}")

set(OUTPUT_DIR linux-x86_64-${CMAKE_BUILD_TYPE})

add_subdirectory(Copper-Engine)
add_subdirectory(Copper-Editor)
add_subdirectory(Copper-Launcher)
