#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Math {

    void DecomposeTransform(const Matrix4& transform, Vector3& position, Quaternion& rotation, Vector3& scale);

}
