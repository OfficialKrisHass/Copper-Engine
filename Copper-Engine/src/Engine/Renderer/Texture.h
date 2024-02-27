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
		inline Texture(const UVector2I& size, Format format = Format::RGB, uint8* pixels = nullptr) { Create(size.x, size.y, format, pixels); }
		inline Texture(uint32 width, uint32 height, Format format = Format::RGB, uint8* pixels = nullptr) { Create(width, height, format, pixels); }
		inline Texture(const std::string& filePath, Format format = Format::RGB) { Create(filePath, format); }

		inline void Create(const UVector2I& size, Format format = Format::RGB, uint8* pixels = nullptr) { Create(size.x, size.y, format, pixels); }
		void Create(uint32 width, uint32 height, Format format = Format::RGB, uint8* pixels = nullptr);
		void Create(const std::string& filePath, Format format = Format::RGB);

		inline void SetPixels(uint8* pixels, Format format = Format::RGB) { Create(m_size.x, m_size.y, format, pixels); }

		void Delete();

		void Bind(uint32 unit = 0) const;
		void Unbind() const;

		inline bool Valid() const { return m_id != 0; }

		inline uint32 ID() const { return m_id; }
		inline const UVector2I& Size() const { return m_size; }

		inline const std::string& Path() const { return m_path; }

		static const Texture* WhiteTexture();

		inline operator bool() const { return Valid(); }

	private:
		uint32 m_id = 0;
		UVector2I m_size = UVector2I::zero;
		
		// TODO: Textures shouldn't store their path
		std::string m_path = "";

	};

}