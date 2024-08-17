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

    void Collider::RecreateShape() {

        CUP_FUNCTION();

        IN_RUNTIME_BEGIN;

        // Remove current shape

        PxShape* shape = nullptr;
        m_rb->m_actor->getShapes(&shape, 1);
        CU_ASSERT(shape, "Could not get shape from RigidBody");

        m_rb->m_actor->detachShape(*shape);

        // Attach new one

        shape = CreateShape();
        CU_ASSERT(shape, "Could not create physx shape on entity '{}'", *GetEntity());

        if (m_trigger) {

            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

        }

        m_rb->m_actor->attachShape(*shape);
        shape->release();

        IN_RUNTIME_END;

    }

    PxShape* BoxCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->Scale() * m_size / 2.0f)), *material);

    }
    PxShape* SphereCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxSphereGeometry(m_radius * GetTransform()->Scale().x), *material);

    }
    PxShape* CapsuleCollider::CreateShape() const {

        CUP_FUNCTION();

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->Scale().x * m_radius, transform->Scale().y * m_height), *material);

    }

}
