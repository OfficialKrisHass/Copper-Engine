#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Field.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper { class ScriptComponent; }

namespace Copper::Scripting {

    struct Assembly;
    
    class Script {

        friend ScriptComponent;

    public:
        Script() = default;
        Script(const std::string& nameSpace, const std::string& name, const Assembly& assembly);
        Script(const std::string& fullName, const Assembly& assembly);

        Script(MonoClass* klass);

        static std::string RemoveNamespace(std::string& name) {

            size_t dot = name.find_last_of('.');

            std::string ret = name.substr(0, dot);
            name.erase(0, dot);
            return ret;

        }

        bool IsSubclassOf(const Script& script) const;
        bool IsSubclassOf(MonoClass* klass) const;

        inline const std::string& Namespace() const { return m_namespace; }
        inline const std::string& Name() const { return m_name; }
        std::string FullName() const;

        inline MonoClass* GetClass() const { return m_class; }

        inline const std::vector<Field>& GetFields() const { return m_fields; }

        inline operator bool() const { return m_class != nullptr; }

    private:
        std::string m_namespace = "";
        std::string m_name = "";

        MonoClass* m_class = nullptr;
        
        std::vector<Field> m_fields;

        void GetClass(const Assembly& assembly);
        void GetFields();

    };

}
