#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/ScriptingCore.h"

#include "Engine/Scene/InternalEntity.h"

#include "Engine/Physics/Raycast.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	bool RaycastFire(Vector3* origin, Vector3* direction, Raycast::Data* hitData, float maxDistance) {

		return Raycast::Fire(*origin, *direction, hitData, maxDistance);;

	}

	MonoObject* RaycastDataGetEntity(InternalEntity* entity) {

		return Scripting::GetScriptEntity(entity->ID());

	}

}