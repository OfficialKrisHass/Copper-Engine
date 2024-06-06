#include "cupch.h"
#include "Engine/Components/Light.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::InternalCalls::Components::Light {

    typedef ::Copper::Light Light;

    Color get_color(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return ptr->color;

    }
    void set_color(MonoObject* light, Color value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->color = value;

    }
    uint32 get_type(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return (uint32) ptr->type;

    }
    void set_type(MonoObject* light, uint32 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->type = (Light::Type) value;

    }

    float get_intensity(MonoObject* light) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        return ptr->intensity;

    }
    void set_intensity(MonoObject* light, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Light*, ptr, light);
        ptr->intensity = value;

    }

}
