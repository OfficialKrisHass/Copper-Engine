#include "cupch.h"
#include "Engine/Scripting/InternalCalls.h"
#include "Engine/Scripting/MonoUtils.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/RigidBody.h"
#include "Engine/Components/ScriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::InternalCalls {

	static std::unordered_map<std::string, std::function<bool(InternalEntity*)>> hasComponentFuncs;
	static std::unordered_map<std::string, std::function<bool(InternalEntity*)>> addComponentFuncs;

	void Component_Initialize() {

		hasComponentFuncs["Transform"] = [](InternalEntity* entity) { return true; };
		hasComponentFuncs["Camera"] = [](InternalEntity* entity) { return entity->HasComponent<Camera>(); };
		hasComponentFuncs["RigidBody"] = [](InternalEntity* entity) { return entity->HasComponent<RigidBody>(); };

		addComponentFuncs["Camera"] = [](InternalEntity* entity) { return entity->AddComponent<Camera>() != nullptr ? true : false; };
		addComponentFuncs["RigidBody"] = [](InternalEntity* entity) { return entity->AddComponent<RigidBody>() != nullptr ? true : false; };

	}

	void AddComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret) {

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
	bool GetComponent(uint32_t eID, MonoReflectionType* type, MonoObject* ret) {

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
	bool HasComponent(uint32_t eID, MonoReflectionType* type) {

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

	void SetComponentEID(MonoReflectionType* type, MonoObject* component, uint32_t eID) {

		MonoType* classType = mono_reflection_type_get_type(type);
		MonoClassField* field = mono_class_get_field_from_name(mono_class_from_mono_type(classType), "eID");
		mono_field_set_value(component, field, &eID);

	}

}