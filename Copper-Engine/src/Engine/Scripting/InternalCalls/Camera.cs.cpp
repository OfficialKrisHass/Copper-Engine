#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CheckComponentPointer(componentPointer) if (!componentPointer) { CauseExceptionInvalid("Camera Component Pointer"); return; }
#define CheckComponentPointerWithReturn(componentPointer, ret) if (!componentPointer) { CauseExceptionInvalid("Camera Component Pointer"); return ret; }

namespace Copper::Scripting::InternalCalls {

	float CameraGetFOV(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((Camera*) componentPointer)->fov;

	}
	float CameraGetNearPlane(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((Camera*) componentPointer)->nearPlane;

	}
	float CameraGetFarPlane(int64_t componentPointer) {

		CheckComponentPointerWithReturn(componentPointer, 0.0f);

		return ((Camera*) componentPointer)->farPlane;

	}
	void CameraSetFOV(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((Camera*) componentPointer)->fov = value;

	}
	void CameraSetNearPlane(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((Camera*) componentPointer)->nearPlane = value;

	}
	void CameraSetFarPlane(int64_t componentPointer, float value) {

		CheckComponentPointer(componentPointer);

		((Camera*) componentPointer)->farPlane = value;

	}

}