#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidDynamic; }

namespace Copper {

    class RigidBody : public Component {

        friend class Scene;

    public:
        bool gravity;
        float mass;

        void UpdatePositionAndRotation();

    private:
        physx::PxRigidDynamic* body;

        void SetupBody();

    };
    
}