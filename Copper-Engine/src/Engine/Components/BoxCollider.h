#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Collider.h"

namespace Copper {

    class BoxCollider : public Collider {

        friend Scene;

    public:
        Vector3 center = Vector3::zero;
        Vector3 size = Vector3::one;

    private:
        void Setup();

    };
    
}