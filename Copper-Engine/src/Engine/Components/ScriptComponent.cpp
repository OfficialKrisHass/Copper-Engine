#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include <mono/metadata/object.h>

namespace Copper {

	void ScriptComponent::Init(const std::string& name) {

		this->name = name;

		instance = Scripting::AddScriptComponent(GetEntity()->ID(), name);
		if (!instance) {

			valid = false;
			create = nullptr;
			update = nullptr;

			return;

		}

		valid = true;
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

		MonoObject* exc = nullptr;
		mono_runtime_invoke(create, instance, nullptr, &exc);

		if (!exc) return;
		Scripting::MonoUtils::PrintExceptionDetails(exc);

	}
	void ScriptComponent::InvokeUpdate() {

		if (!update) return;

		MonoException* exc;
		update(instance, &exc);

		if (!exc) return;
		Scripting::MonoUtils::PrintExceptionDetails((MonoObject*) exc);

	}

	void ScriptComponent::GetFieldValue(const ScriptField& field, void* out) {

		mono_field_get_value(instance, field.field, out);

	}
	void ScriptComponent::GetFieldValue(const ScriptField& field, InternalEntity** out) {

		if (field.type != ScriptField::Type::Entity) { *out = nullptr; return; }

		MonoObject* entity = nullptr;
		entity = mono_field_get_value_object(Scripting::GetAppDomain(), field.field, instance);
		if (!entity) { *out = nullptr; return; }

		uint32_t eID = INVALID_ENTITY_ID;
		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_get_value(entity, eIDField, &eID);
		if (eID == INVALID_ENTITY_ID) { *out = nullptr; return; }

		*out = GetEntityFromID(eID);

	}

	void ScriptComponent::SetFieldValue(const ScriptField& field, void* value) {

		mono_field_set_value(instance, field.field, value);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, InternalEntity** value) {

		if (field.type != ScriptField::Type::Entity) return;

		//It's possible that the field may be null so we can't just get the MonoObject*
		//and set it's objID. Instead for safety purposes we create a temporary CopperObject
		//instance, set it's objID and then set the Scripts field to the temp instance
		MonoObject* entity = mono_object_new(Scripting::GetAppDomain(), Scripting::GetEntityMonoClass());
		mono_runtime_object_init(entity);

		uint32_t entityID = *value ? (*value)->ID() : INVALID_ENTITY_ID;
		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_set_value(entity, eIDField, &entityID);

		mono_field_set_value(instance, field.field, entity);

	}

}