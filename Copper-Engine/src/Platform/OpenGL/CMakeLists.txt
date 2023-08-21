list(APPEND openglSources OpenGLBuffer.cpp OpenGLFrameBuffer.cpp OpenGLRenderer.cpp OpenGLShader.cpp OpenGLTexture.cpp OpenGLVertexArray.cpp)
list(TRANSFORM openglSources PREPEND src/Platform/OpenGL/)

list(APPEND sources ${openglSources})