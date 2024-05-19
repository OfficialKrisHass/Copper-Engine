#pragma once

#include <Engine/Core/Core.h>

extern "C" {

    typedef struct _MonoObject MonoObject;
    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {
    
    void ClearManagedReferences();

    MonoObject* ManagedReference(void* unmanagedPtr, MonoClass* classToCreate = nullptr);
    MonoObject* CreateManagedReference(void* unmanagedPtr, MonoClass* klass);

}
