#include "cupch.h"
#include "Engine/Renderer/FrameBuffer.h"

#include <glad/glad.h>

namespace Copper {

    void FrameBuffer::Create(const UVector2I& size, std::initializer_list<Attachment::Format> attachmentFormats) {

        CUP_FUNCTION();

        m_size = size;

        CU_ASSERT(attachmentFormats.size() < 5, "A max of 4 color attachments are allowed on a FrameBuffer");

        m_attachments.reserve(attachmentFormats.size());
        for(Attachment::Format format : attachmentFormats)
            m_attachments.push_back(format);

        Create();

    }

    void FrameBuffer::Create() {

        CUP_FUNCTION();

        glGenFramebuffers(1, &m_id);
        CU_ASSERT(m_id != 0, "Could not generate the Frame Buffer id.");
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        // Color attachments

        for (uint32 i = 0; i < m_attachments.size(); i++) {

            Attachment& attachment = m_attachments[i];

            glGenTextures(1, &attachment.id);
            CU_ASSERT(attachment.id != 0, "Could not create Color attachment texture for FrameBuffer (id {}), format: {}", m_id, static_cast<uint8>(attachment.format));
            glBindTexture(GL_TEXTURE_2D, attachment.id);

            switch (attachment.format) {

                case Attachment::Format::RGB8: CreateTexture(GL_RGB8, GL_RGB); break;
                case Attachment::Format::RGBA8: CreateTexture(GL_RGBA8, GL_RGBA); break;
                case Attachment::Format::RedInteger: CreateTexture(GL_R32UI, GL_RED_INTEGER); break;
                default: break;

            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, attachment.id, 0);

        }

        // Depth

        glGenTextures(1, &m_depthAttachment);
        CU_ASSERT(m_depthAttachment != 0, "Could not create depth attachment texture for FrameBuffer (id {})", m_id);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_size.x, m_size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        // Finalize
        
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(static_cast<int32>(m_attachments.size()), buffers);

        CU_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Could not create Frame Buffer.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void FrameBuffer::Recreate() {

        CUP_FUNCTION();

        CU_ASSERT(m_id != 0, "Can't delete the default framebuffer (with id 0)");
        CU_ASSERT(m_attachments.size() > 0, "Can't Recreate a frame buffer with no attachments. FrameBuffer id: {}", m_id);

        Delete();
        Create();

    }
    void FrameBuffer::Delete() {

        CUP_FUNCTION();

        CU_ASSERT(m_id != 0, "Can't delete the default framebuffer (with id 0)");

        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_depthAttachment);

        for (Attachment& attachment : m_attachments) {

            glDeleteTextures(1, &attachment.id);
            attachment.id = 0;

        }

        m_id = 0;
        m_depthAttachment = 0;


    }

    void FrameBuffer::CreateTexture(int internalFormat, int format) {

        CUP_FUNCTION();

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_size.x, m_size.y, 0, format, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    void FrameBuffer::Bind() const {

        CUP_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glViewport(0, 0, m_size.x, m_size.y);

    }

    void FrameBuffer::Unbind() const {

        CUP_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    uint32 FrameBuffer::ReadPixel(uint32 attachment, uint32 x, uint32 y) const {

        CUP_FUNCTION();

        CU_ASSERT(attachment < m_attachments.size(), "Can't read from attachment {} (out of range index)", attachment);

        uint32 ret;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &ret);

        return ret;

    }

    void FrameBuffer::ClearAttachment(uint32 attachment, uint32 value) {

        CUP_FUNCTION();

        CU_ASSERT(attachment < m_attachments.size(), "Can't clear attachment {} (out of range index)", attachment);

        glClearTexImage(m_attachments[attachment].id, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &value);

    }

}
