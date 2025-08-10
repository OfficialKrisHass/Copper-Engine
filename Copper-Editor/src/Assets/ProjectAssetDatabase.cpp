#include "ProjectAssetDatabase.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"
#include "Projects/ProjectMetadata.h"

#include "Assets/Serializer.h"
#include "Assets/Model.h"

#include <Engine/Filesystem/WatchConstants.h>

#include <Engine/AssetStorage/AssetStorage.h>

#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

#include <yaml-cpp/yaml.h>

#include <chrono>

namespace Editor::ProjectAssetDatabase {

    using namespace Copper;

    using Timestamp = std::chrono::steady_clock::time_point;

    struct RenameInfo {

        fs::path otherPath;
        FileChangeType type;
        Timestamp timestamp;

    };

    std::unordered_map<fs::path, UUID> assetFiles;
    std::unordered_map<UUID, std::string> assetNames;

    std::unordered_map<uint32, RenameInfo> renameMap;

    std::string emptyString = "";

    void LoadAsset(const fs::path& path, AssetType type, bool newAsset = false);
    void DeleteAsset(const fs::path& path, AssetType type);

    void HandleAssetRename(const fs::path& oldPath, const fs::path& newPath);
    void HandleUnpairedRename(const RenameInfo& info);

    void Initialize() {

        CUP_FUNCTION();

        ProjectMetadata::Deserialize(assetFiles);
        Refresh();

        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tProject asset database initialized.");
        else
            LogStatus("Project asset database initialized.");

    }
    void Update() {

        CUP_FUNCTION();

        Timestamp now = std::chrono::steady_clock::now();
        for (auto it = renameMap.begin(); it != renameMap.end();) {

            const RenameInfo& info = it->second;
            if (now - info.timestamp >= std::chrono::milliseconds(SLEEP_LENGTH * 2)) {

                HandleUnpairedRename(info);
                it = renameMap.erase(it);

                continue;

            }
            
            ++it;

        }

    }
    void Shutdown() {

        CUP_FUNCTION();

        Save();

    }

    void Refresh() {

        CUP_FUNCTION();
        CU_ASSERT(GetProject().IsValid(), "Current project is invalid.");

        const fs::path& dir = GetProject().GetAssetsPath();
        CU_ASSERT(dir != "", "Project has no Assets path.");
        CU_ASSERT(fs::exists(dir), "Project assets path does not exist. {}", dir);

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dir)) {

            // Filter all entries that aren't an Asset file

            if (entry.is_directory()) continue;

            AssetType type = GetAssetTypeFromExtension(entry.path().extension().string());
            if (!IsDatabaseAsset(type)) continue;

            fs::path path = fs::relative(entry.path(), dir);
            LoadAsset(path, type);

        }

#ifdef CU_DEBUG 
        if (GetEngineState() != EngineState::PostInitialization)
            LogStatus("Project asset database refreshed.");
#endif

    }
    void Unload() {

        CUP_FUNCTION();

        LogStatus("Unloading project asset database");

        // Hack, I can't think of a better way of solving this unfortunately
        for (auto& it : assetFiles) {

            std::string extension = it.first.extension().string();
            if (extension == ".png" || extension == ".jpg")
                AssetStorage::GetAssetMap<Texture>().Delete(it.second);
            else if (extension == ".mat")
                AssetStorage::GetAssetMap<Material>().Delete(it.second);
            else if (extension == ".fbs")
                AssetStorage::GetAssetMap<Model>().Delete(it.second);

        }

        assetFiles.clear();
        assetNames.clear();

    }

    void Save() {

        CUP_FUNCTION();

        ProjectMetadata::Serialize(assetFiles);

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::Shutdown)
            LogStatus("\tProject asset database saved.");
#endif

    }

    void AddAsset(const fs::path& path, const UUID &uuid) {

        CUP_FUNCTION();

        if (assetFiles.find(path) != assetFiles.end()) {

            LogError("Asset '{}' ({}) is already loaded.", uuid.ToString(), path.filename().string());
            return;

        }

        assetFiles[path] = uuid;
        assetNames[uuid] = path.filename().string();

    }
    void RemoveAsset(const Copper::fs::path& path) {

        CUP_FUNCTION();

        if (assetFiles.find(path) == assetFiles.end()) {

            LogError("Asset '{}' is not loaded.", path.string());
            return;

        }

        assetNames.erase(assetFiles[path]);
        assetFiles.erase(path);

    }

    void OnAssetChange(const fs::path& path, FileChangeType changeType, AssetType type, uint32 cookie) {

        CUP_FUNCTION();

        // Don't forget to clear the renameMap every few seconds.

        if (!IsDatabaseAsset(type)) return;

        switch (changeType) {

        case FileChangeType::Created: LoadAsset(path, type, true); break;
        case FileChangeType::Changed: LoadAsset(path, type); break;
        case FileChangeType::Deleted: DeleteAsset(path, type); break;

        case FileChangeType::RenamedOld:
        case FileChangeType::RenamedNew: {

            const auto it = renameMap.find(cookie);
            if (it == renameMap.end()) {

                RenameInfo& info = renameMap[cookie];
                info.otherPath = path;
                info.type = changeType;
                info.timestamp = std::chrono::steady_clock::now();

            } else {

                RenameInfo& info = it->second;
                CU_ASSERT(info.type != changeType, "2 Rename events of the same type with the same cookie! Cookie: {}, path: '{}', other path: '{}'", cookie, path, info.otherPath);

                if (changeType == FileChangeType::RenamedNew)
                    HandleAssetRename(info.otherPath, path);
                else
                    HandleAssetRename(path, info.otherPath);

                renameMap.erase(it);

            }

            break;

        }
        default: break;

        }

    }

    void LoadAsset(const fs::path& path, AssetType type, bool newAsset) {

        CUP_FUNCTION();

        CU_ASSERT(IsDatabaseAsset(type), "AssetType '{}' is not a databse asset!", static_cast<uint8>(type));

        const fs::path fullPath = GetProject().GetAssetsPath() / path;

        // Get the asset uuid (or generate a new one if required)

        UUID uuid; 
        if (const auto it = assetFiles.find(path); it != assetFiles.end())
            uuid = it->second;
        else {

#ifdef CU_DEBUG
            if (!newAsset)
                LogWarn("Asset {} was not loaded from ProjectMetadata.cu, creating new UUID", path);
#endif

            UUID::Generate(uuid);
            assetFiles[path] = uuid;

        }
        CU_ASSERT(uuid.IsValid(), "Invalid UUID loaded for asset {}", path);

        // Load and store the asset

        switch (type) {

            case AssetType::Texture: {

                AssetStorage::InsertAsset<Texture>(uuid)->Create(fullPath);
                break;

            }
            case AssetType::Material: {

                if (!AssetFile::DeserializeMaterial(fullPath, uuid).IsValid()) return;
                break;

            }
            case AssetType::Model: {

                AssetStorage::InsertAsset<Model>(uuid)->Load(path);
                break;

            }
            default: break;

        }

        // Update project asset database

        assetNames[uuid] = path.filename().string();

        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\t\tAsset '{}' ({}) loaded.", uuid.ToString(), path.filename().string());
        else
            LogStatus("Asset '{}' ({}) loaded.", uuid.ToString(), path.filename().string());

    }
    void DeleteAsset(const fs::path& path, AssetType type) {

        CUP_FUNCTION();

        CU_ASSERT(IsDatabaseAsset(type), "AssetType '{}' is not a databse asset!", static_cast<uint8>(type));

        const auto it = assetFiles.find(path);
        if (it == assetFiles.end()) {

            LogError("Can't remove an asset that is not loaded! Asset: {}", path);
            return;

        }

        const UUID& uuid = it->second;

        // Delete actual asset

        switch (type) {

            case AssetType::Texture: {

                AssetStorage::DeleteAsset<Texture>(uuid);
                break;

            }
            case AssetType::Material: {

                AssetStorage::DeleteAsset<Material>(uuid);
                break;

            }
            case AssetType::Model: {

                AssetStorage::DeleteAsset<Model>(uuid);
                break;

            }
            default: break;

        } 

        Log("Asset '{}' ({}) removed.", uuid.ToString(), path.filename().string());

        assetNames.erase(uuid);
        assetFiles.erase(path);

    }

    void HandleAssetRename(const fs::path& oldPath, const fs::path& newPath) {

        CUP_FUNCTION();

        // Setup

        AssetType type = GetAssetTypeFromExtension(newPath.extension().string());
        AssetType oldType = GetAssetTypeFromExtension(oldPath.extension().string());

        CU_ASSERT(oldType == type, "Rename event that changed asset types occured! oldPath: '{}', newPath: '{}'", oldPath, newPath);
        CU_ASSERT(IsDatabaseAsset(type), "AssetType '{}' is not a database asset!", static_cast<uint8>(type));

        // Find asset

        const auto oldIt = assetFiles.find(oldPath);
        CU_ASSERT(oldIt != assetFiles.end(), "Old path for rename event is not present in the PAD. Old path: '{}', new path: '{}'", oldPath, newPath);

        const UUID& uuid = oldIt->second;
        CU_ASSERT(uuid.IsValid(), "Invalid UUID was retrieved from PAD at old path for rename event. Old path: '{}', new path '{}'", oldPath, newPath);

        // Update to new path and name

        assetFiles[newPath] = uuid;
        assetFiles.erase(oldIt);

        assetNames[uuid] = newPath.filename().string();

        // TODO: Very temporary, remove asap pls
        //       In the future, models will physically unpackage themself into physical files, in which case this is not required
        //       but at the moment they are stored as subassets of the model and don't physically exist.
        if (type == AssetType::Model)
            AssetStorage::GetAsset<Model>(uuid)->Rename(newPath, assetFiles, assetNames);

    }
    void HandleUnpairedRename(const RenameInfo& info) {

        CUP_FUNCTION();

        // File was moved outside of the assets directory, or deleted (some file browsers generate move events instead of delete events)
        // Handled as a Deleted event.
        if (info.type == FileChangeType::RenamedOld) {

            const fs::path& path = info.otherPath;
            AssetType type = GetAssetTypeFromExtension(path.extension().string());

            CU_ASSERT(assetFiles.contains(path), "Could not find asset in PAD while trying to resolve unpaired RenamedOld event. Path: '{}'", path);
            DeleteAsset(path, type);

            return;

        }

        // File was moved into the assets directory from outside. Handled as a Created event

        CU_ASSERT(info.type == FileChangeType::RenamedNew, "Invalid Rename type in RenameInfo structure while trying to resolve unpaired event. Path: '{}', change type: '{}'", info.otherPath, FileChangeTypeToString(info.type));

        const fs::path& path = info.otherPath;
        AssetType type = GetAssetTypeFromExtension(path.extension().string());

        CU_ASSERT(!assetFiles.contains(path), "PAD already contains asset from unpaired RenamedNew event. Path: '{}', Asset: {} ({})", path, assetFiles[path].ToString(), assetNames[assetFiles[path]]);
        LoadAsset(path, type, true);

    }

    const UUID& GetAssetFromPath(const fs::path& path) {
        
        CUP_FUNCTION();

        const auto it = assetFiles.find(path);
        if (it == assetFiles.end()) {

            LogError("Asset {} isn't loaded.", path);
            return UUID::GetInvalid();

        }

        return it->second;
    
    }
    const std::string& GetAssetName(const Copper::UUID& uuid) {

        CUP_FUNCTION();
        
        const auto it = assetNames.find(uuid);
        if (it == assetNames.end()) {

            LogError("Asset with uuid {} isn't loaded.", uuid.ToString());
            return emptyString;

        }
        return it->second;

    }

}
