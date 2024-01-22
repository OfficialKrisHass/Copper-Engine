#pragma once

#include "Engine/Core/Core.h"

namespace physx { class PxVec3; class PxQuat; }

namespace Copper::PhysicsEngine {

    void Initialize();
    void Shutdown();

    physx::PxVec3 CopperToPhysX(const Vector3& vec);
    Vector3 PhysXToCopper(const physx::PxVec3& vec);

    physx::PxQuat CopperToPhysX(const Quaternion& quat);
    Quaternion PhysXToCopper(const physx::PxQuat& quat);

}