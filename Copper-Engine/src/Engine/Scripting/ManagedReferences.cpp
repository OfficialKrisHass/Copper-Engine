#include "cupch.h"
#include "ManagedReferences.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting {

    std::unordered_map<void*, MonoObject*> references;

    void ClearManagedReferences() {

        CUP_FUNCTION();

        references.clear();

    }

    MonoObject* ManagedReference(void* unmanagedPtr, MonoClass* klass) {

        CUP_FUNCTION();

        if (references.find(unmanagedPtr) == references.end()) {

            if (klass == nullptr) return nullptr;
            return CreateManagedReference(unmanagedPtr, klass);

        }

        return references.at(unmanagedPtr);

    }
    MonoObject* CreateManagedReference(void* unmanagedPtr, MonoClass* klass) {

        CUP_FUNCTION();

        if (klass == nullptr) return nullptr;

        MonoObject* ret = mono_object_new(AppDomain(), klass);
        mono_field_set_value(ret, UnmanagedPtrField(), (void*) &unmanagedPtr);

        references[unmanagedPtr] = ret;
        return ret;

    }
    void RemoveManagedReference(void* unmanagedPtr) {

        CUP_FUNCTION();

        if (references.find(unmanagedPtr) == references.end()) return;
        references.erase(unmanagedPtr);

    }

}
