#pragma once

#include <Copper.h>

#include <Engine/Filesystem/FileChangeType.h>

namespace Editor::ProjectAssetDatabase {

    using namespace Copper;

    void Initialize();
    void Refresh();
    void Shutdown();

    void Unload();

    void Save();

    void AddAsset(const Copper::fs::path& path, const Copper::UUID& uuid);
    void RemoveAsset(const Copper::fs::path& path);

    void OnAssetChange(const fs::path& path, const FileChangeType changeType);

    const Copper::UUID& GetAssetFromPath(const Copper::fs::path& path);
    template<typename T> inline T& GetAssetFromPath(const Copper::fs::path& path) { return *(T*) &GetAssetFromPath(path); } // AssetPtr is basically just an UUID with functions, so we can do this

    const std::string& GetAssetName(const Copper::UUID& uuid);
    template<typename T> inline const std::string& GetAssetName(const T& asset) { return GetAssetName(asset.AssetUUID()); }

}
