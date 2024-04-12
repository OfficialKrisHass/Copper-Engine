#include "cupch.h"
#include "Script.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/class.h>

namespace Copper::Scripting {

    Script::Script(const std::string& nameSpace, const std::string& name, const Assembly& assembly) {

        CUP_FUNCTION();

        m_namespace = nameSpace;
        m_name = name;

        GetClass(assembly);

    }
    Script::Script(const std::string& fullName, const Assembly& assembly) {

        CUP_FUNCTION();

        m_name = fullName;
        m_namespace = RemoveNamespace(m_name);

        GetClass(assembly);

    }

    bool Script::IsSubclassOf(const Script& script) const {

        CUP_FUNCTION();
        return mono_class_is_subclass_of(m_class, script.m_class, false);

    }

    void Script::GetClass(const Assembly& assembly) {

        CUP_FUNCTION();

        m_class = mono_class_from_name_case(assembly.m_image, m_namespace.c_str(), m_name.c_str());
        if (m_class) return;

        LogError("Failed to get Script mono class.\n\tScript name: {}.{}", m_namespace, m_name);

        
    }

}
