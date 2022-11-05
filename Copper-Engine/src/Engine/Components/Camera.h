#pragma once

#include "Engine/Core/Core.h"

#include <CopperECS/Component.h>
#include <GLM/mat4x4.hpp>

namespace Copper {

	class Camera : public Component {

	public:
		Camera() = default;
		Camera(UVector2I size) : size(size) {}

		glm::mat4 CreateViewMatrix();
		glm::mat4 CreateProjectionMatrix();

		inline void Resize(UVector2I size) { this->size = size; }

		virtual void Update() {}

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
	protected:

		UVector2I size;

		Vector3 forward = Vector3(0.0f, 0.0f, -1.0f);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

	};

}