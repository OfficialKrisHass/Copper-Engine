#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

    class BoxCollider : public Component {

        friend Scene;

    public:
        bool trigger;
        
        Vector3 center = Vector3::zero;
        Vector3 size = Vector3::one;

    private:
        void Setup();

    };
    
}