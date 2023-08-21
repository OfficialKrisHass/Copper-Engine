#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Scene.h"

namespace Copper {

	template<typename T> class ComponentView {

	private:
		struct Iterator {

			Iterator(uint32_t index, uint32_t endIndex, Registry::ComponentPool* pool) : index(index), endIndex(endIndex), pool(pool) {}

			T* operator*() {

				return static_cast<T*>(pool->Get(index));

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

				} while (!pool->Valid(index) || !(*((T*) pool->Get(index))->GetEntity()));

				return *this;

			}

		private:
			uint32_t index;
			uint32_t endIndex;
			Registry::ComponentPool* pool;

		};

	public:
		ComponentView(Scene* scene) : scene(scene), pool(scene->GetComponentPool(Registry::GetCID<T>())) {

			if (!pool) {

				beginIndex = MAX_ENTITY_COMPONENTS;
				endIndex = MAX_ENTITY_COMPONENTS - 1;
				return;

			}

			endIndex = MAX_ENTITY_COMPONENTS - 1;
			while (beginIndex != MAX_ENTITY_COMPONENTS && (!pool->Valid(beginIndex) || !(*((T*) pool->Get(beginIndex))->GetEntity()))) {

				beginIndex++;

			}
			while (endIndex >= beginIndex && (!pool->Valid(endIndex) || !(*((T*) pool->Get(endIndex))->GetEntity()))) {

				endIndex--;

			}

		}

		const Iterator begin() const { return Iterator(beginIndex, endIndex, pool); }
		const Iterator end() const { return Iterator(endIndex, endIndex, pool); }

	private:
		Scene* scene = nullptr;
		Registry::ComponentPool* pool = nullptr;

		uint32_t beginIndex = 0;
		uint32_t endIndex = 0;

	};

}