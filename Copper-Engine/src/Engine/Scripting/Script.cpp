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
        GetFields();

    }

    Script::Script(MonoClass* klass) {

        CUP_FUNCTION();

        if (klass == nullptr) {

            LogError("Can't create a script with a nullptr klass");

            m_class = nullptr;
            m_namespace.clear();
            m_name.clear();
            m_fields.clear();

            return;

        }

        m_class = klass;
        m_name = mono_class_get_name(m_class);
        m_namespace = RemoveNamespace(m_name);

        GetFields();

    }

    bool Script::IsSubclassOf(MonoClass* klass) const {

        CUP_FUNCTION();
        return mono_class_is_subclass_of(m_class, klass, false);

    }
    std::string Script::FullName() const {

        CUP_FUNCTION();

        if (m_class == nullptr) return "";
        return mono_class_get_name(m_class);

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

            if (attrInfo != nullptr && mono_custom_attrs_has_attr(attrInfo, Scripting::GetClass(Class::HideInEditorAttribute))) continue;
            switch (f.GetAccessibility()) {

            case Field::Accessibility::Public: m_fields.push_back(f); break;
            case Field::Accessibility::Private: {

                if (attrInfo == nullptr || !mono_custom_attrs_has_attr(attrInfo, Scripting::GetClass(Class::ShowInEditorAttribute))) break;

                m_fields.push_back(f);
                break;

            }

            }

        }

    }

}
