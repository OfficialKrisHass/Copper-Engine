#include "cupch.h"
#include "Field.h"

#include "Engine/Core/UUID.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"

#include "Engine/Components/ScriptComponent.h"

#include <mono/jit/jit.h>
#include <mono/metadata/attrdefs.h>

namespace Copper::Scripting {

    Field::Accessibility FieldAccessibility(MonoClassField* field);
    Field::Type FieldType(MonoType* type);

    Field::Field(MonoClassField* field) {

        CUP_FUNCTION();

        if (field == nullptr) return;

        m_field = field;

        m_accessibility = FieldAccessibility(m_field);
        m_type = FieldType(mono_field_get_type(m_field));
        m_name = mono_field_get_name(m_field);

    }

    void Field::GetValue(ScriptComponent* instance, void* out) const {

        CUP_FUNCTION();

        CU_ASSERT(out != nullptr, "Can't get field value with nullptr out parameter");
        CU_ASSERT(instance != nullptr, "Can't get field value on nullptr instance");

        mono_field_get_value(instance->m_instance, m_field, out);

    }
    void Field::SetValue(ScriptComponent* instance, void* value) const {

        CUP_FUNCTION();

        CU_ASSERT(instance != nullptr, "Can't set field value on nullptr instance");

        mono_field_set_value(instance->m_instance, m_field, value);

    }

    void Field::GetRefValue(ScriptComponent* instance, void** out, void* none) const {

        CUP_FUNCTION();

        CU_ASSERT(out != nullptr, "Can't get reference field value with nullptr out parameter");

        MonoObject* tmp = nullptr;
        GetValue(instance, &tmp);
        
        if (tmp == nullptr) {

            *out = none;
            return;

        }

        mono_field_get_value(tmp, UnmanagedPtrField(), out);

    }
    void Field::SetRefValue(ScriptComponent* instance, void* value) const {

        CUP_FUNCTION();

        MonoObject* tmp = GetManagedReference(value);
        CU_ASSERT(tmp != nullptr, "Could not get Managed reference in order to set ref field");

        SetValue(instance, tmp);

    }

    void Field::GetAssetValue(ScriptComponent* instance, UUID* out) const {

        CUP_FUNCTION();

        MonoObject* asset = nullptr;
        GetValue(instance, &asset);

        if (asset == nullptr) {

            *out = UUID::GetNil();
            return;

        }

        MonoArray* uuidBytes = nullptr;
        mono_field_get_value(asset, AssetUUIDField(), &uuidBytes);

        CU_ASSERT(mono_array_length(uuidBytes) == 16, "Invalid uuid bytes array retrieved from asset m_uuid field.");
        *out = mono_array_addr(uuidBytes, uint8, 0);

    }
    void Field::SetAssetValue(ScriptComponent* instance, const UUID& value) const {

        CUP_FUNCTION();

        MonoClass* klass = nullptr;
        switch (m_type) {

            case Field::Type::Material: klass = GetClass(Class::Material); break;
            default: {

                LogError("A non asset field can't call Field::SetAssetValue().");
                return;

            }

        }

        // We have to create a new instance of the asset, get it's uuid byte array, copy value into it and finally set the field
        // Since both UUID in C# and C++ are nothing more than an array of 16 bytes, we can memcpy into it, it is dangerous but whateva.

        MonoObject* asset = mono_object_new(AppDomain(), klass);
        CU_ASSERT(asset != nullptr, "Could not create new managed asset.");

        MonoMethod* ctor = mono_class_get_method_from_name(klass, ".ctor", 0);
        CU_ASSERT(ctor != nullptr, "Could not get assets constructor.");
        mono_runtime_invoke(ctor, asset, nullptr, nullptr);

        MonoArray* uuidBytes = nullptr;
        mono_field_get_value(asset, AssetUUIDField(), &uuidBytes);

        CU_ASSERT(uuidBytes != nullptr, "Could not get uuid bytes array from Asset.m_uuid field.");
        CU_ASSERT(mono_array_length(uuidBytes) == 16, "Invalid array retrieved from Asset.m_uuid field.");

        memcpy(mono_array_addr(uuidBytes, uint8, 0), &value, 16);
        SetValue(instance, asset);

    }

    Field::Accessibility FieldAccessibility(MonoClassField* field) {

        CUP_FUNCTION();

        uint32_t accessibility = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;
        switch (accessibility) {

            case MONO_FIELD_ATTR_PRIVATE: return Field::Accessibility::Private;
            case MONO_FIELD_ATTR_FAMILY: return Field::Accessibility::Protected;
            case MONO_FIELD_ATTR_ASSEMBLY: return Field::Accessibility::Internal;
            case MONO_FIELD_ATTR_PUBLIC: return Field::Accessibility::Public;

            default: return Field::Accessibility::Private;

        }

        return Field::Accessibility::None;

    }
    Field::Type FieldType(MonoType* type) {

        CUP_FUNCTION();

        std::string name = mono_type_get_name(type);
        if (name == "System.Int32") return Field::Type::Int;
        else if (name == "System.UInt32") return Field::Type::UInt;
        else if (name == "System.Single") return Field::Type::Float;
        else if (name == "System.Double") return Field::Type::Double;

        else if (name == "Copper.Vector2") return Field::Type::Vector2;
        else if (name == "Copper.Vector3") return Field::Type::Vector3;
        else if (name == "Copper.Quaternion") return Field::Type::Quaternion;

        else if (name == "Copper.Entity") return Field::Type::Entity;
        else if (name == "Copper.Transform") return Field::Type::Transform;

        else if (name == "Copper.Material") return Field::Type::Material;

        return Field::Type::None;

    }

}
