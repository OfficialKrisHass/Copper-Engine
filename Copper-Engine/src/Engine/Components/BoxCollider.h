#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

    class BoxCollider : public Component {

        friend class Scene;
        friend class RigidBody;

    public:
        bool trigger;
        
        Vector3 center;
        Vector3 size;

    private:
        bool hasRB = false;

        void Setup();

    };
    
}