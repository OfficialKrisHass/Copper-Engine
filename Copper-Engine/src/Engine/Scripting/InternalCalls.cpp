#include "cupch.h"

#include <mono/jit/jit.h>

#define INTERNAL_CALL(class, name) mono_add_internal_call("Copper."#class"::Internal_"#name, (void*) name)

namespace Copper::Scripting {

    void EditorLog(MonoString* msg);
    void EditorLogWarn(MonoString* msg);
    void EditorLogError(MonoString* msg);

    void SetupInternalCalls() {

        CUP_FUNCTION();

        INTERNAL_CALL(Editor, EditorLog);
        INTERNAL_CALL(Editor, EditorLogWarn);
        INTERNAL_CALL(Editor, EditorLogError);

    }

}
