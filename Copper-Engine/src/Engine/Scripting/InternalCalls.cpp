#include "cupch.h"

#include "Engine/Scripting/InternalCalls/Components/Camera.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Component.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Light.cs.h"
#include "Engine/Scripting/InternalCalls/Components/RigidBody.cs.h"
#include "Engine/Scripting/InternalCalls/Components/Transform.cs.h"
#include "Engine/Scripting/InternalCalls/Core/Entity.cs.h"
#include "Engine/Scripting/InternalCalls/Editor/Editor.cs.h"

#include <mono/jit/jit.h>

namespace Copper::Scripting {

    void SetupInternalCalls() {

        CUP_FUNCTION();

        mono_add_internal_call("Copper.Camera::get_fov", (void*) Camera::GetFov);
        mono_add_internal_call("Copper.Camera::set_fov", (void*) Camera::SetFov);
        mono_add_internal_call("Copper.Entity::get_name", (void*) Entity::GetName);
        mono_add_internal_call("Copper.Entity::set_name", (void*) Entity::SetName);

    }

}