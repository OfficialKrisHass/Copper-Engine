#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/ScriptEngine.h"

#include <CopperECS/Object.h>

#include <mono/metadata/object.h>

namespace Copper {

	ScriptComponent::ScriptComponent(std::string nameSpace, std::string scriptName, Object& obj) : nameSpace(nameSpace), scriptName(scriptName) {

		instance = ScriptEngine::AddScriptComponent(obj, nameSpace, scriptName);
		valid = instance ? true : false;

		MonoClass* instanceClass = mono_object_get_class(instance);
		onCreate = mono_class_get_method_from_name(instanceClass, "OnCreate", 0);

		MonoMethod* update = mono_class_get_method_from_name(instanceClass, "OnUpdate", 0);
		if (!update) { onUpdate = nullptr; return; }
		onUpdate = (void (*) (MonoObject*, MonoException**)) mono_method_get_unmanaged_thunk(update);

	}

	void ScriptComponent::OnCreate() {

		if (!onCreate) return;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(onCreate, instance, nullptr, &exception);

	}

	void ScriptComponent::OnUpdate() {

		if (!onUpdate) return;

		MonoException* exc;
		onUpdate(instance, &exc);

	}

}