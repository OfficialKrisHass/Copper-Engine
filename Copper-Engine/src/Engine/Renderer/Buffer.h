#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Renderer/ElementType.h"

// Wrapper for platform dependant Vertex and Index buffers, source is in Platform/<API>/<API>Buffer.cpp

namespace Copper {

	class VertexBuffer {

	public:
		VertexBuffer() = default;
		VertexBuffer(float* vertices, uint32 size, const std::initializer_list<ElementType>& layout);

		void Bind() const;
		void Unbind() const;

		void SetData(float* vertices, uint32 count);

		inline uint32 Stride() const { return m_stride; }
		inline uint32 ElementCount() const { return (uint32) m_offsets.size(); }

		inline uint32 GetOffset(uint32 index) const { return m_offsets[index].second; }
		inline ElementType GetType(uint32 index) const { return m_offsets[index].first; }

	private:
		uint32 m_id = 0;
		uint32 m_stride = 0;
		std::vector<std::pair<ElementType, uint32>> m_offsets;

		void CalculateOffsetsAndStride(const std::initializer_list<ElementType>& layout);

	};

	class IndexBuffer {

	public:
		IndexBuffer() = default;
		IndexBuffer(uint32* indices, uint32 size);

		void Bind() const;
		void Unbind() const;

		void SetData(uint32* indices, uint32 count);

		uint32 Count() const { return m_count; }

	private:
		uint32 m_id = 0;
		uint32 m_count = 0;

	};

}