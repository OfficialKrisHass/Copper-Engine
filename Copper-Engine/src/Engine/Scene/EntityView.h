#pragma once

#include "Scene.h"

namespace Copper {

	class EntityView {

	private:
		struct Iterator {

			Iterator(uint32_t index, Scene* scene, uint32_t endIndex = 0) : index(index), scene(scene), endIndex(endIndex) {}

			InternalEntity* operator*() {

				return scene->GetEntityFromID(index);

			}

			bool operator!=(const Iterator& other) {

				return index != other.index + 1;

			}
			Iterator& operator++() {

				if (index == endIndex) {

					index++;
					return *this;

				}

				do {

					index++;

				} while (!(*scene->GetEntityFromID(index)));

				return *this;

			}

		private:
			uint32_t index;
			Scene* scene;
			uint32_t endIndex;

		};

	public:
		EntityView(Scene* scene) : scene(scene) {

			if (scene->GetNumOfEntities() == 0) {

				beginIndex = 1;
				endIndex = 0;
				return;

			}

			endIndex = scene->GetNumOfEntities() - 1;

			while (beginIndex != endIndex + 1 && !(*scene->GetEntityFromID(beginIndex))) { beginIndex++; }
			while (endIndex >= beginIndex && !(*scene->GetEntityFromID(endIndex))) { endIndex--; }

		}

		const Iterator begin() const { return Iterator(beginIndex, scene, endIndex); }
		const Iterator end() const { return Iterator(endIndex, scene); }

	private:
		Scene* scene = nullptr;
		uint32_t beginIndex = 0;
		uint32_t endIndex = 0;

	};

}