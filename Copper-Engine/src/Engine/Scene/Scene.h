#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Registry.h"

namespace YAML { class Emitter; class Node; }
namespace physx { class PxScene; class PxRigidActor; }

namespace Copper {

    namespace Filesystem { class Path; }
    namespace Scripting { struct Field; }

    class ScriptComponent;

    class Scene {

        friend InternalEntity;
        friend class Entity;

        friend class RigidBody;
        friend class Collider;

        friend class Raycast;

        friend class OldSceneVersionSerializer;

    public:
        Scene() {

            CUP_FUNCTION();

            m_registry.Initialize();

            m_registry.AddEntityCreatedEventFunc(BindEventFunc(Scene::EntityCreated));
            m_registry.AddEntityRemovedEventFunc(BindEventFunc(Scene::EntityRemoved));

            m_registry.AddComponentAddedEventFunc(BindEventFunc(Scene::ComponentAdded));
            m_registry.AddComponentRemovedEventFunc(BindEventFunc(Scene::ComponentRemoved));

        }
        ~Scene();

        void Initialize();
        void Cleanup();

        InternalEntity* CreateEntity(const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity") {

            CUP_FUNCTION();
            return m_registry.CreateEntity(this, position, rotation, scale, name);

        }
        InternalEntity* CreateEntityFromID(uint32 id, const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity", bool returnIfExists = true) {

            CUP_FUNCTION();
            return m_registry.CreateEntityFromID(id, this, position, rotation, scale, name, returnIfExists);

        }

        InternalEntity* GetEntityFromID(uint32 id) {

            CUP_FUNCTION();
            return m_registry.GetEntityFromID(id);

        }
        void RemoveEntity(InternalEntity* entity) {

            CUP_FUNCTION();
            m_registry.RemoveEntity(entity->m_id);

        }
        void RemoveEntityFromID(uint32 id) {

            CUP_FUNCTION();
            m_registry.RemoveEntity(id);

        }

        void Update(float deltaTime);

        void Render(class Camera* cam, bool gizmos = true);

        inline bool IsInitialized() const { return initialized; }
        inline const std::string& GetName() const { return m_name; }

        inline Camera* GetMainCamera() const { return m_cam; }

        Registry::ComponentPool* GetComponentPool(int32 cID) const { CUP_FUNCTION(); return m_registry.GetComponentPool(cID); }
        uint32 GetNumOfEntities() const { return (uint32) m_registry.m_entities.size(); }

        inline void SetName(const std::string& value) { m_name = value; }

    private:
        bool initialized = false;

        std::string m_name = "";

        Registry m_registry;
        Camera* m_cam = nullptr;

        physx::PxScene* m_physicsScene = nullptr;
        bool m_physicsInitialized = false;

        bool EntityCreated(const Event& e);
        bool EntityRemoved(const Event& e);

        bool ComponentAdded(const Event& e);
        bool ComponentRemoved(const Event& e);

        // Runtime

        void RuntimeUpdateEntity(InternalEntity* entity, float deltaTime);

        // Defined in PhysicsEngine.cpp so that we dont have physx includes in Scene.cpp

        void InitializePhysics();
        void UpdatePhysics(float deltaTime);
        void ShutdownPhysics();

        void AddPhysicsBody(physx::PxRigidActor* body);
        void RemovePhysicsBody(physx::PxRigidActor* body);

    };

    // Definition in Engine.cpp
    Scene* GetScene();

    inline InternalEntity* CreateEntity(const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity") {

        CUP_FUNCTION();
        return GetScene()->CreateEntity(position, rotation, scale, name);

    }
    inline InternalEntity* CreateEntityFromID(uint32 id, const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity", bool returnIfExists = true) {

        CUP_FUNCTION();
        return GetScene()->CreateEntityFromID(id, position, rotation, scale, name, returnIfExists);

    }
    inline InternalEntity* GetEntityFromID(uint32 id) {

        CUP_FUNCTION();
        return GetScene()->GetEntityFromID(id);

    }
    inline void RemoveEntity(InternalEntity* entity) {

        CUP_FUNCTION();
        GetScene()->RemoveEntity(entity);

    }
    inline void RemoveEntityFromID(uint32 id) {

        CUP_FUNCTION();
        GetScene()->RemoveEntityFromID(id);

    }

    inline uint32 GetNumOfEntities() {

        CUP_FUNCTION();
        return GetScene()->GetNumOfEntities();

    }

}
