#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"
#include "Engine/Components/Collider.h"

#ifdef CU_EDITOR
namespace YAML { class Emitter; }
namespace Editor {

    class Properties;
    namespace SceneSerializer {

        void SerializeEntityComponents(Copper::InternalEntity* entity, YAML::Emitter& out);
        void DeserializeEntityComponents(Copper::InternalEntity* entity, const YAML::Node& data);

    }

}
#endif

namespace Copper {

    class CapsuleCollider : public Collider {

        friend Scene;

#ifdef CU_EDITOR
        friend Editor::Properties;
        friend void Editor::SceneSerializer::SerializeEntityComponents(Copper::InternalEntity *entity, YAML::Emitter &out);
        friend void Editor::SceneSerializer::DeserializeEntityComponents(Copper::InternalEntity *entity, const YAML::Node &data);
#endif

    public:
        // Getters

        float GetRadius() const { return m_radius; }
        float GetHeight() const { return m_height; }

        void SetRadius(float value) {

            CUP_FUNCTION();

            if (m_radius == value) return;
            m_radius = value;

            RecreateShape();

        }
        void SetHeight(float value) {

            CUP_FUNCTION();

            if (m_height == value) return;
            m_height = value;

            RecreateShape();

        }

    private:
        float m_radius = 0.5f;
        float m_height = 1.0f;

        physx::PxShape* CreateShape() const override;

    };

}
