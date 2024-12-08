#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

namespace Copper {

    using namespace Scripting;

    void ScriptComponent::Setup(const Script* script) {

        CUP_FUNCTION();

        if (!script->IsSubclassOf(GetMonoClass<Component>())) {

            LogError("Can not create Script Component with a non Component script.\n\tScript name: {}", script->FullName());
            return;

        }

        this->m_script = script;
        this->m_instance = mono_object_new(AppDomain(), m_script->m_class);

        if (!m_instance) {

            LogError("Could not instantiate the ScriptComponent.\n\tScript name: {}\n\tEntity: {} ({})", m_script->FullName(), GetEntity()->name, GetEntity()->ID());
            return;

        }

        CallBaseConstructor();

        m_begin = mono_class_get_method_from_name(script->m_class, "OnBegin", 0);

        MonoMethod* onUpdate = mono_class_get_method_from_name(m_script->m_class, "OnUpdate", 0);
        if (onUpdate)
            m_update = (UpdateFunc) mono_method_get_unmanaged_thunk(onUpdate);

    }

    void ScriptComponent::CallBaseConstructor() {

        CUP_FUNCTION();

        MonoMethod* constructor = mono_class_get_method_from_name(GetMonoClass<Component>(), ".ctor", 0);
        CU_ASSERT(constructor, "Could not get the Component Constructor from the Component class");

        ScriptComponent* value = this;
        mono_field_set_value(m_instance, UnmanagedPtrField(), (void*) &value);

        mono_runtime_invoke(constructor, m_instance, nullptr, nullptr);

    }

    void ScriptComponent::OnBegin() const {

        CUP_FUNCTION();

        if (!m_begin) return;

        MonoObject* exc = nullptr;
        mono_runtime_invoke(m_begin, m_instance, nullptr, &exc);

        if (!exc) return;
        MonoUtils::PrintExceptionDetails(exc);

    }
    void ScriptComponent::OnUpdate() const {

        CUP_FUNCTION();

        if (!m_update) return;

        MonoException* exc = nullptr;
        m_update(m_instance, &exc);

        if (!exc) return;
        MonoUtils::PrintExceptionDetails((MonoObject*) exc);

    }

}
