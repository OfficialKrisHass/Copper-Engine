#include "cupch.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Collider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxShape* noColliderShape;

    }

    using namespace PhysicsEngine;

    void RigidBody::Setup() {

        CUP_FUNCTION();

        // In a case we changed some parameters we need to first delete the body
        // NOTE: Maybe rename this to something like Generate or Regenerate ????

        if (m_body)
            GetScene()->RemovePhysicsBody(m_body);

        this->m_collider = GetEntity()->GetComponent<Collider>();

        // Default Case 2: Rigid Body with no Collider - Why though ?
        // NOTE: This is the default as its simpler then to create a shape
        //       and then check if we were supposed to create said shape
        //       .... and also I felt smarter when I came up with this :)

        PxShape* shape = noColliderShape;

        // Case 3: Rigid Body with a Collider

        if (this->m_collider) {

            shape = this->m_collider->CreateShape();
            this->m_collider->m_rb = this;

            if (this->m_collider->trigger) {

                shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
                shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

            }

        }

        if (isStatic)
            CreateStatic(shape);
        else
            CreateDynamic(shape);

        if (this->m_collider)
            shape->release();

        GetScene()->AddPhysicsBody(m_body);

        m_body->setName(GetEntity()->name.c_str());
        m_body->userData = (void*) GetEntity();

    }

    void RigidBody::CreateDynamic(PxShape* shape) {

        CUP_FUNCTION();

        if(this->m_collider)
            m_body = PxCreateDynamic(*physics, PxTransform(CopperToPhysX(GetTransform()->Position() - this->m_collider->center), CopperToPhysX(GetTransform()->Rotation())), *shape, 1.0f);
        else
            m_body = PxCreateDynamic(*physics, PxTransform(CopperToPhysX(GetTransform()->Position()), CopperToPhysX(GetTransform()->Rotation())), *shape, 1.0f);

        ((PxRigidDynamic*) m_body)->setMass(mass);
        if (!gravity)
            m_body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

        // This hurts my eyes but I am like 99% sure there is no other way
        // NOTE: As Always, there is :))))
        /*typedef PxRigidDynamicLockFlag LockFlag;
        typedef PxRigidDynamicLockFlags LockFlags;
        LockFlags flags = positionLock[0] ? LockFlag::eLOCK_LINEAR_X : (LockFlag::Enum) 0;
        if (positionLock[1]) flags |= LockFlag::eLOCK_LINEAR_Y;
        if (positionLock[2]) flags |= LockFlag::eLOCK_LINEAR_Z;
        if (rotationLock[0]) flags |= LockFlag::eLOCK_ANGULAR_X;
        if (rotationLock[1]) flags |= LockFlag::eLOCK_ANGULAR_Y;
        if (rotationLock[2]) flags |= LockFlag::eLOCK_ANGULAR_Z;*/

        ((PxRigidDynamic*) m_body)->setRigidDynamicLockFlags((PxRigidDynamicLockFlag::Enum) m_lockMask);

    }
    void RigidBody::CreateStatic(PxShape* shape) {

        CUP_FUNCTION();

        if (this->m_collider)
            m_body = PxCreateStatic(*physics, PxTransform(CopperToPhysX(GetTransform()->Position() - this->m_collider->center), CopperToPhysX(GetTransform()->Rotation())), *shape);
        else
            m_body = PxCreateStatic(*physics, PxTransform(CopperToPhysX(GetTransform()->Position()), CopperToPhysX(GetTransform()->Rotation())), *shape);

    }

    void RigidBody::UpdatePositionAndRotation() {

        CUP_FUNCTION();

        if (!m_body || isStatic) return;

        Transform* transform = GetTransform();

        if (this->m_collider)
            transform->SetPosition( PhysXToCopper(m_body->getGlobalPose().p) + m_collider->center);
        else
            transform->SetPosition(PhysXToCopper(m_body->getGlobalPose().p));

        transform->SetRotation(PhysXToCopper(m_body->getGlobalPose().q));

    }

    void RigidBody::SetPosition(const Vector3& position) {

        CUP_FUNCTION();

        if (!m_body || isStatic) return;

        PxTransform pose = m_body->getGlobalPose();
        pose.p = CopperToPhysX(position);
        m_body->setGlobalPose(pose);

    }
    void RigidBody::SetRotation(const Quaternion& rotation) {

        CUP_FUNCTION();

        if (!m_body || isStatic) return;

        PxTransform pose = m_body->getGlobalPose();
        pose.q = CopperToPhysX(rotation);
        m_body->setGlobalPose(pose);

    }

    void RigidBody::AddForce(const Vector3& force, const ForceMode mode) {

        CUP_FUNCTION();

        if (isStatic) {

            LogError("Cant add force to a static rigidBody on entity {}", *GetEntity(), GetEntity()->ID());
            return;

        }

        ((PxRigidDynamic*) m_body)->addForce(CopperToPhysX(force), (PxForceMode::Enum) mode);

    }
    void RigidBody::AddTorque(const Vector3& torque, const ForceMode mode) {

        CUP_FUNCTION();

        if (isStatic) {

            LogError("Cant add torque to a static rigidBody on entity {}", *GetEntity());
            return;

        }

        ((PxRigidDynamic*) m_body)->addTorque(CopperToPhysX(torque), (PxForceMode::Enum) mode);

    }

}