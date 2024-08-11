#pragma once
// source in Engine/Physics/RigidBody.cpp

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

namespace physx {

    class PxRigidActor;
    class PxShape;

}

#ifdef CU_EDITOR
namespace YAML { class Node; }
namespace Editor {

    class Properties;
    namespace SceneSerializer {

        void DeserializeEntityComponents(Copper::InternalEntity* entity, const YAML::Node& data);

    }

}
#endif

namespace Copper {

    class Collider;

    class RigidBody : public Component {

        friend class Scene;
        friend Collider;

#ifdef CU_EDITOR
        friend Editor::Properties;
        friend void Editor::SceneSerializer::DeserializeEntityComponents(Copper::InternalEntity *entity, const YAML::Node &data);
#endif

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
