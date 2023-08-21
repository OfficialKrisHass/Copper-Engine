list(APPEND scriptingSources Field.h MonoUtils.h ScriptingCore.h
                             InternalCalls.cpp MonoUtils.cpp ScriptingCore.cpp)
list(TRANSFORM scriptingSources PREPEND src/Engine/Scripting/)

list(APPEND sources ${scriptingSources})