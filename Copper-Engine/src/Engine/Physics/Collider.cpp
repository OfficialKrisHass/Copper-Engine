#include "cupch.h"
#include "Engine/Components/Collider.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxMaterial* material;

    }

    using namespace PhysicsEngine;

    void Collider::Setup() {

        CUP_FUNCTION();

        m_rb = GetEntity()->GetComponent<RigidBody>();
        if (m_rb) {

            m_rb->m_collider = this;
            return;

        }

        // Case 1: Collider with no Rigid Body

        PxShape* shape = CreateShape();
        if (trigger) {

            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        }

        PxRigidStatic* body = PxCreateStatic(*physics, PxTransform(CopperToPhysX(GetTransform()->Position()), CopperToPhysX(GetTransform()->Rotation())), *shape);

        body->setName(GetEntity()->name.c_str());
        body->userData = (void*) GetEntity();

        GetScene()->AddPhysicsBody(body);

    }

    PxShape* BoxCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->Scale() * size / 2.0f)), *material);

    }
    PxShape* SphereCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxSphereGeometry(radius * GetTransform()->Scale().x), *material);

    }
    PxShape* CapsuleCollider::CreateShape() const {

        CUP_FUNCTION();

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->Scale().x * radius, transform->Scale().y * height), *material);

    }

}