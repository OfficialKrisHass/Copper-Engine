#pragma once

// .cpp in Engine/Physics/RigidBody.cpp

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace physx { class PxRigidActor; class PxShape; }
#ifdef CU_EDITOR
    namespace Editor { class Properties; }
#endif

namespace Copper {

    class Collider;

    namespace Scripting::InternalCalls {    

        void RigidBodySetGravity(int64_t componentPointer, bool value);
        void RigidBodySetIsStatic(int64_t componentPointer, bool value);
        void RigidBodySetMass(int64_t componentPointer, float value);

    }

    enum class ForceMode : uint8_t {

        Force = 0,
        Impulse = 1,
        VelocityChange = 2,
        Acceleration = 3

    };

    class RigidBody : public Component {

        friend class Scene;
    #ifdef CU_EDITOR
        friend Editor::Properties;
    #endif
        friend void Scripting::InternalCalls::RigidBodySetGravity(int64_t componentPointer, bool value);
        friend void Scripting::InternalCalls::RigidBodySetIsStatic(int64_t componentPointer, bool value);
        friend void Scripting::InternalCalls::RigidBodySetMass(int64_t componentPointer, float value);
        
    public:
        bool isStatic = false;
        bool gravity = true;

        float mass = 1.0f;

        void UpdatePositionAndRotation();

        void SetPosition(const Vector3& position);
        void SetRotation(const Quaternion& rotation);

        void AddForce(const Vector3& force, const ForceMode mode);
        void AddTorque(const Vector3& torque, const ForceMode mode);

    private:
        physx::PxRigidActor* body = nullptr;

        uint8_t lockMask = 0;

        void Setup();

        void CreateDynamic(physx::PxShape* shape, const Collider* collider);
        void CreateStatic(physx::PxShape* shape, const Collider* collider);

    public:
        enum LockFlags : uint8_t {

            POS_X = 1 << 0, POS_Y = 1 << 1, POS_Z = 1 << 2,
            ROT_X = 1 << 3, ROT_Y = 1 << 4, ROT_Z = 1 << 5,

        };

    };
    
}