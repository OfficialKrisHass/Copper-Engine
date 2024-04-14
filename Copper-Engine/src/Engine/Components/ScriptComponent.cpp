#include "cupch.h"
#include "ScriptComponent.h"

#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper {

    using namespace Scripting;

    void ScriptComponent::Setup(const Script* script) {

        CUP_FUNCTION();

        if (!script->IsSubclassOf(BaseComponent())) {

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

        MonoMethod* constructor = mono_class_get_method_from_name(BaseComponent().GetClass(), "Test()", -1);
        CU_ASSERT(constructor, "Could not get the Component Constructor from the Component class");

        uint32 id = GetEntity()->ID();
        void* param = &id;
        MonoObject* exception = nullptr;

        mono_runtime_invoke(constructor, m_instance, &param, &exception);
        if (!exception) return;

        LogError("Exception was thrown invoking component constructor!");

    }

}
