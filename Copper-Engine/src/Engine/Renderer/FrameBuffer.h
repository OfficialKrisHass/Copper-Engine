#pragma once
// Source in Platform/<API>/<API>FrameBuffer.cpp

#include "Engine/Core/Core.h"

namespace Copper {

    // Higher level wrapper around a Frame Buffer indenpendent of OS or API
	class FrameBuffer {

	public:
		FrameBuffer() = default;
		FrameBuffer(const UVector2I& size);

        // Modification

		void Resize(const UVector2I& size);
        
        // Deletion

		void Recreate();
        void Delete();

        // Using

		void Bind() const;
		void Unbind() const;

        // Getters

		inline uint32 GetColorTextureID() const { return m_color; }
		inline uint32 GetDepthTextureID() const { return m_depth; }

		inline uint32 GetWidth() const { return m_size.x; }
		inline uint32 GetHeight() const { return m_size.y; }
		inline UVector2I GetSize() const { return m_size; }

	private:
		uint32 m_id = 0;
		uint32 m_color = 0;
		uint32 m_depth = 0;

		UVector2I m_size = UVector2I::zero;

	};

    const FrameBuffer& GetMainFBO();

}
