#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClassField MonoClassField;

}
namespace Copper { class ScriptComponent; }

namespace Copper::Scripting {

    struct Field {

    public:
        enum class Accessibility : uint8 {

            None = 0,

            Private = FLAG(0),
            Protected = FLAG(1),
            Internal = FLAG(2),
            Public = FLAG(3),

        };
        enum class Type : uint8 {

            None = 0,

            Int,
            UInt,
            Float,
            Double,

            Vector2,
            Vector3,
            Quaternion,

            Entity,
            Transform,

        };

        Field() = default;
        Field(MonoClassField* field);

        void GetValue(ScriptComponent* instance, void* out) const;
        void SetValue(ScriptComponent* instance, void* value) const;

        void GetRefValue(ScriptComponent* instance, void** out, void* none = nullptr) const;
        void SetRefValue(ScriptComponent* instance, void* value) const;
        
        inline Accessibility GetAccessibility() const { return m_accessibility; }
        inline Type GetType() const { return m_type; }
        inline const std::string& GetName() const { return m_name; }

        inline MonoClassField* GetField() const { return m_field; }

    private:
        Accessibility m_accessibility = Accessibility::None;
        Type m_type = Type::None;
        std::string m_name = "";

        MonoClassField* m_field = nullptr;

    };

}
