#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoReflectionType MonoReflectionType;

}

namespace Copper::Scripting::Entity {

    void Initialize();

    MonoString* GetName(MonoObject* entity);
    void SetName(MonoObject* entity, MonoString* value);

    MonoObject* GetTransform(MonoObject* entity);

    MonoObject* AddComponent(MonoObject* entity, MonoReflectionType* type);
    MonoObject* GetComponent(MonoObject* entity, MonoReflectionType* type);
    bool HasComponent(MonoObject* entity, MonoReflectionType* type);
    void RemoveComponent(MonoObject* entity, MonoReflectionType* type);

}
