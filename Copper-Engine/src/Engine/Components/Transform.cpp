#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Copper {

	Vector3 VecFromGLM(const glm::vec3& vec) { return Vector3(vec.x, vec.y, vec.z); }

	Matrix4 Transform::CreateMatrix() {

		Matrix4 ret;

		if (parent) {

			ret *= parent->CreateMatrix();

		}

		CMath::TranslateMatrix(ret, position);
		CMath::RotateMatrix(ret, Vector3(1.0f, 0.0f, 0.0f), rotation.x);
		CMath::RotateMatrix(ret, Vector3(0.0f, 1.0f, 0.0f), rotation.y);
		CMath::RotateMatrix(ret, Vector3(0.0f, 0.0f, 1.0f), rotation.z);
		CMath::ScaleMatrix(ret, scale);

		return ret;

	}

	void Transform::Update() {

		glm::quat quat = glm::quat(glm::vec3(glm::radians(-rotation.x), glm::radians(-rotation.y), glm::radians(-rotation.z)));

		this->forward	= VecFromGLM(glm::rotate(quat, glm::vec3(0.0f, 0.0f, -1.0f)));
		this->right		= VecFromGLM(glm::rotate(quat, glm::vec3(1.0f, 0.0f,  0.0f)));
		this->up		= VecFromGLM(glm::rotate(quat, glm::vec3(0.0f, 1.0f,  0.0f)));
		this->back = -forward;
		this->left = -right;
		this->down = -up;

	}

	Vector3 Transform::GlobalPosition() const {

		if (parent) return position + parent->GlobalPosition();
		else return position;

	}

	Transform* Transform::GetChild(int index) const { return GetEntityFromID(children[index])->GetTransform(); }

	void Transform::SetParent(Transform* parent) {

		if (parent == this->parent) return;
		if (parent == nullptr) {

			position = GlobalPosition();

			this->parent->children.erase(this->parent->children.begin() + parentChildIndex);
			this->parent = nullptr;

			parentChildIndex = -1;

			return;

		}
		if (this->parent) {

			position += this->parent->GlobalPosition();

			this->parent->children.erase(this->parent->children.begin() + parentChildIndex);

			this->parent = parent;
			this->parentChildIndex = (uint32_t) parent->children.size();

			parent->children.push_back(GetEntity()->ID());

			position -= parent->GlobalPosition();

			return;

		}

		this->parent = parent;
		this->parentChildIndex = (uint32_t) parent->children.size();

		parent->children.push_back(GetEntity()->ID());

		position -= parent->GlobalPosition();

	}

	void Transform::AddChild(Transform* transform) {

		if (transform->parent == this || !transform) return;
		if (transform->parent) {

			transform->position += transform->parent->GlobalPosition();
			transform->parent->children.erase(transform->parent->children.begin() + transform->parentChildIndex);

		}

		transform->parent = this;
		transform->parentChildIndex = (uint32_t) children.size();

		children.push_back(transform->GetEntity()->ID());

		transform->position -= GlobalPosition();

	}
	void Transform::RemoveChild(int index) {

		Transform* child = GetEntityFromID(children[index])->GetTransform();
		
		child->position = child->GlobalPosition();
		child->parent = nullptr;
		child->parentChildIndex = -1;

		children.erase(children.begin() + index);
	
	}
	void Transform::RemoveChild(Transform* transform) {

		RemoveChild(transform->parentChildIndex);

	}

	bool Transform::operator==(const Transform& other) const {

		return GetEntity() == other.GetEntity();

	}
	
}