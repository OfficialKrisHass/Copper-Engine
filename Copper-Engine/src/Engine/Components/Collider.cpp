#include "cupch.h"
#include "Collider.h"

#include "Engine/Components/RigidBody.h"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

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

    

}
