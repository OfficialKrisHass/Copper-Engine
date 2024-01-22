#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("RigidBody Component Pointer"); return; }
#define CheckComponentPointerWithReturn(componentPointer, ret) if (!componentPointer) { CauseExceptionInvalid("RigidBody Component Pointer"); return ret; }

namespace Copper::Scripting::InternalCalls {

	bool RigidBodyGetIsStatic(uint64 componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, false);

		return ((RigidBody*) componentPointer)->isStatic;

	}
	bool RigidBodyGetGravity(uint64 componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, true);

		return ((RigidBody*) componentPointer)->gravity;

	}
	float RigidBodyGetMass(uint64 componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((RigidBody*) componentPointer)->mass;

	}

	void RigidBodySetIsStatic(uint64 componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->isStatic= value;
		rb->Setup();

	}
	void RigidBodySetGravity(uint64 componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->gravity = value;
		rb->Setup();

	}
	void RigidBodySetMass(uint64 componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->mass = value;
		rb->Setup();

	}

	void RigidBodyAddForce(uint64 componentPointer, Vector3* force, uint8_t mode) {

		CheckComponentPointer(componentPointer);

		((RigidBody*) componentPointer)->AddForce(*force, (ForceMode) mode);

	}
	void RigidBodyAddTorque(uint64 componentPointer, Vector3* torque, uint8_t mode) {

		CheckComponentPointer(componentPointer);

		((RigidBody*) componentPointer)->AddTorque(*torque, (ForceMode) mode);

	}

}