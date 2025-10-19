list(APPEND icallRendererSources Material.cs.cpp)
list(TRANSFORM icallCoreSources PREPEND src/Engine/Scripting/InternalCalls/Renderer/)

list(APPEND sources ${icallCoreSources})
