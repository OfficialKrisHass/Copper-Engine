#pragma once

#include <string>
#include <bitset>

//#include "Component.h"

#include "Engine/Components/Transform.h"
#include "Engine/Components/Tag.h"

namespace Copper {

	const int maxComponents = 32;

	class Object {

		friend class Scene;
		friend class Registry;

	public:
		Object() = default;

		Tag* tag = nullptr;
		Transform* transform = nullptr;

		template<typename T> T* AddComponent() {

			T* component = scene->registry.AddComponent<T>(*this);

			component->object = this;
			component->transform = transform;

			return component;

		}
		template<typename T> T* GetComponent() { return scene->registry.GetComponent<T>(id); }
		template<typename T> bool HasComponent() { return scene->registry.HasComponent<T>(id); }
		template<typename T> void RemoveComponent() { scene->registry.RemoveComponent<T>(*this); }

		operator bool() const { return id != -1 && scene != nullptr; }
		operator int32_t() const { return id; }

		bool operator==(const Object& other) const { return id == other.id && scene == other.scene; }
		bool operator!=(const Object& other) const { return !(*this == other); }

		int32_t GetID() const { return id; }
		std::bitset<maxComponents> GetComponentMask() const { return componentMask; }

	public:
		int32_t id = -1;
		std::bitset<maxComponents> componentMask;

		Scene* scene = nullptr;

	};

}