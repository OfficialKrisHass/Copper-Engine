#include "cupch.h"
#include "Editor.cs.h"

#include "Engine/Scripting/MonoUtils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::Editor {

    void EditorLog(MonoString* msg) {

        CUP_FUNCTION();

        std::string str;
        MonoUtils::MonoStringToString(msg, str);

        Log(str);

    }
    void EditorLogWarn(MonoString* msg) {

        CUP_FUNCTION();

        std::string str;
        MonoUtils::MonoStringToString(msg, str);

        LogWarn(str);

    }
    void EditorLogError(MonoString* msg) {

        CUP_FUNCTION();

        std::string str;
        MonoUtils::MonoStringToString(msg, str);

        LogError(str);

    }

}
