message(STATUS "")
message(STATUS "BUILDING GLAD")

add_library(GLAD STATIC include/KHR/khrplatform.h include/glad/glad.h src/glad.c)
set_target_properties(GLAD PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Copper-Engine/lib/GLAD/Build/linux-x86_64-${CMAKE_BUILD_TYPE}")

target_include_directories(GLAD PRIVATE include)