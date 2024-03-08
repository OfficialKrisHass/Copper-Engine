#pragma once

#include <Copper.h>

namespace Editor::AssetFile {

	void SerializeMaterial(const Copper::fs::path& path, const Copper::Material& material);
	Copper::Material DeserializeMaterial(const Copper::fs::path& path, const Copper::UUID& uuid);

}