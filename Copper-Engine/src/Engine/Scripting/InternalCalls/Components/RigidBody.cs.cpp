#include "cupch.h"
#include "RigidBody.cs.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::RigidBody {

	typedef ::Copper::RigidBody RigidBody;

	bool GetIsStatic(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->IsStatic();

	}
	void SetIsStatic(MonoObject* rigidBody, bool value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetIsStatic(value);

	}
	bool GetGravity(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->Gravity();

	}
	void SetGravity(MonoObject* rigidBody, bool value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetGravity(value);

	}

	float GetMass(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->Mass();

	}
	void SetMass(MonoObject* rigidBody, float value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetMass(value);

	}

	uint8 GetLockMask(MonoObject* rigidBody) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		return ptr->LockMask();

	}
	void SetLockMask(MonoObject* rigidBody, uint8 value) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->SetLockMask(value);

	}

	void AddForce(MonoObject* rigidBody, const Vector3& force, uint8 forceMode) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->AddForce(force, (ForceMode) forceMode);

	}
	void AddTorque(MonoObject* rigidBody, const Vector3& torque, uint8 forceMode) {

		CUP_FUNCTION();

		GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
		ptr->AddTorque(torque, (ForceMode) forceMode);

	}

}
