#include "cupch.h"
#include "Scene.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/EntityView.h"
#include "Engine/Scene/OldSceneVersionSerializer.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/Light.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Copper {

	int cCounter = 0;

	void Scene::StartRuntime() {

		runtimeRunning = true;

	}
	void Scene::Update(bool render) {

		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		for (InternalEntity* entity : EntityView(this)) {

			entity->transform->Update();

			if (entity->HasComponent<Light>()) light = entity->GetComponent<Light>();
			if (entity->HasComponent<Camera>()) cam = entity->GetComponent<Camera>();
			if (entity->HasComponent<MeshRenderer>()) {

				MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
				for (Mesh mesh : renderer->meshes) {

					Renderer::AddMesh(&mesh, entity->transform);

				}

			}
			if (runtimeRunning && entity->HasComponent<ScriptComponent>()) {

				ScriptComponent* script = entity->GetComponent<ScriptComponent>();

				if (!runtimeStarted) script->InvokeCreate();
				script->InvokeUpdate();

			}

		}
		if (runtimeRunning && !runtimeStarted) runtimeStarted = true;

		if (cam && render) Render(cam);

	}
	void Scene::Render(Camera* camera) {

		Renderer::RenderFrame(camera, light);

	}

	void Scene::Serialize(const std::filesystem::path& path) {

		this->path = path;
		this->name = path.stem().string();

		YAML::Emitter out;
		if (VERSION_STAGE == "Beta" && VERSION_MAJOR == 1 && VERSION_MINOR == 0 && VERSION_DEV == 0) {

			OldSceneVersionSerializer::SerializeVersion_Beta1_0_0(out, this);
			return;

		}
		
	}
	bool Scene::Deserialize(const std::filesystem::path& path) {

		this->path = path;

		YAML::Node data;
		try { data = YAML::LoadFile(path.string()); } catch(YAML::ParserException e) {

			LogError("Failed to Read .scene file. {0}\n    {1}", path.string(), e.what());
			return false;
			
		}

		YAML::Node version = data["Version"];
		std::string versionStage = version[0].as<std::string>();
		uint32_t versionMajor = version[1].as<uint32_t>();
		uint32_t versionMinor = version[2].as<uint32_t>();
		uint32_t versionDev = version[3].as<uint32_t>();

		if (versionStage == "Beta" && versionMajor == 1 && versionMinor == 0 && versionDev == 0)
			return OldSceneVersionSerializer::DeserializeVersion_Beta1_0_0(data, this);

		return true;
		
	}

}