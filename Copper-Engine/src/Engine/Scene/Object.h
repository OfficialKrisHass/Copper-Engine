#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	class Object {

		friend class Scene;

	public:
		Object() = default;
		Object(uint64_t id, Scene* scene) : id(id), scene(scene) {}
		~Object() {}

		std::string name;

		template<typename T> T* AddComponent() { return scene->registry.AddComponent<T>(id); }
		template<typename T> T* GetComponent() { return scene->registry.GetComponent<T>(id); }
		template<typename T> void RemoveComponent() { scene->registry.RemoveComponent<T>(id); }

		operator bool() const { return ECS::IsEntityValid(id); }

		bool operator==(const Object& other) const { return id == other.id && scene == other.scene; }
		bool operator!=(const Object& other) const { return !(*this == other); }

	public:
		uint64_t id;
		Scene* scene;

	};

}