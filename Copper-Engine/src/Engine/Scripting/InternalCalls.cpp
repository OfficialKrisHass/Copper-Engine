#include "cupch.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/class.h>

#define INTERNAL_CALL(class, name) mono_add_internal_call("Copper."#class"::Internal_"#name, (void*) name)

namespace Copper::Scripting {

    void EditorLog(MonoString* msg);
    void EditorLogWarn(MonoString* msg);
    void EditorLogError(MonoString* msg);

    MonoString* get_name(MonoObject* entity);
    void set_name(MonoObject* entity, MonoString* value);

    MonoObject* get_entity(MonoObject* component);

    void SetupInternalCalls() {

        CUP_FUNCTION();

        INTERNAL_CALL(Editor, EditorLog);
        INTERNAL_CALL(Editor, EditorLogWarn);
        INTERNAL_CALL(Editor, EditorLogError);

        mono_add_internal_call("Copper.Entity::get_name", (void*) get_name);
        mono_add_internal_call("Copper.Entity::set_name", (void*) set_name);

        mono_add_internal_call("Copper.Component::get_entity", (void*) get_entity);

    }

}
