#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {

    enum class Class : uint8 {

        Base = 0,

        Entity,
        Component,
        Transform,

        Camera,
        Light,

        RigidBody,
        BoxCollider,
        SphereCollider,
        CapsuleCollider,

        ShowInEditorAttribute,
        HideInEditorAttribute,

        None,
        COUNT = None,

    };

    void InitializeClasses();

    MonoClass* GetClass(Class klass);
    
}
