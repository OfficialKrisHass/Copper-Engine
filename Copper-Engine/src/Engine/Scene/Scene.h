#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Registry.h"

namespace YAML { class Emitter; class Node; }
namespace physx { class PxScene; class PxRigidActor; }

namespace Copper {

	namespace Filesystem { class Path; }

	class Scene {

		friend InternalEntity;
		friend class RigidBody;
		friend class Collider;
		friend class Entity;
		friend class OldSceneVersionSerializer;

	public:
		Scene() {

			m_registry.Initialize();

		}

		std::string name = "";
		fs::path path = "";

		class Camera* cam = nullptr;

		InternalEntity* CreateEntity(ENTITY_DEFAULT_PROPERTIES_DECLARATION) { return m_registry.CreateEntity(this, position, rotation, scale, name); }
		InternalEntity* CreateEntityFromID(uint32 id, ENTITY_DEFAULT_PROPERTIES_DECLARATION, bool returnIfExists = true) {

			InternalEntity* ret = m_registry.CreateEntityFromID(id, this, position, rotation, scale, name, returnIfExists);
			return ret;

		}
		InternalEntity* GetEntityFromID(uint32 id) { return m_registry.GetEntityFromID(id); }
		void RemoveEntity(InternalEntity* entity) { m_registry.RemoveEntity(entity->m_id); }
		void RemoveEntityFromID(uint32 id) { m_registry.RemoveEntity(id); }

		void StartRuntime();
		void Update(float deltaTime);

		void Render(Camera* cam);

		void Serialize(const fs::path& path);
		bool Deserialize(const fs::path& path);

		Registry::ComponentPool* GetComponentPool(int32 cID) { return m_registry.GetComponentPool(cID); }
		uint32 GetNumOfEntities() { return (uint32) m_registry.m_entities.size(); }

		bool IsRuntimeRunning() { return m_runtimeRunning; }

	private:
		Registry m_registry;

		physx::PxScene* m_physicsScene = nullptr;

		bool m_runtimeRunning = false;
		bool m_runtimeStarted = false;
		bool m_physicsInitialized = false;

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

		template<typename T> void SerializeScriptField(const struct ScriptField& field, class ScriptComponent* instance, YAML::Emitter& out);
		template<typename T> void DeserializeScriptField(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode);

		template<> void SerializeScriptField<Transform*>(const struct ScriptField& field, class ScriptComponent* instance, YAML::Emitter& out);
		template<> void DeserializeScriptField<Transform*>(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode);

	};
	
	Scene* GetScene();
	bool IsSceneRuntimeRunning();

}