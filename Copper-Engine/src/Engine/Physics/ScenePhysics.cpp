#include "cupch.h"
#include "Engine/Scene/CopperECS.h"

#include "Engine/Physics/CollisionNotifier.cpp"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxDefaultCpuDispatcher* dispatcher;

        extern PxVec3 CopperToPhysX(const Vector3& vec);
        extern PxQuat CopperToPhysX(const Quaternion& vec);

        extern Vector3 PhysXToCopper(const PxVec3& vec);
        extern Quaternion PhysXToCopper(const PxQuat& vec);

        PxMaterial* material;
        PxShape* noColliderShape;

        CollisionNotifier collisionNotifier;

        // Custom Simulation Shader
        PxFilterFlags DefaultSimulationShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize);

    }

    using namespace PhysicsEngine;

    void Scene::InitializePhysics() {

        physicsInitialized = true;

        PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = dispatcher;
        sceneDesc.filterShader = DefaultSimulationShader;

        physicsScene = physics->createScene(sceneDesc);
        material = physics->createMaterial(0.5f, 0.5f, 0.6f);

        noColliderShape = physics->createShape(PxSphereGeometry(1.0f), *material, false);
        noColliderShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

        physicsScene->setSimulationEventCallback(&collisionNotifier);

    }
    void Scene::UpdatePhysics(float deltaTime) {

        physicsScene->simulate(deltaTime);
        physicsScene->fetchResults(true);

    }
    void Scene::ShutdownPhysics() {

        physicsInitialized = false;
        physicsScene->release();

    }

    void Scene::AddPhysicsBody(PxRigidActor* body) {

        physicsScene->addActor(*body);

    }
    void Scene::RemovePhysicsBody(PxRigidActor* body) {

        physicsScene->removeActor(*body);

    }

}