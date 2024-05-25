list(APPEND icallComponentsSources Component.cs.cpp Transform.cs.cpp)
list(TRANSFORM icallComponentsSources PREPEND src/Engine/Scripting/InternalCalls/Components/)

list(APPEND sources ${icallComponentsSources})
