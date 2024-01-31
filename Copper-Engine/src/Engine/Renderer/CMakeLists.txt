list(APPEND rendererSources Buffer.h Color.h FrameBuffer.h Mesh.h Primitives.h Renderer.h Shader.h Texture.h VertexArray.h
                            Buffer.h Color.cpp Renderer.cpp)
list(TRANSFORM rendererSources PREPEND src/Engine/Renderer/)

list(APPEND sources ${rendererSources})