list(APPEND icallEditorSources Editor.cs.cpp)
list(TRANSFORM icallEditorSources PREPEND src/Engine/Scripting/InternalCalls/Editor/)

list(APPEND sources ${icallEditorSources})
