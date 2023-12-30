#include "cupch.h"
#include "Registry.h"

#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"
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

		GetCID<RigidBody>(); // 4

		GetCID<Collider>(); // 5
		GetCID<BoxCollider>(); // 6
		GetCID<SphereCollider>(); // 7
		GetCID<CapsuleCollider>(); // 8

		pools.resize(cCounter, nullptr);

	}

}