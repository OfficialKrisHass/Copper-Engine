#include "cupch.h"
#include "Camera.h"

#include "Engine/Core/Engine.h"

#include "Engine/Components/Transform.h"

#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/ext/matrix_transform.hpp>

namespace Copper {

	glm::mat4 Camera::CreateViewMatrix() {

		glm::mat4 ret(1.0f);

		ret = glm::lookAt((glm::vec3) GetTransform()->position, (glm::vec3) (GetTransform()->position + GetTransform()->Forward()), (glm::vec3) GetTransform()->Up());

		return ret;

	}

	glm::mat4 Camera::CreateProjectionMatrix() {

		glm::mat4 ret(1.0f);
		
	#ifdef CU_EDITOR
		ret = glm::perspective(glm::radians(fov), static_cast<float>(size.x) / size.y, nearPlane, farPlane);
	#else
		ret = glm::perspective(glm::radians(fov), GetWindowAspectRatio(), nearPlane, farPlane);
	#endif

		return ret;

	}

}