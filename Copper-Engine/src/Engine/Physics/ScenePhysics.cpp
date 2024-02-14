#include "cupch.h"
#include "Engine/Scene/CopperECS.h"

#include "Engine/Physics/PhysicsEngine.h"
#include "Engine/Physics/CollisionNotifier.cpp"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxDefaultCpuDispatcher* dispatcher;

        PxMaterial* material;
        PxShape* noColliderShape;

        CollisionNotifier collisionNotifier;

        // Custom Simulation Shader

        PxFilterFlags DefaultSimulationShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize);

    }

    using namespace PhysicsEngine;

    void Scene::InitializePhysics() {

        CUP_FUNCTION();

        m_physicsInitialized = true;

        PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = dispatcher;
        sceneDesc.filterShader = DefaultSimulationShader;

        m_physicsScene = physics->createScene(sceneDesc);
        material = physics->createMaterial(0.5f, 0.5f, 0.6f);

        noColliderShape = physics->createShape(PxSphereGeometry(1.0f), *material, false);
        noColliderShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

        m_physicsScene->setSimulationEventCallback(&collisionNotifier);

    }
    void Scene::UpdatePhysics(float deltaTime) {

        CUP_FUNCTION();

        m_physicsScene->simulate(deltaTime);
        m_physicsScene->fetchResults(true);

    }
    void Scene::ShutdownPhysics() {

        CUP_FUNCTION();

        m_physicsInitialized = false;
        m_physicsScene->release();

    }

    void Scene::AddPhysicsBody(PxRigidActor* body) {

        CUP_FUNCTION();

        m_physicsScene->addActor(*body);

    }
    void Scene::RemovePhysicsBody(PxRigidActor* body) {

        CUP_FUNCTION();

        m_physicsScene->removeActor(*body);

    }

}