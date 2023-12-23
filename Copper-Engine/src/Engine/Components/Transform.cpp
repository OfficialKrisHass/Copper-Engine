#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

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
		ret = ret * (Matrix4) rotation;
		CMath::ScaleMatrix(ret, scale);

		return ret;

	}

	void Transform::Update() {

		this->forward	= GlobalRotation() * Vector3(0.0f, 0.0f, -1.0f);
		this->right		= GlobalRotation() * Vector3(1.0f, 0.0f,  0.0f);
		this->up		= GlobalRotation() * Vector3(0.0f, 1.0f,  0.0f);

	}

	Vector3 Transform::GlobalPosition() const {

		if (parent) return position + parent->GlobalPosition();
		else return position;

	}
	Quaternion Transform::GlobalRotation() const {

		if (parent) return rotation * parent->GlobalRotation();
		else return rotation;

	}
	Vector3 Transform::GlobalScale() const {

		if (parent) return scale * parent->GlobalScale();
		else return scale;

	}

	Transform* Transform::GetChild(int index) const { return GetEntityFromID(children[index])->GetTransform(); }

	void Transform::SetParent(Transform* parent) {

		if (parent == this->parent) return;

		// Case 1: Removing a parent (new parent == nullptr)
		if (parent == nullptr) {

			position += this->parent->GlobalPosition();

			this->parent->children.erase(this->parent->children.begin() + parentChildIndex);
			this->parent = nullptr;

			parentChildIndex = -1;
			return;

		}

		// Case 2: Changing a parent (old parent != nullptr)
		if (this->parent) {

			position += this->parent->GlobalPosition();

			this->parent->children.erase(this->parent->children.begin() + parentChildIndex);

			this->parent = parent;
			this->parentChildIndex = (uint32_t) parent->children.size();

			parent->children.push_back(GetEntity()->ID());

			position -= this->parent->GlobalPosition();

			return;

		}

		// Case 3: Setting a parent (old parent == nullptr)

		this->parent = parent;
		this->parentChildIndex = (uint32_t) parent->children.size();

		parent->children.push_back(GetEntity()->ID());

		position -= this->parent->GlobalPosition();

	}

	void Transform::AddChild(Transform* child) {

		if (child->parent == this || !child) return;
		if (child->parent) {

			child->position += child->parent->GlobalPosition();
			child->parent->children.erase(child->parent->children.begin() + child->parentChildIndex);

		}

		child->parent = this;
		child->parentChildIndex = (uint32_t) children.size();

		children.push_back(child->GetEntity()->ID());

		child->position -= GlobalPosition();

	}
	void Transform::RemoveChild(int index) {

		Transform* child = GetEntityFromID(children[index])->GetTransform();
		
		child->position += child->parent->GlobalPosition();
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