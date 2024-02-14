#include "cupch.h"
#include "Transform.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Utilities/Math.h"

#define POSITION_CHANGED 1 // 1 << 0
#define ROTATION_CHANGED 2 // 1 << 1
#define SCALE_CHANGED 4 // 1 << 2

namespace Copper {

	Transform::Transform() : m_changed(POSITION_CHANGED | ROTATION_CHANGED | SCALE_CHANGED) {}
	Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) : m_position(position), m_rotation(rotation), m_scale(scale), m_changed(POSITION_CHANGED | ROTATION_CHANGED | SCALE_CHANGED) {}
	Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : m_position(position), m_rotation(rotation), m_scale(scale), m_changed(POSITION_CHANGED | ROTATION_CHANGED | SCALE_CHANGED) {}

	void Transform::Update() {

		CUP_FUNCTION();

		if (m_changed == 0) return;

		CalculateMatrix();
		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(m_mat, pos, rot, scale);

		if (m_changed & POSITION_CHANGED)
			m_globalPosition = pos;

		if (m_changed & SCALE_CHANGED)
			m_globalScale = scale;

		if (m_changed & ROTATION_CHANGED) {

			m_globalRotation = (Vector3) rot;

			m_forward = m_globalRotation * Vector3(0.0f, 0.0f, -1.0f);
			m_right = m_globalRotation * Vector3(1.0f, 0.0f, 0.0f);
			m_up = m_globalRotation * Vector3(0.0f, 1.0f, 0.0f);

		}

		m_changed = 0;
		m_calculated = false;

	}
	
	void Transform::SetPosition(const Vector3& position) {

		CUP_FUNCTION();

		m_position = position;
		m_changed |= POSITION_CHANGED;

		for (uint32 id : m_children)
			GetEntityFromID(id)->GetTransform()->SetChanged(m_changed);

	}

	void Transform::SetRotation(const Quaternion& rotation) {

		CUP_FUNCTION();

		m_rotation = rotation;
		m_changed |= ROTATION_CHANGED;

		for (uint32 id : m_children)
			GetEntityFromID(id)->GetTransform()->SetChanged(m_changed);

	}

	void Transform::SetScale(const Vector3& scale) {

		CUP_FUNCTION();

		m_scale = scale;
		m_changed |= SCALE_CHANGED;

		for (uint32 id : m_children)
			GetEntityFromID(id)->GetTransform()->SetChanged(m_changed);

	}


	// Parent

	void Transform::SetParent(Transform* parent) {

		CUP_FUNCTION();

		if (parent == m_parent) return;

		// Case 1: Removing a parent (new parent == nullptr)

		if (parent == nullptr) {

			m_parent->RemoveChild(this);
			return;

		}

		// Case 2: Changing a parent (old parent != nullptr)

		if (m_parent) {

			m_parent->RemoveChild(this);
			Matrix4 local = m_mat * CMath::Inverse(parent->m_mat);

			m_parent = parent;
			parent->m_children.push_back(GetEntity()->ID());

			glm::vec3 pos, rot, scale;
			Math::DecomposeTransform(local, pos, rot, scale);

			m_position = pos;
			m_rotation = (Vector3) rot;
			m_scale = scale;

			return;

		}

		// Case 3: Setting a parent (old parent == nullptr)

		Matrix4 local = m_mat * CMath::Inverse(parent->m_mat);

		m_parent = parent;
		parent->m_children.push_back(GetEntity()->ID());

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(local, pos, rot, scale);

		m_position = pos;
		m_rotation = (Vector3) rot;
		m_scale = scale;

	}

	// Children

	void Transform::AddChild(Transform* child) {

		CUP_FUNCTION();

		if (child->m_parent == this || !child) return;
		if (child->m_parent)
			child->m_parent->RemoveChild(child);

		Matrix4 childGlobal = child->m_mat;

		child->m_parent = this;
		m_children.push_back(child->GetEntity()->ID());

		Matrix4 childLocal = childGlobal * CMath::Inverse(m_mat);

		glm::vec3 pos, rot, scale;
		Math::DecomposeTransform(childLocal, pos, rot, scale);

		child->m_position = pos;
		child->m_rotation = (Vector3) rot;
		child->m_scale = scale;

	}
	void Transform::RemoveChild(uint32 index) {

		CUP_FUNCTION();

		if (index < 0 || index > m_children.size()) {

			LogError("Can't remove an invalid index child. Parent: {}, index: {}", *GetEntity(), index);
			return;

		}

		Transform* child = GetEntityFromID(m_children[index])->GetTransform();
		
		child->m_parent = nullptr;

		child->m_position = child->m_globalPosition;
		child->m_rotation = child->m_globalRotation;
		child->m_scale = child->m_globalScale;

		m_children.erase(m_children.begin() + index);
	
	}
	void Transform::RemoveChild(Transform* transform) {

		CUP_FUNCTION();

		for (uint32 i = 0; i < m_children.size(); i++) {

			if (m_children[i] != transform->GetEntity()->ID()) continue;

			RemoveChild(i);
			return;

		}

		LogError("Can't remove a child. Parent: {}, Child: {}", *GetEntity(), *transform->GetEntity());

	}

	void Transform::CalculateMatrix() {

		CUP_FUNCTION();

		if (m_calculated) return;
		m_mat = Matrix4(1.0f);

		if (m_parent) {

			m_parent->CalculateMatrix();
			m_mat *= m_parent->m_mat;

		}

		CMath::TranslateMatrix(m_mat, m_position);
		m_mat = m_mat * (Matrix4) m_rotation;
		CMath::ScaleMatrix(m_mat, m_scale);

		m_calculated = true;

	}

	void Transform::SetChanged(ChangeMask value) {

		CUP_FUNCTION();

		m_changed = value;
		for (uint32 id : m_children)
			GetEntityFromID(id)->GetTransform()->SetChanged(value);

	}
	
}