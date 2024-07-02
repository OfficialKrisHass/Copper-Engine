#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {
    
    MonoClass* BaseClass();

    template<typename T> MonoClass* GetMonoClass();

    MonoClass* ShowInEditorAttributeClass();
    MonoClass* HideInEditorAttributeClass();

}
