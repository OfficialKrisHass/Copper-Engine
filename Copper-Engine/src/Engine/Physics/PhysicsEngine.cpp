#include "cupch.h"
#include "PhysicsEngine.h"

#include "Engine/Components/Transform.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    struct Data {

        physx::PxDefaultAllocator allocator;
        physx::PxDefaultErrorCallback errCallback;
        physx::PxDefaultCpuDispatcher* dispatcher = nullptr;

        physx::PxFoundation* foundation = nullptr;
        physx::PxPhysics* physics = nullptr;

        physx::PxMaterial* material = nullptr;

    };
    Data data;

    physx::PxQuat CopperToPhysX(const Quaternion& quat) {

        return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);

    }
    physx::PxVec3 CopperToPhysX(const Vector3& vec) {

        return physx::PxVec3(vec.x, vec.y, vec.z);

    }

    void Initialize() {

        data.foundation = PxCreateFoundation(PX_PHYSICS_VERSION, data.allocator, data.errCallback);
        data.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *data.foundation, physx::PxTolerancesScale());
        data.dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

        data.material = data.physics->createMaterial(0.5f, 0.5f, 0.6f);

    }
    void Shutdown() {

        data.dispatcher->release();
        data.physics->release();
        data.foundation->release();

    }

    void UpdateScene(physx::PxScene* scene) {

        scene->simulate(1.0f / 60.0f);
        scene->fetchResults(true);

    }

    physx::PxScene* CreateScene() {

        physx::PxSceneDesc sceneDesc(data.physics->getTolerancesScale());
        sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = data.dispatcher;
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

        return data.physics->createScene(sceneDesc);

    }
    physx::PxRigidActor* CreateRigidActor(Transform* transform, bool staticBody) {

        physx::PxShape* shape = data.physics->createShape(physx::PxBoxGeometry(transform->scale.x / 2.0f, transform->scale.y / 2.0f, transform->scale.z / 2.0f), *data.material);
        physx::PxRigidActor* ret = nullptr;

        if(!staticBody)
            ret = physx::PxCreateDynamic(*data.physics, physx::PxTransform(CopperToPhysX(transform->position), CopperToPhysX(transform->rotation)), *shape, 10.0f);
        else
            ret = physx::PxCreateStatic(*data.physics, physx::PxTransform(CopperToPhysX(transform->position), CopperToPhysX(transform->rotation)), *shape);

        shape->release();
        return ret;

    }

}