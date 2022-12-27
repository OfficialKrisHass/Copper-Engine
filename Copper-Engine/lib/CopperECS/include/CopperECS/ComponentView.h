#pragma once

#include "Scene.h"

namespace Copper {

	struct ComponentViewIterator {

	public:
		ComponentViewIterator(uint32_t index, ComponentPool* pool) : index(index), pool(pool) {}

		Component* operator*() {

			return (Component*) pool->Get(index);

		}

		bool operator!=(const ComponentViewIterator& other) const {

			return index != other.index;

		}

		ComponentViewIterator& operator++() {

			do {

				index++;

			} while (index < pool->GetCount() && !Valid());

			return *this;

		}

	private:
		uint32_t index;
		ComponentPool* pool;

		bool Valid() const { return ((Component*) pool->Get(index))->Valid(); }

	};

	template<typename Component> class ComponentView {

	public:
		ComponentView(Scene* scene) : registry(scene->GetRegistry()), pool(registry->GetComponentPool(GetCID<Component>())) {

			endIndex = pool->GetCount();

			while (!((Component*) pool->Get(beginIndex))->Valid()) { beginIndex++; }

		}

		const ComponentViewIterator begin() const { return ComponentViewIterator(beginIndex, pool); }
		const ComponentViewIterator end() const { return ComponentViewIterator(endIndex, pool); }

	private:
		Registry* registry;
		ComponentPool* pool;

		uint32_t beginIndex = 0;
		uint32_t endIndex;

	};

}