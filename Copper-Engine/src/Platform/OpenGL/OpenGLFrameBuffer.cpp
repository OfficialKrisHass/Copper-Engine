#include "cupch.h"
#include "Engine/Renderer/FrameBuffer.h"

#include <glad/glad.h>

namespace Copper {

	FrameBuffer::FrameBuffer(UVector2I size) : size(size) { Recreate(); }
	FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &ID); }

	void FrameBuffer::Recreate() {

		if (ID) {

			glDeleteFramebuffers(1, &ID);
			glDeleteTextures(1, &color);
			glDeleteTextures(1, &depth);

		}

		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		//Color
		glGenTextures(1, &color);
		glBindTexture(GL_TEXTURE_2D, color);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

		//Depth
		glGenTextures(1, &depth);
		glBindTexture(GL_TEXTURE_2D, depth);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { LogError("FrameBuffer is incomplete"); }

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void FrameBuffer::Resize(UVector2I size) {

		this->size = size;
		Recreate();

	}

	void FrameBuffer::Bind() {

		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glViewport(0, 0, size.x, size.y);

	}

	void FrameBuffer::Unbind() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

}