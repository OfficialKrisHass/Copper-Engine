#pragma once

#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

namespace Copper::ECS {

	//Variables
	const int maxComponents = 32;
	extern int cCounter;

	//Function to get an ID of a Component
	template<typename T> int GetCID() {

		static int cID = cCounter++;
		return cID;

	}

	//Functions for Getting the Entity Information
	inline uint64_t EntityID(uint32_t index, uint32_t version) { return ((uint64_t) index << 32) | ((uint64_t) version); }
	inline uint32_t EntityIndex(uint64_t id) { return id >> 32; }
	inline uint32_t EntityVersion(uint64_t id) { return (uint32_t) id; }
	inline bool IsEntityValid(uint64_t id) { return id >> 32 != uint32_t(-1); }

	//Component Pool, a Memory Pool that holds all of the components of a certain type in one structure
	struct ComponentPool {

		ComponentPool(uint32_t cSize) : cSize(cSize), data(new char[cSize * maxComponents]) {}
		~ComponentPool() { delete data; }

		inline void* Get(uint64_t index) { return data + index * cSize; }

	private:
		uint32_t cSize;
		char* data = nullptr;

	};

	//Registry, the main handlerer of the ECS
	class Registry {

	public:
		//Handling Entities
		uint64_t CreateEntity() {

			if (!freeEntities.empty()) {

				uint32_t index = freeEntities.back();
				uint64_t id = EntityID(index, EntityVersion(entities[index].id));

				freeEntities.pop_back();
				entities[index].id = id;

				return entities[index].id;

			}

			entities.push_back({ EntityID(uint32_t(entities.size()), 0), std::bitset<maxComponents>() });
			return entities.back().id;

		}
		void DestroyEntitiy(uint64_t id) {

			uint64_t newID = EntityID(uint32_t(-1), EntityVersion(id) + 1);

			entities[EntityIndex(id)].id = newID;
			entities[EntityIndex(id)].cMask.reset();

			freeEntities.push_back(EntityIndex(id));

		}

		//Handing Components
		template<typename T> T* AddComponent(uint64_t id) {

			if (entities[EntityIndex(id)].id != id) return nullptr;

			int cID = GetCID<T>();

			if (pools.size() <= cID) pools.resize(cID + 1, nullptr);
			if (pools[cID] == nullptr) pools[cID] = new ComponentPool(sizeof(T));

			T* component = new (pools[cID]->Get(EntityIndex(id))) T();
			entities[EntityIndex(id)].cMask.set(cID);

			return component;

		}
		template<typename T> T* GetComponent(uint64_t id) {

			if (entities[EntityIndex(id)].id != id) return nullptr;

			int cID = GetCID<T>();
			if (!entities[EntityIndex(id)].cMask.test(cID)) { return nullptr; }

			T* component = static_cast<T*>(pools[cID]->Get(EntityIndex(id)));
			return component;

		}
		template<typename T> void RemoveComponent(uint64_t id) {

			if (entities[EntityIndex(id)].id != id) return;

			int cID = GetCID<T>();

			if (!entities[EntityIndex(id)].cMask.test(cID)) return;

			entities[EntityIndex(id)].cMask.reset(cID);

		}

	public:
		struct Entity {

			uint64_t id;
			std::bitset<maxComponents> cMask;

		};

		std::vector<Entity> entities;
		std::vector<uint32_t> freeEntities;
		std::vector<ComponentPool*> pools;

	};

}