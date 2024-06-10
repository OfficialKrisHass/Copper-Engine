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

    enum class ForceMode : uint8 {

        Force = 0,
        Impulse = 1,
        VelocityChange = 2,
        Acceleration = 3

    };

    class RigidBody : public Component {

        friend class Scene;
        friend class Collider;
        
    public:
        enum LockFlags : uint8 {

            POS_X = 1 << 0, POS_Y = 1 << 1, POS_Z = 1 << 2,
            ROT_X = 1 << 3, ROT_Y = 1 << 4, ROT_Z = 1 << 5,

        };

        void AddForce(const Vector3& force, const ForceMode mode = ForceMode::Force);
        void AddTorque(const Vector3& torque, const ForceMode mode = ForceMode::Force);

        // Getters

        inline bool IsStatic() const { return m_isStatic; }
        inline bool Gravity() const { return m_gravity; }

        inline float Mass() const { return m_mass; }

        inline uint8 LockMask() const { return m_lockMask; }

        // Setters

        void SetIsStatic(bool value);
        void SetGravity(bool value);

        void SetMass(float value);

        void SetLockMask(uint8 value);

    private:
        physx::PxRigidActor* m_body = nullptr;
        Collider* m_collider = nullptr;

        bool m_isStatic = false;
        bool m_gravity = true;

        float m_mass = 1.0f;

        uint8 m_lockMask = 0;

        void Setup();

        void CreateDynamic(physx::PxShape* shape);
        void CreateStatic(physx::PxShape* shape);

        void UpdatePositionAndRotation();

    };
    
}
