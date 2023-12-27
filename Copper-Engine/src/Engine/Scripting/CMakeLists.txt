list(APPEND scriptingSources Field.h MonoUtils.h ScriptingCore.h InternalCalls.h
                             MonoUtils.cpp ScriptingCore.cpp InternalCalls.cpp)
list(TRANSFORM scriptingSources PREPEND src/Engine/Scripting/)

list(APPEND sources ${scriptingSources})

include(${PROJECT_SOURCE_DIR}/Copper-Engine/src/Engine/Scripting/InternalCalls/CMakeLists.txt)