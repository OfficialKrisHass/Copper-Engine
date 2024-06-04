#pragma once

#define GET_UNMANAGED_PTR(type, name, instance) CU_ASSERT(instance, "Can not get {} Unmanaged pointer from nullptr C# instance", #type);\
                                                type name; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &name);\
                                                CU_ASSERT(name, "Could not get {} Unmanaged pointer from C# instance", #type);
