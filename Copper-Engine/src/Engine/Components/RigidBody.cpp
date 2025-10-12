#include "cupch.h"
#include "RigidBody.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Components/Collider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

#define DynamicBody static_cast<PxRigidDynamic*>(m_actor)

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;

    }

    using namespace PhysicsEngine;

    void RigidBody::Initialize() {

        CUP_FUNCTION();

        // First remove if already existing

        if (m_actor != nullptr)
            Remove();

        m_collider = GetEntity()->GetComponent<Collider>();
        if (m_collider == nullptr) {

            LogError("Entity '{}' has no Collider!", *GetEntity());
            return;

        }


        PxShape* shape = m_collider->CreateShape();
        CU_ASSERT(shape != nullptr, "Could not create physx shape on entity '{}'", *GetEntity());

        m_collider->m_rb = this;
        shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_collider->m_trigger);
        shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_collider->m_trigger);

        if (m_static)
            InitializeStatic(shape);
        else
            InitializeDynamic(shape);

        shape->release();

        m_actor->setName(GetEntity()->name.c_str());
        m_actor->userData = (void*) (uint64) GetEntity()->GetID();

        GetScene()->AddPhysicsBody(m_actor);

    }
    void RigidBody::Remove() {

        CUP_FUNCTION();

        GetScene()->RemovePhysicsBody(m_actor);
        m_actor = nullptr;

    }

    void RigidBody::InitializeStatic(physx::PxShape* shape) {

        CUP_FUNCTION();

        PxVec3 position = CopperToPhysX(GetTransform()->GetPosition() - m_collider->m_center);
        PxQuat rotation = CopperToPhysX(GetTransform()->GetRotation());
        m_actor = PxCreateStatic(*physics, PxTransform(position, rotation), *shape);

        CU_ASSERT(m_actor != nullptr, "Failed to create RigidStatic actor on entity {}", *GetEntity());

    }
    void RigidBody::InitializeDynamic(physx::PxShape* shape) {

        CUP_FUNCTION();

        PxVec3 position = CopperToPhysX(GetTransform()->GetPosition() - m_collider->m_center);
        PxQuat rotation = CopperToPhysX(GetTransform()->GetRotation());
        m_actor = PxCreateDynamic(*physics, PxTransform(position, rotation), *shape, 1.0f);

        CU_ASSERT(m_actor != nullptr, "Failed to create RigidDynamic actor on entity {}", *GetEntity());

        DynamicBody->setMass(m_mass);
        m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_gravity);

        DynamicBody->setRigidDynamicLockFlags(static_cast<PxRigidDynamicLockFlag::Enum>(m_lockMask));

    }

    void RigidBody::UpdatePositionAndRotation() {

        CUP_FUNCTION();

        CU_ASSERT(m_actor != nullptr, "Rigidbody actor is nullptr on entity '{}'", *GetEntity());
        CU_ASSERT(m_collider != nullptr, "m_collider is nullptr!");

        if (m_static) return;
    
        GetTransform()->SetPosition(PhysXToCopper(m_actor->getGlobalPose().p) + m_collider->m_center);
        GetTransform()->SetRotation(PhysXToCopper(m_actor->getGlobalPose().q));

    }

    void RigidBody::AddForce(const Vector3& force, const ForceMode mode) {

        CUP_FUNCTION();

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        if (m_static) {

            LogError("Can't add force to a static RigidBody on entity '{}'", *GetEntity());
            return;

        }

        IN_RUNTIME(DynamicBody->addForce(CopperToPhysX(force), (PxForceMode::Enum) mode));

    }
    void RigidBody::AddTorque(const Vector3& torque, const ForceMode mode) {

        CUP_FUNCTION();

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        if (m_static) {

            LogError("Can't add torque to a static RigidBody on entity '{}'", *GetEntity());
            return;

        }

        IN_RUNTIME(DynamicBody->addTorque(CopperToPhysX(torque), (PxForceMode::Enum) mode));

    }

    void RigidBody::SetMass(float value) {

        CUP_FUNCTION();

        if (m_static) return;

        m_mass = value;
        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        DynamicBody->setMass(m_mass);

    }

    void RigidBody::SetStatic(bool value) {

        CUP_FUNCTION();

        m_static = value;
        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        Initialize();

    }
    void RigidBody::SetGravity(bool value) {

        CUP_FUNCTION();

        m_gravity = value;
        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_gravity);

    }
    void RigidBody::SetLockMask(uint8 value) {

        CUP_FUNCTION();

        if (m_static) return;

        m_lockMask = value;
        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        DynamicBody->setRigidDynamicLockFlags((PxRigidDynamicLockFlag::Enum) m_lockMask);

    }

    void RigidBody::SetPosition(const Vector3& position) {

        CUP_FUNCTION();

        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        if (m_static) return;

        PxTransform pose = m_actor->getGlobalPose();
        pose.p = CopperToPhysX(position);
        m_actor->setGlobalPose(pose);

    }
    void RigidBody::SetRotation(const Quaternion& rotation) {

        CUP_FUNCTION();

        IN_NOT_RUNTIME(return);

        CU_ASSERT(m_actor != nullptr, "RigidActor is nullptr on entity '{}'", *GetEntity());
        if (m_static) return;

        PxTransform pose = m_actor->getGlobalPose();
        pose.q = CopperToPhysX(rotation);
        m_actor->setGlobalPose(pose);

    }

}
