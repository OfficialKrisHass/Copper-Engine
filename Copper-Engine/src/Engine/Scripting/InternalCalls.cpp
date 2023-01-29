#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include <CopperECS/CopperECS.h>

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CauseExceptionInvalid(argument) mono_raise_exception(mono_get_exception_argument_null(argument))
#define CauseException(message, argument)  mono_raise_exception(mono_get_exception_argument(argument, message))

namespace Copper::Scripting::InternalCalls {

	static std::unordered_map<std::string, std::function<bool(const Object&)>> hasComponentFuncs;
	static std::unordered_map<std::string, std::function<bool(Object&)>> addComponentFuncs;

	void Initialize() {

		hasComponentFuncs["Transform"] = [](const Object& obj) { return true; };
		hasComponentFuncs["Camera"] =    [](const Object& obj) { return obj.HasComponent<Camera>(); };

		addComponentFuncs["Camera"] = [](Object& obj) { return obj.AddComponent<Camera>() != nullptr ? true : false; };

	}

	//Logging
	static void EditorLog(MonoString* msg) {
		
		Log(MonoUtils::MonoToString(msg));

	}
	static void EditorLogWarn(MonoString* msg) {
		
		LogWarn(MonoUtils::MonoToString(msg));
	
	}
	static void EditorLogError(MonoString* msg) {
		
		LogError(MonoUtils::MonoToString(msg));

	}

	//Input
	static bool IsKey(int keyCode) { return Input::IsKey((KeyCode) keyCode); }
	static bool IsKeyDown(int keyCode) { return Input::IsKeyDown((KeyCode) keyCode); }
	static bool IsKeyReleased(int keyCode) { return Input::IsKeyReleased((KeyCode) keyCode); }

	static float GetAxis(MonoString* axisName) {

		return Input::GetAxis(MonoUtils::MonoToString(axisName));

	}

	//Object
	static MonoString* GetObjectName(int objID) {

		const Object& obj = GetScene()->GetObjectFromID(objID);

		return MonoUtils::StringToMono(obj.tag->name);

	}
	static void SetObjectName(int objID, MonoString* out) {

		const Object& obj = GetScene()->GetObjectFromID(objID);

		obj.tag->name = MonoUtils::MonoToString(out);

	}

	static MonoObject* GetCopperObject(int objID) {

		MonoObject* ret = mono_object_new(Scripting::GetAppDomain(), Scripting::GetCopperObjectMonoClass());
		mono_runtime_object_init(ret);

		MonoClassField* objIDField = mono_class_get_field_from_name(Scripting::GetCopperObjectMonoClass(), "objID");
		mono_field_set_value(ret, objIDField, &objID);

		return ret;

	}

	//Components
	static void AddComponent(int objID, MonoReflectionType* type, MonoObject* ret) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		Object& obj = GetObjectFromID(objID);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(managedType), "objID");
		mono_field_set_value(ret, field, &objID);

		if (addComponentFuncs.find(scriptName) != addComponentFuncs.end()) {

			if (addComponentFuncs[scriptName](obj)) return;
			CauseException("Only one component of this type can be on a single object. Type", scriptName.c_str());
			
		} else {

			ScriptComponent* script = obj.AddComponent<ScriptComponent>();
			script->Init(obj.GetID(), typeName);

		}

	}
	static bool GetComponent(int objID, MonoReflectionType* type, MonoObject* ret) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return false; }

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

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return false; }

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

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetScene()->GetObjectFromID(objID).transform->position;

	}
	static void GetRotation(int objID, Vector3* out) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetScene()->GetObjectFromID(objID).transform->rotation;

	}
	static void GetScale(int objID, Vector3* out) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetScene()->GetObjectFromID(objID).transform->scale;

	}
	static void SetPosition(int objID, Vector3* value) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		GetScene()->GetObjectFromID(objID).transform->position = *value;

	}
	static void SetRotation(int objID, Vector3* value) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		GetScene()->GetObjectFromID(objID).transform->rotation = *value;

	}
	static void SetScale(int objID, Vector3* value) {

		if (objID == -1) { CauseExceptionInvalid("Copper Object"); return; }
		GetScene()->GetObjectFromID(objID).transform->scale = *value;

	}

	static void GetForward(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->forward; }
	static void GetBack(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->back; }
	static void GetRight(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->right; }
	static void GetLeft(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->left; }
	static void GetUp(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->up; }
	static void GetDown(int objID, Vector3* out) { *out = GetObjectFromID(objID).transform->down; }

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