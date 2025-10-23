#include "cupch.h"
#include "MeshRenderer.cs.h"
#include "Engine/Components/MeshRenderer.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::MeshRenderer {

    typedef ::Copper::MeshRenderer MeshRenderer;

    MonoObject* GetMaterial(MonoObject* instance) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(MeshRenderer, ptr, instance);

        // We have to construct the material and set it's uuid
        // This feels a bit weird but whatever.

        MonoObject* ret = mono_object_new(AppDomain(), GetClass(Class::Material));
        MonoMethod* constructor = mono_class_get_method_from_name(GetClass(Class::Material), ".ctor", 0);
        CU_ASSERT(constructor != nullptr, "Could not get the default constructor from material.");

        mono_runtime_invoke(constructor, ret, nullptr, nullptr);

        MonoArray* uuidBytes = nullptr;
        mono_field_get_value(ret, AssetUUIDField(), &uuidBytes);

        CU_ASSERT(uuidBytes != nullptr, "Could not get uuid bytes array from Material instance.");
        CU_ASSERT(mono_array_length(uuidBytes) == 16, "Byte array retrieved from Material instance is invalid.");

        memcpy(mono_array_addr(uuidBytes, uint8, 0), &ptr->material, 16);

        return ret;

    }
    void SetMaterial(MonoObject* instance, MonoObject* value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(MeshRenderer, ptr, instance);

        if (value == nullptr) {

            Log("Setting material to none.");
            ptr->material = Material::WhiteMaterial();

            return;

        }

        MonoArray* uuidBytes = nullptr;
        mono_field_get_value(value, AssetUUIDField(), &uuidBytes);

        CU_ASSERT(uuidBytes != nullptr, "Could not get uuid bytes array from Material instance.");
        CU_ASSERT(mono_array_length(uuidBytes) == 16, "Byte array retrieved from Material instance is invalid.");
        
        ptr->material = mono_array_addr(uuidBytes, uint8, 0);

    }

}
