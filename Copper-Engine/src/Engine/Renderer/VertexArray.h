#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Buffer.h"

namespace Copper {

	class VertexArray {

	public:
		VertexArray() = default;
		VertexArray(IndexBuffer* ibo);

		void Bind() const;
		void Unbind() const;

		void SetVertexBuffer(VertexBuffer* vbo);
		void SetIndexBuffer(IndexBuffer* ibo);

		inline uint32 Count() const { return ibo->Count(); }

	private:
		uint32 m_id;
		IndexBuffer* ibo;

	};

}