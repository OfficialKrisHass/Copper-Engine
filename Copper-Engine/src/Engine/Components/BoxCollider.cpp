#include "cupch.h"
#include "BoxCollider.h"

#include "Engine/Physics/PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxMaterial* material;

    }

    using namespace PhysicsEngine;

    PxShape* BoxCollider::CreateShape() const {

        CUP_FUNCTION();

        return physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->GetScale() * m_size / 2.0f)), *material);

    }

}
