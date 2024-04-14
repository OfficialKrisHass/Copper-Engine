#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

extern "C" {

    typedef struct _MonoObject MonoObject;

}

namespace Copper {

    namespace Scripting { class Script; }

    class ScriptComponent : public Component {

    public:
        ScriptComponent() = default;
        
        void Setup(const Scripting::Script* script);

        inline const Scripting::Script* GetScript() const { return m_script; }

        inline operator bool() const { return m_instance != nullptr; }

    private:
        const Scripting::Script* m_script = nullptr;
        MonoObject* m_instance = nullptr;

        void CallBaseConstructor();

    };

}
