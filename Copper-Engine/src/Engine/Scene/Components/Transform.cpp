#include "cupch.h"
#include "Transform.h"

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

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

	Vector3 Transform::Forward() {

		glm::quat quat = glm::quat(glm::vec3(glm::radians(-rotation.x), glm::radians(-rotation.y), glm::radians(-rotation.z)));
		glm::vec3 forward = glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f));
		Vector3 ret(forward.x, forward.y, forward.z);
		
		return ret;
		
	}
	Vector3 Transform::Right() {
		glm::quat quat = glm::quat(glm::vec3(glm::radians(-rotation.x), glm::radians(-rotation.y), glm::radians(-rotation.z)));
		glm::vec3 forward = glm::rotate(quat, glm::vec3(1.0f, 0.0f, 0.0f));
		Vector3 ret(forward.x, forward.y, forward.z);

		return ret;
		
	}
	Vector3 Transform::Up() {

		glm::quat quat = glm::quat(glm::vec3(glm::radians(-rotation.x), glm::radians(-rotation.y), glm::radians(-rotation.z)));
		glm::vec3 forward = glm::rotate(quat, glm::vec3(0.0f, 1.0f, 0.0f));
		Vector3 ret(forward.x, forward.y, forward.z);

		return ret;
		
	}
	Vector3 Transform::Backward() { return -Forward(); }
	Vector3 Transform::Left() { return -Right(); }
	Vector3 Transform::Down() { return -Up(); }
	
}
