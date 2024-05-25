#include "cupch.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

namespace Copper::Scripting::InternalCalls::ECS::Entity {

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

    MonoObject* get_transform(MonoObject* entity) {

        CUP_FUNCTION();

        InternalEntity* ptr = nullptr;
        mono_field_get_value(entity, UnmanagedPtrField(), (void*) &ptr);

        Transform* transform = ptr->GetTransform();
        MonoObject* ret = ManagedReference(transform);
        
        CU_ASSERT(ret, "Could not get Transform Managed reference from Entity");
        return ret;

    }

}
