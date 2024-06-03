#pragma once

#define GET_UNMANAGED_PTR(type, instance) CU_ASSERT(instance, "Can not get {} Unmanaged pointer from nullptr C# instance", #type);\
                                          type* ptr = nullptr; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &ptr);\
                                          CU_ASSERT(ptr, "Could not get {} Unmanaged pointer from C# instance", #type);
