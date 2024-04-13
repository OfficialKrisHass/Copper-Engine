list(APPEND scriptingSources ScriptingEngine.cpp Assembly.cpp Script.cpp InternalCalls.cpp)
list(TRANSFORM scriptingSources PREPEND src/Engine/Scripting/)

list(APPEND sources ${scriptingSources})

include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Scripting/InternalCalls/CMakeLists.txt)
