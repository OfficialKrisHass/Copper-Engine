#include "SceneMeta.h"

#include "Core/EditorApp.h"
#include "Viewport/SceneCamera.h"

#include "Engine/YAMLOverloads/Everything.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

using namespace Copper;
using std::filesystem::exists;

namespace Editor {

	void SceneMeta::Serialize(const fs::path& path) {

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		// Scene Camera

		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap; // Scene Camera
		const SceneCamera& sceneCam = GetSceneCam();

		out << YAML::Key << "Position" << YAML::Value << sceneCam.GetTransform()->Position();
		out << YAML::Key << "Rotation" << YAML::Value << sceneCam.GetTransform()->Rotation();

		out << YAML::Key << "Speed" << YAML::Value << GetSceneCam().speed;
		out << YAML::Key << "Sensitivity" << YAML::Value << GetSceneCam().sensitivity;

		out << YAML::EndMap; // SceneCamera

		out << YAML::EndMap; // Main
		std::ofstream file;
		file.open(path.string() + ".cum");
		file << out.c_str();
		file.close();

	}
	void SceneMeta::Deserialize(const fs::path& path) {

		if (!exists(path.string() + ".cum")) {

			Serialize(path);
			return;

		}

		YAML::Node main;
		try {
			
			main = YAML::LoadFile(path.string() + ".cum");
		
		} catch (YAML::ParserException e) {

			LogError("Failed to Read The Scene Meta Data file ({})\n    {}", path.string(), e.what());
			return;

		}

		try {

		YAML::Node sceneCam = main["Scene Camera"];
		SceneCamera& cam = GetSceneCam();

		cam.GetTransform()->SetPosition(sceneCam["Position"].as<Vector3>());
		cam.GetTransform()->SetRotation(sceneCam["Rotation"].as<Quaternion>());

		cam.speed = sceneCam["Speed"].as<float>();
		cam.sensitivity = sceneCam["Sensitivity"].as<float>();

		} catch (YAML::Exception e) {

			Log("Encountered an exception when trying to Deserializae the {} Scene meta file!\n\t{}", path.string(), e.msg);
			return;

		}

	}

}
