#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Buffer.h"

namespace Copper {

	class VertexArray {

	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

		void SetVertexBuffer(Shared<VertexBuffer> vbo);
		void SetIndexBuffer(Shared<IndexBuffer> ibo);

		inline uint32_t Count() { return ibo->Count(); }

	private:
		uint32_t ID;
		Shared<IndexBuffer> ibo;

	};

}