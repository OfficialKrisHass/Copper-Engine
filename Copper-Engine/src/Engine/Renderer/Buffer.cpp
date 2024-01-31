#include "cupch.h"
#include "Buffer.h"

namespace Copper {

	void VertexBuffer::CalculateOffsetsAndStride(const std::initializer_list<ElementType>& layout) {

		m_offsets.resize(layout.size());

		uint32 tmp = 0;
		uint32 i = 0;
		for (const ElementType type : layout) {

			m_offsets[i].first = type;
			m_offsets[i].second = tmp;

			tmp += TypeSize(type);
			m_stride += TypeSize(type);

			i++;

		}

	}

}