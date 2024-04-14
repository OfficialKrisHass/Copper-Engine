#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {

    struct Assembly;
    
    class Script {

    public:
        Script() = default;
        Script(const std::string& nameSpace, const std::string& name, const Assembly& assembly);
        Script(const std::string& fullName, const Assembly& assembly);

        static std::string RemoveNamespace(std::string& name) {

            size_t dot = name.find_last_of('.');

            std::string ret = name.substr(0, dot);
            name.erase(0, dot);
            return ret;

        }

        bool IsSubclassOf(const Script& script) const;

        inline const std::string& Namespace() const { return m_namespace; }
        inline const std::string& Name() const { return m_name; }
        inline std::string FullName() const {

            if (m_namespace.empty()) return m_name;
            return fmt::format("{}.{}", m_namespace, m_name);

        }

        inline MonoClass* GetClass() const { return m_class; }

    private:
        std::string m_namespace = "";
        std::string m_name = "";

        MonoClass* m_class = nullptr;

        void GetClass(const Assembly& assembly);

    };

}
