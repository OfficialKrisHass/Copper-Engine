#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Collider.h"

COMPONENT_FORWARD_DECL()

namespace Copper {

    class SphereCollider : public Collider {

        COMPONENT_FRIEND_CLASSES();

    public:
        float GetRadius() const { return m_radius; }

        void SetRadius(float value) {

            CUP_FUNCTION();

            if (m_radius == value) return;
            m_radius = value;

            RecreateShape();
            
        }

    private:
        float m_radius = 0.5f;

        physx::PxShape* CreateShape() const override;

    };

}
