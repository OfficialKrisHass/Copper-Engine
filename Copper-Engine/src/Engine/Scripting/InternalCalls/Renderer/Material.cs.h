#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper::Scripting::Material {

    Color GetAlbedo(MonoObject* material);
    float GetTiling(MonoObject* material);

    void SetAlbedo(MonoObject* material, Color value);
    void SetTiling(MonoObject* material, float value);

}
