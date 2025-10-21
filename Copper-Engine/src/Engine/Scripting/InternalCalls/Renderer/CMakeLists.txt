list(APPEND icallRendererSources Material.cs.cpp)
list(TRANSFORM icallRendererSources PREPEND src/Engine/Scripting/InternalCalls/Renderer/)

list(APPEND sources ${icallRendererSources})
