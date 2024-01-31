#include "cupch.h"
#include "Engine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Copper {

	constexpr GLenum TypeToOpenGL(ElementType type) {

		switch (type) {

		case ElementType::Float:
		case ElementType::Vec2:
		case ElementType::Vec3:
		case ElementType::Vec4:		return GL_FLOAT;

		case ElementType::Int:
		case ElementType::Vec2I:
		case ElementType::Vec3I:
		case ElementType::Vec4I:	return GL_INT;

		case ElementType::Bool:		return GL_BOOL;

		}

		LogWarn("Couldn't get the OpenGL data type for {} (ElementType)", (uint32) type);
		return 0;

	}

	VertexArray::VertexArray(IndexBuffer* ibo) {

		this->ibo = ibo;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

	}

	void VertexArray::SetVertexBuffer(VertexBuffer* vbo) {

		uint32 count = vbo->ElementCount();
		for (uint32 i = 0; i < count; i++) {

			ElementType type = vbo->GetType(i);

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, TypeDimensions(type), TypeToOpenGL(type), GL_FALSE, vbo->Stride(), (void*) (uint64) vbo->GetOffset(i));

		}

	}
	void VertexArray::SetIndexBuffer(IndexBuffer* ibo) { this->ibo = ibo; }

	void VertexArray::Bind() const { glBindVertexArray(m_id); }
	void VertexArray::Unbind() const { glBindVertexArray(0); }

}