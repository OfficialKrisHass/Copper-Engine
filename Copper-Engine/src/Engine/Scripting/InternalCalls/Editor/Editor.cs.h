#pragma once

#include "Engine/Core/Core.h"

extern "C" { typedef struct _MonoString MonoString; }

namespace Copper::Scripting::Editor {

    void EditorLog(MonoString* msg);
    void EditorLogWarn(MonoString* msg);
    void EditorLogError(MonoString* msg);

}
