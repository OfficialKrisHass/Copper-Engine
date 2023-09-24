#include "cupch.h"
#include "PhysicsEngine.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"
#include "Engine/Components/BoxCollider.h"

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

    }

    void Shutdown() {

        data.dispatcher->release();
        data.physics->release();
        data.foundation->release();

    }

    PxVec3 CopperToPhysX(const Vector3& vec) { return PxVec3(vec.x, vec.y, vec.z); }
    Vector3 PhysXToCopper(const PxVec3& vec) { return Vector3(vec.x, vec.y, vec.z); }

    PxQuat CopperToPhysX(const Quaternion& quat) { return PxQuat(quat.x, quat.y, quat.z, quat.w); }
    Quaternion PhysXToCopper(const PxQuat& quat) { return Quaternion(quat.w, quat.x, quat.y, quat.z); }

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
    void Scene::AddRigidBody(RigidBody* rb) {

        physicsScene->addActor(*rb->body);

    }
    void Scene::RemoveRigidBody(RigidBody* rb) {

        physicsScene->removeActor(*rb->body);

    }

    void RigidBody::SetupBody() {

        if (body) {

            GetScene()->RemoveRigidBody(this);

        }

        BoxCollider* collider = GetEntity()->GetComponent<BoxCollider>();
        if (!collider) { LogError("Rigidbody on Entity {} has no Collider!", GetEntity()->name); return; }

        PxShape* shape = data.physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->scale * collider->size / 2.0f)), *data.material);

        if (isStatic)
            body = PxCreateStatic(*data.physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape);
        else
            body = PxCreateDynamic(*data.physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape, 1.0f);

        if (!gravity && !isStatic)
            body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

        GetScene()->AddRigidBody(this);
        shape->release();

        body->setName(GetEntity()->name.c_str());
        if (!isStatic)
            ((PxRigidDynamic*) body)->setMass(mass);

    }
    void RigidBody::UpdatePositionAndRotation() {

        GetTransform()->position = PhysXToCopper(body->getGlobalPose().p);
        GetTransform()->rotation = PhysXToCopper(body->getGlobalPose().q);

    }

}