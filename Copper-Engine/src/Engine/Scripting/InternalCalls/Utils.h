#pragma once

#define GET_UNMANAGED_COMPONENT_PTR(type, name, instance) \
    CU_ASSERT(instance != nullptr, "Can not get {} unmanaged pointer from nullptr C# instance", #type);\
    \
    type* name = nullptr; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &name);\
    CU_ASSERT(name != nullptr, "Could not get {} unmanaged pointer from C# instance", #type);\
    \
    if (!name->GetEntity().IsValid() && name->GetEntity().ID() != INVALID_ENTITY_ID) {\
        LogError("Entity {} (ID) has been deleted or is invalid, but is still being accessed.", name->GetEntity().ID());\
        Profiler::PopTopScope();\
        mono_raise_exception(mono_get_exception_null_reference());\
    }
