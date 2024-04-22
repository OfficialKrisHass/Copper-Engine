#include "cupch.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting {

    MonoObject* get_entity(MonoObject* component) {

        CUP_FUNCTION();

        Component* ptr = nullptr;
        mono_field_get_value(component, UnmanagedPtrField(), (void*) &ptr);

        // TODO: Temporary, Replace with storing references to C# data
        MonoClass* klass = mono_class_from_name(ScriptingAPIAssembly().GetImage(), "Copper", "Entity");
        MonoObject* entity = mono_object_new(AppDomain(), klass);

        InternalEntity* value = ptr->GetEntity();
        mono_field_set_value(entity, UnmanagedPtrField(), (void*) &value);
        
        return entity;

    }

}
