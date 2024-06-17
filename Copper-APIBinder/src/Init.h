#pragma once

#include "Base.h"

extern "C" {

    typedef struct _MonoDomain MonoDomain;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;

}

namespace APIBinder {

    MonoDomain* InitMono();
    MonoAssembly* LoadAssembly(MonoImage** outImage);

}
