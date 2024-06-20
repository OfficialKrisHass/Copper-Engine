#include "cupch.h"
#include "Transform.cs.h"

#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::Transform {

    typedef ::Copper::Transform Transform;

    Vector3 GetPosition(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Position();

    }
    void SetPosition(MonoObject* transform, Vector3 value) {

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
    void SetRotation(MonoObject* transform, const Quaternion* value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);
        
        ptr->SetRotation(*value);

    }
    Vector3 GetScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Scale();

    }
    void SetScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        ptr->SetScale(value);

    }

    Vector3 GetGlobalPosition(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GlobalPosition();

    }
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        *ret = ptr->GlobalRotation();

    }
    Vector3 GetGlobalScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GlobalScale();

    }

    Vector3 GetForward(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Forward();

    }
    Vector3 GetRight(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->Right();

    }
    Vector3 GetUp(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) ptr);

        return ptr->Up();

    }

}
