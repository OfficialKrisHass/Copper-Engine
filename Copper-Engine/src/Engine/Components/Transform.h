#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#include "cupch.h"

#include <GLM/mat4x4.hpp>

#include <GLM/gtx/euler_angles.hpp>

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

		Vector3 GlobalPosition() const;
		Quaternion GlobalRotation() const;
		Vector3 GlobalScale() const;

		//Parent
		void SetParent(Transform* parent);
		Transform* Parent() const { return parent; }

		//Children
		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(int index);
		Transform* GetChild(int index) const;

		uint32_t NumOfChildren() const { return (uint32_t) children.size(); }

		const Vector3& Forward() const { return forward; }
		const Vector3& Right()   const { return right; }
		const Vector3& Up()      const { return up; }

		Matrix4 CreateMatrix();

		bool operator==(const Transform& other) const;

		void Update();

	private:
		Vector3 forward;
		Vector3 right;
		Vector3 up;

		//Parent Data
		Transform* parent = nullptr;

		//Children Data
		std::vector<int32_t> children;

	};

}