#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Collider.h"
#include "Engine/Components/BoxCollider.h"
#include "Engine/Components/SphereCollider.h"
#include "Engine/Components/CapsuleCollider.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("BoxCollider Component Pointer"); return; }
#define CheckComponentPointerWithReturn(componentPointer, ret) if (!componentPointer) { CauseExceptionInvalid("BoxCollider Component Pointer"); return ret; }

namespace Copper::Scripting::InternalCalls {

	bool ColliderGetTrigger(uint64 componentPointer) {

		CUP_FUNCTION();

		CheckComponentPointerWithReturn(componentPointer, false);

		return ((Collider*) componentPointer)->trigger;

	}
	void ColliderGetCenter(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		*out = ((Collider*) componentPointer)->center;

	}

	void ColliderSetTrigger(uint64 componentPointer, bool value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((Collider*) componentPointer)->trigger = value;

	}
	void ColliderSetCenter(uint64 componentPointer, Vector3* value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((Collider*) componentPointer)->center = *value;

	}

	void BoxColliderGetSize(uint64 componentPointer, Vector3* out) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		*out = ((BoxCollider*) componentPointer)->size;

	}
	void BoxColliderSetSize(uint64 componentPointer, Vector3* value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((BoxCollider*) componentPointer)->size = *value;

	}

	float SphereColliderGetRadius(uint64 componentPointer) {

		CUP_FUNCTION();

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((SphereCollider*) componentPointer)->radius;

	}
	void SphereColliderSetRadius(uint64 componentPointer, float value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((SphereCollider*) componentPointer)->radius = value;

	}

	float CapsuleColliderGetRadius(uint64 componentPointer) {

		CUP_FUNCTION();

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((CapsuleCollider*) componentPointer)->radius;

	}
	float CapsuleColliderGetHeight(uint64 componentPointer) {
		
		CUP_FUNCTION();

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((CapsuleCollider*) componentPointer)->height;

	}

	void CapsuleColliderSetRadius(uint64 componentPointer, float value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((CapsuleCollider*) componentPointer)->radius = value;

	}
	void CapsuleColliderSetHeight(uint64 componentPointer, float value) {

		CUP_FUNCTION();

		CheckComponentPointer(componentPointer);

		((CapsuleCollider*) componentPointer)->height = value;

	}

}