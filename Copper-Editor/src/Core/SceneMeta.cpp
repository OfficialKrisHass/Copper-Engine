#include "SceneMeta.h"

#include "Core/EditorApp.h"

#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/Everything.h"

using namespace Copper;

using std::filesystem::exists;

namespace Editor::MetaFile {

	void SceneMeta::Serialize() {

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		// Entity Order

		out << YAML::Key << "Entity Order" << YAML::Value << YAML::BeginSeq; // Entity Order
		for (uint32_t objID : objectIDs) {

			out << objID;

		}
		out << YAML::EndSeq; // Entity Order

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
		file.open(scene->path.string() + ".cum");
		file << out.c_str();
		file.close();

	}
	void SceneMeta::Deserialize(Scene* scene) {

		objectIDs.clear();
		this->scene = scene;

		if (!exists(scene->path.string() + ".cum")) {

			InitDefault();
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

		YAML::Node order = main["Entity Order"];
		for (int i = 0; i < order.size(); i++) {

			objectIDs.push_back(order[i].as<uint32_t>());

		}

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

	void SceneMeta::InitDefault() {

		objectIDs.reserve(GetNumOfEntities());
		for (InternalEntity* entity : EntityView(GetScene()))
			objectIDs.push_back(entity->ID());

		Serialize();

	}

}