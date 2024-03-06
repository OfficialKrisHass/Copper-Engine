#pragma once

#include <Copper.h>

namespace Editor::AssetFileDatabase {

	void Initialize();
	void Refresh();

	const Copper::UUID& GetAssetFromPath(const Copper::fs::path& path);
	template<typename T> T& GetAssetFromPath(const Copper::fs::path& path) { return *(T*) &GetAssetFromPath(path); } // AssetPtr is basically just an UUID with functions, so we can do this

}