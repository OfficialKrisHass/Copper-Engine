#include "cupch.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls::Components::RigidBody {

	typedef ::Copper::RigidBody RigidBody;

	bool get_isStatic(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->IsStatic();

	}
	void set_isStatic(MonoObject* rigidBody, bool value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetIsStatic(value);

	}
	bool get_gravity(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->Gravity();

	}
	void set_gravity(MonoObject* rigidBody, bool value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetGravity(value);

	}

	float get_mass(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->Mass();

	}
	void set_mass(MonoObject* rigidBody, float value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetMass(value);

	}

	uint8 get_lockMask(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->LockMask();

	}
	void set_lockMask(MonoObject* rigidBody, uint8 value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetLockMask(value);

	}

	void AddForce(MonoObject* rigidBody, const Vector3* force, uint8 forceMode) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->AddForce(*force, (ForceMode) forceMode);

	}
	void AddTorque(MonoObject* rigidBody, const Vector3* torque, uint8 forceMode) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->AddTorque(*torque, (ForceMode) forceMode);

	}

}