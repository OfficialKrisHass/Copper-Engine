#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Filesystem/Path.h"

#include "Engine/Scene/InternalEntity.h"
#include "Engine/Scene/Registry.h"

namespace Copper {

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

		InternalEntity* CreateEntity(Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name = "Entity") {

			return registry.CreateEntity(this, position, rotation, scale, name);

		}
		InternalEntity* CreateEntityFromID(uint32_t id, Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name = "Entity", bool returnIfExists = true) {

			InternalEntity* ret = registry.CreateEntityFromID(id, this, position, rotation, scale, name, returnIfExists);

			return ret;

		}
		InternalEntity* CreateEntityFromID(uint32_t id, const std::string& name, Vector3 position, Vector3 rotation, Vector3 scale, bool returnIfExists = true) {

			return registry.CreateEntityFromID(id, this, position, rotation, scale, name, returnIfExists);

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

		bool runtimeRunning = false;
		bool runtimeStarted = false;

		void SerializeEntity(InternalEntity* entity, YAML::Emitter& out);
		void DeserializeEntity(InternalEntity* entity, const YAML::Node& node);

		template<typename T> void SerializeScriptField(const struct ScriptField& field, class ScriptComponent* instance, class YAML::Emitter& out);
		//template<> void SerializeScriptField<InternalEntity*>(const struct ScriptField& field, class ScriptComponent* instance, class YAML::Emitter& out);
		template<typename T> void DeserializeScriptField(const ScriptField& field, ScriptComponent* instance, const class YAML::Node& fieldNode);

	};

}