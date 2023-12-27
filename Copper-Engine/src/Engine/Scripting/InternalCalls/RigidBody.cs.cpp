#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	bool RigidBodyGetIsStatic(uint32_t eID) {

		CheckValidEntityWithReturn(eID, false);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<RigidBody>()->isStatic;

	}
	bool RigidBodyGetGravity(uint32_t eID) {

		CheckValidEntityWithReturn(eID, true);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<RigidBody>()->gravity;

	}
	float RigidBodyGetMass(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<RigidBody>()->mass;

	}

	void RigidBodySetIsStatic(uint32_t eID, bool value) {

		CheckValidEntity(eID);

		RigidBody* rb = GetEntityFromID(eID)->GetComponent<RigidBody>();
		rb->isStatic= value;
		rb->Setup();

	}
	void RigidBodySetGravity(uint32_t eID, bool value) {

		CheckValidEntity(eID);

		RigidBody* rb = GetEntityFromID(eID)->GetComponent<RigidBody>();
		rb->gravity = value;
		rb->Setup();

	}
	void RigidBodySetMass(uint32_t eID, float value) {

		CheckValidEntity(eID);

		RigidBody* rb = GetEntityFromID(eID)->GetComponent<RigidBody>();
		rb->mass = value;
		rb->Setup();

	}

	void RigidBodyAddForce(uint32_t eID, Vector3* force, uint8_t mode) {

		CheckValidEntity(eID);

		GetEntityFromID(eID)->GetComponent<RigidBody>()->AddForce(*force, (ForceMode) mode);

	}
	void RigidBodyAddTorque(uint32_t eID, Vector3* torque, uint8_t mode) {

		CheckValidEntity(eID);

		GetEntityFromID(eID)->GetComponent<RigidBody>()->AddTorque(*torque, (ForceMode) mode);

	}

}