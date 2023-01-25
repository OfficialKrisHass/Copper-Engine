#pragma once

#include "Registry.h"

#include "Engine/Core/Core.h"
#include "cupch.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/Light.h"

#define ObjectDefaultParams Vector3 position = Vector3::zero, Vector3 rotation = Vector3::zero, Vector3 scale = Vector3::one, const std::string& name = "Object"

namespace Copper {

	class Scene {

		friend class Object;

	public:

		std::string name;
		std::filesystem::path path;

		Camera* cam;

		//Objects
		Object CreateObject(ObjectDefaultParams);
		Object CreateObject(const std::string& name = "Object");
		
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
		Object& CreateObjectFromID(int32_t id, ObjectDefaultParams) { return registry.CreateObjectFromID(id, this, position, rotation, scale, name); }

		uint32_t GetNumOfObjects() { return (uint32_t) registry.objects.size(); }

		bool IsRuntimeRunning() { return runtimeRunning; }

	private:
		Registry registry;

		Light* light = nullptr;

		bool runtimeRunning = false;
		bool runtimeStarted = false;

		template<typename T> void WriteField(YAML::Emitter& out, ScriptField& field, ScriptComponent* scriptInstance);
		template<typename T> void ReadField(YAML::Node& value, ScriptField& field, ScriptComponent* scriptInstance);

	};

}