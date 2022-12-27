#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scripting/MonoUtils.h"

#include <CopperECS/CopperECS.h>

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	static std::unordered_map<std::string, std::function<bool(const Object&)>> hasComponentFuncs;

	void Initialize() {

		hasComponentFuncs["Transform"] = [](const Object& obj) { return true; };

	}

	//Logging
	static void EditorLog(MonoString* msg) { Log(MonoUtils::MonoToString(msg)); }
	static void EditorLogWarn(MonoString* msg) { LogWarn(MonoUtils::MonoToString(msg)); }
	static void EditorLogError(MonoString* msg) { LogError(MonoUtils::MonoToString(msg)); }

	//Object
	static MonoString* GetObjectName(int objID) {

		const Object& obj = GetScene()->GetObjectFromID(objID);

		return MonoUtils::StringToMono(obj.tag->name);

	}
	static void SetObjectName(int objID, MonoString* out) {

		const Object& obj = GetScene()->GetObjectFromID(objID);

		obj.tag->name = MonoUtils::MonoToString(out);

	}

	//Components
	static bool HasComponent(int objID, MonoReflectionType* type) {

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		Scene* scene = GetScene();
		Object& obj = scene->GetObjectFromID(objID);

		if (hasComponentFuncs.find(typeName) != hasComponentFuncs.end()) {

			return hasComponentFuncs[typeName](obj);

		} else {

			for (ScriptComponent* script : obj.GetComponents<ScriptComponent>()) {

				if (script->name == typeName) return true;

			}

		}

		return false;

	}

	//Transform
	static void GetPosition(int objID, Vector3* out) {

		*out = GetScene()->GetObjectFromID(objID).transform->position;

	}
	static void GetRotation(int objID, Vector3* out) {

		*out = GetScene()->GetObjectFromID(objID).transform->rotation;

	}
	static void GetScale(int objID, Vector3* out) {

		*out = GetScene()->GetObjectFromID(objID).transform->scale;

	}
	static void SetPosition(int objID, Vector3* value) {

		GetScene()->GetObjectFromID(objID).transform->position = *value;

	}
	static void SetRotation(int objID, Vector3* value) {

		GetScene()->GetObjectFromID(objID).transform->rotation = *value;

	}
	static void SetScale(int objID, Vector3* value) {

		GetScene()->GetObjectFromID(objID).transform->scale = *value;

	}

}