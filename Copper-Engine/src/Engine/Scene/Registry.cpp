#include "cupch.h"
#include "Registry.h"

#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Components/RigidBody.h"

#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

namespace Copper {

	EntityEvent entityCreatedEvent;
	EntityEvent entityRemovedEvent;
	ComponentEvent componentAddedEvent;
	ComponentEvent componentRemovedEvent;
	
	void AddEntityCreatedEventFunc(std::function<bool(const Event&)> func) { entityCreatedEvent += func; }
	void AddEntityRemovedEventFunc(std::function<bool(const Event&)> func) { entityRemovedEvent += func; }
	void AddComponentAddedEventFunc(std::function<bool(const Event&)> func) { componentAddedEvent += func; }
	void AddComponentRemovedEventFunc(std::function<bool(const Event&)> func) { componentRemovedEvent += func; }

	void Registry::Initialize() {

		CUP_FUNCTION();

		int32 tcid = GetCID<Transform>(); // 0

		GetCID<MeshRenderer>(); // 1
		GetCID<Camera>(); // 2
		GetCID<Light>(); // 3

		GetCID<ScriptComponent>(); // 4

		GetCID<RigidBody>(); // 5

		GetCID<Collider>(); // 6
		GetCID<BoxCollider>(); // 7
		GetCID<SphereCollider>(); // 8
		GetCID<CapsuleCollider>(); // 9

		m_pools.resize(cCounter, nullptr);

	}

	template<> Collider* Registry::AddComponent<Collider>(uint32 eID) {

		CUP_FUNCTION();

		LogError("Can't add a base Collider component to entity. Entity: {}", m_entities[eID]);
		return nullptr;

	}
	template<> BoxCollider* Registry::AddComponent<BoxCollider>(uint32 eID) { CUP_FUNCTION(); return AddColliderComponent<BoxCollider>(eID, Collider::Type::Box); }
	template<> SphereCollider* Registry::AddComponent<SphereCollider>(uint32 eID) { CUP_FUNCTION(); return AddColliderComponent<SphereCollider>(eID, Collider::Type::Sphere); }
	template<> CapsuleCollider* Registry::AddComponent<CapsuleCollider>(uint32 eID) { CUP_FUNCTION(); return AddColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule); }

	template<> Collider* Registry::GetComponent<Collider>(uint32 eID) {

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!m_entities[eID]) return nullptr;

		int cID = GetCID<Collider>();
		if (!m_entities[eID].m_cMask.test(cID)) return nullptr;

		uint8_t type = *(uint8_t*) m_pools[cID]->Get(eID);
		Collider* component = static_cast<Collider*>(m_pools[cID + type]->Get(eID));
		return component;

	}
	template<> BoxCollider* Registry::GetComponent<BoxCollider>(uint32 eID) { CUP_FUNCTION(); return GetColliderComponent<BoxCollider>(eID, Collider::Type::Box); }
	template<> SphereCollider* Registry::GetComponent<SphereCollider>(uint32 eID) { CUP_FUNCTION(); return GetColliderComponent<SphereCollider>(eID, Collider::Type::Sphere); }
	template<> CapsuleCollider* Registry::GetComponent<CapsuleCollider>(uint32 eID) { CUP_FUNCTION(); return GetColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule); }

	template<> bool Registry::HasComponent<Collider>(uint32 eID) { CUP_FUNCTION(); return HasColliderComponent(eID, 0); }
	template<> bool Registry::HasComponent<BoxCollider>(uint32 eID) { CUP_FUNCTION(); return HasColliderComponent(eID, Collider::Type::Box); }
	template<> bool Registry::HasComponent<SphereCollider>(uint32 eID) { CUP_FUNCTION(); return HasColliderComponent(eID, Collider::Type::Sphere); }
	template<> bool Registry::HasComponent<CapsuleCollider>(uint32 eID) { CUP_FUNCTION(); return HasColliderComponent(eID, Collider::Type::Capsule); }

	template<> void Registry::RemoveComponent<Collider>(uint32 eID) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID) return;
		if (!m_entities[eID]) return;

		int32 cID = GetCID<Collider>();
		if (!m_entities[eID].m_cMask.test(cID)) return;

		uint8 type = *(uint8*) m_pools[cID]->Get(eID);

		m_pools[cID + type]->Remove(eID);
		m_entities[eID].m_cMask.reset(cID + type);

		m_pools[cID]->Remove(eID);
		m_entities[eID].m_cMask.reset(cID);

		Collider* component = static_cast<Collider*>(m_pools[cID + type]->Get(eID));
		componentRemovedEvent.component = (Component*) component;
		componentRemovedEvent();

		component->m_valid = false;

	}
	template<> void Registry::RemoveComponent<BoxCollider>(uint32 eID) { CUP_FUNCTION(); RemoveColliderComponent<BoxCollider>(eID, Collider::Type::Box); }
	template<> void Registry::RemoveComponent<SphereCollider>(uint32 eID) { CUP_FUNCTION(); RemoveColliderComponent<BoxCollider>(eID, Collider::Type::Sphere); }
	template<> void Registry::RemoveComponent<CapsuleCollider>(uint32 eID) { CUP_FUNCTION(); RemoveColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule); }

	template<typename T> T* Registry::AddColliderComponent(uint32 eID, uint8 type) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!m_entities[eID]) return nullptr;

		int32 cID = GetCID<Collider>();

	#ifdef CU_DEBUG
		if (m_pools.size() < cID + COLLIDER_TYPES + 1) m_pools.resize(cID + COLLIDER_TYPES + 1, nullptr);
	#endif
		if (!m_pools[cID]) m_pools[cID] = new ComponentPool(sizeof(Collider::Type));
		if (!m_pools[cID + type]) m_pools[cID + type] = new ComponentPool(sizeof(T));

		*(uint8*) m_pools[cID]->Add(eID) = (Collider::Type) type;
		T* component = new (m_pools[cID + type]->Add(eID)) T();

		component->m_entity = &m_entities[eID];
		component->m_transform = m_entities[eID].m_transform;
		component->m_valid = true;

		component->m_type = (Collider::Type) type;

		m_entities[eID].m_cMask.set(cID);
		m_entities[eID].m_cMask.set(cID + type);

		componentAddedEvent.component = (Component*) component;
		componentAddedEvent();

		return component;

	}
	template<typename T> T* Registry::GetColliderComponent(uint32 eID, uint8 type) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!m_entities[eID]) return nullptr;

		int32 cID = GetCID<Collider>();
		if (!m_entities[eID].m_cMask.test(cID + type)) return nullptr;

		T* component = static_cast<T*>(m_pools[cID + type]->Get(eID));
		return component;

	}
	bool Registry::HasColliderComponent(uint32 eID, uint8 type) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID) return false;
		if (!m_entities[eID]) return false;

		int32 cID = GetCID<Collider>();
		return m_entities[eID].m_cMask.test(cID + type);

	}
	template<typename T> void Registry::RemoveColliderComponent(uint32 eID, uint8 type) {

		CUP_FUNCTION();

		if (eID == INVALID_ENTITY_ID) return;
		if (!m_entities[eID]) return;

		int32 cID = GetCID<Collider>();
		if (!m_entities[eID].m_cMask.test(cID + type)) return;

		m_pools[cID + type]->Remove(eID);
		m_entities[eID].m_cMask.reset(cID + type);

		m_pools[cID]->Remove(eID);
		m_entities[eID].m_cMask.reset(cID);

		T* component = static_cast<T*>(m_pools[cID + type]->Get(eID));
		componentRemovedEvent.component = (Component*) component;
		componentRemovedEvent();

		component->m_valid = false;

	}

}