#include "cupch.h"
#include "Transform.h"

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>

namespace Copper {

	glm::mat4 Transform::CreateMatrix() {

		glm::mat4 ret(1.0f);

		ret = glm::translate(ret, (glm::vec3) position);

		ret = glm::rotate(ret, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		ret = glm::rotate(ret, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		ret = glm::rotate(ret, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		ret = glm::scale(ret, (glm::vec3) scale);

		return ret;

	}

}