list(APPEND icallCoreSources Editor.cs.cpp)
list(TRANSFORM icallCoreSources PREPEND src/Engine/Scripting/InternalCalls/Core/)

list(APPEND sources ${icallCoreSources})
