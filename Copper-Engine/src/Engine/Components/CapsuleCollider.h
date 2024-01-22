#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"
#include "Engine/Components/Collider.h"

namespace Copper {

    class CapsuleCollider : public Collider {

        friend Scene;

    public:
        float radius = 0.5f;
        float height = 1.0f;

    private:
        physx::PxShape* CreateShape() const override;

    };

}