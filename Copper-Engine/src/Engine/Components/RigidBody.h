#pragma once
// source in Engine/Physics/RigidBody.cpp

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

namespace physx {

    class PxRigidActor;
    class PxShape;

}

COMPONENT_FORWARD_DECL()

namespace Copper {

    class Collider;

    class RigidBody : public Component {

        COMPONENT_FRIEND_CLASSES();

        friend Collider;

    public:
        enum class LockFlags : uint8 {

            POS_X = FLAG(0), POS_Y = FLAG(1), POS_Z = FLAG(2),
            ROT_X = FLAG(3), ROT_Y = FLAG(4), ROT_Z = FLAG(5),

        };
        enum class ForceMode : uint8 {

            Force = 0,
            Impulse = 1,
            VelocityChange = 2,
            Acceleration = 3,

        };

        void AddForce(const Vector3& force, const ForceMode mode = ForceMode::Force);
        void AddTorque(const Vector3& torque, const ForceMode mode = ForceMode::Force);

        // Getters
        
        inline bool IsValid() const { return (bool) *this && m_collider != nullptr; }

        inline float GetMass() const { return m_mass; }

        inline bool GetStatic() const { return m_static; }
        inline bool GetGravity() const { return m_gravity; }
        inline uint8 GetLockMask() const { return m_lockMask; }

        inline Collider* GetCollider() const { return m_collider; }

        // Setters
        
        void SetMass(float value);

        void SetStatic(bool value);
        void SetGravity(bool value);
        void SetLockMask(uint8 value);

        // TODO: Remove this in favour of shared Position and Rotation between PhysX and Copper
        void SetPosition(const Vector3& position);
        void SetRotation(const Quaternion& rotation);

    private:
        physx::PxRigidActor* m_actor = nullptr;
        Collider* m_collider = nullptr;

        float m_mass = 1.0f;

        bool m_static = false;
        bool m_gravity = true;
        uint8 m_lockMask = 0;

        void Initialize();
        void Remove();

        void InitializeStatic(physx::PxShape* shape);
        void InitializeDynamic(physx::PxShape* shape);

        void UpdatePositionAndRotation();

    };

}
