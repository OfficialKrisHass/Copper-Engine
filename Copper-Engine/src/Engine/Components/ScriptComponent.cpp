#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Core/Engine.h"

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
	void ScriptComponent::GetFieldValue(const ScriptField& field, Object* out) {

		if (field.type != ScriptField::Type::CopperObject) return;

		MonoObject* obj = nullptr;
		obj = mono_field_get_value_object(Scripting::GetAppDomain(), field.field, instance);
		if (!obj) return;

		int objID = -1;
		MonoClassField* objIDField = mono_class_get_field_from_name(Scripting::GetCopperObjectClass(), "objID");
		mono_field_get_value(obj, objIDField, &objID);
		if (objID == -1) return;

		*out = GetObjectFromID(objID);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, void* value) {

		mono_field_set_value(instance, field.field, value);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, Object* value) {

		if (field.type != ScriptField::Type::CopperObject) return;

		//It's possible that the field may be null so we can't just get the MonoObject*
		//and set it's objID. Instead for safety purposes we create a temporary CopperObject
		//instance, set it's objID and then set the Scripts field to the temp instance
		MonoObject* obj = mono_object_new(Scripting::GetAppDomain(), Scripting::GetCopperObjectClass());
		mono_runtime_object_init(obj);

		int objID = value->GetID();
		MonoClassField* objIDField = mono_class_get_field_from_name(Scripting::GetCopperObjectClass(), "objID");
		mono_field_set_value(obj, objIDField, &objID);

		mono_field_set_value(instance, field.field, obj);

	}

}