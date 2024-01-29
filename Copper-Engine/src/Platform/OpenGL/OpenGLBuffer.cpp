#include "cupch.h"
#include "Engine/Renderer/Buffer.h"

#include <glad/glad.h>

// OpenGL Implementation for the Vertex and Index Buffers

namespace Copper {

	//--------------
	// Vertex Buffer
	//--------------

	VertexBuffer::VertexBuffer(std::vector<float> vertices) {

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	}
	VertexBuffer::VertexBuffer(uint32 size) {

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	}

	void VertexBuffer::SetData(float* vertices, uint32 count) {

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float), vertices);

	}

	void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
	void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

	//-------------s
	// Index Buffer
	//-------------

	IndexBuffer::IndexBuffer(const std::vector<uint32>& indices) : m_count((uint32) indices.size()) {

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), indices.data(), GL_STATIC_DRAW);

	}
	IndexBuffer::IndexBuffer(uint32 size) : m_count(size / sizeof(uint32)) {

		glGenBuffers(1, &m_id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	}

	void IndexBuffer::SetData(uint32* indices, uint32 count) {

		m_count = count;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32), indices);

	}

	void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
	void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}