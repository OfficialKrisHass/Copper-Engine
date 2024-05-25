list(APPEND icallMathSources Quaternion.cs.cpp)
list(TRANSFORM icallMathSources PREPEND src/Engine/Scripting/InternalCalls/Math/)

list(APPEND sources ${icallMathSources})
