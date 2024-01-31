 #pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class FrameBuffer {

	public:
		FrameBuffer() = default;
		FrameBuffer(const UVector2I& size);

		void Resize(const UVector2I& size);

		void Bind() const;
		void Unbind() const;

		inline uint32 ColorTextureID() const { return m_color; }
		inline uint32 DepthTextureID() const { return m_depth; }

		inline uint32 Width() const { return m_size.x; }
		inline uint32 Height() const { return m_size.y; }
		inline UVector2I Size() const { return m_size; }

	private:
		uint32 m_id = 0;
		uint32 m_color = 0;
		uint32 m_depth = 0;

		UVector2I m_size;

		void Recreate();

	};

	uint32 GetMainFBOTexture();

}