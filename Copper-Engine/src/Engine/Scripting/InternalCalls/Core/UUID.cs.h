#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoString MonoString;
    typedef struct _MonoArray MonoArray;

}

namespace Copper::Scripting::UUID {

    void Constructor(MonoArray* data, uint64 x, uint64 y);

    void GenerateUUID(MonoArray* data);

    void SetBytes(MonoArray* data, MonoArray* bytes);

    void SetString(MonoArray* data, MonoString* string);
    MonoString* ToString(MonoArray* data);

    bool Equals(MonoArray* a, MonoArray* b);
    bool LessThan(MonoArray* a, MonoArray* b);

    int32 Hash(MonoArray* data);

}
