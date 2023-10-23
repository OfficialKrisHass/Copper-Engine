#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidActor; }
#ifdef CU_EDITOR
    namespace Editor { class Properties; }
#endif

namespace Copper {

    namespace Scripting::InternalCalls {    

        static void RigidBodySetGravity(uint32_t eID, bool value);
        static void RigidBodySetIsStatic(uint32_t eID, bool value);
        static void RigidBodySetMass(uint32_t eID, float value);

    }

    class RigidBody : public Component {

        friend class Scene;
    #ifdef CU_EDITOR
        friend Editor::Properties;
    #endif
        friend void Scripting::InternalCalls::RigidBodySetGravity(uint32_t eID, bool value);
        friend void Scripting::InternalCalls::RigidBodySetIsStatic(uint32_t eID, bool value);
        friend void Scripting::InternalCalls::RigidBodySetMass(uint32_t eID, float value);

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