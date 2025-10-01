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

        Transform* transform = GetTransform();
        return physics->createShape(PxCapsuleGeometry(transform->GetScale().x * m_radius, transform->GetScale().y * m_height), *material);

    }

}
