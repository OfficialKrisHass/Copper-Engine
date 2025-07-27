#include "ProjectAssetDatabase.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"
#include "Projects/ProjectMetadata.h"

#include "Assets/Serializer.h"
#include "Assets/Model.h"

#include <Engine/AssetStorage/AssetStorage.h>

#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

#include <yaml-cpp/yaml.h>

namespace Editor::ProjectAssetDatabase {

    using namespace Copper;

    std::unordered_map<fs::path, UUID> assetFiles;
    std::unordered_map<UUID, std::string> assetNames;

    std::string emptyString = "";

    void LoadAsset(const fs::path& path, AssetType type, bool newAsset = false);
    void RemoveAsset(const fs::path& path, AssetType type);

    void Initialize() {

        CUP_FUNCTION();

        ProjectMetadata::Deserialize(assetFiles);
        Refresh();

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tProject asset database initialized.");
        else
#endif
            Log("Project asset database initialized.");

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
            Log("Project asset database refreshed.");
#endif

    }
    void Shutdown() {

        CUP_FUNCTION();

        Save();

    }

    void Unload() {

        CUP_FUNCTION();

        Log("Unloading project asset database");

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

    void OnAssetChange(const fs::path& path, FileChangeType changeType, AssetType type) {

        CUP_FUNCTION();

        if (!IsDatabaseAsset(type)) return;

        switch (changeType) {

        case FileChangeType::Created:
        case FileChangeType::Changed:
        case FileChangeType::RenamedNew: LoadAsset(path, type, true); break;
        case FileChangeType::Deleted:
        case FileChangeType::RenamedOld: RemoveAsset(path, type); break;
        default: break;

        }

    }

    void LoadAsset(const fs::path& path, AssetType type, bool newAsset) {

        CUP_FUNCTION();

        CU_ASSERT(IsDatabaseAsset(type), "AssetType '{}' is not a databse asset!", static_cast<uint8>(type));

        const fs::path fullPath = GetProject().GetAssetsPath() / path;

        // Get the asset uuid (or generate a new one if required)

        UUID uuid; 
        if (const auto& it = assetFiles.find(path); it != assetFiles.end())
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

                AssetStorage::InsertAsset<Texture>(uuid, fullPath);
                break;

            }
            case AssetType::Material: {

                if (!AssetFile::DeserializeMaterial(fullPath, uuid).IsValid()) return;
                break;

            }
            case AssetType::Model: {

                AssetStorage::InsertAsset<Model>(uuid, path);
                break;

            }
            default: break;

        }

        // Update project asset database

        assetNames[uuid] = path.filename().string();

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\t\tAsset '{}' ({}) loaded.", uuid.ToString(), path.filename().string());
        else
#endif
            Log("Asset '{}' ({}) loaded.", uuid.ToString(), path.filename().string());

    }
    void RemoveAsset(const fs::path& path, AssetType type) {

        CUP_FUNCTION();

        CU_ASSERT(IsDatabaseAsset(type), "AssetType '{}' is not a databse asset!", static_cast<uint8>(type));

        const auto& it = assetFiles.find(path);
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

        assetNames.erase(uuid);
        assetFiles.erase(path);

        Log("Asset '{}' ({}) removed.", uuid.ToString(), path.filename().string());

    }

    const UUID& GetAssetFromPath(const fs::path& path) {
        
        CUP_FUNCTION();

        const auto& it = assetFiles.find(path);
        if (it == assetFiles.end()) {

            LogError("Asset {} isn't loaded.", path);
            return UUID::GetInvalid();

        }

        return it->second;
    
    }
    const std::string& GetAssetName(const Copper::UUID& uuid) {

        CUP_FUNCTION();
        
        const auto& it = assetNames.find(uuid);
        if (it == assetNames.end()) {

            LogError("Asset with uuid {} isn't loaded.", uuid.ToString());
            return emptyString;

        }
        return it->second;

    }

}
