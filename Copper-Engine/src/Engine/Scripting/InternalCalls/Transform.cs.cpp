#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("Transform Component Pointer"); return; }

namespace Copper::Scripting::InternalCalls {

	void GetPosition(uint64 componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->position;

	}
	void GetRotation(uint64 componentPointer, Quaternion* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->rotation;

	}
	void GetScale(uint64 componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->scale;

	}

	void GetForward(uint64 componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Forward();

	}
	void GetRight(uint64 componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Right();

	}
	void GetUp(uint64 componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Up();

	}

	void SetPosition(uint64 componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->position = *value;

		if (!rb) return;
		rb->SetPosition(*value);

	}
	void SetRotation(uint64 componentPointer, Quaternion* value) {

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->rotation = *value;

		if (!rb) return;
		rb->SetRotation(*value);

	}
	void SetScale(uint64 componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);
		((Transform*) componentPointer)->scale = *value;

	}

}