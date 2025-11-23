#include "cupch.h"
#include "Transform.cs.h"
#include "Engine/Components/Transform.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"
#include "Engine/Scripting/ManagedReferences.h"

#include <mono/metadata/object.h>
#include <mono/metadata/exception.h>

namespace Copper::Scripting::Transform {

    typedef ::Copper::Transform Transform;

    // Position rotation and scale

    Vector3 GetPosition(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetPosition();

    }
    void GetRotation(MonoObject* transform, Quaternion* ret) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        *ret = ptr->GetRotation();

    }
    Vector3 GetScale(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetScale();

    }

    void SetPosition(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetPosition(value);

    }
    void SetRotation(MonoObject* transform, const Quaternion* value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetRotation(*value);

    }
    void SetScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();
        
        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetScale(value);

    }

    // Globals

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

    void SetGlobalPosition(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetGlobalPosition(value);

    }
    void SetGlobalRotation(MonoObject* transform, Quaternion* value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetGlobalRotation(*value);

    }
    void SetGlobalScale(MonoObject* transform, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->SetGlobalScale(value);

    }

    // Directionals

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

    // Parent & Child

    MonoObject* GetParent(MonoObject* transform) {
        
        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);

        Transform* parent = ptr->GetParent();
        if (parent == nullptr) return nullptr;

        return GetManagedReference(parent, Class::Transform);

    }
    void SetParent(MonoObject* transform, MonoObject* parent) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        if (parent == nullptr) {

            Log("Setting parrent to nullptr.");
            ptr->SetParent(nullptr);
            return;

        }

        GET_UNMANAGED_COMPONENT_PTR(Transform, newParent, parent);
        ptr->SetParent(newParent);

    }

    MonoObject* GetChild(MonoObject* transform, uint32 index) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);

        Transform* child = ptr->GetChild(index);
#ifdef CU_EDITOR
        if (child == nullptr) {

            CUP_POP_TOP();
            mono_raise_exception(mono_get_exception_argument_null("index"));

        }
#endif

        return GetManagedReference(child, Class::Transform);

    }
    uint32 GetChildCount(MonoObject* transform) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        return ptr->GetChildCount();

    }

    void AddChild(MonoObject* transform, MonoObject* child) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        
#ifdef CU_EDITOR
        if (child == nullptr) {

            CUP_POP_TOP();
            mono_raise_exception(mono_get_exception_argument_null("child"));

        }
#endif

        GET_UNMANAGED_COMPONENT_PTR(Transform, unmanagedChild, child);
        ptr->AddChild(unmanagedChild);

    }
    void RemoveChild(MonoObject* transform, uint32 index) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);
        ptr->RemoveChild(index);

    }
    void RemoveChildTransform(MonoObject* transform, MonoObject* child) {

        CUP_FUNCTION();

        GET_UNMANAGED_COMPONENT_PTR(Transform, ptr, transform);

#ifdef CU_EDITOR
        if (child == nullptr) {

            CUP_POP_TOP();
            mono_raise_exception(mono_get_exception_argument_null("child"));

        }
#endif

        GET_UNMANAGED_COMPONENT_PTR(Transform, unmanagedChild, child);
        ptr->RemoveChild(unmanagedChild);

    }

}
