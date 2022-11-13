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

	VertexArray::VertexArray() {

		glGenVertexArrays(1, &ID);
		glBindVertexArray(ID);

	}

	VertexArray::~VertexArray() {

		glDeleteVertexArrays(1, &ID);

	}

	void VertexArray::SetVertexBuffer(Shared<VertexBuffer> vbo) {

		uint32_t index = 0;

		for (const Element& e : *vbo) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, e.Dimensions(), DataTypeToOpenGL(e.type), GL_FALSE, vbo->Stride(), (void*) (uint64_t) e.offset);

			index++;

		}

	}

	void VertexArray::SetIndexBuffer(Shared<IndexBuffer> ibo) { this->ibo = ibo; }


	void VertexArray::Bind() { glBindVertexArray(ID); }
	void VertexArray::Unbind() { glBindVertexArray(0); }

}