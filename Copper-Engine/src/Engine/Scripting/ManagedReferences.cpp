#include "cupch.h"
#include "ManagedReferences.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

namespace Copper::Scripting {

    struct ManagedReference {

        MonoObject* reference = nullptr;
#ifdef CU_DEBUG
        MonoClass* klass = nullptr;

        ManagedReference(MonoObject* reference, MonoClass* klass) : reference(reference), klass(klass) {}
#endif

        ManagedReference(MonoObject* reference) : reference(reference) {}

    };
    std::unordered_map<void*, ManagedReference> referenceMap;

    void ClearManagedReferences() {

        CUP_FUNCTION();

        referenceMap.clear();

    }

    MonoObject* GetManagedReference(void* unmanagedPtr, Class klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != Class::None, "Can't get managed reference of invalid class (Class::None).");
        return GetManagedReference(unmanagedPtr, GetClass(klass));

    }
    MonoObject* GetManagedReference(void* unmanagedPtr, MonoClass* klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != nullptr, "Can't get managed reference of invalid class (nullptr).");

        const auto it = referenceMap.find(unmanagedPtr);
        if (it == referenceMap.end())
            return CreateManagedReference(unmanagedPtr, klass);

        CU_ASSERT(it->second.klass == klass, "Retrieved Managed Reference does not match klass. Asked: '{}', retrieved: '{}'", mono_class_get_name(klass), mono_class_get_name(it->second.klass));
        return it->second.reference;

    }
    MonoObject* GetManagedReference(void* unmanagedPtr) {

        CUP_FUNCTION();

        const auto it = referenceMap.find(unmanagedPtr);
        if (it == referenceMap.end()) return nullptr;

        return it->second.reference;

    }

    MonoObject* CreateManagedReference(void* unmanagedPtr, Class klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != Class::None, "Can't create managed reference from invalid class (Class::None).");
        return CreateManagedReference(unmanagedPtr, GetClass(klass));


    }
    MonoObject* CreateManagedReference(void* unmanagedPtr, MonoClass* klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != nullptr, "Can't create managed reference from invalid class (nullptr).");

        MonoObject* ret = mono_object_new(AppDomain(), klass);
        mono_field_set_value(ret, UnmanagedPtrField(), (void*) &unmanagedPtr);

#ifdef CU_DEBUG
        referenceMap.emplace(std::piecewise_construct, std::forward_as_tuple(unmanagedPtr), std::forward_as_tuple(ret, klass));
#elif CU_RELEASE
        referenceMap.emplace(std::piecewise_construct, std::forward_as_tuple(unmanagedPtr), std::forward_as_tuple(ret));
#endif

        return ret;

    }

    void RemoveManagedReference(void* unmanagedPtr) {

        CUP_FUNCTION();

        const auto it = referenceMap.find(unmanagedPtr);
        if (it == referenceMap.end()) return;

        referenceMap.erase(it);

    }

}
