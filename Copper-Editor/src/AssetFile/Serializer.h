#pragma once

#include <Copper.h>

namespace Editor::AssetFile {

	void SerializeMaterial(const Copper::fs::path& path, const Copper::MaterialAsset& material);
	Copper::MaterialAsset DeserializeMaterial(const Copper::fs::path& path, const Copper::UUID& uuid);

}