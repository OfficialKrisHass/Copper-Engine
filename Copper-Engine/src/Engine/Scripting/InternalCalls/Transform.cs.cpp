#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/RigidBody.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("Transform Component Pointer"); return; }

namespace Copper::Scripting::InternalCalls {

	void GetPosition(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->position;

	}
	void GetRotation(int64_t componentPointer, Quaternion* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->rotation;

	}
	void GetScale(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->scale;

	}

	void GetForward(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Forward();

	}
	void GetRight(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Right();

	}
	void GetUp(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Up();

	}

	void SetPosition(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->position = *value;

		if (!rb) return;
		rb->SetPosition(*value);

	}
	void SetRotation(int64_t componentPointer, Quaternion* value) {

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->rotation = *value;

		if (!rb) return;
		rb->SetRotation(*value);

	}
	void SetScale(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);
		((Transform*) componentPointer)->scale = *value;

	}

}