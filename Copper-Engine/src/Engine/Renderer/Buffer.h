#pragma once

#include "Engine/Core/Core.h"

// Wrapper for platform dependant Vertex and Index buffers, source is in Platform/<API>/<API>Buffer.cpp
// TODO: Maybe remove this so it is not exposed ?
// TODO: Or at least make my own implementation and not the one I stole from the Cherno

namespace Copper {

	enum class ElementType {

		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Bool,

	};
	static uint32 CalculateSize(ElementType type) {

		switch (type) {

		case ElementType::Float:    return 4;
		case ElementType::Float2:   return 4 * 2;
		case ElementType::Float3:   return 4 * 3;
		case ElementType::Float4:   return 4 * 4;
		case ElementType::Int:      return 4;
		case ElementType::Int2:     return 4 * 2;
		case ElementType::Int3:     return 4 * 3;
		case ElementType::Int4:     return 4 * 4;
		case ElementType::Bool:     return 1;

		}

		return 0;

	}
	struct Element {

	public:
		ElementType type;
		uint32 size;
		uint32 offset;

		Element() = default;
		Element(const char* name, ElementType type) : type(type), size(CalculateSize(type)), offset(0) {}

		uint32 Dimensions() const {

			switch (type) {

			case ElementType::Float:    return 1;
			case ElementType::Float2:   return 2;
			case ElementType::Float3:   return 3;
			case ElementType::Float4:   return 4;
			case ElementType::Int:      return 1;
			case ElementType::Int2:     return 2;
			case ElementType::Int3:     return 3;
			case ElementType::Int4:     return 4;
			case ElementType::Bool:     return 1;

			}

			return 0;

		}

	};

	class VertexBuffer {

	private:

	public:
		VertexBuffer() = default;
		VertexBuffer(std::vector<float> vertices);
		VertexBuffer(uint32 size);

		void Bind() const;
		void Unbind() const;

		void SetLayout(const std::initializer_list<Element>& elements) { this->elements = elements; CalculateStuff(); }
		void SetData(const std::vector<float>& vertices);

		std::vector<Element>::iterator begin() { return elements.begin(); }
		std::vector<Element>::iterator end() { return elements.end(); }
		std::vector<Element>::const_iterator begin() const { return elements.begin(); }
		std::vector<Element>::const_iterator end() const { return elements.end(); }

		uint32 Stride() const { return m_stride; }

	private:
		uint32 m_id = 0;
		uint32 m_stride = 0;
		std::vector<Element> elements;

		void CalculateStuff() {

			uint32 offset = 0;

			for (Element& e : elements) {

				e.offset = offset;
				offset += e.size;
				m_stride += e.size;

			}

		}

	};

	class IndexBuffer {

	public:
		IndexBuffer() = default;
		IndexBuffer(const std::vector<uint32>& indices);
		IndexBuffer(uint32 size);

		void Bind() const;
		void Unbind() const;

		void SetData(const std::vector<uint32>& indices);

		uint32 Count() const { return m_count; }

	private:
		uint32 m_id = 0;
		uint32 m_count = 0;

	};

}