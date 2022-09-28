#pragma once

namespace Copper {

    class Texture {

    public:
        Texture() = default;
        Texture(uint32_t width, uint32_t height);
        Texture(UVector2I size);
        Texture(const std::string& path);

        inline uint32_t GetID() { return this->ID; }
        
    private:
        uint32_t ID;
        UVector2I size;
        std::string path;
        
    };
    
}