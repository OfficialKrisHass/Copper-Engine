#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/Input.h"
#include "Engine/Input/AxisManager.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/ScriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define CauseExceptionInvalid(argument) mono_raise_exception(mono_get_exception_argument_null(argument))
#define CauseException(message, argument)  mono_raise_exception(mono_get_exception_argument(argument, message))

#define CheckValidEntity(eID) if (eID == invalidID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) {\
						      CauseExceptionInvalid("Entity"); return; }
#define CheckValidEntityWithReturn(eID, ret) if (eID == invalidID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) {\
											 CauseExceptionInvalid("Entity"); return ret; }

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
	static bool IsKey(int keyCode) {
		
	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKey((KeyCode) keyCode);
	
	}
	static bool IsKeyDown(int keyCode) {
		
	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKeyDown((KeyCode) keyCode);
	
	}
	static bool IsKeyReleased(int keyCode) {
		
	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return false;
	#endif
		return Input::IsKeyReleased((KeyCode) keyCode);
	
	}

	static float GetAxis(MonoString* axisName) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return 0.0f;
	#endif
		return Input::GetAxis(MonoUtils::MonoToString(axisName));

	}

	static void SetCursorVisible(bool visible) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return;
	#endif
		Input::SetCursorVisible(visible);

	}
	static void SetCursorLocked(bool locked) {

	#ifdef CU_EDITOR
		if (!AcceptInputDuringRuntime()) return;
	#endif
		Input::SetCursorLocked(locked);

	}

	//Entity
	static MonoString* GetEntityName(uint32_t eID) {

		CheckValidEntityWithReturn(eID, nullptr);
		InternalEntity* entity = GetEntityFromID(eID);

		return MonoUtils::StringToMono(entity->name);

	}
	static void SetEntityName(uint32_t eID, MonoString* out) {

		CheckValidEntity(eID);
		InternalEntity* entity = GetEntityFromID(eID);

		entity->name = MonoUtils::MonoToString(out);

	}

	static bool IsEntityValid(uint32_t eID) {

		if (eID == invalidID || eID >= GetNumOfEntities() || !(*GetEntityFromID(eID))) return false;
		return true;

	}

	static MonoObject* GetEntity(uint32_t eID) {

		CheckValidEntityWithReturn(eID, nullptr);

		MonoObject* ret = mono_object_new(Scripting::GetAppDomain(), Scripting::GetEntityMonoClass());
		mono_runtime_object_init(ret);

		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_set_value(ret, eIDField, &eID);

		return ret;

	}

	//Components
	static void AddComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret) {

		CheckValidEntity(eID);

		MonoType* managedType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(managedType);

		std::string scriptName = typeName;
		MonoUtils::RemoveNamespace(scriptName);

		InternalEntity* entity = GetEntityFromID(eID);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(managedType), "eID");
		mono_field_set_value(ret, field, &eID);

		if (addComponentFuncs.find(scriptName) != addComponentFuncs.end()) {

			if (addComponentFuncs[scriptName](entity)) return;
			CauseException("Only one component of this type can be on a single object. Type", scriptName.c_str());
			
		} else {

			ScriptComponent* script = entity->AddComponent<ScriptComponent>();
			script->Init(typeName);

		}

	}
	static bool GetComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret) {

		CheckValidEntityWithReturn(eID, false);

		InternalEntity* entity = GetEntityFromID(eID);

		MonoType* classType = mono_reflection_type_get_type(type);
		std::string name = mono_type_get_name(classType);

		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "eID");
		mono_field_set_value(ret, field, &eID);

		ScriptComponent* script = entity->GetComponent<ScriptComponent>();
		if (script->name != name) return false;

		script->CopyTo(ret);

		return true;

	}
	static bool HasComponent(uint32_t eID, MonoReflectionType* type) {

		CheckValidEntityWithReturn(eID, false);

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

	static void SetComponentObjID(MonoReflectionType* type, MonoObject* component, uint32_t eID) {

		MonoType* classType = mono_reflection_type_get_type(type);
		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "eID");
		mono_field_set_value(component, field, &eID);

	}

	//Transform
	static void GetPosition(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->position;

	}
	static void GetRotation(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->rotation;

	}
	static void GetScale(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->scale;

	}
	static void SetPosition(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->position = *value;

	}
	static void SetRotation(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->rotation = *value;

	}
	static void SetScale(uint32_t eID, Vector3* value) {

		CheckValidEntity(eID);
		GetEntityFromID(eID)->GetTransform()->scale = *value;

	}

	static void GetForward(uint32_t eID, Vector3* out) {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Forward();

	}
	static void GetBack(uint32_t eID, Vector3* out)    {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Back();
	
	}
	static void GetRight(uint32_t eID, Vector3* out)   {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Right();
	
	}
	static void GetLeft(uint32_t eID, Vector3* out)    {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Left();
	
	}
	static void GetUp(uint32_t eID, Vector3* out)      {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Up();
	
	}
	static void GetDown(uint32_t eID, Vector3* out)    {

		CheckValidEntity(eID);
		*out = GetEntityFromID(eID)->GetTransform()->Down();
	
	}

	//Camera
	static float CameraGetFOV(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->fov;

	}
	static float CameraGetNearPlane(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->nearPlane;

	}
	static float CameraGetFarPlane(uint32_t eID) {

		CheckValidEntityWithReturn(eID, 0.0f);

		InternalEntity* entity = GetEntityFromID(eID);
		return entity->GetComponent<Camera>()->farPlane;

	}
	static void CameraSetFOV(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->fov = value;

	}
	static void CameraSetNearPlane(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->nearPlane = value;

	}
	static void CameraSetFarPlane(uint32_t eID, float value) {

		CheckValidEntity(eID);

		InternalEntity* entity = GetEntityFromID(eID);
		entity->GetComponent<Camera>()->farPlane = value;

	}

}