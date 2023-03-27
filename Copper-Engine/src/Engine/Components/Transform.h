#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Component.h"

#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Transform : public Component {

		friend class Scene;
		friend class OldSceneDeserialization;

	public:
		Transform() = default;
		Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

		Vector3 position = Vector3::zero;
		Vector3 rotation = Vector3::zero;
		Vector3 scale = Vector3::one;

		Vector3 GlobalPosition() const;

		//Parent
		void SetParent(Transform* parent);
		Transform* Parent() const { return parent; }

		//Children
		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);
		void RemoveChild(int index);
		Transform* GetChild(int index) const;

		uint32_t NumOfChildren() const { return (uint32_t) children.size(); }

		//Rotation
		const Vector3& Forward() const { return forward; }
		const Vector3& Back()    const { return back; }
		const Vector3& Up()      const { return up; }
		const Vector3& Down()    const { return down; }
		const Vector3& Right()   const { return right; }
		const Vector3& Left()    const { return left; }

		glm::mat4 CreateMatrix();

		bool operator==(const Transform& other) const;

		void Update();

	private:
		Vector3 forward;
		Vector3 back;
		Vector3 up;
		Vector3 down;
		Vector3 left;
		Vector3 right;

		//Parent Data
		Transform* parent = nullptr;
		int32_t parentChildIndex = -1;

		//Children Data
		std::vector<int32_t> children;

	};

	//class Object;
	//class Scene;
	//class Registry;

	//class Transform {

	//	friend class Scene;
	//	friend class Registry;

	//public:
	//	Transform() = default;
	//	Transform(Vector3 position, Vector3 rotation, Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

	//	Vector3 position = Vector3::zero;
	//	Vector3 rotation = Vector3::zero;
	//	Vector3 scale = Vector3::one;

	//	Vector3 GlobalPosition() const;

	//	//Parent
	//	void SetParent(Transform* parent);
	//	Transform* Parent() const { return parent; }

	//	//Children
	//	void AddChild(Transform* transform);
	//	void RemoveChild(Transform* transform);
	//	void RemoveChild(int index);
	//	Transform* GetChild(int index) const;

	//	int NumOfChildren() const { return numOfChildren; }

	//	//Rotation
	//	const Vector3& Forward() const { return forward; }
	//	const Vector3& Back()    const { return back; }
	//	const Vector3& Up()      const { return up; }
	//	const Vector3& Down()    const { return down; }
	//	const Vector3& Right()   const { return right; }
	//	const Vector3& Left()    const { return left; }

	//	Object* ObjectGet() const { return object; }

	//	glm::mat4 CreateMatrix();

	//	bool operator==(const Transform& other) const;

	//	void Update();

	//private:
	//	Vector3 forward;
	//	Vector3 back;
	//	Vector3 up;
	//	Vector3 down;
	//	Vector3 left;
	//	Vector3 right;

	//	//Object Data
	//	//Shared<class Object> object;
	//	Object* object;

	//	//Parent Data
	//	Transform* parent = nullptr;
	//	int parentChildIndex = -1;

	//	//Children Data
	//	std::vector<int32_t> children;
	//	int numOfChildren = 0;

	//	//Registry needed Data
	//	uint32_t index = 0;
	//	const static bool multipleOnOneObject = false;
	//	bool valid = false;

	//};

}