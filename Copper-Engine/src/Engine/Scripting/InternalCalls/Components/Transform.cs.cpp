#include "cupch.h"
#include "Transform.cs.h"
#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::Transform {

    typedef ::Copper::Transform Transform;

    Vector3 GetPosition(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetPosition();

    }
    void SetPosition(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetPosition(value);

    }
    void GetRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        *ret = ptr->GetRotation();

    }
    void SetRotation(MonoObject* transform, const Quaternion* value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetRotation(*value);

    }
    Vector3 GetScale(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetScale();

    }
    void SetScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();
        
        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetScale(value);

    }

    Vector3 GetGlobalPosition(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetGlobalPosition();

    }
    void GetGlobalRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        *ret = ptr->GetGlobalRotation();

    }
    Vector3 GetGlobalScale(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetGlobalScale();

    }

    Vector3 GetForward(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetForward();

    }
    Vector3 GetRight(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetRight();

    }
    Vector3 GetUp(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetUp();

    }

}
