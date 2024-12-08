#include "cupch.h"
#include "RigidBody.cs.h"
#include "Engine/Components/RigidBody.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting::RigidBody {

    typedef ::Copper::RigidBody RigidBody;

    float GetMass(MonoObject* rigidBody) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        return ptr->GetMass();

    }
    void SetMass(MonoObject* rigidBody, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->SetMass(value);

    }

    bool GetIsStatic(MonoObject* rigidBody) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        return ptr->GetStatic();

    }
    void SetIsStatic(MonoObject* rigidBody, bool value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->SetStatic(value);

    }
    bool GetGravity(MonoObject* rigidBody) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        return ptr->GetGravity();

    }
    void SetGravity(MonoObject* rigidBody, bool value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->SetGravity(value);

    }
    uint8 GetLockMask(MonoObject* rigidBody) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        return ptr->GetLockMask();

    }
    void SetLockMask(MonoObject* rigidBody, uint8 value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->SetLockMask(value);

    }

    MonoObject* GetCollider(MonoObject* rigidBody) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);

        Collider* collider = ptr->GetCollider();
        CU_ASSERT(collider, "Could not get Collider from RigidBody '{}'", *ptr->GetEntity());

        MonoObject* ret = ManagedReference(collider);
        CU_ASSERT(ret, "Could not get Managed Reference to Collider on '{}'", *ptr->GetEntity());

        return ret;

    }

    void AddForce(MonoObject* rigidBody, const Vector3& force, uint8 forceMode) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->AddForce(force, (RigidBody::ForceMode) forceMode);

    }
    void AddTorque(MonoObject* rigidBody, const Vector3& torque, uint8 forceMode) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(RigidBody*, ptr, rigidBody);
        ptr->AddTorque(torque, (RigidBody::ForceMode) forceMode);

    }

}
