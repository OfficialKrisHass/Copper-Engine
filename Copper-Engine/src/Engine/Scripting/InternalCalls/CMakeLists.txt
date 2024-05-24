list(APPEND scriptingInternalCallsSources Editor.cs.cpp Entity.cs.cpp Component.cs.cpp Transform.cs.cpp Quaternion.cs.cpp)
list(TRANSFORM scriptingInternalCallsSources PREPEND src/Engine/Scripting/InternalCalls/)

list(APPEND sources ${scriptingInternalCallsSources})
