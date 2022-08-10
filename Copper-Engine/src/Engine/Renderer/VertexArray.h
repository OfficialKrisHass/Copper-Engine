#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Buffer.h"

namespace Copper {

	class COPPER_API VertexArray {

	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

		void SetVertexBuffer(VertexBuffer* vbo);
		void SetIndexBuffer(IndexBuffer* ibo);

		inline uint32_t Count() { return ibo->Count(); }

	private:
		uint32_t ID;
		IndexBuffer* ibo;

	};

}