#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Utilities/Math.h"

namespace Copper {

	Vector3 VecFromGLM(const glm::vec3& vec) { return Vector3(vec.x, vec.y, vec.z); }

	Matrix4 Transform::CreateMatrix() {

		Matrix4 ret;

		if (m_parent)
			ret *= m_parent->CreateMatrix();

		CMath::TranslateMatrix(ret, this->position);
		ret = ret * (Matrix4) this->rotation;
		CMath::ScaleMatrix(ret, this->scale);

		return ret;

	}

	void Transform::Update() {

		m_forward	= GlobalRotation() * Vector3(0.0f, 0.0f, -1.0f);
		m_right		= GlobalRotation() * Vector3(1.0f, 0.0f,  0.0f);
		m_up		= GlobalRotation() * Vector3(0.0f, 1.0f,  0.0f);

		Matrix4 globalMat = CreateMatrix();
		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(globalMat, pos, rot, scale);

		m_globalPosition = pos;
		m_globalRotation = (Vector3) rot;
		m_globalScale = scale;

	}

	void Transform::SetParent(Transform* parent) {

		if (parent == m_parent) return;

		// Case 1: Removing a parent (new parent == nullptr)

		if (parent == nullptr) {

			m_parent->RemoveChild(this);
			return;

		}

		// Case 2: Changing a parent (old parent != nullptr)

		if (m_parent) {

			m_parent->RemoveChild(this);
			Matrix4 local = CreateMatrix() * CMath::Inverse(parent->CreateMatrix());

			m_parent = parent;
			parent->m_children.push_back(GetEntity()->ID());

			glm::vec3 pos, rot, scale;
			Math::DecomposeTransform(local, pos, rot, scale);

			this->position = pos;
			this->rotation = (Vector3) rot;
			this->scale = scale;

			return;

		}

		// Case 3: Setting a parent (old parent == nullptr)

		Matrix4 local = CreateMatrix() * CMath::Inverse(parent->CreateMatrix());

		m_parent = parent;
		parent->m_children.push_back(GetEntity()->ID());

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(local, pos, rot, scale);

		this->position = pos;
		this->rotation = (Vector3) rot;
		this->scale = scale;

	}

	void Transform::AddChild(Transform* child) {

		if (child->m_parent == this || !child) return;
		if (child->m_parent)
			child->m_parent->RemoveChild(child);

		Matrix4 childGlobal = child->CreateMatrix();

		child->m_parent = this;
		m_children.push_back(child->GetEntity()->ID());

		Matrix4 childLocal = childGlobal * CMath::Inverse(CreateMatrix());

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(childLocal, pos, rot, scale);

		child->position = pos;
		child->rotation = (Vector3) rot;
		child->scale = scale;

	}
	void Transform::RemoveChild(uint32 index) {

		if (index < 0 || index > m_children.size()) {

			LogError("Can't remove an invalid index child. Parent: {}, index: {}", *GetEntity(), index);
			return;

		}

		Transform* child = GetEntityFromID(m_children[index])->GetTransform();
		
		child->m_parent = nullptr;

		child->position = child->m_globalPosition;
		child->rotation = child->m_globalRotation;
		child->scale = child->m_globalScale;

		m_children.erase(m_children.begin() + index);
	
	}
	void Transform::RemoveChild(Transform* transform) {

		for (uint32 i = 0; i < m_children.size(); i++) {

			if (m_children[i] != transform->GetEntity()->ID()) continue;

			RemoveChild(i);
			return;

		}

		LogError("Can't remove a child. Parent: {}, Child: {}", *GetEntity(), *transform->GetEntity());

	}

	bool Transform::operator==(const Transform& other) const {

		return GetEntity() == other.GetEntity();

	}
	
}