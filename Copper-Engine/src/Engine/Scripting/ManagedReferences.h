#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Classes.h"

extern "C" {

    typedef struct _MonoObject MonoObject;
    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {

    void ClearManagedReferences();

    MonoObject* GetManagedReference(void* unmanagedPtr, Class klass);
    MonoObject* GetManagedReference(void* unmanagedPtr, MonoClass* klass);
    MonoObject* GetManagedReference(void* unmanagedPtr);

    MonoObject* CreateManagedReference(void* unmanagedPtr, Class klass);
    MonoObject* CreateManagedReference(void* unmanagedPtr, MonoClass* klass);

    void RemoveManagedReference(void* unmanagedPtr);

}
