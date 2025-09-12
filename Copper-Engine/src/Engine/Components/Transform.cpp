#include "cupch.h"
#include "Transform.h"

#include "Engine/Scene/Scene.h"

#include "Engine/Utilities/Math.h"

namespace Copper {

    Quaternion ExtractRotation(const Matrix4& mat, const Vector3& scale);

    void Transform::SetPosition(const Vector3& position) {

        CUP_FUNCTION();

        m_position = position;
        UpdatePosition();

    }
    void Transform::SetRotation(const Quaternion& rotation) {

        CUP_FUNCTION();

        m_rotation = rotation;
        UpdateRotation();

    }
    void Transform::SetScale(const Vector3& scale) {

        CUP_FUNCTION();

        m_scale = scale;
        UpdateScale();

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
        m_rotation = ExtractRotation(mat, m_parent->m_globalScale);

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

        if (m_parent) {

            m_parent->RemoveChild(this);
            Matrix4 local = m_mat * CMath::Inverse(parent->m_mat);

            m_parent = parent;
            parent->m_children.push_back(GetEntity()->GetID());

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
        parent->m_children.push_back(GetEntity()->GetID());

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
        m_children.push_back(child->GetEntity()->GetID());

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

            if (m_children[i] != transform->GetEntity()->GetID()) continue;

            RemoveChild(i);
            return;

        }

        LogError("Can't remove a child. Parent: {}, Child: {}", *GetEntity(), *transform->GetEntity());

    }
    Transform* Transform::GetChild(uint32 index) const {

        CUP_FUNCTION();

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

        m_globalRotation = ExtractRotation(m_mat, m_globalScale);

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

    Quaternion Transform::ExtractRotation(const Matrix4& mat, const Vector3& scale) {

        CUP_FUNCTION();

        // Guard against zero scale

        if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f)
            return Quaternion::identity;

        // Get and normalize rotation axes

        Vector3 col1 = mat.cols[0] / scale.x;
        Vector3 col2 = mat.cols[1] / scale.y;
        Vector3 col3 = mat.cols[2] / scale.z;

        Matrix3 rot = Matrix3(col1, col2, col3);
        float trace = rot[0][0] + rot[1][1] + rot[2][2];
        Quaternion ret;

        if (trace > 0.0f) {

            float s = std::sqrt(trace + 1.0f) * 2.0f;

            ret.w = 0.25f * s;
            ret.x = (rot[1][2] - rot[2][1]) / s;
            ret.y = (rot[2][0] - rot[0][2]) / s;
            ret.z = (rot[0][1] - rot[1][0]) / s;

            return ret;

        }

        if (rot[0][0] > rot[1][1] && rot[0][0] > rot[2][2]) {

            float s = std::sqrt(1.0f + rot[0][0] - rot[1][1] - rot[2][2]) * 2.0f;

            ret.w = (rot[1][2] - rot[2][1]) / s;
            ret.x = 0.25f * s;
            ret.y = (rot[0][1] + rot[1][0]) / s;
            ret.z = (rot[0][2] + rot[2][0]) / s;

            return ret;

        } else if (rot[1][1] > rot[2][2]) {

            float s = std::sqrt(1.0f + rot[1][1] - rot[0][0] - rot[2][2]) * 2.0f;

            ret.w = (rot[2][0] - rot[0][2]) / s;
            ret.x = (rot[0][1] + rot[1][0]) / s;
            ret.y = 0.25f * s;
            ret.z = (rot[1][2] + rot[2][1]) / s;

            return ret;

        }

        float s = std::sqrt(1.0f + rot[2][2] - rot[0][0] - rot[1][1]) * 2.0f;

        ret.w = (rot[0][1] - rot[1][0]) / s;
        ret.x = (rot[0][2] + rot[2][0]) / s;
        ret.y = (rot[1][2] + rot[2][1]) / s;
        ret.z = 0.25f * s;

        return ret;

    }

}
