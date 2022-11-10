#pragma once

#include "Scene.h"

namespace Copper {

	struct SceneViewIterator {

	public:
		SceneViewIterator(Registry registry, std::bitset<maxComponents> cMask, bool all, int32_t index, int32_t endIndex)
			: registry(registry), componentMask(cMask), all(all), index(index), endIndex(endIndex) {}

		Object operator*() const {

			return registry.GetObjectFromID(index);

		}

		bool operator==(const SceneViewIterator& other) const {

			return index == other.index || index == registry.GetNumOfObjects();

		}

		bool operator!=(const SceneViewIterator& other) const {

			return index != other.index && index != registry.GetNumOfObjects();

		}

		SceneViewIterator& operator++() {

			do {

				index++;

			} while (index < endIndex && ((!all && componentMask != (componentMask & registry.GetObjectFromID(index).GetComponentMask())) || !registry.GetObjectFromID(index)));

			return *this;

		}

	private:
		int32_t index;
		int32_t endIndex;
		Registry registry;
		std::bitset<maxComponents> componentMask;
		bool all = false;

	};

	template<typename ... Components> class SceneView {

	public:
		SceneView(Scene scene) : registry(scene.registry), endIndex((int32_t) registry.GetNumOfObjects()) {

			if (sizeof...(Components) == 0) { all = true; } else {

				int componentIDs[] = {0, GetCID<Components>()...};

				for (int i = 1; i < (sizeof...(Components) + 1); i++) {

					componentMask.set(componentIDs[i]);

				}

			}

			while (beginIndex < registry.GetNumOfObjects() - 1 && (!registry.GetObjectFromID(beginIndex) || componentMask != (componentMask & registry.GetObjectFromID(beginIndex).GetComponentMask()))) { beginIndex++; }
			/*while (endIndex >= 0 && endIndex > beginIndex && (!registry.GetObjectFromID(endIndex) || componentMask != (componentMask & registry.GetObjectFromID(endIndex).GetComponentMask()))) { endIndex--; }

			endIndex++;*/

		}

		const SceneViewIterator begin() const { return SceneViewIterator(registry, componentMask, all, beginIndex, endIndex); }
		const SceneViewIterator end() const { return SceneViewIterator(registry, componentMask, all, endIndex, endIndex); }

	private:
		Registry registry = nullptr;
		std::bitset<maxComponents> componentMask;

		int32_t beginIndex = 0;
		int32_t endIndex;

		bool all = false;

	};

}