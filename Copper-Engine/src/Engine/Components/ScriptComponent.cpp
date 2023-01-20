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

	void ScriptComponent::CopyTo(MonoObject* other) {

		if (instance == other) return;

		for (ScriptField& field : Scripting::GetScriptFields(name)) {

			int tmp;
			GetFieldValue(field, &tmp);
			mono_field_set_value(other, field.field, &tmp);

		}

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

	void ScriptComponent::GetFieldValue(const ScriptField& field, void* out) {

		mono_field_get_value(instance, field.field, out);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, void* value) {

		mono_field_set_value(instance, field.field, value);

	}

}