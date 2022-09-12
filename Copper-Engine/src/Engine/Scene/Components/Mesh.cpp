#include "cupch.h"
#include "Mesh.h"

#include "Engine/Renderer/Buffer.h"

namespace Copper {

	void Mesh::Regenerate() {

		vao = new VertexArray();
		VertexBuffer* vbo = new VertexBuffer(vertices, { {"Position", ElementType::Float3}, {"Color", ElementType::Float3}, {"Normal", ElementType::Float3} });
		IndexBuffer* ibo = new IndexBuffer(indices);

		vao->SetVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);

		vbo->Unbind();
		vao->Unbind();

	}

}