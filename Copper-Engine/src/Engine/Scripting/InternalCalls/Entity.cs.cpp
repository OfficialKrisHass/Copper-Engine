#include "cupch.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

namespace Copper::Scripting {

    MonoString* get_name(MonoObject* entity) {

        CUP_FUNCTION();

        InternalEntity* ptr = nullptr;
        mono_field_get_value(entity, UnmanagedPtrField(), (void*) &ptr);
        
        CU_ASSERT(ptr, "Could not get Entity Unmanaged Pointer");
        return MonoUtils::StringToMonoString(ptr->name);

    }
    void set_name(MonoObject* entity, MonoString* value) {

        CUP_FUNCTION();

        std::string name;
        MonoUtils::MonoStringToString(value, name);

        InternalEntity* ptr = nullptr;
        mono_field_get_value(entity, UnmanagedPtrField(), (void*) &ptr);

        CU_ASSERT(ptr, "Could not get Entity Unmanaged Pointer");
        ptr->name = name;

    }

}
