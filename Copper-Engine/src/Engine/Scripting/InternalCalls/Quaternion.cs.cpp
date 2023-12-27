#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	Vector3 QuaternionToEulerAngles(Quaternion* quaternion) {

		return quaternion->EulerAngles();

	}
	Quaternion QuaternionFromEulerAngles(Vector3* euler) {

		return Quaternion(*euler);

	}

}