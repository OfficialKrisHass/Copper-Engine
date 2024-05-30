#include "cupch.h"
#include "Script.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/Classes.h"

#include <mono/metadata/class.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/reflection.h>

namespace Copper::Scripting {

    Script::Script(const std::string& nameSpace, const std::string& name, const Assembly& assembly) {

        CUP_FUNCTION();

        m_namespace = nameSpace;
        m_name = name;

        GetClass(assembly);
        GetFields();

    }
    Script::Script(const std::string& fullName, const Assembly& assembly) {

        CUP_FUNCTION();

        m_name = fullName;
        m_namespace = RemoveNamespace(m_name);

        GetClass(assembly);

    }

    bool Script::IsSubclassOf(MonoClass* klass) const {

        CUP_FUNCTION();
        return mono_class_is_subclass_of(m_class, klass, false);

    }

    void Script::GetClass(const Assembly& assembly) {

        CUP_FUNCTION();

        m_class = mono_class_from_name_case(assembly.m_image, m_namespace.c_str(), m_name.c_str());
        if (m_class) return;

        LogError("Failed to get Script mono class.\n\tScript name: {}.{}", m_namespace, m_name);

        
    }
    void Script::GetFields() {

        CUP_FUNCTION();

        void* iter = nullptr;
        MonoClassField* field = nullptr;
        while (field = mono_class_get_fields(m_class, &iter)) {

            Field f = Field(field);
            MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(m_class, field);

            // Add if Public and no HideInEditor attribute
            if (f.GetAccessibility() == Field::Accessibility::Public) {

                if (attrInfo != nullptr && mono_custom_attrs_has_attr(attrInfo, HideInEditorAttributeClass())) continue;
                m_fields.push_back(f);

                continue;

            }

            // Add if ShowInEditor attribute

            if (attrInfo == nullptr || !mono_custom_attrs_has_attr(attrInfo, ShowInEditorAttributeClass())) continue;
            m_fields.push_back(f);

        }

    }

}
