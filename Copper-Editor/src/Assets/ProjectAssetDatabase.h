#pragma once

#include <Copper.h>

namespace Editor::ProjectAssetDatabase {

    void Initialize();
    void Refresh();
    void Shutdown();

    void Unload();

    void Save();

    void AddAsset(const Copper::fs::path& path, const Copper::UUID& uuid);
    void RemoveAsset(const Copper::fs::path& path);

    const Copper::UUID& GetAssetFromPath(const Copper::fs::path& path);
    template<typename T> inline T& GetAssetFromPath(const Copper::fs::path& path) { return *(T*) &GetAssetFromPath(path); } // AssetPtr is basically just an UUID with functions, so we can do this

    const std::string& GetAssetName(const Copper::UUID& uuid);
    template<typename T> inline const std::string& GetAssetName(const T& asset) { return GetAssetName(asset.AssetUUID()); }

}
