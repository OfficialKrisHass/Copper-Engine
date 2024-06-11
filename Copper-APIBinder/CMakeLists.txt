message(STATUS "")
message(STATUS "BUILDING Copper-APIBinder")

# Sources

include(${PROJECT_SOURCE_DIR}/Copper-APIBinder/src/CMakeLists.txt)

# Include Dirs

set(includeDirs src ../Copper-Engine/lib/mono/include)

# Defines

set(defines CU_CMAKE CU_LINUX)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(defines ${defines} CU_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(defines ${defines} CU_RELEASE)
endif()

# Core setup

add_executable(Copper-APIBinder ${sources})
set_target_properties(Copper-APIBinder PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Build/${OUTPUT_DIR}/Copper-APIBinder")

target_include_directories(Copper-APIBinder PRIVATE ${includeDirs})
target_compile_definitions(Copper-APIBinder PRIVATE ${defines})

target_link_directories(Copper-APIBinder PRIVATE ../Copper-Editor/lib/mono/bin/linux/${CMAKE_BUILD_TYPE})
target_link_libraries(Copper-APIBinder PRIVATE monosgen-2.0)
