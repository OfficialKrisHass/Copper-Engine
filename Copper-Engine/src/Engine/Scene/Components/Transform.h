#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Transform {

	public:
		Transform() = default;
		Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

		Vector3 position = Vector3::Zero();
		Vector3 rotation = Vector3::Zero();
		Vector3 scale = Vector3::One();

		glm::mat4 CreateMatrix();

		Vector3 Forward();
		Vector3 Right();
		Vector3 Up();
		Vector3 Backward();
		Vector3 Left();
		Vector3 Down();

	private:
		//

	};

}