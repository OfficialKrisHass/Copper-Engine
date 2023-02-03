#include "MetaFileSerialization.h"

#include "Panels/SceneHierarchy.h"

#include <yaml-cpp/yaml.h>

using namespace Copper;

namespace Editor::MetaFile {

	void SceneMeta::Serialize(Scene* scene) {

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
		for (int32_t objID : objectIDs) {

			out << objID;

		}
		out << YAML::EndSeq; //Objects

		out << YAML::EndMap; //Main Map
		std::ofstream file(scene->path.string() + ".cum");
		file << out.c_str();

	}
	void SceneMeta::Deserialize(Scene* scene) {

		YAML::Node main;
		try { main = YAML::LoadFile(scene->path.string() + ".cum"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Scene Meta Data file\n{}\n    {}", scene->path, e.what());
			return;

		}

		YAML::Node objects = main["Objects"];
		objectIDs.clear();
		for (int i = 0; i < objects.size(); i++) {

			objectIDs.push_back(objects[i].as<int32_t>());

		}

	}

}