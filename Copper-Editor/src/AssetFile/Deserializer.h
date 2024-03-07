#pragma once

#include <Copper.h>

namespace Editor::AssetFile {

	Copper::Material DeserializeMaterial(const Copper::fs::path& path, const Copper::UUID& uuid);
	void SerializeMaterial(const Copper::fs::path& path, const Copper::Material& material);

}