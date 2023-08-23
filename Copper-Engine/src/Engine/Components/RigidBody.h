#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidActor; }

namespace Copper {

    class RigidBody : public Component {

        friend class Scene;

    public:
        bool isStatic;
        bool gravity;

        float mass;

        void UpdatePositionAndRotation();

    private:
        physx::PxRigidActor* body;

        void SetupBody();

    };
    
}