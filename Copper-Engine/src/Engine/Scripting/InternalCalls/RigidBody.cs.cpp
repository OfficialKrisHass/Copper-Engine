#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("RigidBody Component Pointer"); return; }
#define CheckComponentPointerWithReturn(componentPointer, ret) if (!componentPointer) { CauseExceptionInvalid("RigidBody Component Pointer"); return ret; }

namespace Copper::Scripting::InternalCalls {

	bool RigidBodyGetIsStatic(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, false);

		return ((RigidBody*) componentPointer)->isStatic;

	}
	bool RigidBodyGetGravity(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, true);

		return ((RigidBody*) componentPointer)->gravity;

	}
	float RigidBodyGetMass(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((RigidBody*) componentPointer)->mass;

	}

	void RigidBodySetIsStatic(int64_t componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->isStatic= value;
		rb->Setup();

	}
	void RigidBodySetGravity(int64_t componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->gravity = value;
		rb->Setup();

	}
	void RigidBodySetMass(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		RigidBody* rb = (RigidBody*) componentPointer;
		rb->mass = value;
		rb->Setup();

	}

	void RigidBodyAddForce(int64_t componentPointer, Vector3* force, uint8_t mode) {

		CheckComponentPointer(componentPointer);

		((RigidBody*) componentPointer)->AddForce(*force, (ForceMode) mode);

	}
	void RigidBodyAddTorque(int64_t componentPointer, Vector3* torque, uint8_t mode) {

		CheckComponentPointer(componentPointer);

		((RigidBody*) componentPointer)->AddTorque(*torque, (ForceMode) mode);

	}

}