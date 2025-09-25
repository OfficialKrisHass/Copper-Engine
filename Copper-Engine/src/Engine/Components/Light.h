#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#define LIGHT_TYPES 2

#define MAX_LIGHTS 8

COMPONENT_FORWARD_DECL()

namespace Copper {

    class Light : public Component {

        COMPONENT_FRIEND_CLASSES();

    public:
        enum class Type : uint8 {
            
            Point = 0,
            Directional = 1,
        
        };

        Type type = Type::Point;

        Color color = Color::white;
        float intensity = 1.0f;

    private:
        //
        
    };

}
