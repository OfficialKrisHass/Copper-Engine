#include "cupch.h"
#include "Camera.h"

#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/ext/matrix_transform.hpp>

namespace Copper {

	glm::mat4 Camera::CreateViewMatrix() {

		glm::mat4 ret(1.0f);

		ret = glm::lookAt((glm::vec3) transform->position, (glm::vec3) (transform->position + transform->forward), (glm::vec3) transform->up);

		return ret;

	}

	glm::mat4 Camera::CreateProjectionMatrix() {

		glm::mat4 ret(1.0f);
		
		ret = glm::perspective(glm::radians(fov), static_cast<float>(size.x) / size.y, nearPlane, farPlane);

		return ret;

	}

}