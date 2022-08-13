#pragma once

#include "Engine/Core/Core.h"
#include "cupch.h"

namespace Copper {

	class FrameBuffer {

	public:
		FrameBuffer(UVector2I size);
		~FrameBuffer();

		void Resize(UVector2I size);

		void Bind();
		void Unbind();

		inline uint32_t GetColorAttachment() { return color; }

		inline uint32_t Width() { return size.x; }
		inline uint32_t Height() { return size.y; }

	private:
		uint32_t ID;
		uint32_t color;
		uint32_t depth;

		UVector2I size;

		void Recreate();

	};

}