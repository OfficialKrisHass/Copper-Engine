#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Scene/InternalEntity.h"

#include "Engine/Physics/Raycast.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	bool RaycastFire(Vector3* origin, Vector3* direction, float maxDistance, Raycast::Data* hitData) {

		const Raycast raycast = Raycast(*origin, *direction, maxDistance);
		const Raycast::Data& data = raycast.GetData();

		if (!data.hit) {

			hitData->hit = false;
			return false;

		}

		*hitData = data;
		return true;

	}

	MonoObject* RaycastDataGetEntity(InternalEntity* entity) {

		return Scripting::GetScriptEntity(entity->ID());

	}

}