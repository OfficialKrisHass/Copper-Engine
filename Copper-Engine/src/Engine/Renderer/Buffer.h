#pragma once

#include "Engine/Core/Core.h"
#include "cupch.h"

namespace Copper {

	enum class ElementType {

		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Bool,

	};
	static uint32_t CalculateSize(ElementType type) {

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

		ElementType type;
		uint32_t size;
		uint32_t offset;

		Element() = default;
		Element(const char* name, ElementType type) : type(type), size(CalculateSize(type)), offset(0) {}

		uint32_t Dimensions() const {

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
		VertexBuffer(uint32_t size);
		~VertexBuffer();

		void Bind();
		void Unbind();

		void SetLayout(std::initializer_list<Element> elements) { this->elements = elements; CalculateStuff(); }
		void SetData(std::vector<float> vertices);

		std::vector<Element>::iterator begin() { return elements.begin(); }
		std::vector<Element>::iterator end() { return elements.end(); }
		std::vector<Element>::const_iterator begin() const { return elements.begin(); }
		std::vector<Element>::const_iterator end() const { return elements.end(); }

		uint32_t Stride() { return stride; }

	private:
		uint32_t ID;
		uint32_t stride = 0;
		std::vector<Element> elements;

		void CalculateStuff() {

			uint32_t offset = 0;

			for (Element& e : elements) {

				e.offset = offset;
				offset += e.size;
				stride += e.size;

			}

		}

	};

	class IndexBuffer {

	public:
		IndexBuffer() = default;
		IndexBuffer(std::vector<uint32_t> indices);
		IndexBuffer(uint32_t size);
		~IndexBuffer();

		void Bind();
		void Unbind();

		void SetData(std::vector<uint32_t> indices);

		uint32_t Count() { return count; }

	private:
		uint32_t ID;
		uint32_t count;

	};

}