#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

    class BoxCollider : public Component {

    public:
        bool trigger;
        
        Vector3 center;
        Vector3 size;

    private:
        //

    };
    
}