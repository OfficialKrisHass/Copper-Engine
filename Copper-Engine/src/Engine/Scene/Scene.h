#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Registry.h"

#include "Engine/Filesystem/Path.h"

#define ENTITY_DEFAULT_PROPERTIES_DECLARATION const Vector3& position = Vector3::zero, const Quaternion& rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), const Vector3& scale = Vector3::one, const std::string& name = "Entity"
#define ENTITY_PROPERTIES_DECLARATION const Vector3& position, const Quaternion& rotation, const Vector3& scale, const std::string& name

namespace YAML { class Emitter; class Node; }
namespace physx { class PxScene; }

namespace Copper {

	namespace Filesystem { class Path; }

	class Light;
	class Camera;

	class Scene {

		friend InternalEntity;
		friend class Entity;
		friend class OldSceneVersionSerializer;

	public:
		std::string name;
		Filesystem::Path path;

		Camera* cam = nullptr;

		InternalEntity* CreateEntity(ENTITY_DEFAULT_PROPERTIES_DECLARATION) {

			return registry.CreateEntity(this, position, rotation, scale, name);

		}
		InternalEntity* CreateEntityFromID(uint32_t id, ENTITY_DEFAULT_PROPERTIES_DECLARATION, bool returnIfExists = true) {

			InternalEntity* ret = registry.CreateEntityFromID(id, this, position, rotation, scale, name, returnIfExists);

			return ret;

		}
		InternalEntity* GetEntityFromID(uint32_t id) {

			return registry.GetEntityFromID(id);

		}
		void RemoveEntity(InternalEntity* entity) {

			registry.RemoveEntity(entity->id);

		}
		void RemoveEntityFromID(uint32_t id) {

			registry.RemoveEntity(id);

		}

		void StartRuntime();
		void Update(bool render, float deltaTime);

		void Render(Camera* cam);

		void Serialize(const Filesystem::Path& path);
		bool Deserialize(const Filesystem::Path& path);

		Registry::ComponentPool* GetComponentPool(int cID) {

			return registry.GetComponentPool(cID);

		}
		uint32_t GetNumOfEntities() { return (uint32_t) registry.entities.size(); }

		bool IsRuntimeRunning() { return runtimeRunning; }

	private:
		Registry registry;

		Light* light;

		physx::PxScene* physicsScene;

		bool runtimeRunning = false;
		bool runtimeStarted = false;

		// Defined in PhysicsEngine.cpp so that we dont have physx includes in Scene.cpp

		bool physicsInitialized = false;

		void InitializePhysics();
		void UpdatePhysics(float deltaTime);
		void ShutdownPhysics();

		void SerializeEntity(InternalEntity* entity, YAML::Emitter& out);
		void DeserializeEntity(InternalEntity* entity, const YAML::Node& node);

		template<typename T> void SerializeScriptField(const struct ScriptField& field, class ScriptComponent* instance, YAML::Emitter& out);
		template<typename T> void DeserializeScriptField(const ScriptField& field, ScriptComponent* instance, const YAML::Node& fieldNode);

	};

	Scene* GetScene();
	void SetShouldRenderScene(bool value);

	uint32_t GetNumOfEntities();
	bool IsSceneRuntimeRunning();

	InternalEntity* CreateEntity(ENTITY_DEFAULT_PROPERTIES_DECLARATION);
	InternalEntity* CreateEntityFromID(uint32_t id, ENTITY_DEFAULT_PROPERTIES_DECLARATION, bool returnIfExists = true);
	InternalEntity* GetEntityFromID(uint32_t id);
	void RemoveEntity(InternalEntity* entity);
	void RemoveEntityFromID(uint32_t id);

}