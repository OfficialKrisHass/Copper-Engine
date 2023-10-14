#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidActor; }
#ifdef CU_EDITOR
    namespace Editor { class Properties; }
#endif

namespace Copper {

    class RigidBody : public Component {

        friend class Scene;
    #ifdef CU_EDITOR
        friend Editor::Properties;
    #endif

    public:
        bool isStatic;
        bool gravity = true;

        float mass = 1.0f;

        void UpdatePositionAndRotation();

    private:
        physx::PxRigidActor* body;

        void Setup();

    };
    
}