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

        return ptr->GetPosition();

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

        *ret = ptr->GetRotation();

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

        return ptr->GetScale();

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

        return ptr->GetGlobalPosition();

    }
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        *ret = ptr->GetGlobalRotation();

    }
    Vector3 GetGlobalScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GetGlobalScale();

    }

    Vector3 GetForward(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GetForward();

    }
    Vector3 GetRight(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) &ptr);

        return ptr->GetRight();

    }
    Vector3 GetUp(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = nullptr;
        mono_field_get_value(transform, UnmanagedPtrField(), (void*) ptr);

        return ptr->GetUp();

    }

}
