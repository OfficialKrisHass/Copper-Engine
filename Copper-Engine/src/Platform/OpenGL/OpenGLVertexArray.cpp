#include "cupch.h"
#include "Engine/Renderer/VertexArray.h"

#include "Platform/OpenGL/OpenglTypes.h"

#include <glad/glad.h>

namespace Copper {

	VertexArray::VertexArray(IndexBuffer* ibo) {

		this->ibo = ibo;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

	}

	void VertexArray::SetVertexBuffer(VertexBuffer* vbo) {

		uint32 count = vbo->ElementCount();
		for (uint32 i = 0; i < count; i++) {

			ElementType type = vbo->GetType(i);

			glVertexAttribPointer(i, TypeDimensions(type), TypeToOpenGL(type), GL_FALSE, vbo->Stride(), (void*) (uint64) vbo->GetOffset(i));
			glEnableVertexAttribArray(i);

		}

	}
	void VertexArray::SetIndexBuffer(IndexBuffer* ibo) { this->ibo = ibo; }

	void VertexArray::Bind() const { glBindVertexArray(m_id); }
	void VertexArray::Unbind() const { glBindVertexArray(0); }

}