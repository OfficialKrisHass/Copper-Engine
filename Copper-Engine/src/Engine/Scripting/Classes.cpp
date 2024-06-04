#include "cupch.h"
#include "Classes.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/class.h>

#define SET_CLASS(var, name) classes.var = mono_class_from_name(ScriptingAPIAssembly().GetImage(), "Copper", name)

#define GET_CLASS_FUNC(name, var) MonoClass* name ## Class() { CUP_FUNCTION();\
    CU_ASSERT(classes.var, "{} Class reference is nullptr. Make Sure you assigned it in InitializeClasses()", #name);\
    return classes.var; }

namespace Copper::Scripting {

    struct Classes {

        MonoClass* baseClass = nullptr;

        MonoClass* entityClass = nullptr;

        MonoClass* componentClass = nullptr;
        MonoClass* transformClass = nullptr;
        MonoClass* cameraClass = nullptr;
        MonoClass* lightClass = nullptr;

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

        SET_CLASS(showInEditorAttrClass, "ShowInEditorAttribute");
        SET_CLASS(hideInEditorAttrClass, "HideInEditorAttribute");

    }

    GET_CLASS_FUNC(Base, baseClass);
    
    GET_CLASS_FUNC(Entity, entityClass);

    GET_CLASS_FUNC(Component, componentClass);
    GET_CLASS_FUNC(Transform, transformClass);
    GET_CLASS_FUNC(Camera, cameraClass);
    GET_CLASS_FUNC(Light, lightClass);

    GET_CLASS_FUNC(ShowInEditorAttribute, showInEditorAttrClass);
    GET_CLASS_FUNC(HideInEditorAttribute, hideInEditorAttrClass);

}
