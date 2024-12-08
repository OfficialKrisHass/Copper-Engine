#pragma once
// Source is in Platform/<API>/<API>Buffer.cpp

#include "Engine/Core/Core.h"

#include "Engine/Renderer/ElementType.h"

namespace Copper {

    // Higher level, platform indenpendant wrapper for the Vertex Buffer
    class VertexBuffer {

    public:
        VertexBuffer() = default;
        VertexBuffer(float* vertices, uint32 size, const std::initializer_list<ElementType>& layout);

        // Modification

        void SetData(float* vertices, uint32 count);

        // Using

        void Bind() const;
        void Unbind() const;

        // Getters

        inline uint32 GetStride() const { return m_stride; }
        inline uint32 GetElementCount() const { return (uint32) m_offsets.size(); }

        inline ElementType GetType(uint32 index) const { return m_offsets[index].first; }
        inline uint32 GetOffset(uint32 index) const { return m_offsets[index].second; }

    private:
        uint32 m_id = 0;
        uint32 m_stride = 0;
        std::vector<std::pair<ElementType, uint32>> m_offsets;

        void CalculateOffsetsAndStride(const std::initializer_list<ElementType>& layout) {

            CUP_FUNCTION();

            m_offsets.resize(layout.size());

            uint32 i = 0;
            for (const ElementType type : layout) {

                m_offsets[i].first = type;
                m_offsets[i].second = m_stride;

                m_stride += TypeSize(type);

                i++;

            }

        }

    };

    // Higher level, platform indenpendant wrapper for the Index Buffer
    class IndexBuffer {

    public:
        IndexBuffer() = default;
        IndexBuffer(uint32* indices, uint32 size);

        // Modification

        void SetData(uint32* indices, uint32 count);

        // Using

        void Bind() const;
        void Unbind() const;

        // Getters

        inline uint32 GetCount() const { return m_count; }

    private:
        uint32 m_id = 0;
        uint32 m_count = 0;

    };

}
