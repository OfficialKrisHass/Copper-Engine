#pragma once
// Source is in Platform/<API>/<API>VertexArray.cpp

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Buffer.h"


namespace Copper {

	// Higher level, platform indenpendant wrapper for the VertexArray
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
		uint32 m_id = 0;
		IndexBuffer* ibo = nullptr;

	};

}