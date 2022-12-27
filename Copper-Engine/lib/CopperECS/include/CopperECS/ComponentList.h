#pragma once

#include "Component.h"

namespace Copper {

	struct ComponentPool;

	template<typename T> struct ComponentListIterator {

	public:
		ComponentListIterator(ComponentPool* pool, int32_t objID, uint32_t index) : pool(pool), objID(objID), index(index) {}

		T* operator*() {

			return (T*) pool->Get(objID, index);

		}

		bool operator!=(const ComponentListIterator& other) {

			return index <= other.index && pool;

		}

		ComponentListIterator operator++() {

			do {

				index++;

			} while (index < pool->GetCount(objID) && !((T*) pool->Get(objID, index))->Valid());

			return *this;

		}

	private:
		ComponentPool* pool;
		int32_t objID;
		uint32_t index;

	};

	template<typename T> class ComponentList {

	public:
		ComponentList() : valid(false) {}
		ComponentList(Scene* scene, int32_t objID, uint32_t cID) : valid(true), objID(objID), cID(cID), registry(scene->GetRegistry()) {}

		const ComponentListIterator<T> begin() const {

			if (!valid) return ComponentListIterator<T>(nullptr, -1, 0);

			uint32_t index = 0;
			while (index < registry->GetComponentPool(cID)->GetCount(objID) && !((T*) registry->GetComponentPool(cID)->Get(objID, index))->Valid()) {

				index++;

			}

			return ComponentListIterator<T>(registry->GetComponentPool(cID), objID, index);

		}
		const ComponentListIterator<T> end() const {

			if (!valid) return ComponentListIterator<T>(nullptr, -1, 0);
			return ComponentListIterator<T>(registry->GetComponentPool(cID), objID, registry->GetComponentPool(cID)->GetCount(objID) - 1);

		}

	private:
		bool valid;
		int32_t objID;
		uint32_t cID;
		Registry* registry;

	};

}