#include "cupch.h"
#include "Engine/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Copper {

	VertexBuffer::VertexBuffer(std::vector<float> vertices) {

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	}

	VertexBuffer::VertexBuffer(uint32_t size) {

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	}

	VertexBuffer::~VertexBuffer() {

		glDeleteBuffers(1, &ID);

	}

	void VertexBuffer::SetData(std::vector<float> vertices) {

		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

	}

	void VertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void VertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }



	IndexBuffer::IndexBuffer(std::vector<uint32_t> indices) : count((uint32_t) indices.size()) {

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	}

	IndexBuffer::IndexBuffer(uint32_t size) : count(size / sizeof(uint32_t)) {

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	}

	IndexBuffer::~IndexBuffer() {

		glDeleteBuffers(1, &ID);

	}

	void IndexBuffer::SetData(std::vector<uint32_t> indices) {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(uint32_t), indices.data());

	}

	void IndexBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	void IndexBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}