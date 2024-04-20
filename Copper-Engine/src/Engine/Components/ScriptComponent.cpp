#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

namespace Copper {

    using namespace Scripting;

    void ScriptComponent::Setup(const Script* script) {

        CUP_FUNCTION();

        if (!script->IsSubclassOf(ComponentClass())) {

            LogError("Can not create Script Component with a non Component script.\n\tScript name: {}", script->FullName());
            return;

        }

        this->m_script = script;
        this->m_instance = mono_object_new(AppDomain(), m_script->GetClass());

        if (!m_instance) {

           LogError("Could not instantiate the ScriptComponent.\n\tScript name: {}\n\tEntity: {} ({})", m_script->FullName(), GetEntity()->name, GetEntity()->ID());
           return;

        }

        CallBaseConstructor();

    }
    
    void ScriptComponent::CallBaseConstructor() {

        CUP_FUNCTION();

        MonoMethod* constructor = mono_class_get_method_from_name(ComponentClass().GetClass(), ".ctor", 0);
        CU_ASSERT(constructor, "Could not get the Component Constructor from the Component class");

        ScriptComponent* value = this;
        mono_field_set_value(m_instance, UnmanagedPtrField(), (void*) &value);

        mono_runtime_invoke(constructor, m_instance, nullptr, nullptr);

    }

}
