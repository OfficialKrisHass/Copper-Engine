#pragma once

#include "Scene.h"

#include <vector>

namespace Copper {

	struct SceneViewIterator {

	public:
		SceneViewIterator(Registry* registry, std::vector<uint32_t> cMask, bool all, int32_t index, int32_t endIndex)
			: registry(registry), componentMask(cMask), all(all), index(index), endIndex(endIndex) {}

		Object operator*() const {

			return registry->GetObjectFromID(index);

		}

		bool operator!=(const SceneViewIterator& other) const {

			return index != other.index && ValidObject();

		}

		SceneViewIterator& operator++() {

			do {

				index++;

			} while (index < endIndex && !ValidObject());

			return *this;

		}

	private:
		int32_t index;
		int32_t endIndex;
		Registry* registry;
		std::vector<uint32_t> componentMask;
		bool all = false;

		bool ValidObject() const {

			if (!registry->GetObjectFromID(index)) return false;
			if (all) return true;

			for (uint32_t cID : componentMask) {

				if (registry->GetObjectFromID(index).GetComponentMask().size() <= cID || registry->GetObjectFromID(index).GetComponentMask()[cID] <= 0) {

					return false;

				}

			}

			return true;

		}

	};

	template<typename ... Components> class SceneView {

	public:
		SceneView(Scene* scene) : registry(scene->GetRegistry()), endIndex(registry->GetNumOfObjects()) {

			if (sizeof...(Components) == 0) { all = true; } else {

				int componentIDs[] = {0, GetCID<Components>()...};

				for (int i = 1; i < (sizeof...(Components) + 1); i++) {

					componentMask.push_back(componentIDs[i]);

				}

			}

			while (beginIndex < registry->GetNumOfObjects() - 1) {

				if (!registry->GetObjectFromID(beginIndex)) { beginIndex++; continue; }
				if (all) break;

				bool invalid = false;
				for (uint32_t cID : componentMask) {

					if (registry->GetObjectFromID(beginIndex).GetComponentMask().size() <= cID || registry->GetObjectFromID(beginIndex).GetComponentMask()[cID] <= 0) {

						beginIndex++;
						invalid = true;
						break;

					}

				}
				if (invalid) continue;

				break;

			}

		}

		const SceneViewIterator begin() const { return SceneViewIterator(registry, componentMask, all, beginIndex, endIndex); }
		const SceneViewIterator end() const { return SceneViewIterator(registry, componentMask, all, endIndex, endIndex); }

	private:
		Registry* registry = nullptr;
		std::vector<uint32_t> componentMask;

		int beginIndex = 0;
		int endIndex;

		bool all = false;

	};

}