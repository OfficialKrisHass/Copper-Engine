#include "cupch.h"
#include "PhysicsEngine.h"

#include "Engine/Scene/Scene.h"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    struct PhysicsEngineData {

        PxDefaultAllocator allocator;
        PxDefaultErrorCallback errCallback;
        PxDefaultCpuDispatcher* dispatcher = nullptr;

        PxFoundation* foundation = nullptr;
        PxPhysics* physics = nullptr;

        PxMaterial* material = nullptr;

    };
    PhysicsEngineData data;

    void Initialize() {

        data.foundation = PxCreateFoundation(PX_PHYSICS_VERSION, data.allocator, data.errCallback);
        data.physics = PxCreatePhysics(PX_PHYSICS_VERSION, *data.foundation, PxTolerancesScale());
        data.dispatcher = PxDefaultCpuDispatcherCreate(1);

        /*PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = dispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

        scene = physics->createScene(sceneDesc);
        material = physics->createMaterial(0.5f, 0.5f, 0.6f);

        PxRigidStatic* groundPlane = PxCreatePlane(*physics, PxPlane(0, 1, 0, 0), *material);
        scene->addActor(*groundPlane);

        PxShape* shape = physics->createShape(PxSphereGeometry(10.0f), *material);
        PxRigidDynamic* dynamic = PxCreateDynamic(*physics, PxTransform(PxVec3(0, 40, 0)), *shape, 10.0f);
        scene->addActor(*dynamic);
        shape->release();

        for(unsigned int i = 0; i < 100; i++) {

            scene->simulate(1.0f/60.0f);
            scene->fetchResults(true);

            if (i % 20 != 0) continue;
            std::cout << "Position of the Sphere: " << dynamic->getGlobalPose().p.x << ", " << dynamic->getGlobalPose().p.y << ", " << dynamic->getGlobalPose().p.z << std::endl;

        }
        std::cout << "Position of the Sphere: " << dynamic->getGlobalPose().p.x << ", " << dynamic->getGlobalPose().p.y << ", " << dynamic->getGlobalPose().p.z << std::endl;

        scene->release();
        dispatcher->release();
        physics->release();
        foundation->release();*/

    }

}
namespace Copper {

    using namespace PhysicsEngine;

    void Scene::InitializePhysics() {

        physicsInitialized = true;

        PxSceneDesc sceneDesc(data.physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = data.dispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;

        physicsScene = data.physics->createScene(sceneDesc);
        data.material = data.physics->createMaterial(0.5f, 0.5f, 0.6f);

    }
    void Scene::UpdatePhysics(float deltaTime) {

        physicsScene->simulate(deltaTime);
        physicsScene->fetchResults(true);

    }
    void Scene::ShutdownPhysics() {

        physicsInitialized = false;
        physicsScene->release();

    }

}