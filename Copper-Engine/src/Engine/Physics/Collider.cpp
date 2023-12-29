#include "cupch.h"
#include "Engine/Components/Collider.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxMaterial* material;

        extern PxVec3 CopperToPhysX(const Vector3& vec);
        extern PxQuat CopperToPhysX(const Quaternion& vec);

        extern Vector3 PhysXToCopper(const PxVec3& vec);
        extern Quaternion PhysXToCopper(const PxQuat& vec);

    }

    using namespace PhysicsEngine;

    void Collider::Setup() {

        RigidBody* rb = GetEntity()->GetComponent<RigidBody>();
        if (rb) return;

        // Case 1: Collider with no Rigid Body

        PxShape* shape = CreateShape();
        if (trigger) {

            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        }

        PxRigidStatic* body = PxCreateStatic(*physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape);

        body->setName(GetEntity()->name.c_str());
        body->userData = (void*) GetEntity();

        GetScene()->AddPhysicsBody(body);

    }

    PxShape* BoxCollider::CreateShape() const {

        return physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->scale * size / 2.0f)), *material);

    }
    PxShape* SphereCollider::CreateShape() const {

        return physics->createShape(PxSphereGeometry(radius * GetTransform()->scale.x), *material);

    }
    PxShape* CapsuleCollider::CreateShape() const {

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->scale.x * radius, transform->scale.y * height), *material);

    }

}