#include "cupch.h"
#include "Engine/Renderer/VertexArray.h"

#include "Platform/OpenGL/OpenglTypes.h"

#include <glad/glad.h>

namespace Copper {

    void VertexArray::Create(IndexBuffer* ibo) {

        CUP_FUNCTION();

        m_ibo = ibo;

        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);

    }

    void VertexArray::SetVertexBuffer(VertexBuffer* vbo) {

        CUP_FUNCTION();

        uint32 count = vbo->GetElementCount();
        for (uint32 i = 0; i < count; i++) {

            ElementType type = vbo->GetType(i);


            glEnableVertexAttribArray(i);
            switch (type) {

            case ElementType::Float:
            case ElementType::Vec2:
            case ElementType::Vec3:
            case ElementType::Vec4: glVertexAttribPointer(i, TypeDimensions(type), TypeToOpenGL(type), GL_FALSE, vbo->GetStride(), (void*) (uint64) vbo->GetOffset(i)); break;

            case ElementType::Int:
            case ElementType::UInt:     
            case ElementType::Vec2I:
            case ElementType::Vec3I:
            case ElementType::Vec4I:
            case ElementType::Bool: glVertexAttribIPointer(i, TypeDimensions(type), TypeToOpenGL(type), vbo->GetStride(), (void*) (uint64) vbo->GetOffset(i)); break;

            }

        }

    }

    void VertexArray::Bind() const {

        CUP_FUNCTION();

        glBindVertexArray(m_id);

    }
    void VertexArray::Unbind() const {

        CUP_FUNCTION();

        glBindVertexArray(0);

    }

}
