#include "cupch.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

#define GET_ENTITY(name, instance) GET_UNMANAGED_PTR(uint64, id, instance); InternalEntity* name = GetEntityFromID(id); CU_ASSERT(name, "Could not get Unmanaged entity from ID '{}' got from C# instance", id)

namespace Copper::Scripting::InternalCalls::ECS::Entity {

    MonoString* get_name(MonoObject* entity) {

        CUP_FUNCTION();

        GET_ENTITY(ptr, entity);
        return MonoUtils::StringToMonoString(ptr->name);

    }
    void set_name(MonoObject* entity, MonoString* value) {

        CUP_FUNCTION();

        std::string name;
        MonoUtils::MonoStringToString(value, name);

        GET_ENTITY(ptr, entity);
        ptr->name = name;

    }

    MonoObject* get_transform(MonoObject* entity) {

        CUP_FUNCTION();

        GET_ENTITY(ptr, entity);
        Transform* transform = ptr->GetTransform();
        MonoObject* ret = ManagedReference(transform);
        
        CU_ASSERT(ret, "Could not get Transform Managed reference from Entity");
        return ret;

    }

    bool HasComponent(MonoObject* entity, MonoReflectionType* type) {

        CUP_FUNCTION();

        // Get Component ID

        MonoClass* klass = mono_type_get_class(mono_reflection_type_get_type(type));
        MonoMethod* method = mono_class_get_method_from_name(klass, "ComponentID", 0);
        CU_ASSERT(method, "Could not get ComponentID method from component class '{}'", mono_class_get_name(klass));

        int32 cID = *(int32*) mono_object_unbox(mono_runtime_invoke(method, nullptr, nullptr, nullptr));

        // Return

        GET_ENTITY(ptr, entity);
        return ptr->HasComponent(cID);

    }

}
