#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/MonoUtils.h"

#include <mono/metadata/object.h>

// TODO: Remove profiler include
#include "Engine/Debug/Profiler.h"

#define RETURN_NULLPTR { *out = nullptr; return; }

#define Test Jajaj

namespace Copper {

	void ScriptComponent::Init(const std::string& name) {

		CUP_FUNCTION();

		this->name = name;

		m_instance = Scripting::AddScriptComponent(GetEntity()->ID(), name);
		if (!m_instance) {

			Invalidate();
			return;

		}

		MonoClass* klass = mono_object_get_class(m_instance);

		m_create = mono_class_get_method_from_name(klass, "Create", 0);
		m_onCollisionBegin = mono_class_get_method_from_name(klass, "OnCollisionBegin", 1);
		m_onCollisionEnd = mono_class_get_method_from_name(klass, "OnCollisionEnd", 1);
		m_onTriggerEnter = mono_class_get_method_from_name(klass, "OnTriggerEnter", 1);
		m_onTriggerLeave = mono_class_get_method_from_name(klass, "OnTriggerLeave", 1);
		
		MonoMethod* update = mono_class_get_method_from_name(klass, "Update", 0);
		MonoMethod* onCollisionPersist = mono_class_get_method_from_name(klass, "OnCollisionPersist", 1);

		if (update)
			m_update = (UpdateFunc) mono_method_get_unmanaged_thunk(update);
		else { m_update = nullptr; }

		if (onCollisionPersist)
			m_onCollisionPersist = (OnCollisionPersistFunc) mono_method_get_unmanaged_thunk(onCollisionPersist);
		else { m_onCollisionPersist = nullptr; }

	}

	void ScriptComponent::CopyTo(MonoObject* other) {

		CUP_FUNCTION();

		if (m_instance == other) return;

		for (ScriptField& field : Scripting::GetScriptFields(name)) {

			int32 tmp;
			GetFieldValue(field, &tmp);
			mono_field_set_value(other, field.m_field, &tmp);

		}

	}

	void ScriptComponent::InvokeCreate() {

		if (!m_create) return;

		MonoObject* exc = nullptr;
		mono_runtime_invoke(m_create, m_instance, nullptr, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

	}
	void ScriptComponent::InvokeUpdate() {

		if (!m_update) return;

		MonoException* exc = nullptr;
		m_update(m_instance, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails((MonoObject*) exc);

	}

    void ScriptComponent::InvokeOnCollisionBegin(InternalEntity *other) {

		if (!m_onCollisionBegin || !other)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(m_onCollisionBegin, m_instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }
    void ScriptComponent::InvokeOnCollisionPersist(InternalEntity *other) {

		if (!m_onCollisionPersist || !other)
			return;

		MonoException* exc;
		m_onCollisionPersist(m_instance, Scripting::GetScriptEntity(other->ID()), &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails((MonoObject*) exc);

    }
	void ScriptComponent::InvokeOnCollisionEnd(InternalEntity *other) {

		if (!m_onCollisionEnd || !other)
			return;
		
		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(m_onCollisionEnd, m_instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }

    void ScriptComponent::InvokeOnTriggerEnter(InternalEntity *other) {

		if (!m_onTriggerEnter || !other)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(m_onTriggerEnter, m_instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }
	void ScriptComponent::InvokeOnTriggerLeave(InternalEntity *other) {

		if (!m_onTriggerLeave)
			return;

		void* param = (void*) Scripting::GetScriptEntity(other->ID());
		MonoObject* exc = nullptr;
		mono_runtime_invoke(m_onTriggerLeave, m_instance, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

    }

    void ScriptComponent::GetFieldValue(const ScriptField& field, void* out) {

		mono_field_get_value(m_instance, field.m_field, out);

	}
	void ScriptComponent::GetFieldValue(const ScriptField& field, InternalEntity** out) {

		if (field.type != ScriptField::Type::Entity) RETURN_NULLPTR;

		MonoObject* entity = nullptr;
		entity = mono_field_get_value_object(Scripting::GetAppDomain(), field.m_field, m_instance);

		if (!entity) RETURN_NULLPTR;

		uint32 eID = INVALID_ENTITY_ID;
		MonoClassField* eIDField = mono_class_get_field_from_name(Scripting::GetEntityMonoClass(), "id");
		mono_field_get_value(entity, eIDField, &eID);

		if (eID == INVALID_ENTITY_ID) RETURN_NULLPTR

		*out = GetEntityFromID(eID);

	}

	void ScriptComponent::GetFieldValue(const ScriptField& field, Transform** out) {

		if (field.type != ScriptField::Type::Transform) RETURN_NULLPTR

		MonoObject* transform = nullptr;
		transform = mono_field_get_value_object(Scripting::GetAppDomain(), field.m_field, m_instance);

		if (!transform) RETURN_NULLPTR;

		uint64 componentPointer = NULL;
		MonoClassField* componentPointerField = mono_class_get_field_from_name(Scripting::GetTransformMonoClass(), "componentPointer");
		mono_field_get_value(transform, componentPointerField, &componentPointer);

		if (componentPointer == NULL) RETURN_NULLPTR;

		*out = (Transform*) componentPointer;

	}

	void ScriptComponent::SetFieldValue(const ScriptField& field, void* value) {

		mono_field_set_value(m_instance, field.m_field, value);

	}
	void ScriptComponent::SetFieldValue(const ScriptField& field, InternalEntity** value) {

		if (field.type != ScriptField::Type::Entity) return;

		uint32 entityID = *value ? (*value)->ID() : INVALID_ENTITY_ID;
		mono_field_set_value(m_instance, field.m_field, Scripting::GetScriptEntity(entityID));

	}

	void ScriptComponent::SetFieldValue(const ScriptField& field, Transform** value) {

		if (field.type != ScriptField::Type::Transform) return;

		if (*value == nullptr) {

			mono_field_set_value(m_instance, field.m_field, nullptr);
			return;

		}

		MonoClass* transformClass = Scripting::GetTransformMonoClass();
		MonoObject* transform = mono_object_new(Scripting::GetAppDomain(), transformClass);

		if (!transform) return;

		MonoMethod* constructor = mono_class_get_method_from_name(transformClass, ".ctor", 1);

		if (!constructor) return;

		uint32 eID = (*value)->GetEntity()->ID();
		void* param = &eID;
		MonoObject* exc = nullptr;
		mono_runtime_invoke(constructor, transform, &param, &exc);

		if (exc)
			Scripting::MonoUtils::PrintExceptionDetails(exc);

		mono_field_set_value(m_instance, field.m_field, transform);


	}

	void ScriptComponent::Invalidate() {

		m_create = nullptr;
		m_update = nullptr;

		m_onCollisionBegin = nullptr;
		m_onCollisionPersist = nullptr;
		m_onCollisionEnd = nullptr;

		m_onTriggerEnter = nullptr;
		m_onTriggerLeave = nullptr;

	}

}