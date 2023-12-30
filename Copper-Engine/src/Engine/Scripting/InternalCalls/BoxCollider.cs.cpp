#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/BoxCollider.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("BoxCollider Component Pointer"); return; }
#define CheckComponentPointerWithReturn(componentPointer, ret) if (!componentPointer) { CauseExceptionInvalid("BoxCollider Component Pointer"); return ret; }

namespace Copper::Scripting::InternalCalls {

	bool BoxColliderGetTrigger(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, false);

		return ((BoxCollider*) componentPointer)->trigger;

	}
	void BoxColliderGetCenter(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);

		*out = ((BoxCollider*) componentPointer)->center;

	}
	void BoxColliderGetSize(int64_t componentPointer, Vector3* out) {

		CheckComponentPointer(componentPointer);

		*out = ((BoxCollider*) componentPointer)->size;

	}

	void BoxColliderSetTrigger(int64_t componentPointer, bool value) {

		CheckComponentPointer(componentPointer);

		((BoxCollider*) componentPointer)->trigger = value;

	}
	void BoxColliderSetCenter(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		((BoxCollider*) componentPointer)->center = *value;

	}
	void BoxColliderSetSize(int64_t componentPointer, Vector3* value) {

		CheckComponentPointer(componentPointer);

		((BoxCollider*) componentPointer)->size = *value;

	}

}