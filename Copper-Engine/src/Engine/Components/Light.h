﻿#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

namespace Copper {

    class Light : public Component {

    public:
        Color color;
        float intensity = 1.0f;

    private:
        //
        
    };
    
}