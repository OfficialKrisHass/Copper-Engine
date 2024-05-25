#include "cupch.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::InternalCalls::Components::Component {

    typedef ::Copper::Component Component;
    
    MonoObject* get_entity(MonoObject* component) {

        CUP_FUNCTION();
        
        Component* ptr = nullptr;
        mono_field_get_value(component, UnmanagedPtrField(), (void*) &ptr);

        InternalEntity* entity = ptr->GetEntity();
        MonoObject* ret = ManagedReference(entity);

        CU_ASSERT(ret, "Could not get Managed Entity Reference from Component");
        return ret;

    }
    MonoObject* get_transform(MonoObject* component) {

        CUP_FUNCTION();

        Component* ptr = nullptr;
        mono_field_get_value(component, UnmanagedPtrField(), (void*) &ptr);

        Transform* transform = ptr->GetTransform();
        MonoObject* ret = ManagedReference(transform);
        
        CU_ASSERT(ret, "Could not get Managed Transform reference from Component");
        return ret;

    }

}
