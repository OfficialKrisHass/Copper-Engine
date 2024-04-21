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
    
    void PrintExceptionDetails(MonoObject* exception) {

        CUP_FUNCTION();

        MonoClass* klass = mono_object_get_class(exception);
        if (!klass) {

            LogError("Could not get mono exception class");
            return;

        }

        MonoProperty* msgProperty = mono_class_get_property_from_name(klass, "Message");
        MonoProperty* stcProperty = mono_class_get_property_from_name(klass, "StackTrace");
        
        std::string msg;
        std::string stackTrace;

        MonoStringToString((MonoString*) mono_runtime_invoke(mono_property_get_get_method(msgProperty), exception, nullptr, nullptr), msg);
        MonoStringToString((MonoString*) mono_runtime_invoke(mono_property_get_get_method(stcProperty), exception, nullptr, nullptr), stackTrace);

        LogError("Unhandle exception has been caught: {}\n\nStack trace:\n{}", msg, stackTrace);

    }

}
