#include "SceneMeta.h"

#include "Core/EditorApp.h"

#include "Engine/YAMLOverloads/Everything.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

using namespace Copper;
using std::filesystem::exists;

namespace Editor::MetaFile {

	void SceneMeta::Serialize() {

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		// Scene Camera

		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap; // Scene Camera
		const SceneCamera& sceneCam = GetSceneCam();

		out << YAML::Key << "Position" << YAML::Value << sceneCam.GetTransform()->position;
		out << YAML::Key << "Rotation" << YAML::Value << sceneCam.GetTransform()->rotation;

		out << YAML::Key << "Speed" << YAML::Value << GetSceneCam().speed;
		out << YAML::Key << "Sensitivity" << YAML::Value << GetSceneCam().sensitivity;

		out << YAML::EndMap; // SceneCamera

		out << YAML::EndMap; // Main
		std::ofstream file;
		file.open(m_scene->path.string() + ".cum");
		file << out.c_str();
		file.close();

	}
	void SceneMeta::Deserialize(Scene* scene) {

		this->m_scene = scene;

		if (!exists(scene->path.string() + ".cum")) {

			Serialize();
			return;

		}

		YAML::Node main;
		try {
			
			main = YAML::LoadFile(scene->path.string() + ".cum");
		
		} catch (YAML::ParserException e) {

			LogError("Failed to Read The Scene Meta Data file ({})\n    {}", scene->path.string(), e.what());
			return;

		}

		try {

		YAML::Node sceneCam = main["Scene Camera"];
		SceneCamera& cam = GetSceneCam();

		cam.GetTransform()->position = sceneCam["Position"].as<Vector3>();
		cam.GetTransform()->rotation = sceneCam["Rotation"].as<Quaternion>();

		cam.speed = sceneCam["Speed"].as<float>();
		cam.sensitivity = sceneCam["Sensitivity"].as<float>();

		} catch (YAML::Exception e) {

			Log("Encountered an exception when trying to Deserializae the {} Scene meta file!\n\t{}", scene->name, e.msg);
			return;

		}

	}

}