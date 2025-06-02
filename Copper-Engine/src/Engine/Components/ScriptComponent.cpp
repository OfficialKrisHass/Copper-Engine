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

        m_scriptName = script->FullName();
        m_instance = mono_object_new(AppDomain(), script->m_class);
        m_state = State::Begin;

        m_updateFuncs[0] = nullptr;
        m_updateFuncs[1] = nullptr;

        if (!m_instance) {

            LogError("Could not instantiate the ScriptComponent.\n\tScript name: {}\n\tEntity: {}", m_scriptName, GetEntity());
            return;

        }

        CallBaseConstructor();

        if (MonoMethod* method = mono_class_get_method_from_name(script->m_class, "OnBegin", 0))
            m_updateFuncs[0] = (UpdateFunc) mono_method_get_unmanaged_thunk(method);
        if (MonoMethod* method = mono_class_get_method_from_name(script->m_class, "OnUpdate", 0))
            m_updateFuncs[1] = (UpdateFunc) mono_method_get_unmanaged_thunk(method);

    }

    void ScriptComponent::CallBaseConstructor() {

        CUP_FUNCTION();

        MonoMethod* constructor = mono_class_get_method_from_name(GetMonoClass<Component>(), ".ctor", 0);
        CU_ASSERT(constructor, "Could not get the Component Constructor from the Component class");

        ScriptComponent* value = this;
        mono_field_set_value(m_instance, UnmanagedPtrField(), (void*) &value);

        mono_runtime_invoke(constructor, m_instance, nullptr, nullptr);

    }

    void ScriptComponent::Update() {

        CUP_FUNCTION();

        UpdateFunc func = m_updateFuncs[static_cast<uint8>(m_state)];
        if (func != nullptr)
            ExecuteFunction(func);

        switch (m_state) {

            case State::Begin: m_state = State::Update; break;
            case State::Update: break;
            default: break;

        }

    }

    void ScriptComponent::ExecuteFunction(UpdateFunc func) {

        CUP_FUNCTION();

        MonoException* exc = nullptr;
        func(m_instance, &exc);

        if (!exc) return;
        MonoUtils::PrintExceptionDetails(exc);

    }

}
