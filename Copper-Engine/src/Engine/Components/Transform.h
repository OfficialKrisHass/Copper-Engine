#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

namespace Copper {

	// Stores data about position, rotation, scale and parents/children. Every entity has one
	// Position, rotation and scale are local-space, to get the global-space version use the functiom GlobalPosition/Rotation/Scale()
	class Transform : public Component {

		friend class Registry;
		friend class Scene;
		friend class OldSceneDeserialization;

	#ifdef CU_EDITOR
		friend Editor::SceneCamera;
	#endif

	public:
		Transform();
		Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
		Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

		// Setters

		void SetPosition(const Vector3& position);
		void SetRotation(const Quaternion& rotation);
		void SetScale(const Vector3& scale);

		inline void AddPosition(const Vector3& position) { SetPosition(m_position + position); }
		inline void AddRotation(const Quaternion& rotation) { SetRotation(m_rotation + rotation); }
		inline void AddScale(const Vector3& scale) { SetScale(m_scale + scale); }

		// Getters

		inline const Vector3& Position() const { return m_position; }
		inline const Quaternion& Rotation() const { return m_rotation; }
		inline const Vector3& Scale() const { return m_scale; }

		inline const Vector3& GlobalPosition() const { return m_globalPosition; }
		inline const Quaternion& GlobalRotation() const { return m_globalRotation; }
		inline const Vector3& GlobalScale() const { return m_globalScale; }

		inline const Vector3& Forward() const { return m_forward; }
		inline const Vector3& Right()   const { return m_right; }
		inline const Vector3& Up()      const { return m_up; }

		// Parent

		void SetParent(Transform* parent);

		inline Transform* Parent() const { return m_parent; }

		// Children

		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(uint32 index);

		inline Transform* GetChild(uint32 index) const { return GetEntityFromID(m_children[index])->GetTransform(); }
		inline uint32 NumOfChildren() const { return (uint32) m_children.size(); }

		// Transform matrix

		inline const Matrix4& TransformMatrix() const { return m_mat; }

		// Operator overloads

		inline bool operator==(const Transform& other) const { return GetEntity() == other.GetEntity(); }

	protected:
		void Update();

	private:
		Vector3 m_position = Vector3::zero;
		Quaternion m_rotation = Quaternion(1.0f, 0.0, 0.0f, 0.0f);
		Vector3 m_scale = Vector3::one;

		Matrix4 m_mat = Matrix4(1.0f);
		bool m_calculated = false;

		// Global-space data

		Vector3 m_globalPosition = Vector3::zero;
		Quaternion m_globalRotation = Quaternion(1.0f, 0.0, 0.0f, 0.0f);
		Vector3 m_globalScale = Vector3::one;

		// Directions based on rotation

		Vector3 m_forward = Vector3(0.0f, 0.0f, -1.0f);
		Vector3 m_right = Vector3(1.0f, 0.0f, 0.0f);
		Vector3 m_up = Vector3(0.0f, 1.0f, 0.0f);

		// ChangeMask

		typedef uint8 ChangeMask;
		ChangeMask m_changed = 0;

		// Parent Data

		Transform* m_parent = nullptr;

		// Children Data

		std::vector<uint32> m_children;

		void CalculateMatrix();

		void SetChanged(ChangeMask value);

	};

} 