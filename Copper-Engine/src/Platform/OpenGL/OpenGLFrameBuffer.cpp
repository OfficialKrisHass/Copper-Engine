#include "cupch.h"
#include "Engine/Renderer/FrameBuffer.h"

#include <glad/glad.h>

namespace Copper {

	FrameBuffer::FrameBuffer(const UVector2I& size) : m_size(size) {
		
		Recreate();
	
	}

	void FrameBuffer::Recreate() {

		if (m_id) {

			glDeleteFramebuffers(1, &m_id);
			glDeleteTextures(1, &m_color);
			glDeleteTextures(1, &m_depth);

		}

		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		// Color

		glGenTextures(1, &m_color);
		glBindTexture(GL_TEXTURE_2D, m_color);

		glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_size.x, m_size.y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color, 0);

		// Depth

		glGenTextures(1, &m_depth);
		glBindTexture(GL_TEXTURE_2D, m_depth);

		glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_size.x, m_size.y);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

		// Finalize

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { LogError("FrameBuffer is incomplete"); }

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void FrameBuffer::Resize(const UVector2I& size) {

		this->m_size = size;
		Recreate();

	}

	void FrameBuffer::Bind() const {

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glViewport(0, 0, m_size.x, m_size.y);

	}

	void FrameBuffer::Unbind() const {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

}