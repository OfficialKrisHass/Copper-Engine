list(APPEND icallCoreSources Entity.cs.cpp Game.cs.cpp UUID.cs.cpp)
list(TRANSFORM icallCoreSources PREPEND src/Engine/Scripting/InternalCalls/Core/)

list(APPEND sources ${icallCoreSources})
