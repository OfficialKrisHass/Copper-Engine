#include "cupch.h"
#include "Engine/Components/Collider.h"

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

        IN_NOT_RUNTIME(return);

        // Remove current shape

        CU_ASSERT(m_rb != nullptr, "Collider on entity '{}' has no rigidBody", *GetEntity());

        PxShape* shape = nullptr;
        m_rb->m_actor->getShapes(&shape, 1);
        CU_ASSERT(shape != nullptr, "Could not get the collider shape on entity '{}'", *GetEntity());

        m_rb->m_actor->detachShape(*shape);

        // Attach new one

        shape = CreateShape();
        CU_ASSERT(shape != nullptr, "Could not create the collider shape on entity '{}'", *GetEntity());

        shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_trigger);
        shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_trigger);

        m_rb->m_actor->attachShape(*shape);
        shape->release();

    }

    PxShape* BoxCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->GetScale() * m_size / 2.0f)), *material);

    }
    PxShape* SphereCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxSphereGeometry(m_radius * GetTransform()->GetScale().x), *material);

    }
    PxShape* CapsuleCollider::CreateShape() const {

        CUP_FUNCTION();

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->GetScale().x * m_radius, transform->GetScale().y * m_height), *material);

    }

}
