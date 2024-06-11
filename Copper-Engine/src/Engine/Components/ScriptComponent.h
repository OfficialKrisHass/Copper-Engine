#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

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
        
        void Setup(const Scripting::Script* script);

        void OnBegin() const;
        void OnUpdate() const;

        inline const Scripting::Script* GetScript() const { return m_script; }

        inline operator bool() const { return m_instance != nullptr; }

    private:
        const Scripting::Script* m_script = nullptr;
        MonoObject* m_instance = nullptr;

        MonoMethod* m_begin = nullptr;

        typedef void (*UpdateFunc)(MonoObject* instance, MonoException** exception);
        UpdateFunc m_update = nullptr;

        void CallBaseConstructor();

    };

}
