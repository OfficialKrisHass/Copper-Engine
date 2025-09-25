#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"
#include "Engine/Components/Collider.h"

COMPONENT_FORWARD_DECL()

namespace Copper {

    class CapsuleCollider : public Collider {

        COMPONENT_FRIEND_CLASSES(); 

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
