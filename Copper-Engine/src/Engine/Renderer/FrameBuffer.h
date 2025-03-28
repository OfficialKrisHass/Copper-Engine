#pragma once
// Source in Platform/<API>/<API>FrameBuffer.cpp

#include "Engine/Core/Core.h"

namespace Copper {

    // Higher level wrapper around a Frame Buffer indenpendent of OS or API
    class FrameBuffer {

    public:
        struct Attachment {

            enum class Format : uint8 {

                None = 0,

                RGB8,
                RGBA8,

                RedInteger,

            };

            Format format = Format::None;
            uint32 id = 0;

            Attachment() = default;
            Attachment(Format format) : format(format) {}

        };

        FrameBuffer() = default;
        FrameBuffer(const UVector2I& size, std::initializer_list<Attachment::Format> attachmentFormats);

        void Recreate();
        void Delete();

        // Modification

        inline void Resize(const UVector2I& size) {

            CUP_FUNCTION();

            m_size = size;
            Recreate();

        }

        // Using

        void Bind() const;
        void Unbind() const;

        uint32 ReadPixel(uint32 attachment, uint32 x, uint32 y) const;

        void ClearAttachment(uint32 attachment, uint32 value);

        // Getters

        inline uint32 GetColorAttachmentID(uint32 index) const {

            CU_ASSERT(index < m_attachments.size(), "Invalid color attachment index ({})", index);
            if (index >= m_attachments.size()) return 0;

            return m_attachments[index].id;

        }
        inline uint32 GetDepthAttachmentID() const { return m_depthAttachment; }

        inline const UVector2I& GetSize() const { return m_size; }

    private:
        uint32 m_id = 0;

        std::vector<Attachment> m_attachments;
        uint32 m_depthAttachment = 0;

        UVector2I m_size = UVector2I::zero;

        void CreateTexture(int internalFormat, int format);

    };

    const FrameBuffer& GetMainFBO();

}
