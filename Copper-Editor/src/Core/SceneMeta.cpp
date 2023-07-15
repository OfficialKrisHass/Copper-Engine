#include "SceneMeta.h"

#include "Panels/SceneHierarchy.h"

#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/YAMLOverloads.h"

using namespace Copper;

namespace Editor::MetaFile {

	void SceneMeta::Serialize(Scene* scene) {

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;
		for (uint32_t objID : objectIDs) {

			out << objID;

		}
		out << YAML::EndSeq; //Objects

		out << YAML::EndMap; //Main Map
		std::ofstream file;
		file.open(scene->path.String() + ".cum");
		file << out.c_str();
		file.close();

	}
	void SceneMeta::Deserialize(Scene* scene) {

		objectIDs.clear();

		if (!std::experimental::filesystem::exists(scene->path.String() + "cum")) {

			for (InternalEntity* entity : EntityView(scene)) {

				objectIDs.push_back(entity->ID());

			}
			Serialize(scene);

			return;

		}

		YAML::Node main;
		try {
			
			main = YAML::LoadFile(scene->path.String() + ".cum");
		
		} catch (YAML::ParserException e) {

			LogError("Failed to Read The Scene Meta Data file\n{}\n    {}", scene->path.String(), e.what());
			return;

		}

		YAML::Node objects = main["Objects"];
		for (int i = 0; i < objects.size(); i++) {

			objectIDs.push_back(objects[i].as<uint32_t>());

		}

	}

}