#include "cupch.h"
#include "Component.cs.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Scripting/MonoUtils.h"
#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/ManagedReferences.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/metadata/object.h>

namespace Copper::Scripting::Component {

    typedef ::Copper::Component Component;
    
    MonoObject* GetEntity(MonoObject* component) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Component*, ptr, component);
        MonoObject* ret = ManagedReference((void*) (uint64) ptr->GetEntity()->ID());

        CU_ASSERT(ret, "Could not get Managed Entity Reference from Component");
        return ret;

    }
    MonoObject* GetTransform(MonoObject* component) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Component*, ptr, component);
        MonoObject* ret = ManagedReference(ptr->GetTransform());
        
        CU_ASSERT(ret, "Could not get Managed Transform reference from Component");
        return ret;

    }

}
