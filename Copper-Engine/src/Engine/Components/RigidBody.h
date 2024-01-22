#pragma once
// source in Engine/Physics/RigidBody.cpp

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

namespace physx { class PxRigidActor; class PxShape; }
#ifdef CU_EDITOR
    namespace Editor { class Properties; }
#endif

namespace Copper {

    class Collider;

    namespace Scripting::InternalCalls {    

        void RigidBodySetGravity(uint64 componentPointer, bool value);
        void RigidBodySetIsStatic(uint64 componentPointer, bool value);
        void RigidBodySetMass(uint64 componentPointer, float value);

    }

    enum class ForceMode : uint8 {

        Force = 0,
        Impulse = 1,
        VelocityChange = 2,
        Acceleration = 3

    };

    class RigidBody : public Component {

        friend class Scene;
        friend class Collider;

        friend void Scripting::InternalCalls::RigidBodySetGravity(uint64 componentPointer, bool value);
        friend void Scripting::InternalCalls::RigidBodySetIsStatic(uint64 componentPointer, bool value);
        friend void Scripting::InternalCalls::RigidBodySetMass(uint64 componentPointer, float value);

    #ifdef CU_EDITOR
        friend Editor::Properties;
    #endif
        
    public:
        enum LockFlags : uint8 {

            POS_X = 1 << 0, POS_Y = 1 << 1, POS_Z = 1 << 2,
            ROT_X = 1 << 3, ROT_Y = 1 << 4, ROT_Z = 1 << 5,

        };

        bool isStatic = false;
        bool gravity = true;

        float mass = 1.0f;

        void UpdatePositionAndRotation();

        void SetPosition(const Vector3& position);
        void SetRotation(const Quaternion& rotation);

        void AddForce(const Vector3& force, const ForceMode mode = ForceMode::Force);
        void AddTorque(const Vector3& torque, const ForceMode mode = ForceMode::Force);

    private:
        physx::PxRigidActor* m_body = nullptr;
        Collider* m_collider = nullptr;

        uint8 m_lockMask = 0;

        void Setup();

        void CreateDynamic(physx::PxShape* shape);
        void CreateStatic(physx::PxShape* shape);

    };
    
}