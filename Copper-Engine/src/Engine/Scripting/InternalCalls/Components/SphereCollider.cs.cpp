#include "cupch.h"
#include "SphereCollider.cs.h"
#include "Engine/Components/SphereCollider.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::SphereCollider {

    typedef ::Copper::SphereCollider SphereCollider;

    float GetRadius(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(SphereCollider*, ptr, collider);
        return ptr->GetRadius();

    }
    void SetRadius(MonoObject* collider, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(SphereCollider*, ptr, collider);
        ptr->SetRadius(value);

    }

}
