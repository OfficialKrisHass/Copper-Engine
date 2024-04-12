list(APPEND scriptingInternalCallsSources )
list(TRANSFORM scriptingInternalCallsSources PREPEND src/Engine/Scripting/InternalCalls/)

list(APPEND sources ${scriptingInternalCallsSources})
