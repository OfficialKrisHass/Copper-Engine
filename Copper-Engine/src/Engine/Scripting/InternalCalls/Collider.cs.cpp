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

	bool ColliderGetTrigger(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, false);

		return ((Collider*) componentPointer)->trigger;

	}
	void ColliderGetCenter(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);

		*out = ((Collider*) componentPointer)->center;

	}

	void ColliderSetTrigger(int64_t componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		((Collider*) componentPointer)->trigger = value;

	}
	void ColliderSetCenter(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		((Collider*) componentPointer)->center = *value;

	}

	void BoxColliderGetSize(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);

		*out = ((BoxCollider*) componentPointer)->size;

	}
	void BoxColliderSetSize(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		((BoxCollider*) componentPointer)->size = *value;

	}

	float SphereColliderGetRadius(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((SphereCollider*) componentPointer)->radius;

	}
	void SphereColliderSetRadius(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((SphereCollider*) componentPointer)->radius = value;

	}

	float CapsuleColliderGetRadius(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((CapsuleCollider*) componentPointer)->radius;

	}
	float CapsuleColliderGetHeight(int64_t componentPointer) {
		
		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((CapsuleCollider*) componentPointer)->height;

	}

	void CapsuleColliderSetRadius(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((CapsuleCollider*) componentPointer)->radius = value;

	}
	void CapsuleColliderSetHeight(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((CapsuleCollider*) componentPointer)->height = value;

	}

}