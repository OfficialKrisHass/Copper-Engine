#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Component {

    MonoObject* GetEntity(MonoObject* component);
    MonoObject* GetTransform(MonoObject* component);

}
