#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Registry.h"

#include "Engine/Components/Light.h"
#include "Engine/Components/Camera.h"

namespace Copper {

	class Scene {

		friend InternalEntity;
		friend class Entity;
		friend class OldSceneVersionSerializer;

	public:
		std::string name;
		std::filesystem::path path;

		Camera* cam = nullptr;

		InternalEntity* CreateEntity(Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name = "Entity") {

			return registry.CreateEntity(this, position, rotation, scale, name);

		}
		InternalEntity* CreateEntityFromID(uint32_t id, Vector3 position, Vector3 rotation, Vector3 scale, const std::string& name = "Entity", bool returnIfExists = true) {

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
		void Update(bool render);

		void Render(Camera* cam);

		void Serialize(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);

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

	};

}