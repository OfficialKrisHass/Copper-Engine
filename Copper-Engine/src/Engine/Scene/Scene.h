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

		}
        ~Scene();

        void Initialize();

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

		void Serialize(const fs::path& path);
		bool Deserialize(const fs::path& path);

        inline const std::string& GetName() const { return m_name; }
        inline const fs::path& GetPath() const { return m_path; }

        inline Camera* GetMainCamera() const { return m_cam; }

		Registry::ComponentPool* GetComponentPool(int32 cID) const { CUP_FUNCTION(); return m_registry.GetComponentPool(cID); }
		uint32 GetNumOfEntities() const { return (uint32) m_registry.m_entities.size(); }

        // TODO: Remove
        bool IsRuntimeRunning() const { return runtimeRunning; }
        void SetIsRuntimeRunning(bool value) { runtimeRunning = value; }

	private:
        std::string m_name = "";
		fs::path m_path = "";

		Registry m_registry;
		Camera* m_cam = nullptr;

		physx::PxScene* m_physicsScene = nullptr;
        bool m_hasPhysics = true;
		bool m_physicsInitialized = false;

        // tmp
        bool runtimeRunning = false;

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

		physx::PxScene* GetPhysicsScene() { return m_physicsScene; }

		// Serialization

		void SerializeEntity(InternalEntity* entity, YAML::Emitter& out);
		void DeserializeEntity(InternalEntity* entity, const YAML::Node& node);

        template<typename T> void SerializeField(YAML::Emitter& out, ScriptComponent* instance, const Scripting::Field& field);
        template<typename T> void DeserializeField(const YAML::Node& fieldNode, ScriptComponent* instance, const Scripting::Field& field);

	};

    // Definition in Engine.cpp
	Scene* GetScene();

    inline bool IsRuntimeRunning() { return GetScene()->IsRuntimeRunning(); }
    inline void SetIsRuntimeRunning(bool value) { GetScene()->SetIsRuntimeRunning(value); }

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
