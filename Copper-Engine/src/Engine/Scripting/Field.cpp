#include "cupch.h"
#include "Field.h"

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

        return Field::Type::None;

    }

}
