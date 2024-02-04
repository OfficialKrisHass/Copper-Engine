#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#define LIGHT_TYPES 2

#define MAX_LIGHTS 8

namespace Copper {

    class Light : public Component {

    public:
        enum class Type : uint8 {
            
            Point = 0,
            Directional = 1,
        
        };

        Color color = Color::white;
        Type type = Type::Point;

        float intensity = 1.0f;

    private:
        //
        
    };
    
}