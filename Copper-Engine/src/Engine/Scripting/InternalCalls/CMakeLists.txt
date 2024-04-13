list(APPEND scriptingInternalCallsSources Editor.cs.cpp)
list(TRANSFORM scriptingInternalCallsSources PREPEND src/Engine/Scripting/InternalCalls/)

list(APPEND sources ${scriptingInternalCallsSources})
