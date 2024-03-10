#include "Serializer.h"

#include "AssetFile/MetaFile.h"

#include <Engine/AssetStorage/AssetMap.h>
#include <Engine/AssetStorage/AssetStorage.h>

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Editor::AssetFile {

	using namespace Copper;

	void SerializeMaterial(const fs::path& path, const MaterialAsset& material) {

		CUP_FUNCTION();

		// Prepare

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		// Asset

		out << YAML::Key << "Texture" << YAML::Value << material->texture;
		out << YAML::Key << "Albedo" << YAML::Value << material->albedo;
		out << YAML::Key << "Tiling" << YAML::Value << material->tiling;

		out << YAML::EndMap; // Main

		// Finish

		std::ofstream file(path);
		file << out.c_str();
		file.close();

	}
	MaterialAsset DeserializeMaterial(const fs::path& path, const UUID& uuid) {

		CUP_FUNCTION();

		YAML::Node node;
		try { node = YAML::LoadFile(path.string()); } catch (YAML::Exception e) {

			LogError("An exception occured when trying to read Material asset file. Exception message:\n\t{}\n\n\tpath: {}", e.msg, path.string());
			return MaterialAsset();

		}

		MaterialAsset ret = AssetStorage::InsertAsset<Material>(uuid);

		ret->texture = node["Texture"].as<TextureAsset>();
		ret->albedo = node["Albedo"].as<Color>();
		ret->tiling = node["Tiling"].as<float>();

		return ret;

	}

}