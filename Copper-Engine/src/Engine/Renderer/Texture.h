#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class Texture {

	public:
		enum class Format : uint8 {

			RGB,
			RGBA

		};

		Texture() = default;
		Texture(const UVector2I& size, Format format = Format::RGB, uint8* pixels = nullptr) { Create(size.x, size.y, format, pixels); }
		Texture(uint32 width, uint32 height, Format format = Format::RGB, uint8* pixels = nullptr) { Create(width, height, format, pixels); }
		Texture(const std::string& filePath, Format format = Format::RGB) { Create(filePath, format); }

		void Create(const UVector2I& size, Format format = Format::RGB, uint8* pixels = nullptr) { Create(size.x, size.y, format, pixels); }
		void Create(uint32 width, uint32 height, Format format = Format::RGB, uint8* pixels = nullptr);
		void Create(const std::string& filePath, Format format = Format::RGB);

		void SetPixels(uint8* pixels, Format format = Format::RGB) { Create(m_size.x, m_size.y, format, pixels); }

		void Delete();

		void Bind() const;
		void Unbind() const;

		uint32 ID() const { return m_id; }
		const UVector2I& Size() const { return m_size; }

	private:
		uint32 m_id = 0;
		UVector2I m_size = UVector2I::zero;

	};

}