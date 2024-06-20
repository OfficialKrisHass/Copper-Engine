#include "cupch.h"
#include "Quaternion.cs.h"

namespace Copper::Scripting::Quaternion {

    Vector3 ToEuler(const Quaternion* quat) { return quat->EulerAngles(); }
    void FromEuler(const Vector3& eulerAngles, Quaternion* quat) { *quat = Quaternion(eulerAngles); }
    
}
