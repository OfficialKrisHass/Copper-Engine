#include "cupch.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/class.h>

#define INTERNAL_CALL(class, name) mono_add_internal_call("Copper."#class"::Internal_"#name, (void*) name)

namespace Copper::Scripting {

    void EditorLog(MonoString* msg);
    void EditorLogWarn(MonoString* msg);
    void EditorLogError(MonoString* msg);

    void EntityName(MonoObject* component) {

        CUP_FUNCTION();

        Component* comp = nullptr;
        mono_field_get_value(component, UnmanagedPtrField(), &comp);

        Log(comp->GetEntity()->name);

    }

    void SetupInternalCalls() {

        CUP_FUNCTION();

        INTERNAL_CALL(Editor, EditorLog);
        INTERNAL_CALL(Editor, EditorLogWarn);
        INTERNAL_CALL(Editor, EditorLogError);

        mono_add_internal_call("Copper.Component::EntityName", (void*) EntityName);

    }

}
