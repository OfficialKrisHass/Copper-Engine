#include "cupch.h"

#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::Transform {

    typedef ::Copper::Transform Transform;

    Vector3* get_position(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return (Vector3*) &ptr->Position();

    }
    void set_position(MonoObject* transform, Vector3* value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        ptr->SetPosition(*value);

    }

    Vector3* get_scale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return (Vector3*) &ptr->Scale();

    }
    void set_scale(MonoObject* transform, Vector3* value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &value);

        ptr->SetScale(*value);

    }

    Vector3* get_forward(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return (Vector3*) &ptr->Forward();

    }
    Vector3* get_right(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return (Vector3*) &ptr->Right();

    }
    Vector3* get_up(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) ptr);

        return (Vector3*) &ptr->Up();

    }

}
