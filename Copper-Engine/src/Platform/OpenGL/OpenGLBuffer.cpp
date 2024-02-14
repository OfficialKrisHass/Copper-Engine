#include "cupch.h"
#include "Engine/Renderer/Buffer.h"

#include <glad/glad.h>

// OpenGL Implementation for the Vertex and Index Buffers

namespace Copper {

	//--------------
	// Vertex Buffer
	//--------------

	VertexBuffer::VertexBuffer(float* vertices, uint32 size, const std::initializer_list<ElementType>& layout) {

		CUP_FUNCTION();

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);

		CalculateOffsetsAndStride(layout);

	}

	void VertexBuffer::SetData(float* vertices, uint32 count) {

		CUP_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), vertices);

	}

	void VertexBuffer::Bind() const { CUP_FUNCTION(); glBindBuffer(GL_ARRAY_BUFFER, m_id); }
	void VertexBuffer::Unbind() const { CUP_FUNCTION(); glBindBuffer(GL_ARRAY_BUFFER, 0); }

	//-------------
	// Index Buffer
	//-------------

	IndexBuffer::IndexBuffer(uint32* indices, uint32 size) {

		CUP_FUNCTION();

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);

		m_count = size / sizeof(uint32);
		Log(sizeof(uint32));

	}

	void IndexBuffer::SetData(uint32* indices, uint32 count) {

		CUP_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32), indices);

		m_count = count;

	}

	void IndexBuffer::Bind() const { CUP_FUNCTION(); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
	void IndexBuffer::Unbind() const { CUP_FUNCTION(); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}