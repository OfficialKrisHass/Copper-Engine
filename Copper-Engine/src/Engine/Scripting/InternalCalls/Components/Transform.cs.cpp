#include "cupch.h"
#include "Transform.cs.h"

#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::Transform {

    typedef ::Copper::Transform Transform;

    Transform* GetTransform(MonoObject* instance) {

        Transform* ret = nullptr;
        mono_field_get_value(instance, UnmanagedPtrField(), (void*) &ret);

        CU_ASSERT(ret != nullptr, "Received Transform from Unamanged Pointer field is invalid.");

        if (!ret->GetEntity().IsValid() && ret->GetEntity().ID() != INVALID_ENTITY_ID) {

            LogError("Entity has been deleted or is invalid, but it's transform is still being used.");

            Profiler::PopTopScope();
            mono_raise_exception(mono_get_exception_null_reference());

            return nullptr;

        }

        return ret;

    }

    Vector3 GetPosition(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetPosition();

    }
    void SetPosition(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform); 
        ptr->SetPosition(value);

    }
    void GetRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        *ret = ptr->GetRotation();

    }
    void SetRotation(MonoObject* transform, const Quaternion* value) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        ptr->SetRotation(*value);

    }
    Vector3 GetScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetScale();

    }
    void SetScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        ptr->SetScale(value);

    }

    Vector3 GetGlobalPosition(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetGlobalPosition();

    }
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        *ret = ptr->GetGlobalRotation();

    }
    Vector3 GetGlobalScale(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetGlobalScale();

    }

    Vector3 GetForward(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetForward();

    }
    Vector3 GetRight(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetRight();

    }
    Vector3 GetUp(MonoObject* transform) {

        CUP_FUNCTION();

        Transform* ptr = GetTransform(transform);
        return ptr->GetUp();

    }

}
