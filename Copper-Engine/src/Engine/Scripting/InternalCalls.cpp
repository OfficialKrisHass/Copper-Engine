#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scripting/MonoUtils.h"

#include <CopperECS/CopperECS.h>

#include <mono/jit/jit.h>

namespace Copper::Scripting::InternalCalls {

	static std::unordered_map<std::string, std::function<bool(const Object&)>> hasComponentFuncs;
	static std::unordered_map<std::string, std::function<void(Object&)>> addComponentFuncs;

	void Initialize() {

		hasComponentFuncs["Transform"] = [](const Object& obj) { return true; };
		hasComponentFuncs["Camera"] =    [](const Object& obj) { return obj.HasComponent<Camera>(); };

		addComponentFuncs["Camera"] = [](Object& obj) { obj.AddComponent<Camera>(); };

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
	static void AddComponent(int objID, MonoReflectionType* type, MonoObject* ret) {

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		Object& obj = GetObjectFromID(objID);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(managedType), "objID");
		mono_field_set_value(ret, field, &objID);

		if (addComponentFuncs.find(scriptName) != addComponentFuncs.end()) {

			addComponentFuncs[scriptName](obj);

		} else {

			ScriptComponent* script = obj.AddComponent<ScriptComponent>();
			script->Init(obj.GetID(), typeName);

		}

	}
	static bool GetComponent(int objID, MonoReflectionType* type, MonoObject* ret) {

		Object& obj = GetObjectFromID(objID);

		MonoType* classType = mono_reflection_type_get_type(type);
		std::string name = mono_type_get_name(classType);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "objID");
		mono_field_set_value(ret, field, &objID);

		ScriptComponent* script = nullptr;
		for (ScriptComponent* s : obj.GetComponents<ScriptComponent>()) {

			if (s->name == name) script = s;

		}
		if (!script) return false;

		script->CopyTo(ret);

		return true;

	}
	static bool HasComponent(int objID, MonoReflectionType* type) {

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		Object& obj = GetObjectFromID(objID);

		if (hasComponentFuncs.find(scriptName) != hasComponentFuncs.end()) {

			return hasComponentFuncs[scriptName](obj);

		} else {

			for (ScriptComponent* script : obj.GetComponents<ScriptComponent>()) {

				if (script->name == typeName) return true;

			}

		}

		return false;

	}

	static void SetComponentObjID(MonoReflectionType* type, MonoObject* component, int objID) {

		MonoType* classType = mono_reflection_type_get_type(type);
		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "objID");
		mono_field_set_value(component, field, &objID);

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

	//Camera
	static float CameraGetFOV(int objID) {

		Object& obj = GetObjectFromID(objID);
		return obj.GetComponent<Camera>()->fov;

	}
	static float CameraGetNearPlane(int objID) {

		Object& obj = GetObjectFromID(objID);
		return obj.GetComponent<Camera>()->nearPlane;

	}
	static float CameraGetFarPlane(int objID) {

		Object& obj = GetObjectFromID(objID);
		return obj.GetComponent<Camera>()->farPlane;

	}
	static void CameraSetFOV(int objID, float value) {

		Object& obj = GetObjectFromID(objID);
		obj.GetComponent<Camera>()->fov = value;

	}
	static void CameraSetNearPlane(int objID, float value) {

		Object& obj = GetObjectFromID(objID);
		obj.GetComponent<Camera>()->nearPlane = value;

	}
	static void CameraSetFarPlane(int objID, float value) {

		Object& obj = GetObjectFromID(objID);
		obj.GetComponent<Camera>()->farPlane = value;

	}

}