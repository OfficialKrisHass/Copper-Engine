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

			this->parent->RemoveChild(this);
			this->parent = nullptr;

			return;

		}

		// Case 2: Changing a parent (old parent != nullptr)
		if (this->parent) {

			position += this->parent->GlobalPosition();

			this->parent->RemoveChild(this);
			this->parent = parent;
			parent->children.push_back(GetEntity()->ID());

			position -= this->parent->GlobalPosition();

			return;

		}

		// Case 3: Setting a parent (old parent == nullptr)

		this->parent = parent;
		parent->children.push_back(GetEntity()->ID());

		position -= this->parent->GlobalPosition();

	}

	void Transform::AddChild(Transform* child) {

		if (child->parent == this || !child) return;
		if (child->parent) {

			child->position += child->parent->GlobalPosition();

			child->parent->RemoveChild(child);

		}

		child->parent = this;
		children.push_back(child->GetEntity()->ID());

		child->position -= GlobalPosition();

	}
	void Transform::RemoveChild(int index) {

		if (index < 0 || index > children.size()) {

			LogError("Can't remove an invalid index child. Parent: {} ({}), index: {}", GetEntity()->name, GetEntity()->ID(), index);
			return;

		}

		Transform* child = GetEntityFromID(children[index])->GetTransform();
		
		child->position += child->parent->GlobalPosition();
		child->parent = nullptr;

		children.erase(children.begin() + index);
	
	}
	void Transform::RemoveChild(Transform* transform) {

		for (int i = 0; i < children.size(); i++) {

			if (children[i] != transform->GetEntity()->ID()) continue;

			RemoveChild(i);
			return;

		}

		LogError("Can't remove a child. Parent: {} ({}), Child: {} ({})", GetEntity()->name, GetEntity()->ID(), transform->GetEntity()->name, transform->GetEntity()->ID());

	}

	bool Transform::operator==(const Transform& other) const {

		return GetEntity() == other.GetEntity();

	}
	
}