#pragma once

#include <string>
#include <vector>

#include "Engine/Components/Transform.h"
#include "Engine/Components/Tag.h"
#include "Engine/Components/ScriptComponent.h"

namespace Copper {

	const int maxComponents = 32;

	template<typename T> class ComponentList;

	class Object {

		friend class Scene;
		friend class Registry;

	public:
		Object() = default;

		Tag* tag = nullptr;
		Transform* transform = nullptr;

		template<typename T> T* AddComponent() {

			T* component = scene->registry.AddComponent<T>(*this);
			if (!component) return nullptr;

			component->object = this;
			component->transform = transform;

			return component;

		}
		template<typename T> T* GetComponent(uint32_t index = 0) const { return scene->registry.GetComponent<T>(id); }
		template<typename T> bool HasComponent() const { return scene->registry.HasComponent<T>(id); }
		template<typename T> void RemoveComponent(uint32_t index = 0) { scene->registry.RemoveComponent<T>(*this, index); }

		template<typename T> ComponentList<T> GetComponents() { return scene->registry.GetComponents<T>(scene, id); }

		operator bool() const { return id != -1 && scene != nullptr; }
		operator int32_t() const { return id; }

		bool operator==(const Object& other) const { return id == other.id && scene == other.scene; }
		bool operator!=(const Object& other) const { return !(*this == other); }

		int32_t GetID() const { return id; }

		std::vector<uint32_t> GetComponentMask() const { return componentMask; }

	private:
		int32_t id = -1;
		std::vector<uint32_t> componentMask;

		Scene* scene = nullptr;

	};

}