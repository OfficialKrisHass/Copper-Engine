#pragma once

#include "Scene.h"
#include "Engine/Core/Core.h"

#include "Engine/Scene/Components/Name.h"
#include "Engine/Scene/Components/Transform.h"

namespace Copper {

	class Object {

		friend class Scene;

	public:
		Object() = default;
		Object(uint64_t id, Scene* scene) : id(id), scene(scene) {}
		~Object() {}

		Name* name;
		Transform* transform;

		template<typename T> T* AddComponent() { 
			
			T* component = scene->registry.AddComponent<T>(id);

			component->transform = transform;

			return component;
		
		}
		template<typename T> T* GetComponent() { return scene->registry.GetComponent<T>(id); }
		template<typename T> void RemoveComponent() { scene->registry.RemoveComponent<T>(id); }
		template<typename T> bool HasComponent() { return scene->registry.HasComponent<T>(id); }

		static Object Null() { return Object(ECS::EntityID(-1, -1), nullptr); }
		
		operator bool() const { return id != ECS::EntityID(-1, -1); }
		operator uint64_t() const { return            id; }
		operator uint32_t() const { return (uint32_t) id; }

		bool operator==(const Object& other) const { return id == other.id && scene == other.scene; }
		bool operator!=(const Object& other) const { return !(*this == other); }

	public:
		uint64_t id;
		Scene* scene;

	};

}