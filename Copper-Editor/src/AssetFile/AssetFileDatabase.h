#pragma once

#include <Copper.h>

namespace Editor::AssetFileDatabase {

	void Initialize();
	void Refresh();

	const Copper::UUID& GetAssetFromPath(const Copper::fs::path& path);
	template<typename T> inline T& GetAssetFromPath(const Copper::fs::path& path) { return *(T*) &GetAssetFromPath(path); } // AssetPtr is basically just an UUID with functions, so we can do this

	const std::string& GetAssetName(const Copper::UUID& uuid);
	template<typename T> inline const std::string& GetAssetName(const T& asset) { return GetAssetName(asset.AssetUUID()); }

}