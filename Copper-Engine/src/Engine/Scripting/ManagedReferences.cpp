#include "cupch.h"
#include "ManagedReferences.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

namespace Copper::Scripting {

    struct ManagedReference {

        MonoObject* reference = nullptr;
        Class klass = Class::None;

        ManagedReference(MonoObject* reference, Class klass) : reference(reference), klass(klass) {}

    };
    std::unordered_map<void*, ManagedReference> referenceMap;

    void RefreshManagedReferences() {

        CUP_FUNCTION();

        for (auto& it : referenceMap) {

            ManagedReference& reference = it.second;
            CU_ASSERT(reference.klass != Class::None, "Managed reference has no class associated with it!");

            reference.reference =  mono_object_new(AppDomain(), GetClass(reference.klass));
            mono_field_set_value(reference.reference, UnmanagedPtrField(), (void*) &it.first);

        }

    }
    void ClearManagedReferences() {

        CUP_FUNCTION();

        referenceMap.clear();

    }

    MonoObject* GetManagedReference(void* unmanagedPtr, Class klass) {

        CUP_FUNCTION();

        CU_ASSERT(klass != Class::None, "Can't get managed reference of invalid class (Class::None).");

        const auto it = referenceMap.find(unmanagedPtr);
        if (it == referenceMap.end())
            return CreateManagedReference(unmanagedPtr, klass);

        CU_ASSERT(it->second.klass == klass, "Retrieved Managed Reference does not match klass. Asked: '{}', retrieved: '{}'", ClassToString(klass), ClassToString(it->second.klass));
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

        MonoObject* ret = mono_object_new(AppDomain(), GetClass(klass));
        mono_field_set_value(ret, UnmanagedPtrField(), (void*) &unmanagedPtr);

        referenceMap.emplace(std::piecewise_construct, std::forward_as_tuple(unmanagedPtr), std::forward_as_tuple(ret, klass));

        return ret;

    }

    void RemoveManagedReference(void* unmanagedPtr) {

        CUP_FUNCTION();

        const auto it = referenceMap.find(unmanagedPtr);
        if (it == referenceMap.end()) return;

        referenceMap.erase(it);

    }

}
