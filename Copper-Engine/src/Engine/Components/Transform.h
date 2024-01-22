#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

namespace Copper {

	class Transform : public Component {

		friend class Registry;
		friend class Scene;
		friend class OldSceneDeserialization;

	public:
		Transform() = default;
		Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) : position(position), rotation(rotation), scale(scale) {}
		Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : position(position), rotation(rotation), scale(scale) {}

		Vector3 position = Vector3::zero;
		Quaternion rotation = Quaternion(1.0f, 0.0, 0.0f, 0.0f);
		Vector3 scale = Vector3::one;

		inline Vector3 GlobalPosition() const { return m_globalPosition; }
		inline Quaternion GlobalRotation() const { return m_globalRotation; }
		inline Vector3 GlobalScale() const { return m_globalScale; }

		// Parent

		void SetParent(Transform* parent);
		inline Transform* Parent() const { return m_parent; }

		// Children

		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(uint32 index);
		Transform* GetChild(uint32 index) const { return GetEntityFromID(m_children[index])->GetTransform(); }

		uint32 NumOfChildren() const { return (uint32) m_children.size(); }

		// Directionals

		const Vector3& Forward() const { return m_forward; }
		const Vector3& Right()   const { return m_right; }
		const Vector3& Up()      const { return m_up; }

		Matrix4 CreateMatrix();

		bool operator==(const Transform& other) const;

		void Update();

	private:
		Vector3 m_forward;
		Vector3 m_right;
		Vector3 m_up;

		Vector3 m_globalPosition;
		Quaternion m_globalRotation;
		Vector3 m_globalScale;

		// Parent Data

		Transform* m_parent = nullptr;

		// Children Data

		std::vector<uint32> m_children;

	};

} 