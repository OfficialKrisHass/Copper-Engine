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

		int tcid = GetCID<Transform>(); // 0

		GetCID<MeshRenderer>(); // 1
		GetCID<Camera>(); // 2
		GetCID<Light>(); // 3

		GetCID<ScriptComponent>(); // 4

		GetCID<RigidBody>(); // 5

		GetCID<Collider>(); // 6
		GetCID<BoxCollider>(); // 7
		GetCID<SphereCollider>(); // 8
		GetCID<CapsuleCollider>(); // 9

		pools.resize(cCounter, nullptr);

	}

	template<> Collider* Registry::AddComponent<Collider>(uint32_t eID) {

		LogError("Can't add a base Collider component to entity. Entity: {}", entities[eID]);
		return nullptr;

	}
	template<> BoxCollider* Registry::AddComponent<BoxCollider>(uint32_t eID) {

		return AddColliderComponent<BoxCollider>(eID, Collider::Type::Box);

	}
	template<> SphereCollider* Registry::AddComponent<SphereCollider>(uint32_t eID) {

		return AddColliderComponent<SphereCollider>(eID, Collider::Type::Sphere);

	}
	template<> CapsuleCollider* Registry::AddComponent<CapsuleCollider>(uint32_t eID) {

		return AddColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule);

	}

	template<> Collider* Registry::GetComponent<Collider>(uint32_t eID) {

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!entities[eID]) return nullptr;

		int cID = GetCID<Collider>();
		if (!entities[eID].cMask.test(cID)) return nullptr;

		uint8_t type = *(uint8_t*) pools[cID]->Get(eID);
		Collider* component = static_cast<Collider*>(pools[cID + type]->Get(eID));
		return component;

	}
	template<> BoxCollider* Registry::GetComponent<BoxCollider>(uint32_t eID) {

		return GetColliderComponent<BoxCollider>(eID, Collider::Type::Box);

	}
	template<> SphereCollider* Registry::GetComponent<SphereCollider>(uint32_t eID) {

		return GetColliderComponent<SphereCollider>(eID, Collider::Type::Sphere);

	}
	template<> CapsuleCollider* Registry::GetComponent<CapsuleCollider>(uint32_t eID) {

		return GetColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule);

	}

	template<> bool Registry::HasComponent<Collider>(uint32_t eID) {

		return HasColliderComponent(eID, 0);

	}
	template<> bool Registry::HasComponent<BoxCollider>(uint32_t eID) {

		return HasColliderComponent(eID, Collider::Type::Box);

	}
	template<> bool Registry::HasComponent<SphereCollider>(uint32_t eID) {

		return HasColliderComponent(eID, Collider::Type::Sphere);

	}
	template<> bool Registry::HasComponent<CapsuleCollider>(uint32_t eID) {

		return HasColliderComponent(eID, Collider::Type::Capsule);

	}

	template<> void Registry::RemoveComponent<Collider>(uint32_t eID) {

		if (eID == INVALID_ENTITY_ID) return;
		if (!entities[eID]) return;

		int cID = GetCID<Collider>();
		if (!entities[eID].cMask.test(cID)) return;

		uint8_t type = *(uint8_t*) pools[cID]->Get(eID);

		pools[cID + type]->Remove(eID);
		entities[eID].cMask.reset(cID + type);

		pools[cID]->Remove(eID);
		entities[eID].cMask.reset(cID);

		Collider* component = static_cast<Collider*>(pools[cID + type]->Get(eID));
		componentRemovedEvent.component = (Component*) component;
		componentRemovedEvent();

		component->Removed();
		component->valid = false;

	}
	template<> void Registry::RemoveComponent<BoxCollider>(uint32_t eID) {

		RemoveColliderComponent<BoxCollider>(eID, Collider::Type::Box);

	}
	template<> void Registry::RemoveComponent<SphereCollider>(uint32_t eID) {

		RemoveColliderComponent<BoxCollider>(eID, Collider::Type::Sphere);

	}
	template<> void Registry::RemoveComponent<CapsuleCollider>(uint32_t eID) {

		RemoveColliderComponent<CapsuleCollider>(eID, Collider::Type::Capsule);

	}

	template<typename T> T* Registry::AddColliderComponent(uint32_t eID, uint8_t type) {

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!entities[eID]) return nullptr;

		int cID = GetCID<Collider>();

	#ifdef CU_DEBUG
		if (pools.size() < cID + COLLIDER_TYPES + 1) pools.resize(cID + COLLIDER_TYPES + 1, nullptr);
	#endif
		if (!pools[cID]) pools[cID] = new ComponentPool(sizeof(Collider::Type));
		if (!pools[cID + type]) pools[cID + type] = new ComponentPool(sizeof(T));

		*(uint8_t*) pools[cID]->Add(eID) = (Collider::Type) type;
		T* component = new (pools[cID + type]->Add(eID)) T();

		component->entity = &entities[eID];
		component->transform = entities[eID].transform;
		component->valid = true;

		component->type = (Collider::Type) type;

		component->Added();

		entities[eID].cMask.set(cID);
		entities[eID].cMask.set(cID + type);

		componentAddedEvent.component = (Component*) component;
		componentAddedEvent();

		return component;

	}
	template<typename T> T* Registry::GetColliderComponent(uint32_t eID, uint8_t type) {

		if (eID == INVALID_ENTITY_ID) return nullptr;
		if (!entities[eID]) return nullptr;

		int cID = GetCID<Collider>();
		if (!entities[eID].cMask.test(cID + type)) return nullptr;

		T* component = static_cast<T*>(pools[cID + type]->Get(eID));
		return component;

	}
	bool Registry::HasColliderComponent(uint32_t eID, uint8_t type) {

		if (eID == INVALID_ENTITY_ID) return false;
		if (!entities[eID]) return false;

		int cID = GetCID<Collider>();
		return entities[eID].cMask.test(cID + type);

	}
	template<typename T> void Registry::RemoveColliderComponent(uint32_t eID, uint8_t type) {

		if (eID == INVALID_ENTITY_ID) return;
		if (!entities[eID]) return;

		int cID = GetCID<Collider>();
		if (!entities[eID].cMask.test(cID + type)) return;

		pools[cID + type]->Remove(eID);
		entities[eID].cMask.reset(cID + type);

		pools[cID]->Remove(eID);
		entities[eID].cMask.reset(cID);

		T* component = static_cast<T*>(pools[cID + type]->Get(eID));
		componentRemovedEvent.component = (Component*) component;
		componentRemovedEvent();

		component->Removed();
		component->valid = false;

	}

}