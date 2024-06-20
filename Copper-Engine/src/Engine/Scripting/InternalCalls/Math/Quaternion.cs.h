#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Scripting::Quaternion {

    typedef ::Copper::CMath::Quaternion Quaternion;

    Vector3 ToEuler(const Quaternion* quat);
    void FromEuler(const Vector3& eulerAngles, Quaternion* ret);

}
