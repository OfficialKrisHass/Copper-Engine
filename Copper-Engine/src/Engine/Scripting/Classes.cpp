#include "cupch.h"
#include "Classes.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/class.h>

#define SET_CLASS(var, name) classes.var = mono_class_from_name(ScriptingAPIAssembly().GetImage(), "Copper", name);\
    CU_ASSERT(classes.var, "Could not get Copper.{} class", name)

#define GET_CLASS_FUNC(name, var) MonoClass* name ## Class() { CUP_FUNCTION();\
    CU_ASSERT(classes.var, "{} Class reference is nullptr. Make Sure you assigned it in InitializeClasses()", #name);\
    return classes.var; }

#define GET_TMPL_CLASS_FUNC(type, var) template<> MonoClass* GetMonoClass<type>() { CUP_FUNCTION();\
    CU_ASSERT(classes.var, "{} is nullptr, make sure you assigned it in InitializeClasses()", #var);\
    return classes.var; }

namespace Copper {

    class InternalEntity;
    class Component;
    class Transform;

    class Camera;
    class Light;

    class RigidBody;
    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;

}

namespace Copper::Scripting {

    struct Classes {

        MonoClass* baseClass = nullptr;

        MonoClass* entityClass = nullptr;
        MonoClass* componentClass = nullptr;
        MonoClass* transformClass = nullptr;

        MonoClass* cameraClass = nullptr;
        MonoClass* lightClass = nullptr;

        MonoClass* rigidBodyClass = nullptr;
        MonoClass* boxColliderClass = nullptr;
        MonoClass* sphereColliderClass = nullptr;
        MonoClass* capsuleColliderClass = nullptr;

        MonoClass* showInEditorAttrClass = nullptr;
        MonoClass* hideInEditorAttrClass = nullptr;

    };
    Classes classes;

    void InitializeClasses() {

        SET_CLASS(baseClass, "Base");

        SET_CLASS(entityClass, "Entity");
        SET_CLASS(componentClass, "Component");
        SET_CLASS(transformClass, "Transform");

        SET_CLASS(cameraClass, "Camera");
        SET_CLASS(lightClass, "Light");

        SET_CLASS(rigidBodyClass, "RigidBody");
        SET_CLASS(boxColliderClass, "BoxCollider");
        SET_CLASS(sphereColliderClass, "SphereCollider");
        SET_CLASS(capsuleColliderClass, "CapsuleCollider");

        SET_CLASS(showInEditorAttrClass, "ShowInEditorAttribute");
        SET_CLASS(hideInEditorAttrClass, "HideInEditorAttribute");

    }

    GET_CLASS_FUNC(Base, baseClass);
    
    GET_TMPL_CLASS_FUNC(InternalEntity, entityClass);
    GET_TMPL_CLASS_FUNC(Component, componentClass);
    GET_TMPL_CLASS_FUNC(Transform, transformClass);

    GET_TMPL_CLASS_FUNC(Camera, cameraClass);
    GET_TMPL_CLASS_FUNC(Light, lightClass);

    GET_TMPL_CLASS_FUNC(RigidBody, rigidBodyClass);
    GET_TMPL_CLASS_FUNC(BoxCollider, boxColliderClass);
    GET_TMPL_CLASS_FUNC(SphereCollider, sphereColliderClass);
    GET_TMPL_CLASS_FUNC(CapsuleCollider, capsuleColliderClass);

    GET_CLASS_FUNC(ShowInEditorAttribute, showInEditorAttrClass);
    GET_CLASS_FUNC(HideInEditorAttribute, hideInEditorAttrClass);

}
