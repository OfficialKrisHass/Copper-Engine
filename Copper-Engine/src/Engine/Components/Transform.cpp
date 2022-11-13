#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include <CopperECS/CopperECS.h>

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Copper {

	glm::mat4 Transform::CreateMatrix() {

		glm::mat4 ret(1.0f);

		if (parent) {

			ret *= parent->CreateMatrix();

		}

		ret = glm::translate(ret, (glm::vec3) position);

		ret = glm::rotate(ret, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		ret = glm::rotate(ret, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		ret = glm::rotate(ret, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		ret = glm::scale(ret, (glm::vec3) scale);

		return ret;

	}

	void Transform::Update() {

		glm::quat quat = glm::quat(glm::vec3(glm::radians(-rotation.x), glm::radians(-rotation.y), glm::radians(-rotation.z)));

		//glm::vec3 f = glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f));
		//glm::vec3 r = glm::rotate(quat, glm::vec3(1.0f, 0.0f,  0.0f));
		//glm::vec3 u = glm::rotate(quat, glm::vec3(0.0f, 1.0f,  0.0f));

		this->forward	= glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f));
		this->right		= glm::rotate(quat, glm::vec3(1.0f, 0.0f, 0.0f));
		this->up		= glm::rotate(quat, glm::vec3(0.0f, 1.0f, 0.0f));
		//this->forward = Vector3(f.x, f.y, f.z);
		//this->right = Vector3(r.x, r.y, r.z);
		//this->up = Vector3(u.x, u.y, u.z);
		this->backward = -forward;
		this->left = -right;
		this->down = -up;

	}

	/*Vector3 Transform::Forward() {

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
	Vector3 Transform::Down() { return -Up(); }*/

	Vector3 Transform::GlobalPosition() {

		if (parent) return position + parent->GlobalPosition();
		else return position;

	}

	Transform* Transform::GetChild(int index) const { return GetScene()->registry.GetObjectFromID(children[index]).transform; }

	void Transform::AddChild(Transform* transform) {

		children.push_back(transform->object->GetID());
		numOfChildren++;

	}

	void Transform::RemoveChild(int index) {

		children.erase(children.begin() + index);
		numOfChildren--;
	
	}
	void Transform::RemoveChild(Transform* transform) {

		for (int i = 0; i < numOfChildren; i++) {

			if (children[i] == transform->object->GetID()) RemoveChild(i);

		}

	}

	bool Transform::operator==(const Transform& other) const {

		return *object == *other.object;

	}
	
}
