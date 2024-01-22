#pragma once

namespace Copper {

    class Texture {

    public:
        Texture() = default;
        Texture(uint32 width, uint32 height);
        Texture(UVector2I size);
        Texture(const std::string& path);

        inline uint32 GetID() { return this->m_id; }
        
    private:
        uint32 m_id;
        UVector2I m_size;
        std::string m_path;
        
    };
    
}