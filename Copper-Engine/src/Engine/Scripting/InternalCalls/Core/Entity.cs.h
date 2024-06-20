#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;
    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Entity {

    void Initialize();

    MonoString* GetName(MonoObject* entity);
    void SetName(MonoObject* entity, MonoString* value);

}
