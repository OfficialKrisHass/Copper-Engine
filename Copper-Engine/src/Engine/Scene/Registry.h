#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/RegistryEvent.h"

#include "Engine/Scene/InternalEntity.h"

#include "Engine/Components/Transform.h"

#include <vector>

#define TRANSFORM_CID 0

#define MESH_RENDERER_CID 1
#define CAMERA_CID 2
#define LIGHT_CID 3

#define SCRIPT_COMPONENT_CID 4

#define RIGIDBODY_CID 5
#define COLLIDER_CID 6
#define BOX_COLLIDER_CID 7
#define SPHERE_COLLIDER_CID 8
#define CAPSULE_COLLIDER_CID 9

namespace Copper {

	class Collider;
	class BoxCollider;
	class SphereCollider;
	class CapsuleCollider;
	extern int cCounter;

	extern EntityEvent entityCreatedEvent;
	extern EntityEvent entityRemovedEvent;
	extern ComponentEvent componentAddedEvent;
	extern ComponentEvent componentRemovedEvent;

	void AddEntityCreatedEventFunc(std::function<bool(const Event&)> func);
	void AddEntityRemovedEventFunc(std::function<bool(const Event&)> func);
	void AddComponentAddedEventFunc(std::function<bool(const Event&)> func);
	void AddComponentRemovedEventFunc(std::function<bool(const Event&)> func);

	class Registry {

		friend class Scene;
		friend class Entity;

	public:
		struct ComponentPool {

			ComponentPool() = default;
			ComponentPool(size_t size) : cSize(size), data(new char[size * MAX_ENTITY_COMPONENTS]) {}
			~ComponentPool() {

				delete[] data;

			}

			void* Add(uint32_t index) { validComponents.set(index); count++; return Get(index); }
			void* Get(uint32_t index) { return data + index * cSize; }
			void Remove(uint32_t index) { validComponents.reset(index); count--; }

			bool Valid(uint32_t index) { return validComponents.test(index); }
			uint32_t Count() { return count; }

		private:
			char* data = nullptr;
			std::bitset<MAX_ENTITY_COMPONENTS> validComponents;

			size_t cSize;
			uint32_t count = 0;

		};

	public:
		void Initialize();

		InternalEntity* CreateEntity(Scene* scene, Vector3 position, Quaternion rotation, Vector3 scale, const std::string& name) {

			uint32_t id;
			if (gaps.empty()) {

				entities.push_back(InternalEntity());
				id = (uint32_t) entities.size() - 1;

			} else {

				id = gaps.back();
				gaps.pop_back();

			}

			entities[id].id = id;
			entities[id].name = name;
			entities[id].scene = scene;

			entities[id].transform = AddComponent<Transform>(id);
			entities[id].transform->position = position;
			entities[id].transform->rotation = rotation;
			entities[id].transform->scale = scale;

			entityCreatedEvent.entity = &entities[id];
			entityCreatedEvent();

			return &entities[id];

		}
		InternalEntity* CreateEntityFromID(uint32_t eID, Scene* scene, Vector3 position, Quaternion rotation, Vector3 scale, const std::string& name, bool returnIfExists) {

			if (eID >= entities.size()) entities.resize(eID + 1, InternalEntity());
			if (returnIfExists && entities[eID]) return &entities[eID];

			bool newEntity = !entities[eID];

			entities[eID].id = eID;
			entities[eID].name = name;
			entities[eID].scene = scene;

			if (!entities[eID].transform) entities[eID].transform = AddComponent<Transform>(eID);
			entities[eID].transform->position = position;
			entities[eID].transform->rotation = rotation;
			entities[eID].transform->scale = scale;

			if (newEntity) {

				entityCreatedEvent.entity = &entities[eID];
				entityCreatedEvent();

			}

			return &entities[eID];

		}
		InternalEntity* GetEntityFromID(uint32_t eID) {

			if (eID == INVALID_ENTITY_ID || eID >= entities.size() || !entities[eID]) return nullptr;
			return &entities[eID];

		}
		void RemoveEntity(uint32_t eID) {

			entityRemovedEvent.entity = &entities[eID];
			entityRemovedEvent();

			//Update the Parent
			if (Transform* parent = entities[eID].transform->parent) {

				parent->RemoveChild(entities[eID].transform);

			}

			//Update the Children
			for (uint32_t childID : entities[eID].transform->children) {

				RemoveEntity(childID);

			}
			entities[eID].transform->children.clear();

			entities[eID].Invalidate();
			gaps.push_back(eID);

			for (ComponentPool* pool : pools) {

				if (!pool || !pool->Valid(eID)) continue;
				pool->Remove(eID);

			}

		}

		template<typename T> T* AddComponent(uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!entities[eID]) return nullptr;

			int cID = GetCID<T>();

		#ifdef CU_DEBUG
			if (pools.size() < cID + 1) pools.resize(cID + 1, nullptr);
		#endif
			if (!pools[cID]) pools[cID] = new ComponentPool(sizeof(T));

			T* component = new (pools[cID]->Add(eID)) T();

			component->entity = &entities[eID];
			component->transform = entities[eID].transform;
			component->valid = true;

			component->Added();

			entities[eID].cMask.set(cID);

			componentAddedEvent.component = (Component*) component;
			componentAddedEvent();

			return component;

		}
		template<typename T> T* GetComponent(uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!entities[eID]) return nullptr;

			int cID = GetCID<T>();
			if (!entities[eID].cMask.test(cID)) return nullptr;

			T* component = static_cast<T*>(pools[cID]->Get(eID));
			return component;

		}
		template<typename T> bool HasComponent(uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return false;
			if (!entities[eID]) return false;

			int cID = GetCID<T>();
			return entities[eID].cMask.test(cID);

		}
		template<typename T> void RemoveComponent(uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return;
			if (!entities[eID]) return;

			int cID = GetCID<T>();
			if (!entities[eID].cMask.test(cID)) return;

			pools[cID]->Remove(eID);
			entities[eID].cMask.reset(cID);

			T* component = static_cast<T*>(pools[cID]->Get(eID));
			componentRemovedEvent.component = (Component*) component;
			componentRemovedEvent();

			component->Removed();
			component->valid = false;

		}

		template<> Collider* AddComponent<Collider>(uint32_t eID);
		template<> BoxCollider* AddComponent<BoxCollider>(uint32_t eID);
		template<> SphereCollider* AddComponent<SphereCollider>(uint32_t eID);
		template<> CapsuleCollider* AddComponent<CapsuleCollider>(uint32_t eID);

		template<> Collider* GetComponent<Collider>(uint32_t eID);
		template<> BoxCollider* GetComponent<BoxCollider>(uint32_t eID);
		template<> SphereCollider* GetComponent<SphereCollider>(uint32_t eID);
		template<> CapsuleCollider* GetComponent<CapsuleCollider>(uint32_t eID);

		template<> bool HasComponent<Collider>(uint32_t eID);
		template<> bool HasComponent<BoxCollider>(uint32_t eID);
		template<> bool HasComponent<SphereCollider>(uint32_t eID);
		template<> bool HasComponent<CapsuleCollider>(uint32_t eID);

		template<> void RemoveComponent<Collider>(uint32_t eID);
		template<> void RemoveComponent<BoxCollider>(uint32_t eID);
		template<> void RemoveComponent<SphereCollider>(uint32_t eID);
		template<> void RemoveComponent<CapsuleCollider>(uint32_t eID);

		void* GetComponent(int componentID, uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!entities[eID]) return nullptr;
			if (!entities[eID].cMask.test(componentID)) return nullptr;

			void* component = static_cast<void*>(pools[componentID]->Get(eID));
			return component;

		}
		bool HasComponent(int componentID, uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return false;
			if (!entities[eID]) return false;

			return entities[eID].cMask.test(componentID);

		}
		void RemoveComponent(int componentID, uint32_t eID) {

			if (eID == INVALID_ENTITY_ID) return;
			if (!entities[eID]) return;
			if (!entities[eID].cMask.test(componentID)) return;

			pools[componentID]->Remove(eID);
			entities[eID].cMask.reset(componentID);

			Component* component = static_cast<Component*>(pools[componentID]->Get(eID));
			componentRemovedEvent.component = component;
			componentRemovedEvent();

			component->Removed();
			component->valid = false;

		}

		void Cleanup() {

			for (ComponentPool* pool : pools) {

				delete pool;

			}

			entities.clear();
			gaps.clear();
			pools.clear();

		}

		template<typename T> static int GetCID() {

			static int cID = cCounter++;
			return cID;

		}

		ComponentPool* GetComponentPool(int cID) {

			if (pools.size() < cID + 1) return nullptr;

			return pools[cID];

		}

	private:
		std::vector<InternalEntity> entities;
		std::vector<ComponentPool*> pools;
		std::vector<uint32_t> gaps;

		template<typename T> T* AddColliderComponent(uint32_t eID, uint8_t type);
		template<typename T> T* GetColliderComponent(uint32_t eID, uint8_t type);
		bool HasColliderComponent(uint32_t eID, uint8_t type);
		template<typename T> void RemoveColliderComponent(uint32_t eID, uint8_t type);

	};

}