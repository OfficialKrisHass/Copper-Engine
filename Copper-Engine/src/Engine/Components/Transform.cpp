#include "cupch.h"
#include "Transform.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Components/RigidBody.h"
#include "Engine/Components/Collider.h"

#include "Engine/Utilities/Math.h"

namespace Copper {

    Quaternion ExtractRotation(const Matrix4& mat, const Vector3& scale);

    void Transform::SetPosition(const Vector3& position) {

        CUP_FUNCTION();

        if (m_position == position) return;

        m_position = position;
        UpdatePosition();

        // TODO: A Transform component should keep a reference to the rigidbody, meaning this is unnecessary
        // however atm there is no way to keep that reference up to date. Fix this pls.

        if (!GetEntity().IsValid()) return;
        if (RigidBody* rb = GetEntity()->GetComponent<RigidBody>())
            rb->SetPosition(position);

    }
    void Transform::SetRotation(const Quaternion& rotation) {

        CUP_FUNCTION();

        if (m_rotation == rotation) return;

        m_rotation = rotation;
        UpdateRotation();

        // TODO: A Transform component should keep a reference to the rigidbody, meaning this is unnecessary
        // however atm there is no way to keep that reference up to date. Fix this pls.

        if (!GetEntity().IsValid()) return;
        if (RigidBody* rb = GetEntity()->GetComponent<RigidBody>())
            rb->SetRotation(rotation);

    }
    void Transform::SetScale(const Vector3& scale) {

        CUP_FUNCTION();

        if (m_scale == scale) return;

        m_scale = scale;
        UpdateScale();

        // TODO: A Transform component should keep a reference to the rigidbody, meaning this is unnecessary
        // however atm there is no way to keep that reference up to date. Fix this pls.

        if (!GetEntity().IsValid()) return;
        if (Collider* collider = GetEntity()->GetComponent<Collider>())
            collider->RecreateShape();

    }

    void Transform::SetGlobalPosition(const Vector3& position) {

        CUP_FUNCTION();

        if (m_parent == nullptr) {

            SetPosition(position);
            return;

        }

        m_position = CMath::Inverse(m_parent->m_mat) * Vector4(position, 1.0f);
        UpdatePosition();

    }
    void Transform::SetGlobalRotation(const Quaternion& rotation) {

        CUP_FUNCTION();

        if (m_parent == nullptr) {

            SetRotation(rotation);
            return;

        }

        Matrix4 mat = CMath::Inverse(m_parent->m_mat) * Matrix4(rotation);

        Vector3 localPos, localScale;
        Quaternion localRot;
        Math::DecomposeTransform(mat, localPos, localRot, localScale);

        m_rotation = localRot;

        UpdateRotation();

    }
    void Transform::SetGlobalScale(const Vector3& scale) {

        CUP_FUNCTION();

        if (m_parent == nullptr) {

            SetScale(scale);
            return;

        }

        m_scale = scale / m_parent->m_globalScale;
        UpdateScale();

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

        if (m_parent != nullptr) {

            m_parent->RemoveChild(this);
            m_parent = parent;

            parent->m_children.push_back(GetEntity()->GetID());

        } else { // Case 3: Setting a parent (old parent == nullptr)

            m_parent = parent;
            parent->m_children.push_back(GetEntity()->GetID());

        }

        Matrix4 local = m_mat * CMath::Inverse(parent->m_mat);

        Vector3 pos, scale;
        Quaternion rot;
        Math::DecomposeTransform(local, pos, rot, scale);

        m_position = pos;
        m_rotation = rot;
        m_scale = scale;

    }

    // Children

    void Transform::AddChild(Transform* child) {

        CUP_FUNCTION();

        if (child == nullptr || child->m_parent == this) return;

        if (child->m_parent != nullptr)
            child->m_parent->RemoveChild(child);

        child->m_parent = this;
        m_children.push_back(child->GetEntity()->GetID());

        Matrix4 childLocal = child->m_mat * CMath::Inverse(m_mat);

        Vector3 pos, scale;
        Quaternion rot;
        Math::DecomposeTransform(childLocal, pos, rot, scale);

        child->m_position = pos;
        child->m_rotation = rot;
        child->m_scale = scale;

    }
    void Transform::RemoveChild(uint32 index) {

        CUP_FUNCTION();

        CU_EDITOR_ASSERT_RETURN(index >= 0 && index < m_children.size(),, "Can't remove child, index out of range. Parent '{}', index: '{}'", *GetEntity(), index);

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

            if (m_children[i] != transform->GetEntity()->GetID()) continue;

            RemoveChild(i);
            return;

        }

        LogError("Child is not child of parent, could not remove. Child: '{}', Parent: '{}'", *transform->GetEntity(), *GetEntity());

    }
    Transform* Transform::GetChild(uint32 index) const {

        CUP_FUNCTION();

        CU_EDITOR_ASSERT_RETURN(index < m_children.size(), nullptr, "Invalid child index '{}'. Transform: '{}'", index, GetEntity());
        CU_ASSERT(GetEntityFromID(m_children[index]) != nullptr, "Transform has an invalid child. Index: '{}'", index);

        return GetEntityFromID(m_children[index])->GetTransform();

    }

    // Calculations

    void Transform::CalculateMatrix() {

        CUP_FUNCTION();

        if (!m_dirty) return;

        // First calculate the matrix

        m_mat = Matrix4(1.0f);

        if (m_parent != nullptr) {

            m_parent->CalculateMatrix();
            m_mat *= m_parent->m_mat;

        }

        CMath::TranslateMatrix(m_mat, m_position);
        m_mat = m_mat * (Matrix4) m_rotation;
        CMath::ScaleMatrix(m_mat, m_scale);
        
        m_dirty = false;

    }

    void Transform::UpdatePosition() {

        CUP_FUNCTION();

        m_dirty = true;
        CalculateMatrix();

        m_globalPosition = Vector3(m_mat[3].x, m_mat[3].y, m_mat[3].z);

        for (uint32 childID : m_children) {

            InternalEntity* child = GetEntityFromID(childID);
            CU_ASSERT(child != nullptr, "Could not get child from Entity {}, childID: {}", *GetEntity(), childID);

            child->GetTransform()->UpdatePosition();

        }

    }
    void Transform::UpdateRotation() {

        CUP_FUNCTION();

        m_dirty = true;
        CalculateMatrix();
        ExtractGlobalRotation();

        m_forward = m_globalRotation * Vector3(0.0f, 0.0f, -1.0f);
        m_right = m_globalRotation * Vector3(1.0f, 0.0f, 0.0f);
        m_up = m_globalRotation * Vector3(0.0f, 1.0f, 0.0f);

        for (uint32 childID : m_children) {

            InternalEntity* child = GetEntityFromID(childID);
            CU_ASSERT(child != nullptr, "Could not get child from Entity {}, childID: {}", *GetEntity(), childID);

            child->GetTransform()->UpdateRotation();

        }

    }
    void Transform::UpdateScale() {

        CUP_FUNCTION();

        m_dirty = true;
        CalculateMatrix();

        Vector3 col1 = m_mat[0];
        Vector3 col2 = m_mat[1];
        Vector3 col3 = m_mat[2];
        m_globalScale = Vector3(col1.Length(), col2.Length(), col3.Length());

        for (uint32 childID : m_children) {

            InternalEntity* child = GetEntityFromID(childID);
            CU_ASSERT(child != nullptr, "Could not get child from Entity {}, childID: {}", *GetEntity(), childID);

            child->GetTransform()->UpdateScale();

        }

    }

    void Transform::ExtractGlobalRotation() {

        CUP_FUNCTION();

        // Guard against zero scale

        if (m_globalScale.x == 0.0f || m_globalScale.y == 0.0f || m_globalScale.z == 0.0f) {

            m_globalRotation = Quaternion::identity;
            return;

        }

        // Get and normalize rotation axes

        Vector3 col1 = m_mat[0] / m_globalScale.x;
        Vector3 col2 = m_mat[1] / m_globalScale.y;
        Vector3 col3 = m_mat[2] / m_globalScale.z;

        Matrix3 rot = Matrix3(col1, col2, col3);
        float trace = rot[0][0] + rot[1][1] + rot[2][2];

        if (trace > 0.0f) {

            float s = std::sqrt(trace + 1.0f) * 2.0f;

            m_globalRotation.w = 0.25f * s;
            m_globalRotation.x = (rot[1][2] - rot[2][1]) / s;
            m_globalRotation.y = (rot[2][0] - rot[0][2]) / s;
            m_globalRotation.z = (rot[0][1] - rot[1][0]) / s;

            return;

        }

        if (rot[0][0] > rot[1][1] && rot[0][0] > rot[2][2]) {

            float s = std::sqrt(1.0f + rot[0][0] - rot[1][1] - rot[2][2]) * 2.0f;

            m_globalRotation.w = (rot[1][2] - rot[2][1]) / s;
            m_globalRotation.x = 0.25f * s;
            m_globalRotation.y = (rot[0][1] + rot[1][0]) / s;
            m_globalRotation.z = (rot[0][2] + rot[2][0]) / s;

            return;

        } else if (rot[1][1] > rot[2][2]) {

            float s = std::sqrt(1.0f + rot[1][1] - rot[0][0] - rot[2][2]) * 2.0f;

            m_globalRotation.w = (rot[2][0] - rot[0][2]) / s;
            m_globalRotation.x = (rot[0][1] + rot[1][0]) / s;
            m_globalRotation.y = 0.25f * s;
            m_globalRotation.z = (rot[1][2] + rot[2][1]) / s;

            return;

        }

        float s = std::sqrt(1.0f + rot[2][2] - rot[0][0] - rot[1][1]) * 2.0f;

        m_globalRotation.w = (rot[0][1] - rot[1][0]) / s;
        m_globalRotation.x = (rot[0][2] + rot[2][0]) / s;
        m_globalRotation.y = (rot[1][2] + rot[2][1]) / s;
        m_globalRotation.z = 0.25f * s;

        return;

    }

}
