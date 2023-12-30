#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include <mono/metadata/object.h>

#define RETURN_NULLPTR { *out = nullptr; return; }

namespace Copper {

	void ScriptComponent::Init(const std::string& name) {

		this->name = name;

		instance = Scripting::AddScriptComponent(GetEntity()->ID(), name);
		if (!instance) {

			Invalidate();
			return;

		}

		valid = true;
		MonoClass* klass = mono_object_get_class(instance);

		create = mono_class_get_method_from_name(klass, "Create", 0);
		onCollisionBegin = mono_class_get_method_from_name(klass, "OnCollisionBegin", 1);
		onCollisionEnd = mono_class_get_method_from_name(klass, "OnCollisionEnd", 1);
		onTriggerEnter = mono_class_get_method_from_name(klass, "OnTriggerEnter", 1);
		onTriggerLeave = mono_class_get_method_from_name(klass, "OnTriggerLeave", 1);
		
		MonoMethod* updateMethod = mono_class_get_method_from_name(klass, "Update", 0);
		MonoMethod* onCollisionPersistMethod = mono_class_get_method_from_name(klass, "OnCollisionPersist", 1);

		if (updateMethod)
			update = (void(*) (MonoObject*, MonoException**)) mono_method_get_unmanaged_thunk(updateMethod);
		else { update = nullptr; }

		if (onCollisionPersistMethod)
			onCollisionPersist = (void(*) (MonoObject*, MonoObject*, MonoException**)) mono_method_get_unmanaged_thunk(onCollisionPersistMethod);
		else { onCollisionPersist = nullptr; }

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

    void ScriptComponent::InvokeOnCollisionBegin(InternalEntity *other) {

		if (!onCollisionBegin || !other)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(onCollisionBegin, instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }
    void ScriptComponent::InvokeOnCollisionPersist(InternalEntity *other) {

		if (!onCollisionPersist || !other)
			return;

		MonoException* exc;
		onCollisionPersist(instance, Scripting::GetScriptEntity(other->ID()), &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails((MonoObject*) exc);

    }
	void ScriptComponent::InvokeOnCollisionEnd(InternalEntity *other) {

		if (!onCollisionEnd || !other)
			return;
		
		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(onCollisionEnd, instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }

    void ScriptComponent::InvokeOnTriggerEnter(InternalEntity *other) {

		if (!onTriggerEnter || !other)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(onTriggerEnter, instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }
	void ScriptComponent::InvokeOnTriggerLeave(InternalEntity *other) {

		if (!onTriggerLeave)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(onTriggerLeave, instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }

    void ScriptComponent::GetFieldValue(const ScriptField& field, void* out) {

		mono_field_get_value(instance, field.field, out);

	}
	void ScriptComponent::GetFieldValue(const ScriptField& field, InternalEntity** out) {

		if (field.type != ScriptField::Type::Entity)
			RETURN_NULLPTR;

		MonoObject* entity = nullptr;
		entity = mono_field_get_value_object(Scripting::GetAppDomain(), field.field, instance);
		if (!entity)
			RETURN_NULLPTR;

		uint32_t eID = INVALID_ENTITY_ID;
		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_get_value(entity, eIDField, &eID);
		if (eID == INVALID_ENTITY_ID)
			RETURN_NULLPTR

		*out = GetEntityFromID(eID);

	}

	void ScriptComponent::GetFieldValue(const ScriptField& field, Transform** out) {

		if (field.type != ScriptField::Type::Transform)
			RETURN_NULLPTR

		MonoObject* transform = nullptr;
		transform = mono_field_get_value_object(Scripting::GetAppDomain(), field.field, instance);
		if (!transform)
			RETURN_NULLPTR;

		int64_t componentPointer = NULL;
		MonoClassField* componentPointerField = mono_class_get_field_from_name(Scripting::GetTransformMonoClass(), "componentPointer");
		mono_field_get_value(transform, componentPointerField, &componentPointer);
		if (componentPointer == NULL)
			RETURN_NULLPTR;

		*out = (Transform*) componentPointer;

	}

	void ScriptComponent::SetFieldValue(const ScriptField& field, void* value) {

		mono_field_set_value(instance, field.field, value);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, InternalEntity** value) {

		if (field.type != ScriptField::Type::Entity) return;

		uint32_t entityID = *value ? (*value)->ID() : INVALID_ENTITY_ID;
		mono_field_set_value(instance, field.field, Scripting::GetScriptEntity(entityID));

	}

	void ScriptComponent::SetFieldValue(const ScriptField& field, Transform** value) {

		if (field.type != ScriptField::Type::Transform) return;

		if (*value == nullptr) {

			mono_field_set_value(instance, field.field, nullptr);
			return;

		}

		MonoClass* transformClass = Scripting::GetTransformMonoClass();
		MonoObject* transform = mono_object_new(Scripting::GetAppDomain(), transformClass);
		if (!transform) return;

		MonoMethod* constructor = mono_class_get_method_from_name(transformClass, ".ctor", 1);
		if (!constructor) return;

		uint64_t eID = (*value)->GetEntity()->ID();
		void* param = &eID;
		MonoObject* exc = nullptr;
		mono_runtime_invoke(constructor, transform, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

		mono_field_set_value(instance, field.field, transform);


	}

	void ScriptComponent::Invalidate() {

		valid = false;
		create = nullptr;
		update = nullptr;

		onCollisionBegin = nullptr;
		onCollisionPersist = nullptr;
		onCollisionEnd = nullptr;

		onTriggerEnter = nullptr;
		onTriggerLeave = nullptr;

	}

}