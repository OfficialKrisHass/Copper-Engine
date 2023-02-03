#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/AxisManager.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/ScriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CauseExceptionInvalid(argument) mono_raise_exception(mono_get_exception_argument_null(argument))
#define CauseException(message, argument)  mono_raise_exception(mono_get_exception_argument(argument, message))

namespace Copper::Scripting::InternalCalls {

	static std::unordered_map<std::string, std::function<bool(InternalEntity*)>> hasComponentFuncs;
	static std::unordered_map<std::string, std::function<bool(InternalEntity*)>> addComponentFuncs;

	void Initialize() {

		hasComponentFuncs["Transform"] = [](InternalEntity* entity) { return true; };
		hasComponentFuncs["Camera"] =    [](InternalEntity* entity) { return entity->HasComponent<Camera>(); };

		addComponentFuncs["Camera"] = [](InternalEntity* entity) { return entity->AddComponent<Camera>() != nullptr ? true : false; };

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

	//Entity
	static MonoString* GetObjectName(int eID) {

		InternalEntity* entity = GetEntityFromID(eID);

		return MonoUtils::StringToMono(entity->name);

	}
	static void SetObjectName(int eID, MonoString* out) {

		InternalEntity* entity = GetEntityFromID(eID);

		entity->name = MonoUtils::MonoToString(out);

	}

	static MonoObject* GetCopperObject(int eID) {

		MonoObject* ret = mono_object_new(Scripting::GetAppDomain(), Scripting::GetCopperObjectMonoClass());
		mono_runtime_object_init(ret);

		MonoClassField* objIDField = mono_class_get_field_from_name(Scripting::GetCopperObjectMonoClass(), "objID");
		mono_field_set_value(ret, objIDField, &eID);

		return ret;

	}

	//Components
	static void AddComponent(int eID, MonoReflectionType* type, MonoObject* ret) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		InternalEntity* entity = GetEntityFromID(eID);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(managedType), "objID");
		mono_field_set_value(ret, field, &eID);

		if (addComponentFuncs.find(scriptName) != addComponentFuncs.end()) {

			if (addComponentFuncs[scriptName](entity)) return;
			CauseException("Only one component of this type can be on a single object. Type", scriptName.c_str());
			
		} else {

			ScriptComponent* script = entity->AddComponent<ScriptComponent>();
			script->Init(entity->ID(), typeName);

		}

	}
	static bool GetComponent(int eID, MonoReflectionType* type, MonoObject* ret) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return false; }

		InternalEntity* entity = GetEntityFromID(eID);

		MonoType* classType = mono_reflection_type_get_type(type);
		std::string name = mono_type_get_name(classType);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "objID");
		mono_field_set_value(ret, field, &eID);

		ScriptComponent* script = entity->GetComponent<ScriptComponent>();
		if (script->name != name) return false;

		script->CopyTo(ret);

		return true;

	}
	static bool HasComponent(int eID, MonoReflectionType* type) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return false; }

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		InternalEntity* entity = GetEntityFromID(eID);

		if (hasComponentFuncs.find(scriptName) != hasComponentFuncs.end()) {

			return hasComponentFuncs[scriptName](entity);

		} else {

			ScriptComponent* script = entity->GetComponent<ScriptComponent>();
			if (script->name != typeName) return false;

		}

		return false;

	}

	static void SetComponentObjID(MonoReflectionType* type, MonoObject* component, int eID) {

		MonoType* classType = mono_reflection_type_get_type(type);
		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "objID");
		mono_field_set_value(component, field, &eID);

	}

	//Transform
	static void GetPosition(int eID, Vector3* out) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetEntityFromID(eID)->GetTransform()->position;

	}
	static void GetRotation(int eID, Vector3* out) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetEntityFromID(eID)->GetTransform()->rotation;

	}
	static void GetScale(int eID, Vector3* out) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		*out = GetEntityFromID(eID)->GetTransform()->scale;

	}
	static void SetPosition(int eID, Vector3* value) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		GetEntityFromID(eID)->GetTransform()->position = *value;

	}
	static void SetRotation(int eID, Vector3* value) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		GetEntityFromID(eID)->GetTransform()->rotation = *value;

	}
	static void SetScale(int eID, Vector3* value) {

		if (eID == invalidID) { CauseExceptionInvalid("Copper Object"); return; }
		GetEntityFromID(eID)->GetTransform()->scale = *value;

	}

	static void GetForward(int eID, Vector3* out) { *out = GetEntityFromID(eID)->GetTransform()->Forward(); }
	static void GetBack(int eID, Vector3* out)  { *out = GetEntityFromID(eID)->GetTransform()->Back(); }
	static void GetRight(int eID, Vector3* out) { *out = GetEntityFromID(eID)->GetTransform()->Right(); }
	static void GetLeft(int eID, Vector3* out)  { *out = GetEntityFromID(eID)->GetTransform()->Left(); }
	static void GetUp(int eID, Vector3* out)    { *out = GetEntityFromID(eID)->GetTransform()->Up(); }
	static void GetDown(int eID, Vector3* out)  { *out = GetEntityFromID(eID)->GetTransform()->Down(); }

	//Camera
	static float CameraGetFOV(int eID) {

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->fov;

	}
	static float CameraGetNearPlane(int eID) {

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->nearPlane;

	}
	static float CameraGetFarPlane(int eID) {

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->farPlane;

	}
	static void CameraSetFOV(int eID, float value) {

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->fov = value;

	}
	static void CameraSetNearPlane(int eID, float value) {

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->nearPlane = value;

	}
	static void CameraSetFarPlane(int eID, float value) {

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->farPlane = value;

	}

}