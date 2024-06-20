#include "cupch.h"
#include "Camera.cs.h"
#include "Engine/Components/Camera.h"

#include "Engine/Scripting/ScriptingEngine.h"
#include "Engine/Scripting/InternalCalls/Utils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/object.h>

namespace Copper::Scripting::Camera {

    typedef ::Copper::Camera Camera;

    float GetFov(MonoObject* camera) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        return ptr->fov;

    }
    void SetFov(MonoObject* camera, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        ptr->fov = value;

    }

    float get_nearPlane(MonoObject* camera) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        return ptr->nearPlane;

    }
    void set_nearPlane(MonoObject* camera, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        ptr->nearPlane = value;

    }
    float get_farPlane(MonoObject* camera) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        return ptr->farPlane;

    }
    void set_farPlane(MonoObject* camera, float value) {

        CUP_FUNCTION();

        GET_UNMANAGED_PTR(Camera*, ptr, camera);
        ptr->farPlane = value;

    }

}
