#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Physics/Raycast.h"

namespace Copper::Scripting::Raycast {

  bool Fire(const Vector3& origin, const Vector3& direction, ::Copper::Raycast::Data* data, float maxDistance);

}
