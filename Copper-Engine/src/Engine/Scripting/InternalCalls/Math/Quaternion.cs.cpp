#include "cupch.h"

namespace Copper::Scripting::InternalCalls::Math::Quaternion {

    typedef ::Copper::CMath::Quaternion Quaternion;

    Vector3 ToEuler(Quaternion* quat) { return quat->EulerAngles(); }
    void FromEuler(Vector3 eulerAngles, Quaternion* quat) { *quat = Quaternion(eulerAngles); }
    
}
