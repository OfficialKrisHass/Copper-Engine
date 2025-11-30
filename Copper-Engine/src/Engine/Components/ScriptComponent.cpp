#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/Classes.h"
#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

#define GET_FUNC(name, parCount, variable, type) if (MonoMethod* method = mono_class_get_method_from_name(script->m_class, name, parCount))\
                                            variable = (type) mono_method_get_unmanaged_thunk(method)

namespace Copper {

    using namespace Scripting;

    void ScriptComponent::Setup(const Script* script) {

        CUP_FUNCTION();

        CU_ASSERT(script != nullptr, "Can't setup script component on entity '{}' with invalid script", *GetEntity());
        CU_ASSERT(script->m_class != nullptr, "Can't setup script component with invalid script. Script: '{}', Entity: '{}'", script->FullName(), *GetEntity());

        if (!script->IsSubclassOf(GetClass(Class::Component))) {

            LogError("Can not create Script Component with a non Component script.\n\tScript name: {}", script->FullName());
            return;

        }

        m_scriptName = script->FullName();
        m_instance = mono_object_new(AppDomain(), script->m_class);
        CU_ASSERT(m_instance != nullptr, "Could not instantiate ScriptComponent '{}' on Entity '{}'", GetScriptName(), *GetEntity());

        m_state = State::Begin;
        m_updateFuncs[0] = nullptr;
        m_updateFuncs[1] = nullptr;

        CallBaseConstructor();

        GET_FUNC("OnBegin", 0, m_updateFuncs[0], UpdateFunc);
        GET_FUNC("OnUpdate", 0, m_updateFuncs[1], UpdateFunc);

        GET_FUNC("OnCollisionBegin", 1, m_onCollisionBegin, CollisionFunc);
        GET_FUNC("OnCollisionPersist", 1, m_onCollisionPersist, CollisionFunc);
        GET_FUNC("OnCollisionEnd", 1, m_onCollisionEnd, CollisionFunc);

        GET_FUNC("OnTriggerEnter", 1, m_onTriggerEnter, CollisionFunc);
        GET_FUNC("OnTriggerLeave", 1, m_onTriggerLeave, CollisionFunc);

    }

    void ScriptComponent::CallBaseConstructor() {

        CUP_FUNCTION();

        MonoMethod* constructor = mono_class_get_method_from_name(GetScript()->m_class, ".ctor", 0);
        CU_ASSERT(constructor != nullptr, "Could not get Script component Constructor.");

        ScriptComponent* value = this;
        mono_field_set_value(m_instance, UnmanagedPtrField(), (void*) &value);

        mono_runtime_invoke(constructor, m_instance, nullptr, nullptr);

    }

    void ScriptComponent::Update() {

        CUP_FUNCTION();

        switch (m_state) {

            case State::Begin: {

                UpdateFunc func = m_updateFuncs[static_cast<uint8>(State::Begin)];
                if (func != nullptr)
                    ExecuteFunction(func);

                m_state = State::Update;

                // This fall through is intentional, we want Update to be called as well.

            }
            case State::Update: {

                UpdateFunc func = m_updateFuncs[static_cast<uint8>(m_state)];
                if (func != nullptr)
                    ExecuteFunction(func);

                break;

            }
            default: break;

        }

    }

    // Collision Functions

    void ScriptComponent::OnCollisionBegin(InternalEntity* other) {

        CUP_FUNCTION();

        if (m_onCollisionBegin == nullptr) return;

        MonoObject* managedOther = GetManagedReference((void*) static_cast<uint64>(other->GetID()), Class::Entity);
        ExecuteCollisionFunction(m_onCollisionBegin, managedOther);

    }
    void ScriptComponent::OnCollisionPersist(InternalEntity* other) {

        CUP_FUNCTION();

        if (m_onCollisionPersist == nullptr) return;

        MonoObject* managedOther = GetManagedReference((void*) static_cast<uint64>(other->GetID()), Class::Entity);
        ExecuteCollisionFunction(m_onCollisionPersist, managedOther);

    }
    void ScriptComponent::OnCollisionEnd(InternalEntity* other) {

        CUP_FUNCTION();

        if (m_onCollisionEnd == nullptr) return;

        MonoObject* managedOther = GetManagedReference((void*) static_cast<uint64>(other->GetID()), Class::Entity);
        ExecuteCollisionFunction(m_onCollisionEnd, managedOther);

    }

    void ScriptComponent::OnTriggerEnter(InternalEntity* other) {

        CUP_FUNCTION();

        if (m_onTriggerEnter == nullptr) return;

        MonoObject* managedOther = GetManagedReference((void*) static_cast<uint64>(other->GetID()), Class::Entity);
        ExecuteCollisionFunction(m_onTriggerEnter, managedOther);

    }
    void ScriptComponent::OnTriggerLeave(InternalEntity* other) {

        CUP_FUNCTION();

        if (m_onTriggerLeave == nullptr) return;

        MonoObject* managedOther = GetManagedReference((void*) static_cast<uint64>(other->GetID()), Class::Entity);
        ExecuteCollisionFunction(m_onTriggerLeave, managedOther);
        
    }

    void ScriptComponent::ExecuteFunction(UpdateFunc func) {

        CUP_FUNCTION();

        MonoException* exc = nullptr;
        func(m_instance, &exc);

        if (exc == nullptr) return;
        MonoUtils::PrintExceptionDetails(exc);

    }
    void ScriptComponent::ExecuteCollisionFunction(CollisionFunc func, MonoObject* other) {

        CUP_FUNCTION();

        MonoException* exc = nullptr;
        func(m_instance, other, &exc);

        if (exc == nullptr) return;
        MonoUtils::PrintExceptionDetails(exc);

    }

}
