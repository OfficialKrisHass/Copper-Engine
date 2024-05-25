#include "cupch.h"

#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::InternalCalls::Components::Transform {

    typedef ::Copper::Transform Transform;

    Vector3 get_position(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Position();

    }
    void set_position(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        ptr->SetPosition(value);

    }

    void GetRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        *ret = ptr->Rotation();

    }
    void SetRotation(MonoObject* transform, Quaternion* value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);
        
        ptr->SetRotation(*value);

    }

    Vector3 get_scale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Scale();

    }
    void set_scale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        ptr->SetScale(value);

    }

    Vector3 get_globalPosition(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GlobalPosition();

    }
    void set_globalPosition(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        CU_ASSERT(false, "Not implemented yet!");

    }

    void GetGlobalRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        *ret = ptr->GlobalRotation();

    }
    void SetGlobalRotation(MonoObject* transform, Quaternion* value) {

        CUP_FUNCTION();
        
        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        CU_ASSERT(false, "Not implemented yet!");

    }

    Vector3 get_globalScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GlobalScale();

    }
    void set_globalScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();
        
        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        CU_ASSERT(false, "Not implemented yet!");

    }

    Vector3 get_forward(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Forward();

    }
    Vector3 get_right(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Right();

    }
    Vector3 get_up(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) ptr);

        return ptr->Up();

    }

}
