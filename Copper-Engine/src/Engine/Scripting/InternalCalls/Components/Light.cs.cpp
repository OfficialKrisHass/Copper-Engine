#include "cupch.h"
#include "Light.cs.h"
#include "Engine/Components/Light.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::Light {

    typedef ::Copper::Light Light;

    Color GetColor(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return ptr->color;

    }
    void SetColor(MonoObject* light, Color value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->color = value;

    }
    uint32 GetType(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return (uint32) ptr->type;

    }
    void SetType(MonoObject* light, uint32 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->type = (Light::Type) value;

    }

    float GetIntensity(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return ptr->intensity;

    }
    void SetIntensity(MonoObject* light, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->intensity = value;

    }

}
