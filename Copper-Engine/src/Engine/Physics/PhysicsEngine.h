#pragma once

#include "Engine/Core/Core.h"

namespace physx {

    template <typename T> class PxVec3T;
    template <typename T> class PxQuatT;

    typedef PxVec3T<float> PxVec3;
    typedef PxQuatT<float> PxQuat;

}

namespace Copper::PhysicsEngine {

    void Initialize();
    void Shutdown();

    // Temporary conversions to satisfy the compiler.
    // In the future, the custom physx fork will be modified to use the same data types as Copper-Engine.

    physx::PxVec3 CopperToPhysX(const Vector3& vec);
    Vector3 PhysXToCopper(const physx::PxVec3& vec);

    physx::PxQuat CopperToPhysX(const Quaternion& quat);
    Quaternion PhysXToCopper(const physx::PxQuat& quat);

}
