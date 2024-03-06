#pragma once

#include <Copper.h>

namespace Editor::AssetFile {

	Copper::Material DeserializeMaterial(const Copper::fs::path& path);

}