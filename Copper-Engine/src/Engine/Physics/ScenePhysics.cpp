#include "cupch.h"
#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Physics/PhysicsEngine.h"
#include "Engine/Physics/CollisionNotifier.cpp"

#include <PxPhysicsAPI.h>

namespace Copper {

    using namespace physx;

    namespace PhysicsEngine {

        extern PxPhysics* physics;
        extern PxDefaultCpuDispatcher* dispatcher;

        PxMaterial* material;

        CollisionNotifier collisionNotifier;

        // Custom Simulation Shader

        PxFilterFlags DefaultSimulationShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize);

    }

    using namespace PhysicsEngine;

    void Scene::InitializePhysics() {

        CUP_FUNCTION();

        PxSceneDesc sceneDesc(physics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = dispatcher;
        sceneDesc.filterShader = DefaultSimulationShader;

        m_physicsScene = physics->createScene(sceneDesc);
        CU_ASSERT(m_physicsScene != nullptr, "Could not create PhysX physics scene.");

        material = physics->createMaterial(0.5f, 0.5f, 0.6f);
        CU_ASSERT(material != nullptr, "Could not create PhysX Physics material.");

        m_physicsScene->setSimulationEventCallback(&collisionNotifier);

        for (RigidBody* rb : ComponentView<RigidBody>(this))
            rb->Initialize();

    }
    void Scene::UpdatePhysics(float deltaTime) {

        CUP_FUNCTION();
        CUP_START_FRAME("Physics");

        m_physicsScene->simulate(deltaTime);
        m_physicsScene->fetchResults(true);

        CUP_END_FRAME();

    }
    void Scene::DeinitializePhysics() {

        CUP_FUNCTION();

        m_physicsScene->release();
        m_physicsScene = nullptr;

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
