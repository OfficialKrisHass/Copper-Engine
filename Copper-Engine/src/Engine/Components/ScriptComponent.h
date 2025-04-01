#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#include "Engine/Scripting/ScriptingEngine.h"

extern "C" {

    typedef struct _MonoObject MonoObject;
    typedef struct _MonoException MonoException;
    typedef struct _MonoMethod MonoMethod;

}

namespace Copper {

    namespace Scripting { class Script; struct Field; }

    class ScriptComponent : public Component {

        friend Scripting::Field;

    public:
        ScriptComponent() = default;
        
        void Setup(const std::string& scriptName) {

            CUP_FUNCTION();

            const Scripting::Script* script = Scripting::GetScript(scriptName);
            if (script == nullptr) {

                Log("Can't setup script component with an invalid script ({})", scriptName);
                return;

            }

            Setup(script);

        }
        void Setup(const Scripting::Script* script);

        void OnBegin() const;
        void OnUpdate() const;

        inline const Scripting::Script* GetScript() const { return Scripting::GetScript(m_scriptName); }
        inline const std::string& GetScriptName() const { return m_scriptName; } 

        inline operator bool() const { return m_instance != nullptr; }

    private:
        std::string m_scriptName;
        MonoObject* m_instance = nullptr;

        MonoMethod* m_begin = nullptr;

        typedef void (*UpdateFunc)(MonoObject* instance, MonoException** exception);
        UpdateFunc m_update = nullptr;

        void CallBaseConstructor();

    };

}
