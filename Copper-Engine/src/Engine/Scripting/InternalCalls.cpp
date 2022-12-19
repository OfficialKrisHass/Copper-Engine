#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scripting/MonoUtils.h"

#include <CopperECS/CopperECS.h>

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	//Logging
	static void EditorLog(MonoString* msg) { Log(MonoUtils::MonoToString(msg)); }
	static void EditorLogWarn(MonoString* msg) { LogWarn(MonoUtils::MonoToString(msg)); }
	static void EditorLogError(MonoString* msg) { LogError(MonoUtils::MonoToString(msg)); }

	//Object
	static MonoString* GetObjectName(int objID) {

		const Object& obj = GetScene()->registry.GetObjectFromID(objID);

		return MonoUtils::StringToMono(obj.tag->name);

	}
	static void SetObjectName(int objID, MonoString* out) {

		const Object& obj = GetScene()->registry.GetObjectFromID(objID);

		obj.tag->name = MonoUtils::MonoToString(out);

	}

	//Transform
	static void GetPosition(int objID, Vector3* out) {

		*out = GetScene()->registry.GetObjectFromID(objID).transform->position;

	}
	static void GetRotation(int objID, Vector3* out) {

		*out = GetScene()->registry.GetObjectFromID(objID).transform->rotation;

	}
	static void GetScale(int objID, Vector3* out) {

		*out = GetScene()->registry.GetObjectFromID(objID).transform->scale;

	}
	static void SetPosition(int objID, Vector3* value) {

		GetScene()->registry.GetObjectFromID(objID).transform->position = *value;

	}
	static void SetRotation(int objID, Vector3* value) {

		GetScene()->registry.GetObjectFromID(objID).transform->rotation = *value;

	}
	static void SetScale(int objID, Vector3* value) {

		GetScene()->registry.GetObjectFromID(objID).transform->scale = *value;

	}

}