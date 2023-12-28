#include "cupch.h"
#include "PhysicsEngine.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    PxFoundation* foundation = nullptr;
    PxPhysics* physics = nullptr;
    PxDefaultCpuDispatcher* dispatcher = nullptr;

    PxDefaultAllocator allocator;
    PxDefaultErrorCallback errCallback;

    void Initialize() {

        foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errCallback);
        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
        dispatcher = PxDefaultCpuDispatcherCreate(1);
        
    }

    void Shutdown() {

        dispatcher->release();
        physics->release();
        foundation->release();

    }

    PxVec3 CopperToPhysX(const Vector3& vec) { return PxVec3(vec.x, vec.y, vec.z); }
    Vector3 PhysXToCopper(const PxVec3& vec) { return Vector3(vec.x, vec.y, vec.z); }

    PxQuat CopperToPhysX(const Quaternion& quat) { return PxQuat(quat.x, quat.y, quat.z, quat.w); }
    Quaternion PhysXToCopper(const PxQuat& quat) { return Quaternion(quat.w, quat.x, quat.y, quat.z); }

}