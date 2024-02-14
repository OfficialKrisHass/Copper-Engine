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

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Position();

	}
	void GetRotation(uint64 componentPointer, Quaternion* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Rotation();

	}
	void GetScale(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Scale();

	}

	void GetGlobalPosition(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->GlobalPosition();

	}
	void GetGlobalRotation(uint64 componentPointer, Quaternion* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->GlobalRotation();

	}
	void GetGlobalScale(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->GlobalScale();

	}

	void GetForward(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Forward();

	}
	void GetRight(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Right();

	}
	void GetUp(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		*out = ((Transform*) componentPointer)->Up();

	}

	void SetPosition(uint64 componentPointer, Vector3* value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->SetPosition(*value);

		if (!rb) return;
		rb->SetPosition(*value);

	}
	void SetRotation(uint64 componentPointer, Quaternion* value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		Transform* transform = ((Transform*) componentPointer);
		InternalEntity* entity = transform->GetEntity();
		RigidBody* rb = entity->GetComponent<RigidBody>();

		transform->SetRotation(*value);

		if (!rb) return;
		rb->SetRotation(*value);

	}
	void SetScale(uint64 componentPointer, Vector3* value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);
		((Transform*) componentPointer)->SetScale(*value);

	}

}