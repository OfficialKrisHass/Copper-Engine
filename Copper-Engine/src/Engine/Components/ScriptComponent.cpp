#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/ScriptEngine.h"

#include <CopperECS/Object.h>

#include <mono/metadata/object.h>

namespace Copper {

	ScriptComponent::ScriptComponent(std::string nameSpace, std::string name, Object& obj) : name(nameSpace + '.' + name) {

		instance = ScriptEngine::AddScriptComponent(obj, nameSpace, name);
		valid = instance ? true : false;

		MonoClass* instanceClass = mono_object_get_class(instance);
		onCreate = mono_class_get_method_from_name(instanceClass, "OnCreate", 0);
		onUpdate = mono_class_get_method_from_name(instanceClass, "OnUpdate", 0);

	}

	void ScriptComponent::OnCreate() {

		if (!onCreate) return;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(onCreate, instance, nullptr, &exception);

	}

	void ScriptComponent::OnUpdate() {

		if (!onUpdate) return;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(onUpdate, instance, nullptr, &exception);

	}

}