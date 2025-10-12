#include "cupch.h"
#include "Entity.cs.h"
#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

#define GET_ENTITY(name, instance) \
    CU_ASSERT(instance != nullptr, "Can not get Entity unmanaged pointer from nullptr C# instance.");\
    \
    uint64 id = INVALID_ENTITY_ID; mono_field_get_value(instance, UnmanagedPtrField(), (void*) &id);\
    CU_ASSERT(id < INVALID_ENTITY_ID, "Got an invalid ID from the Entity unmanaged pointer of a C# Entity instance.");\
    \
    InternalEntity* name = GetEntityFromID((uint32) id);\
    if (name == nullptr && id != INVALID_ENTITY_ID) {\
        LogError("Entity {} (ID) has been deleted or is invalid, but is still being accessed.", id);\
        Profiler::PopTopScope();\
        mono_raise_exception(mono_get_exception_null_reference());\
    }

namespace Copper::Scripting::Entity {

    std::unordered_map<std::string, std::function<void*(InternalEntity*)>> addComponentFuncs;

    void Initialize() {

        CUP_FUNCTION();

        addComponentFuncs["Copper.Transform"] = [](InternalEntity* entity) { return entity->GetTransform(); };
        addComponentFuncs["Copper.Camera"] = [](InternalEntity* entity) { return entity->AddComponent<Camera>(); };
        addComponentFuncs["Copper.Light"] = [](InternalEntity* entity) { return entity->AddComponent<Light>(); };
        addComponentFuncs["Copper.RigidBody"] = [](InternalEntity* entity) { return entity->AddComponent<RigidBody>(); };

    }

    MonoString* GetName(MonoObject* entity) {

        CUP_FUNCTION();

        GET_ENTITY(ptr, entity);
        return MonoUtils::StringToMonoString(ptr->name);

    }
    void SetName(MonoObject* entity, MonoString* value) {

        CUP_FUNCTION();

        std::string name;
        MonoUtils::MonoStringToString(value, name);

        GET_ENTITY(ptr, entity);
        ptr->name = name;

    }

    MonoObject* GetTransform(MonoObject* entity) {

        CUP_FUNCTION();

        GET_ENTITY(ptr, entity);
        Transform* transform = ptr->GetTransform();

        MonoObject* ret = GetManagedReference(transform, Class::Transform);
        CU_ASSERT(ret, "Could not get Transform Managed reference from Entity");

        return ret;

    }

    MonoObject* AddComponent(MonoObject* entity, MonoReflectionType* type) {

        CUP_FUNCTION();

        GET_ENTITY(ptr, entity);

        MonoType* managedType = mono_reflection_type_get_type(type);
        std::string typeName = mono_type_get_name(managedType);

        // Built in component (Transform, BoxCollider, Camera, etc)

        if (addComponentFuncs.find(typeName) != addComponentFuncs.end()) {

            void* comp = addComponentFuncs.at(typeName)(ptr);
            MonoObject* ret = GetManagedReference(comp);
            CU_ASSERT(ret, "Could not get Managed Reference after Adding component '{}' to entity '{}'", typeName, *ptr);

            return ret;

        }

        // Script components

        if (ptr->HasComponent<ScriptComponent>()) return nullptr;

        const ScriptMap& scriptMap = ComponentScripts();
        CU_ASSERT(scriptMap.find(typeName)!= scriptMap.end(), "Script component '{}' could not be found in the script map while trying to add to Entity '{}'", typeName, *ptr);
        const Script* script = &scriptMap.at(typeName);

        ScriptComponent* scriptComponent = ptr->AddComponent<ScriptComponent>();
        scriptComponent->Setup(script);

        MonoObject* ret = CreateManagedReference(scriptComponent, MonoClassToClass(mono_type_get_class(managedType)));
        CU_ASSERT(ret, "Could not Create Managed reference for a Script Component '{}'", typeName);

        return ret;

    }
    MonoObject* GetComponent(MonoObject* entity, MonoReflectionType* type) {

        CUP_FUNCTION();

        // Get Component ID

        MonoClass* klass = mono_type_get_class(mono_reflection_type_get_type(type));
        MonoMethod* method = mono_class_get_method_from_name(klass, "ComponentID", 0);
        CU_ASSERT(method, "Could not get ComponentID method from component class '{}'", mono_class_get_name(klass));

        int32 cID = *(int32*) mono_object_unbox(mono_runtime_invoke(method, nullptr, nullptr, nullptr));

        // Get Component

        GET_ENTITY(ptr, entity);
        
        void* component = nullptr;
        if (cID == Registry::GetCID<Collider>())
            component = ptr->GetComponent<Collider>();
        else
            component = ptr->GetComponent(cID);

        if (component == nullptr)
            return nullptr;

        MonoObject* ret = GetManagedReference(component);
        CU_ASSERT(ret != nullptr, "Could not get Component (cID '{}') Managed reference", cID);

        return ret;

    }
    bool HasComponent(MonoObject* entity, MonoReflectionType* type) {

        CUP_FUNCTION();

        // Get Component ID

        MonoClass* klass = mono_type_get_class(mono_reflection_type_get_type(type));
        MonoMethod* method = mono_class_get_method_from_name(klass, "ComponentID", 0);
        CU_ASSERT(method, "Could not get ComponentID method from component class '{}'", mono_class_get_name(klass));

        int32 cID = *(int32*) mono_object_unbox(mono_runtime_invoke(method, nullptr, nullptr, nullptr));

        if (cID == TRANSFORM_CID) return true;

        // Return

        GET_ENTITY(ptr, entity);
        return ptr->HasComponent(cID);

    }
    void RemoveComponent(MonoObject* entity, MonoReflectionType* type) {

        CUP_FUNCTION();

        // Get Component ID

        MonoClass* klass = mono_type_get_class(mono_reflection_type_get_type(type));
        MonoMethod* method = mono_class_get_method_from_name(klass, "ComponentID", 0);
        CU_ASSERT(method, "Could not get ComponentID method from component class '{}'", mono_class_get_name(klass));

        int32 cID = *(int32*) mono_object_unbox(mono_runtime_invoke(method, nullptr, nullptr, nullptr));

        if (cID == TRANSFORM_CID) return;
        
        // Return

        GET_ENTITY(ptr, entity);
        ptr->RemoveComponent(cID);

    }

}
