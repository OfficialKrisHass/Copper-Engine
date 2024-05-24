#include "cupch.h"

namespace Copper::Scripting::Quaternion {

    typedef ::Copper::CMath::Quaternion Quaternion;

    Vector3 ToEuler(Quaternion* quat) { return quat->EulerAngles(); }
    void FromEuler(Vector3 eulerAngles, Quaternion* quat) { *quat = Quaternion(eulerAngles); }
    
}
