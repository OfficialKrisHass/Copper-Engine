#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <mono/metadata/object.h>

namespace Copper {

	void ScriptComponent::Init(int32_t objID, const std::string& name) {

		this->name = name;
		instance = Scripting::AddScriptComponent(objID, name);
		MonoClass* klass = mono_object_get_class(instance);

		create = mono_class_get_method_from_name(klass, "Create", 0);
		
		MonoMethod* updateMethod = mono_class_get_method_from_name(klass, "Update", 0);
		if (!updateMethod) { update = nullptr; return; }
		update = (void(*) (MonoObject*, MonoException**)) mono_method_get_unmanaged_thunk(updateMethod);

	}

	void ScriptComponent::InvokeCreate() {

		if (!create) return;

		MonoObject* exception = nullptr;
		mono_runtime_invoke(create, instance, nullptr, &exception);

	}
	void ScriptComponent::InvokeUpdate() {

		if (!update) return;

		MonoException* exc;
		update(instance, &exc);

	}

}