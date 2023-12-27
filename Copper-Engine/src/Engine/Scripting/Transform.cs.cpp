#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	void GetPosition(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->position;

	}
	void GetRotation(uint32_t eID, Quaternion* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->rotation;

	}
	void GetScale(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->scale;

	}
	void GetForward(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Forward();

	}
	void GetRight(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Right();

	}
	void GetUp(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Up();

	}

	void SetPosition(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->position = *value;

	}
	void SetRotation(uint32_t eID, Quaternion* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->rotation = *value;

	}
	void SetScale(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->scale = *value;

	}

}