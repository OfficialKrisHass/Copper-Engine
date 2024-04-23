#include "cupch.h"

#include "Engine/Components/Component.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/class.h>

#define INTERNAL_CALL_GROUP(name, calls) namespace name { calls }
#define ADD_INTERNAL_CALL(class, name) mono_add_internal_call("Copper."#class"::Internal_"#name, (void*) class::name)

namespace Copper::Scripting {

    INTERNAL_CALL_GROUP(Editor,
            void EditorLog(MonoString* msg);
            void EditorLogWarn(MonoString* msg);
            void EditorLogError(MonoString* msg););

    INTERNAL_CALL_GROUP(Entity, 
            MonoString* get_name(MonoObject* entity);
            void set_name(MonoObject* entity, MonoString* value););

    INTERNAL_CALL_GROUP(Component,
            MonoObject* get_entity(MonoObject* component););

    INTERNAL_CALL_GROUP(Transform, 
            Vector3* get_position(MonoObject* transform);
            void set_position(MonoObject* transform, Vector3* value);

            Vector3* get_scale(MonoObject* transform);
            void set_scale(MonoObject* transform, Vector3* value);

            Vector3* get_forward(MonoObject* transform);
            Vector3* get_right(MonoObject* transform);
            Vector3* get_up(MonoObject* transform);)

    void SetupInternalCalls() {

        CUP_FUNCTION();

        // Editor

        ADD_INTERNAL_CALL(Editor, EditorLog);
        ADD_INTERNAL_CALL(Editor, EditorLogWarn);
        ADD_INTERNAL_CALL(Editor, EditorLogError);

        // Entity

        ADD_INTERNAL_CALL(Entity, get_name);
        ADD_INTERNAL_CALL(Entity, set_name);

        // Component

        ADD_INTERNAL_CALL(Component, get_entity);

        // Transform

        ADD_INTERNAL_CALL(Transform, get_position);
        ADD_INTERNAL_CALL(Transform, set_position);

        ADD_INTERNAL_CALL(Transform, get_scale);
        ADD_INTERNAL_CALL(Transform, set_scale);

        ADD_INTERNAL_CALL(Transform, get_forward);
        ADD_INTERNAL_CALL(Transform, get_right);
        ADD_INTERNAL_CALL(Transform, get_up);


    }

}
