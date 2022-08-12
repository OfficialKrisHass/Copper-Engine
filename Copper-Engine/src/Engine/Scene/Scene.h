#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/ECS.h"
#include "Engine/Scene/Object.h"

namespace Copper {

	class Object;

	class Scene {

	public:
		Object CreateObject(std::string name = "Object");
		void DestroyObject(Object obj);

		ECS::Registry registry;

	};

	template<typename ... ComponentTypes>
	struct SceneView {

	public:
		//Iterator
		struct Iterator {

		public:
			Iterator(Scene* scene, uint32_t index, std::bitset<ECS::maxComponents> cMask, bool all) : scene(scene), index(index), cMask(cMask), all(all) {}

			Object operator*() const {

				Object obj(scene->registry.entities[index].id, scene);

				return obj;

			}

			bool operator==(const Iterator& other) const {

				return index == other.index || index == scene->registry.entities.size();

			}

			bool operator!=(const Iterator& other) const {

				return index != other.index && index != scene->registry.entities.size();

			}

			Iterator& operator++() {

				do {

					index++;

				} while (index < scene->registry.entities.size() && !ValidIndex());

				return *this;

			}

		private:
			uint32_t index;
			Scene* scene;
			std::bitset<ECS::maxComponents> cMask;
			bool all = false;

			bool ValidIndex() {

				return ECS::IsEntityValid(scene->registry.entities[index].id) && (all || cMask == (cMask & scene->registry.entities[index].cMask));

			}

		};//Iterator

		SceneView(Scene& scene) : scene(&scene) {

			if (sizeof...(ComponentTypes) == 0) {

				all = true;

			} else {

				int componentIDs[] = { 0, ECS::GetCID<ComponentTypes>() ... };

				for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++) {

					cMask.set(componentIDs[i]);

				}

			}

		}

		const Iterator begin() const {

			int firstIndex = 0;

			while (firstIndex < scene->registry.entities.size() && (cMask != (cMask & scene->registry.entities[firstIndex].cMask) || !ECS::IsEntityValid(scene->registry.entities[firstIndex].id))) {

				firstIndex++;

			}

			return Iterator(scene, firstIndex, cMask, all);

		}

		const Iterator end() const {

			return Iterator(scene, uint32_t(scene->registry.entities.size()), cMask, all);

		}

	private:
		Scene* scene = nullptr;
		std::bitset<ECS::maxComponents> cMask;
		bool all = false;

	};

}