#include "cupch.h"
#include "Collider.cs.h"
#include "Engine/Components/Collider.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::Collider {

    typedef ::Copper::Collider Collider;

    uint8 GetType(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);
        return ptr->GetType();

    }
    MonoObject* GetRigidBody(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);

        MonoObject* ret = ManagedReference(ptr->GetRigidBody());
        CU_ASSERT(ret, "Could not get Collider's Rigid body Managed Reference\n\t{}", ptr->GetEntity()->name);
        return ret;

    }

    bool GetTrigger(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);
        return ptr->GetTrigger();

    }
    void SetTrigger(MonoObject* collider, bool value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);
        ptr->SetTrigger(value);

    }
    Vector3 GetCenter(MonoObject* collider) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);
        return ptr->GetCenter();

    }
    void SetCenter(MonoObject *collider, Vector3 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Collider*, ptr, collider);
        ptr->SetCenter(value);

    }

}
