#pragma once

#include "Registry.h"

#include "Engine/Core/Core.h"
#include "cupch.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

namespace Copper {

	class Scene {

		friend class Object;

	public:

		std::string name;
		std::filesystem::path path;

		Camera* cam;

		//Objects
		Object CreateObject(Vector3 position, Vector3 rotation, Vector3 scale, std::string name = "Object");
		Object CreateObject(std::string name = "Object");

		void DestroyObject(Object& obj) { registry.DestroyObject(obj); }

		//Runtime
		void StartRuntime();
		void Update(bool render);

		//Rendering
		void Render(Camera* cam);

		//Serialization
		void Serialize(std::filesystem::path path);
		bool Deserialize(std::filesystem::path path);

		//Getters
		Registry* GetRegistry() { return &registry; }
		Object& GetObjectFromID(int32_t id) { return registry.GetObjectFromID(id); }

	private:
		Registry registry;

		Light* light = nullptr;

		bool runtimeRunning = false;
		bool runtimeStarted = false;

	};

}