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
        Collider,
        BoxCollider,
        SphereCollider,
        CapsuleCollider,

        Asset,
        Texture,
        Material,

        ShowInEditorAttribute,
        HideInEditorAttribute,

        None,
        COUNT = None,

    };

    void InitializeClasses();

    MonoClass* GetClass(Class klass);
    Class MonoClassToClass(MonoClass* klass);

    inline constexpr const char* ClassToString(Class klass) {

        switch (klass) {

            case Class::Base: return "Base";

            case Class::Entity: return "Entity";
            case Class::Component: return "Component";
            case Class::Transform: return "Transform";

            case Class::Camera: return "Camera";
            case Class::Light: return "Camera";

            case Class::RigidBody: return "RigidBody";
            case Class::Collider: return "Collider";
            case Class::BoxCollider: return "BoxCollider";
            case Class::SphereCollider: return "SphereCollider";
            case Class::CapsuleCollider: return "CapsuleCollider";

            case Class::Asset: return "Asset";
            case Class::Texture: return "Texture";
            case Class::Material: return "Material";

            case Class::ShowInEditorAttribute: return "ShowInEditor attribute";
            case Class::HideInEditorAttribute: return "HideIneditor attribute";

            default: break;

        }

        return "";

    }
    
}
