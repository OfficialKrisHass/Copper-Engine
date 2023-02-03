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

	void VertexBuffer::SetData(const std::vector<float>& vertices) {

		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

	}

	void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }



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

	void IndexBuffer::SetData(const std::vector<uint32_t>& indices) {

		count = (uint32_t) indices.size();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(uint32_t), indices.data());

	}

	void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}