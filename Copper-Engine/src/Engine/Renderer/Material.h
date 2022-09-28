#pragma once

#include "Engine/Core/Core.h"

namespace Copper {
    
    class Material {
        
    public:
        Material() = default;

        Color color;

        void Serialize(std::filesystem::path path);

        static Material Default() {

            Material empty;

            empty.color = Color::White();

            return empty;
            
        }
        static Material Null() {

            Material null;

            null.color = Color::None();

            return null;
            
        }

        operator bool() const { return *this != Null(); }

        bool operator==(const Material& other) const { return color == other.color; }
        bool operator!=(const Material& other) const { return !(*this == other); }
        
    };
    
    Material Deserialize(std::filesystem::path path);
    
}