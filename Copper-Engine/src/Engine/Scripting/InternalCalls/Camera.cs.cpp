#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	float CameraGetFOV(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->fov;

	}
	float CameraGetNearPlane(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->nearPlane;

	}
	float CameraGetFarPlane(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->farPlane;

	}
	void CameraSetFOV(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->fov = value;

	}
	void CameraSetNearPlane(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->nearPlane = value;

	}
	void CameraSetFarPlane(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->farPlane = value;

	}

}