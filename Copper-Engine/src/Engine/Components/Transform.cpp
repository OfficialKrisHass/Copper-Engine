#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Utilities/Math.h"

#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/gtx/quaternion.hpp>

namespace Copper {

	Vector3 VecFromGLM(const glm::vec3& vec) { return Vector3(vec.x, vec.y, vec.z); }

	Matrix4 Transform::CreateMatrix() {

		Matrix4 ret;

		CMath::TranslateMatrix(ret, position);
		ret = ret * (Matrix4) rotation;
		CMath::ScaleMatrix(ret, scale);

		if (parent) {

			ret *= parent->CreateMatrix();

		}

		return ret;

	}

	void Transform::Update() {

		this->forward	= GlobalRotation() * Vector3(0.0f, 0.0f, -1.0f);
		this->right		= GlobalRotation() * Vector3(1.0f, 0.0f,  0.0f);
		this->up		= GlobalRotation() * Vector3(0.0f, 1.0f,  0.0f);

		Matrix4 globalMat = CreateMatrix();
		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(globalMat, pos, rot, scale);

		this->globalPosition = pos;
		this->globalRotation = (Vector3) rot;
		this->globalScale = scale;

	}

	Vector3 Transform::GlobalPosition() const {

		return globalPosition;

	}
	Quaternion Transform::GlobalRotation() const {

		return globalRotation;

	}
	Vector3 Transform::GlobalScale() const {

		return globalScale;

	}

	Transform* Transform::GetChild(int index) const { return GetEntityFromID(children[index])->GetTransform(); }

	void Transform::SetParent(Transform* parent) {

		if (parent == this->parent) return;

		// Case 1: Removing a parent (new parent == nullptr)
		if (parent == nullptr) {

			this->parent->RemoveChild(this);
			return;

		}

		// Case 2: Changing a parent (old parent != nullptr)
		if (this->parent) {

			this->parent->RemoveChild(this);
			Matrix4 local = CreateMatrix() * CMath::Inverse(parent->CreateMatrix());

			this->parent = parent;
			parent->children.push_back(GetEntity()->ID());

			glm::vec3 pos, rot, scale;
			Math::DecomposeTransform(local, pos, rot, scale);

			this->position = pos;
			this->rotation = (Vector3) rot;
			this->scale = scale;

			return;

		}

		// Case 3: Setting a parent (old parent == nullptr)

		Matrix4 local = CreateMatrix() * CMath::Inverse(parent->CreateMatrix());

		this->parent = parent;
		parent->children.push_back(GetEntity()->ID());

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(local, pos, rot, scale);

		this->position = pos;
		this->rotation = (Vector3) rot;
		this->scale = scale;

	}

	void Transform::AddChild(Transform* child) {

		if (child->parent == this || !child) return;
		if (child->parent)
			child->parent->RemoveChild(child);

		Matrix4 childGlobal = child->CreateMatrix();

		child->parent = this;
		children.push_back(child->GetEntity()->ID());

		Matrix4 childLocal = childGlobal * CMath::Inverse(CreateMatrix());

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(childLocal, pos, rot, scale);

		child->position = pos;
		child->rotation = (Vector3) rot;
		child->scale = scale;

	}
	void Transform::RemoveChild(int index) {

		if (index < 0 || index > children.size()) {

			LogError("Can't remove an invalid index child. Parent: {}, index: {}", *GetEntity(), index);
			return;

		}

		Transform* child = GetEntityFromID(children[index])->GetTransform();
		
		child->parent = nullptr;

		child->position = child->globalPosition;
		child->rotation = child->globalRotation;
		child->scale = child->globalScale;

		children.erase(children.begin() + index);
	
	}
	void Transform::RemoveChild(Transform* transform) {

		for (int i = 0; i < children.size(); i++) {

			if (children[i] != transform->GetEntity()->ID()) continue;

			RemoveChild(i);
			return;

		}

		LogError("Can't remove a child. Parent: {}, Child: {}", *GetEntity(), *transform->GetEntity());

	}

	bool Transform::operator==(const Transform& other) const {

		return GetEntity() == other.GetEntity();

	}
	
}