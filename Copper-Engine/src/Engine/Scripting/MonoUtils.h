#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;
    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::MonoUtils {

    void MonoStringToString(MonoString* string, std::string& out);
    MonoString* StringToMonoString(const std::string& string);

    void PrintExceptionDetails(MonoObject* exception);

}
