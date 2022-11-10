#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Object;
	class Scene;

	class Transform {

		friend class Scene;

	public:
		Transform() = default;
		Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

		Vector3 position = Vector3::Zero();
		Vector3 rotation = Vector3::Zero();
		Vector3 scale = Vector3::One();

		Shared<Object> object;
		Transform* parent;
		int numOfChildren;

		glm::mat4 CreateMatrix();

		Vector3 Forward();
		Vector3 Right();
		Vector3 Up();
		Vector3 Backward();
		Vector3 Left();
		Vector3 Down();

		Vector3 GlobalPosition();

		Transform* GetChild(int index) const;

		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(int index);

		bool operator==(const Transform& other) const;

	private:
		std::vector<int32_t> children;

	};

}