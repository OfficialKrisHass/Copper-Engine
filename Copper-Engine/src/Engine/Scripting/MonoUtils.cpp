#include "cupch.h"
#include "MonoUtils.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::MonoUtils {

    void MonoStringToString(MonoString *string, std::string &out) {

        CUP_FUNCTION();

        if (string == nullptr || mono_string_length(string) == 0) return;

        char* tmp = mono_string_to_utf8(string);
        out = tmp;

        mono_free(tmp);

    }
    MonoString* StringToMonoString(const std::string& string) {

        CUP_FUNCTION();
        
        if (string.size() == 0) return nullptr;
        return mono_string_new(AppDomain(), string.c_str());

    }

}
