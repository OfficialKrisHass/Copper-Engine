#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/BoxCollider.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	bool BoxColliderGetTrigger(uint32_t eID) {

		CheckValidEntityWithReturn(eID, false);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<BoxCollider>()->trigger;

	}
	void BoxColliderGetCenter(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		*out = entity->GetComponent<BoxCollider>()->center;

	}
	void BoxColliderGetSize(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		*out = entity->GetComponent<BoxCollider>()->size;

	}

	void BoxColliderSetTrigger(uint32_t eID, bool value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<BoxCollider>()->trigger = value;

	}
	void BoxColliderSetCenter(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<BoxCollider>()->center = *value;

	}
	void BoxColliderSetSize(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<BoxCollider>()->size = *value;

	}

}