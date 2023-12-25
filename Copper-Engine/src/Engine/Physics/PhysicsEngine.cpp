#include "cupch.h"
#include "PhysicsEngine.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"
#include "Engine/Components/BoxCollider.h"

#include "Engine/Physics/CollisionNotifier.cpp"

#include <PxPhysicsAPI.h>

namespace Copper::PhysicsEngine {

    using namespace physx;

    // Custom Simulation Shader
    PxFilterFlags DefaultSimulationShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize);

    struct PhysicsEngineData {

        PxDefaultAllocator allocator;
        PxDefaultErrorCallback errCallback;
        PxDefaultCpuDispatcher* dispatcher = nullptr;

        PxFoundation* foundation = nullptr;
        PxPhysics* physics = nullptr;

        PxMaterial* material = nullptr;

        PxShape* noColliderShape = nullptr;

        CollisionNotifier collisionNotifier;

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

    void CreateStaticRigidBody(PxRigidActor* body) {

        //

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
        sceneDesc.filterShader = DefaultSimulationShader;

        physicsScene = data.physics->createScene(sceneDesc);
        data.material = data.physics->createMaterial(0.5f, 0.5f, 0.6f);

        data.noColliderShape = data.physics->createShape(PxSphereGeometry(1.0f), *data.material, false);
        data.noColliderShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

        physicsScene->setSimulationEventCallback(&data.collisionNotifier);

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

    void BoxCollider::Setup() {

        RigidBody* rb = GetEntity()->GetComponent<RigidBody>();
        if (rb) return;

        // Case 1: Collider with no Rigid Body
        
        PxShape* shape = data.physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->scale * size / 2.0f)), *data.material);
        if (trigger) {

            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
            
        }

        PxRigidStatic* body = PxCreateStatic(*data.physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape);
        
        body->setName(GetEntity()->name.c_str());
        body->userData = (void*) GetEntity();
        
        GetScene()->AddPhysicsBody(body);

    }

    void RigidBody::Setup() {

        // In a case we changed some parameters we need to first delete the body
        // NOTE: Maybe rename this to something like Generate or Regenerate ????

        if (body)
            GetScene()->RemovePhysicsBody(body);

        BoxCollider* collider = GetEntity()->GetComponent<BoxCollider>();

        // Default Case 2: Rigid Body with no Collider - Why though ?
        // NOTE: This is the default as its simpler then to create a shape
        //       and then check if we were supposed to create said shape
        //       .... and also I felt smarter when I came up with this :)

        PxShape* shape = data.noColliderShape;

        // Case 3: Rigid Body with a Collider

        if (collider) {

            shape = data.physics->createShape(PxBoxGeometry(CopperToPhysX(GetTransform()->scale * collider->size / 2.0f)), *data.material);
            if (collider->trigger) {

                shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
                shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

            }

        }

        if (isStatic)
            CreateStatic(shape);
        else
            CreateDynamic(shape);

        if (collider)
            shape->release();

        GetScene()->AddPhysicsBody(body);

        body->setName(GetEntity()->name.c_str());
        body->userData = (void*) GetEntity();

    }

    void RigidBody::CreateDynamic(PxShape* shape) {

        body = PxCreateDynamic(*data.physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape, 1.0f);

        ((PxRigidDynamic*) body)->setMass(mass);
        if (!gravity)
            body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

        // This hurts my eyes but I am like 99% sure there is no other way

        typedef PxRigidDynamicLockFlag LockFlag;
        typedef PxRigidDynamicLockFlags LockFlags;
        LockFlags flags = positionLock[0] ? LockFlag::eLOCK_LINEAR_X : (LockFlag::Enum) 0;
        if (positionLock[1]) flags |= LockFlag::eLOCK_LINEAR_Y;
        if (positionLock[2]) flags |= LockFlag::eLOCK_LINEAR_Z;
        if (rotationLock[0]) flags |= LockFlag::eLOCK_ANGULAR_X;
        if (rotationLock[1]) flags |= LockFlag::eLOCK_ANGULAR_Y;
        if (rotationLock[2]) flags |= LockFlag::eLOCK_ANGULAR_Z;

        ((PxRigidDynamic*) body)->setRigidDynamicLockFlags(flags);

    }
    void RigidBody::CreateStatic(PxShape* shape) {

        body = PxCreateStatic(*data.physics, PxTransform(CopperToPhysX(GetTransform()->position), CopperToPhysX(GetTransform()->rotation)), *shape);

    }

    void RigidBody::UpdatePositionAndRotation() {

        if (!body || isStatic)
            return;

        Transform* transform = GetTransform();

        transform->position = PhysXToCopper(body->getGlobalPose().p);
        transform->rotation = PhysXToCopper(body->getGlobalPose().q);

    }

    void RigidBody::AddForce(const Vector3& force, const ForceMode mode) {

        if (isStatic) {

            LogError("Cant add force to a static rigidBody on entity {}", *GetEntity(), GetEntity()->ID());
            return;

        }

        ((PxRigidDynamic*) body)->addForce(CopperToPhysX(force), (PxForceMode::Enum) mode);

    }
    void RigidBody::AddTorque(const Vector3& torque, const ForceMode mode) {

        if (isStatic) {

            LogError("Cant add torque to a static rigidBody on entity {}", *GetEntity());
            return;

        }

        ((PxRigidDynamic*) body)->addTorque(CopperToPhysX(torque), (PxForceMode::Enum) mode);

    }
    

}