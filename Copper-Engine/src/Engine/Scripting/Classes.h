#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoClass MonoClass;

}

namespace Copper::Scripting {
    
    MonoClass* BaseClass();

    MonoClass* EntityClass();

    MonoClass* ComponentClass();
    MonoClass* TransformClass();

    MonoClass* CameraClass();
    MonoClass* LightClass();

    MonoClass* RigidBodyClass();
    MonoClass* BoxColliderClass();
    MonoClass* SphereColliderClass();
    MonoClass* CapsuleColliderClass();

    MonoClass* ShowInEditorAttributeClass();
    MonoClass* HideInEditorAttributeClass();

}
