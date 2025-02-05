#include "cupch.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Collider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

#define DynamicBody ((PxRigidDynamic*) m_actor)

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxShape* noColliderShape;

    }

    using namespace PhysicsEngine;

    void RigidBody::Initialize() {

        CUP_FUNCTION();

        // First remove if already existing

        if (m_actor != nullptr)
            Remove();

        m_collider = GetEntity()->GetComponent<Collider>();
        if (m_collider == nullptr) {

            LogError("No Collider on entity '{}' with a RigidBody component", *GetEntity());
            return;

        }

        PxShape* shape = m_collider->CreateShape();
        CU_ASSERT(shape, "Could not create physx shape on entity '{}'", *GetEntity());

        m_collider->m_rb = this;
        if (m_collider->m_trigger) {

            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        }

        if (m_static)
            InitializeStatic(shape);
        else
            InitializeDynamic(shape);

        shape->release();

        GetScene()->AddPhysicsBody(m_actor);

        m_actor->setName(GetEntity()->name.c_str());
        m_actor->userData = (void*) GetEntity();

    }
    void RigidBody::Remove() {

        CUP_FUNCTION();

        GetScene()->RemovePhysicsBody(m_actor);

    }

    void RigidBody::InitializeStatic(physx::PxShape* shape) {

        CUP_FUNCTION();

        PxVec3 position = CopperToPhysX(GetTransform()->Position() - m_collider->m_center);
        PxQuat rotation = CopperToPhysX(GetTransform()->Rotation());
        m_actor = PxCreateStatic(*physics, PxTransform(position, rotation), *shape);

    }
    void RigidBody::InitializeDynamic(physx::PxShape* shape) {

        CUP_FUNCTION();

        PxVec3 position = CopperToPhysX(GetTransform()->Position() - m_collider->m_center);
        PxQuat rotation = CopperToPhysX(GetTransform()->Rotation());
        m_actor = PxCreateDynamic(*physics, PxTransform(position, rotation), *shape, 1.0f);

        DynamicBody->setMass(m_mass);
        if (!m_gravity)
            m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

        DynamicBody->setRigidDynamicLockFlags((PxRigidDynamicLockFlag::Enum) m_lockMask);

    }

    void RigidBody::UpdatePositionAndRotation() {

        CUP_FUNCTION();

        if (m_static) return;

        CU_ASSERT(m_actor, "m_actor is nullptr!");
        CU_ASSERT(m_collider, "m_collider is nullptr!");
    
        GetTransform()->SetPosition(PhysXToCopper(m_actor->getGlobalPose().p) + m_collider->m_center);
        GetTransform()->SetRotation(PhysXToCopper(m_actor->getGlobalPose().q));

    }

    void RigidBody::AddForce(const Vector3& force, const ForceMode mode) {

        CUP_FUNCTION();

        if (m_static) {

            LogError("Can't add force to a static RigidBody on entity '{}'", *GetEntity());
            return;

        }

        DynamicBody->addForce(CopperToPhysX(force), (PxForceMode::Enum) mode);

    }
    void RigidBody::AddTorque(const Vector3& torque, const ForceMode mode) {

        CUP_FUNCTION();

        if (m_static) {

            LogError("Can't add torque to a static RigidBody on entity '{}'", *GetEntity());
            return;

        }

        DynamicBody->addTorque(CopperToPhysX(torque), (PxForceMode::Enum) mode);

    }

    void RigidBody::SetMass(float value) {

        CUP_FUNCTION();

        if (m_static) return;

        m_mass = value;

        IN_RUNTIME(DynamicBody->setMass(m_mass));

    }

    void RigidBody::SetStatic(bool value) {

        CUP_FUNCTION();

        if (m_static == value) return;

        m_static = value;

        IN_RUNTIME(Initialize());

    }
    void RigidBody::SetGravity(bool value) {

        CUP_FUNCTION();

        m_gravity = value;

        IN_RUNTIME(m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_gravity));

    }
    void RigidBody::SetLockMask(uint8 value) {

        CUP_FUNCTION();

        if (m_static) return;

        m_lockMask = value;

        IN_RUNTIME(DynamicBody->setRigidDynamicLockFlags((PxRigidDynamicLockFlag::Enum) m_lockMask));

    }

}
