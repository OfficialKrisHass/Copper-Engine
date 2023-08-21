 #pragma once

#include "Engine/Core/Core.h"

namespace Copper {

	class FrameBuffer {

	public:
		FrameBuffer() = default;
		FrameBuffer(const UVector2I& size);

		void Resize(const UVector2I& size);

		void Bind();
		void Unbind();

		inline uint32_t GetColorAttachment() { return color; }

		inline uint32_t Width() { return size.x; }
		inline uint32_t Height() { return size.y; }
		inline UVector2I Size() { return size; }

	private:
		uint32_t ID = 0;
		uint32_t color = 0;
		uint32_t depth = 0;

		UVector2I size;

		void Recreate();

	};

	uint32_t GetMainFBOTexture();

}