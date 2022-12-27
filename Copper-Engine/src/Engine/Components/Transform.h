#pragma once

#include "Engine/Core/Core.h"

#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Object;
	class Scene;
	class Registry;

	class Transform {

		friend class Scene;
		friend class Registry;

	public:
		Transform() = default;
		Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

		Vector3 position = Vector3::zero;
		Vector3 rotation = Vector3::zero;
		Vector3 scale = Vector3::one;

		Vector3 forward;
		Vector3 backward;
		Vector3 up;
		Vector3 down;
		Vector3 left;
		Vector3 right;

		Shared<Object> object;
		uint32_t index = 0;

		Transform* parent;
		int numOfChildren;

		glm::mat4 CreateMatrix();
		void Update();

		Vector3 GlobalPosition();

		Transform* GetChild(int index) const;

		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(int index);

		bool operator==(const Transform& other) const;

	private:
		const static bool multipleOnOneObject = false;
		bool valid;

		std::vector<int32_t> children;


	};

}