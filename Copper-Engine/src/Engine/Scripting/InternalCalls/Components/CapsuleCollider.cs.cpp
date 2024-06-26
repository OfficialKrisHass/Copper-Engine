#include "cupch.h"
#include "CapsuleCollider.cs.h"
#include "Engine/Components/CapsuleCollider.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::CapsuleCollider {

    typedef ::Copper::CapsuleCollider CapsuleCollider;

    float GetRadius(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(CapsuleCollider*, ptr, collider);
        return ptr->GetRadius();

    }
    void SetRadius(MonoObject* collider, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(CapsuleCollider*, ptr, collider);
        ptr->SetRadius(value);

    }
    float GetHeight(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(CapsuleCollider*, ptr, collider);
        return ptr->GetHeight();

    }
    void SetHeight(MonoObject* collider, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(CapsuleCollider*, ptr, collider);
        ptr->SetHeight(value);

    }

}
