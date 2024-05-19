#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {
    
    MonoClass* BaseClass();

    MonoClass* EntityClass();

    MonoClass* ComponentClass();
    MonoClass* TransformClass();

}
