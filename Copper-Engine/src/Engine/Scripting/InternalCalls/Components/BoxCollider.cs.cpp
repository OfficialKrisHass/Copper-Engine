#include "cupch.h"
#include "BoxCollider.cs.h"
#include "Engine/Components/BoxCollider.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::BoxCollider {

    typedef ::Copper::BoxCollider BoxCollider;

    Vector3 GetSize(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(BoxCollider*, ptr, collider);
        return ptr->GetSize();

    }
    void SetSize(MonoObject* collider, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(BoxCollider*, ptr, collider);
        ptr->SetSize(value);

    }

}
