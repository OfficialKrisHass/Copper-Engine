#include "cupch.h"
#include "Engine/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Copper {

	VertexBuffer::VertexBuffer(std::vector<float> vertices, std::initializer_list<Element> elements) : elements(elements) {

		CalculateStuff();

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	}

	VertexBuffer::~VertexBuffer() {

		glDeleteBuffers(1, &ID);

	}

	void VertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void VertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }



	IndexBuffer::IndexBuffer(std::vector<uint32_t> indices) : count(indices.size()) {

		glGenBuffers(1, &ID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	}

	IndexBuffer::~IndexBuffer() {

		glDeleteBuffers(1, &ID);

	}

	void IndexBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	void IndexBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

}