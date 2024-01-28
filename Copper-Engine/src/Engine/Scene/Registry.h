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
	extern uint32 cCounter;

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
			ComponentPool(uint32 size) : m_cSize(size), m_data(new char[size * MAX_ENTITY_COMPONENTS]) {}
			~ComponentPool() {

				delete[] m_data;

			}

			void* Add(uint32 index) { m_validComponents.set(index); m_count++; return Get(index); }
			void* Get(uint32 index) { return m_data + index * m_cSize; }
			void Remove(uint32 index) { m_validComponents.reset(index); m_count--; }

			bool Valid(uint32 index) { return m_validComponents.test(index); }
			uint32 Count() { return m_count; }

		private:
			char* m_data = nullptr;
			std::bitset<MAX_ENTITY_COMPONENTS> m_validComponents;

			uint32 m_cSize;
			uint32 m_count = 0;

		};

	public:
		void Initialize();

		InternalEntity* CreateEntity(Scene* scene, Vector3 position, Quaternion rotation, Vector3 scale, const std::string& name) {

			uint32 id;
			if (m_gaps.empty()) {

				m_entities.push_back(InternalEntity());
				id = (uint32) m_entities.size() - 1;

			} else {

				id = m_gaps.back();
				m_gaps.pop_back();

			}

			m_entities[id].m_id = id;
			m_entities[id].name = name;
			m_entities[id].m_scene = scene;
			
			m_entities[id].m_transform = AddComponent<Transform>(id);
			m_entities[id].m_transform->m_position = position;
			m_entities[id].m_transform->m_rotation = rotation;
			m_entities[id].m_transform->m_scale = scale;

			entityCreatedEvent.entity = &m_entities[id];
			entityCreatedEvent();

			return &m_entities[id];

		}
		InternalEntity* CreateEntityFromID(uint32 eID, Scene* scene, Vector3 position, Quaternion rotation, Vector3 scale, const std::string& name, bool returnIfExists) {

			if (eID >= m_entities.size()) m_entities.resize(eID + 1, InternalEntity());
			if (returnIfExists && m_entities[eID]) return &m_entities[eID];

			bool newEntity = !m_entities[eID];

			m_entities[eID].m_id = eID;
			m_entities[eID].name = name;
			m_entities[eID].m_scene = scene;

			if (!m_entities[eID].m_transform) m_entities[eID].m_transform = AddComponent<Transform>(eID);
			m_entities[eID].m_transform->m_position = position;
			m_entities[eID].m_transform->m_rotation = rotation;
			m_entities[eID].m_transform->m_scale = scale;

			if (newEntity) {

				entityCreatedEvent.entity = &m_entities[eID];
				entityCreatedEvent();

			}

			return &m_entities[eID];

		}
		InternalEntity* GetEntityFromID(uint32 eID) {

			if (eID == INVALID_ENTITY_ID || eID >= m_entities.size() || !m_entities[eID]) return nullptr;
			return &m_entities[eID];

		}
		void RemoveEntity(uint32 eID) {

			entityRemovedEvent.entity = &m_entities[eID];
			entityRemovedEvent();

			//Update the Parent
			if (Transform* parent = m_entities[eID].m_transform->m_parent) {

				parent->RemoveChild(m_entities[eID].m_transform);

			}

			//Update the Children
			for (uint32 childID : m_entities[eID].m_transform->m_children) {

				RemoveEntity(childID);

			}
			m_entities[eID].m_transform->m_children.clear();

			m_entities[eID].Invalidate();
			m_gaps.push_back(eID);

			for (ComponentPool* pool : m_pools) {

				if (!pool || !pool->Valid(eID)) continue;
				pool->Remove(eID);

			}

		}

		template<typename T> T* AddComponent(uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!m_entities[eID]) return nullptr;

			int32 cID = GetCID<T>();

		#ifdef CU_DEBUG
			if (m_pools.size() < cID + 1) m_pools.resize(cID + 1, nullptr);
		#endif
			if (!m_pools[cID]) m_pools[cID] = new ComponentPool(sizeof(T));

			T* component = new (m_pools[cID]->Add(eID)) T();

			component->m_entity = &m_entities[eID];
			component->m_transform = m_entities[eID].m_transform;
			component->m_valid = true;

			m_entities[eID].m_cMask.set(cID);

			componentAddedEvent.component = (Component*) component;
			componentAddedEvent();

			return component;

		}
		template<typename T> T* GetComponent(uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!m_entities[eID]) return nullptr;

			int32 cID = GetCID<T>();
			if (!m_entities[eID].m_cMask.test(cID)) return nullptr;

			T* component = static_cast<T*>(m_pools[cID]->Get(eID));
			return component;

		}
		template<typename T> bool HasComponent(uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return false;
			if (!m_entities[eID]) return false;

			int32 cID = GetCID<T>();
			return m_entities[eID].m_cMask.test(cID);

		}
		template<typename T> void RemoveComponent(uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return;
			if (!m_entities[eID]) return;

			int32 cID = GetCID<T>();
			if (!m_entities[eID].m_cMask.test(cID)) return;

			m_pools[cID]->Remove(eID);
			m_entities[eID].m_cMask.reset(cID);

			T* component = static_cast<T*>(m_pools[cID]->Get(eID));
			componentRemovedEvent.component = (Component*) component;
			componentRemovedEvent();

			component->m_valid = false;

		}

		template<> Collider* AddComponent<Collider>(uint32 eID);
		template<> BoxCollider* AddComponent<BoxCollider>(uint32 eID);
		template<> SphereCollider* AddComponent<SphereCollider>(uint32 eID);
		template<> CapsuleCollider* AddComponent<CapsuleCollider>(uint32 eID);

		template<> Collider* GetComponent<Collider>(uint32 eID);
		template<> BoxCollider* GetComponent<BoxCollider>(uint32 eID);
		template<> SphereCollider* GetComponent<SphereCollider>(uint32 eID);
		template<> CapsuleCollider* GetComponent<CapsuleCollider>(uint32 eID);

		template<> bool HasComponent<Collider>(uint32 eID);
		template<> bool HasComponent<BoxCollider>(uint32 eID);
		template<> bool HasComponent<SphereCollider>(uint32 eID);
		template<> bool HasComponent<CapsuleCollider>(uint32 eID);

		template<> void RemoveComponent<Collider>(uint32 eID);
		template<> void RemoveComponent<BoxCollider>(uint32 eID);
		template<> void RemoveComponent<SphereCollider>(uint32 eID);
		template<> void RemoveComponent<CapsuleCollider>(uint32 eID);

		void* GetComponent(uint32 componentID, uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return nullptr;
			if (!m_entities[eID]) return nullptr;
			if (!m_entities[eID].m_cMask.test(componentID)) return nullptr;

			void* component = static_cast<void*>(m_pools[componentID]->Get(eID));
			return component;

		}
		bool HasComponent(uint32 componentID, uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return false;
			if (!m_entities[eID]) return false;

			return m_entities[eID].m_cMask.test(componentID);

		}
		void RemoveComponent(uint32 componentID, uint32 eID) {

			if (eID == INVALID_ENTITY_ID) return;
			if (!m_entities[eID]) return;
			if (!m_entities[eID].m_cMask.test(componentID)) return;

			m_pools[componentID]->Remove(eID);
			m_entities[eID].m_cMask.reset(componentID);

			Component* component = static_cast<Component*>(m_pools[componentID]->Get(eID));
			componentRemovedEvent.component = component;
			componentRemovedEvent();

			component->m_valid = false;

		}

		void Cleanup() {

			for (ComponentPool* pool : m_pools) {

				delete pool;

			}

			m_entities.clear();
			m_gaps.clear();
			m_pools.clear();

		}

		template<typename T> static uint32 GetCID() {

			static int32 cID = cCounter++;
			return cID;

		}

		ComponentPool* GetComponentPool(uint32 cID) {
			
			if (m_pools.size() < cID + 1) return nullptr;

			return m_pools[cID];

		}

	private:
		std::vector<InternalEntity> m_entities;
		std::vector<ComponentPool*> m_pools;
		std::vector<uint32> m_gaps;

		template<typename T> T* AddColliderComponent(uint32 eID, uint8 type);
		template<typename T> T* GetColliderComponent(uint32 eID, uint8 type);
		bool HasColliderComponent(uint32 eID, uint8 type);
		template<typename T> void RemoveColliderComponent(uint32 eID, uint8 type);

	};

}