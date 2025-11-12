#pragma once
// Source is in Platform/<API>/<API>VertexArray.cpp

#include "Engine/Core/Core.h"

#include "Engine/Renderer/Buffer.h"

namespace Copper {

    // Higher level, platform indenpendant wrapper for the Vertex Array
    class VertexArray {

    public:
        VertexArray() = default;
        VertexArray(IndexBuffer* ibo) { Create(ibo); }

        void Create(IndexBuffer* ibo);

        // Using

        void Bind() const;
        void Unbind() const;

        // Modification

        void SetVertexBuffer(VertexBuffer* vbo);
        inline void SetIndexBuffer(IndexBuffer* ibo) { m_ibo = ibo; }

        // Getters

        inline uint32 GetCount() const {

            CUP_FUNCTION();

            CU_ASSERT(m_ibo != nullptr, "Vertex array has no Index Buffer attached");
            return m_ibo->GetCount();

        }

        inline bool IsValid() const { return m_id != 0; }

    private:
        uint32 m_id = 0;
        IndexBuffer* m_ibo = nullptr;

    };

}
