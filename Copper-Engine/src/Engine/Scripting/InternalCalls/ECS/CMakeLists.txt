list(APPEND icallECSSources Entity.cs.cpp)
list(TRANSFORM icallECSSources PREPEND src/Engine/Scripting/InternalCalls/ECS/)

list(APPEND sources ${icallECSSources})
