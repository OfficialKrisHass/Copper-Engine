#pragma once

#include <Copper.h>

namespace Editor::AssetFileDatabase {

	void Initialize();
	void Refresh();

	void* GetAssetFromPath(const Copper::fs::path& path);
	template<typename T> T* GetAssetFromPath(const Copper::fs::path& path) { return (T*) GetAssetFromPath(path); }

}