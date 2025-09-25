#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

COMPONENT_FORWARD_DECL()

#ifdef CU_EDITOR
namespace Editor { class SceneCamera; }
#endif

namespace Copper {

    // Stores data about position, rotation, scale and parents/children. Every entity has one
    // Position, rotation and scale are local-space, to get the global-space version use the functiom GlobalPosition/Rotation/Scale()
    class Transform : public Component {

        COMPONENT_FRIEND_CLASSES();
        friend class Registry;

#ifdef CU_EDITOR
        friend Editor::SceneCamera;
#endif

    public:
        Transform() = default;

        // Setters

        void SetPosition(const Vector3& position);
        void SetRotation(const Quaternion& rotation);
        void SetScale(const Vector3& scale);

        void SetGlobalPosition(const Vector3& position);
        void SetGlobalRotation(const Quaternion& rotation);
        void SetGlobalScale(const Vector3& scale);

        inline void AddPosition(const Vector3& position) { SetPosition(m_position + position); }
        inline void AddRotation(const Quaternion& rotation) { SetRotation(m_rotation + rotation); }
        inline void AddScale(const Vector3& scale) { SetScale(m_scale + scale); }

        // Getters

        inline const Vector3& GetPosition() const { return m_position; }
        inline const Quaternion& GetRotation() const { return m_rotation; }
        inline const Vector3& GetScale() const { return m_scale; }

        inline const Vector3& GetGlobalPosition() const { return m_globalPosition; }
        inline const Quaternion& GetGlobalRotation() const { return m_globalRotation; }
        inline const Vector3& GetGlobalScale() const { return m_globalScale; }

        inline const Vector3& GetForward() const { return m_forward; }
        inline const Vector3& GetRight()   const { return m_right; }
        inline const Vector3& GetUp()      const { return m_up; }

        // Parent

        void SetParent(Transform* parent);

        inline Transform* GetParent() const { return m_parent; }

        // Children

        void AddChild(Transform* transform);
        void RemoveChild(Transform* transform);
        void RemoveChild(uint32 index);

        Transform* GetChild(uint32 index) const;
        inline uint32 GetChildCount() const { return (uint32) m_children.size(); }

        // Transform matrix

        inline const Matrix4& GetTransformMatrix() const { return m_mat; }

    private:
        Vector3 m_position = Vector3::zero;
        Quaternion m_rotation = Quaternion::identity;
        Vector3 m_scale = Vector3::one;

        bool m_dirty = false;
        Matrix4 m_mat = Matrix4(1.0f);

        // Global-space data

        Vector3 m_globalPosition = Vector3::zero;
        Quaternion m_globalRotation = Quaternion::identity;
        Vector3 m_globalScale = Vector3::one;

        // Directions based on rotation

        Vector3 m_forward = Vector3(0.0f, 0.0f, -1.0f);
        Vector3 m_right = Vector3(1.0f, 0.0f, 0.0f);
        Vector3 m_up = Vector3(0.0f, 1.0f, 0.0f);

        // Parent Data

        Transform* m_parent = nullptr;

        // Children Data

        std::vector<uint32> m_children;

        void CalculateMatrix();

        void UpdatePosition();
        void UpdateRotation();
        void UpdateScale();

        void ExtractGlobalRotation();

    };

} 
