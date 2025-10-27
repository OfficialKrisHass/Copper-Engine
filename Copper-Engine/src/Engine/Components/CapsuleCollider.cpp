#include "cupch.h"
#include "CapsuleCollider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxMaterial* material;

    }

    using namespace PhysicsEngine;

    PxShape* CapsuleCollider::CreateShape() const {

        CUP_FUNCTION();

        CU_ASSERT(physics != nullptr, "PhysX Physics not initialized.");
        CU_ASSERT(material != nullptr, "PhysX Physics material is invalid.");

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->GetScale().x * m_radius, transform->GetScale().y * m_height), *material);

    }

}
