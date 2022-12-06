#pragma once

#include "Registry.h"

#include "Engine/Core/Core.h"
#include "cupch.h"

#include "Engine/Components/Camera.h"

namespace Copper {

	class Scene {

		friend class Object;

	public:
		Registry registry;

		std::string name;
		std::filesystem::path path;

		Camera* cam;

		Object CreateObject(Vector3 position, Vector3 rotation, Vector3 scale, std::string name = "Object");
		Object CreateObject(std::string name = "Object");

		void DestroyObject(Object& obj) { registry.DestroyObject(obj); }

		void RuntimeStart();
		void RuntimeUpdate();

		void Update();

		void Serialize(std::filesystem::path path);
		bool Deserialize(std::filesystem::path path);

	};

}