#include "Deserializer.h"

#include <Engine/AssetStorage/AssetMap.h>
#include <Engine/AssetStorage/AssetStorage.h>

#include <yaml-cpp/yaml.h>

namespace Editor::AssetFile {

	using namespace Copper;

	Material* DeserializeMaterial(const Copper::fs::path& path) {

		Material* ret = AssetStorage::CreateAsset<Material>();

		YAML::Node node;
		try { node = YAML::LoadFile(path.string()); } catch (YAML::Exception e) {

			LogError("An exception occured when trying to read Material asset file. Exception message:\n\t{}\n\n\tpath: {}", e.msg, path.string());
			return nullptr;

		}

		ret->texture = (Texture*) Texture::WhiteTexture();
		ret->albedo = node["Albedo"].as<Color>();
		ret->tiling = node["Tiling"].as<float>();

		return ret;

	}

}