#include "cupch.h"
#include "Engine/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace Copper {

	static GLenum DataTypeToOpenGL(ElementType type) {

		switch (type) {

		case ElementType::Float:	return GL_FLOAT;
		case ElementType::Float2:	return GL_FLOAT;
		case ElementType::Float3:  return GL_FLOAT;
		case ElementType::Float4:  return GL_FLOAT;
		case ElementType::Int:		return GL_INT;
		case ElementType::Int2:	return GL_INT;
		case ElementType::Int3:	return GL_INT;
		case ElementType::Int4:	return GL_INT;
		case ElementType::Bool:	return GL_BOOL;


		}

		return 0;

	}

	VertexArray::VertexArray(IndexBuffer* ibo) {

		this->ibo = ibo;

		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

	}

	void VertexArray::SetVertexBuffer(VertexBuffer* vbo) {

		uint32 index = 0;
		for (const Element& e : *vbo) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, e.Dimensions(), DataTypeToOpenGL(e.type), GL_FALSE, vbo->Stride(), (void*) (uint64) e.offset);

			index++;

		}

	}
	void VertexArray::SetIndexBuffer(IndexBuffer* ibo) { this->ibo = ibo; }


	void VertexArray::Bind() const { glBindVertexArray(m_id); }
	void VertexArray::Unbind() const { glBindVertexArray(0); }

}