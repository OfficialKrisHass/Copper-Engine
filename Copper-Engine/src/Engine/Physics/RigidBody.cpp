#include "cupch.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Collider.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;

        extern PxShape* noColliderShape;
    
        extern PxVec3 CopperToPhysX(const Vector3& vec);
        extern PxQuat CopperToPhysX(const Quaternion& vec);

        extern Vector3 PhysXToCopper(const PxVec3& vec);
        extern Quaternion PhysXToCopper(const PxQuat& vec);

    }

    using namespace PhysicsEngine;

    void RigidBody::Setup() {

        // In a case we changed some parameters we need to first delete the body
        // NOTE: Maybe rename this to something like Generate or Regenerate ????

        if (body)
            GetScene()->RemovePhysicsBody(body);

        Collider* collider = GetEntity()->GetComponent<Collider>();

        // Default Case 2: Rigid Body with no Collider - Why though ?
        // NOTE: This is the default as its simpler then to create a shape
        //       and then check if we were supposed to create said shape
        //       .... and also I felt smarter when I came up with this :)

        PxShape* shape = noColliderShape;

        // Case 3: Rigid Body with a Collider

        if (collider) {

            shape = collider->CreateShape();

            if (collider->trigger) {

                shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
                shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

            }

        }

        if (isStatic)
            CreateStatic(shape);
        else
            CreateDynamic(shape);

        if (collider)
            shape->release();

        GetScene()->AddPhysicsBody(body);

        body->setName(GetEntity()->name.c_str());
        body->userData = (void*) GetEntity();

    }

    void RigidBody::CreateDynamic(PxShape* shape) {

        body = PxCreateDynamic(*physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape, 1.0f);

        ((PxRigidDynamic*) body)->setMass(mass);
        if (!gravity)
            body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

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

        ((PxRigidDynamic*) body)->setRigidDynamicLockFlags((PxRigidDynamicLockFlag::Enum) lockMask);

    }
    void RigidBody::CreateStatic(PxShape* shape) {

        body = PxCreateStatic(*physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape);

    }

    void RigidBody::UpdatePositionAndRotation() {

        if (!body || isStatic)
            return;

        Transform* transform = GetTransform();

        transform->position = PhysXToCopper(body->getGlobalPose().p);
        transform->rotation = PhysXToCopper(body->getGlobalPose().q);

    }

    void RigidBody::AddForce(const Vector3& force, const ForceMode mode) {

        if (isStatic) {

            LogError("Cant add force to a static rigidBody on entity {}", *GetEntity(), GetEntity()->ID());
            return;

        }

        ((PxRigidDynamic*) body)->addForce(CopperToPhysX(force), (PxForceMode::Enum) mode);

    }
    void RigidBody::AddTorque(const Vector3& torque, const ForceMode mode) {

        if (isStatic) {

            LogError("Cant add torque to a static rigidBody on entity {}", *GetEntity());
            return;

        }

        ((PxRigidDynamic*) body)->addTorque(CopperToPhysX(torque), (PxForceMode::Enum) mode);

    }

}