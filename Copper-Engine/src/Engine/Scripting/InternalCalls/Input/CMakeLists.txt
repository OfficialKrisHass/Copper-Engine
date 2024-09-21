list(APPEND icallInputSources Input.cs.cpp Cursor.cs.cpp)
list(TRANSFORM icallInputSources PREPEND src/Engine/Scripting/InternalCalls/Input/)

list(APPEND sources ${icallInputSources})
