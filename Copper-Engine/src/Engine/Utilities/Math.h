#pragma once

#include "Engine/Core/Core.h"

#include <GLM/glm.hpp>

namespace Copper::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

}