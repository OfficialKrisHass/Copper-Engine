#include "cupch.h"
#include "SphereCollider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxMaterial* material;

    }

    using namespace PhysicsEngine;

    PxShape* SphereCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxSphereGeometry(m_radius * GetTransform()->GetScale().x), *material);

    }

}
