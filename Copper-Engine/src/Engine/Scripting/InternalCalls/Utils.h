#pragma once

#define GET_UNMANAGED_COMPONENT_PTR(type, name, instance) \
    CU_ASSERT(instance != nullptr, "Can not get {} unmanaged pointer from nullptr C# instance", #type);\
    \
    type* name = nullptr; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &name);\
    CU_ASSERT(name != nullptr, "Could not get {} unmanaged pointer from C# instance", #type);\
    \
    if (!name->GetEntity().IsValid() && name->GetEntity().ID() != INVALID_ENTITY_ID) {\
        LogError("Entity {} (ID) has been deleted or is invalid, but is still being accessed.", name->GetEntity().ID());\
        CUP_POP_TOP();\
        mono_raise_exception(mono_get_exception_null_reference());\
    }

#define GET_UNMANAGED_ENTITY(name, instance) \
    CU_ASSERT(instance != nullptr, "Can not get Entity unmanaged pointer from nullptr C# instance.");\
    \
    uint64 id = INVALID_ENTITY_ID; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &id);\
    CU_ASSERT(id < INVALID_ENTITY_ID, "Got an invalid ID from the Entity unmanaged pointer of a C# Entity instance.");\
    \
    InternalEntity* name = GetEntityFromID((uint32) id);\
    if (name == nullptr && id != INVALID_ENTITY_ID) {\
        LogError("Entity {} (ID) has been deleted or is invalid, but is still being accessed.", id);\
        CUP_POP_TOP();\
        mono_raise_exception(mono_get_exception_null_reference());\
    }
