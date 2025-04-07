#include "ProjectAssetDatabase.h"

#include "Core/FileWatcher.h"
#include "Core/EditorApp.h"

#include "Projects/Project.h"
#include "Projects/ProjectMetadata.h"

#include "Assets/Serializer.h"
#include "Assets/AssetMeta.h"
#include "Assets/ModelLoader.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include <yaml-cpp/yaml.h>

namespace Editor::ProjectAssetDatabase {

    using namespace Copper;

    std::unordered_map<fs::path, UUID> assetFiles;
    std::unordered_map<UUID, std::string> assetNames;

    std::string emptyString = "";

    static void FileChangeCallback(const fs::path& path, const FileWatcher::FileChangeType changeType);

    void LoadAsset(const fs::path& path, const std::string& extension);
    void RemoveAsset(const fs::path& path, const std::string& extension);

    bool CheckExtension(const std::string& extension);

    void Initialize() {

        CUP_FUNCTION();

        FileWatcher::AddCallback(FileChangeCallback);
 
        ProjectMetadata::Deserialize(assetFiles);
        Refresh();

    }
    void Refresh() {

        CUP_FUNCTION();
        CU_ASSERT(GetProject(), "Current project is invalid, make sure you called AssetFileDatabase::Refresh when there is a valid project");

        const fs::path& dir = GetProject().GetAssetsPath();
        CU_ASSERT(dir != "", "Project has no Assets path");

        for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dir)) {

            // Filter all entries that aren't an Asset file

            if (entry.is_directory()) continue;
            fs::path path = fs::relative(entry.path(), dir);

            std::string extension = path.extension().string();
            if (!CheckExtension(extension)) continue;

            LoadAsset(path, extension);

        }

    }
    void Shutdown() {

        CUP_FUNCTION();

        ProjectMetadata::Serialize(assetFiles);

    }

    void LoadAsset(const fs::path& path, const std::string& extension) {

        CUP_FUNCTION();

        UUID assetUUID; 
        if (assetFiles.find(path) != assetFiles.end())
            assetUUID = assetFiles.at(path);
        else {

            LogWarn("{} was not loaded from ProjectMetadata.cum, creating new UUID", path);

            UUID::Generate(assetUUID);
            assetFiles[path] = assetUUID;

        }

        CU_ASSERT(assetUUID != UUID::GetInvalid(), "Invalid UUID loaded for asset {}", path);

        if (extension == ".png" || extension == ".jpg")
            AssetStorage::InsertAsset<Texture>(assetUUID, GetProject().GetAssetsPath() / path);
        else if (extension == ".mat" && !AssetFile::DeserializeMaterial(GetProject().GetAssetsPath() / path, assetUUID)) return;
        else if (extension == ".fbx")
            ModelLoader::Load(GetProject().GetAssetsPath() / path, assetUUID);

        assetNames[assetUUID] = path.filename().string();

    }
    void RemoveAsset(const fs::path& path, const std::string& extension) {

        CUP_FUNCTION();

        if (!assetFiles.contains(path)) {

            LogError("Can't remove an asset that is not loaded!\n\tPath: {}", path.string());
            return;

        }

        const UUID& uuid = assetFiles.at(path);

        // Delete actual asset

        if (extension == ".png" || extension == ".jpg")
            AssetStorage::DeleteAsset<Texture>(uuid);
        else if (extension == ".mat")
            AssetStorage::DeleteAsset<Material>(uuid);

        assetNames.erase(uuid);
        assetFiles.erase(path);

    }

    void FileChangeCallback(const fs::path& path, const FileWatcher::FileChangeType changeType) {

        CUP_FUNCTION();

        const std::string extension = path.extension().string();
        if (!CheckExtension(extension)) return;

        if (changeType == FileWatcher::FileChangeType::Created || changeType == FileWatcher::FileChangeType::RenamedNewName)
            LoadAsset(path, extension);
        else if (changeType == FileWatcher::FileChangeType::Deleted || changeType == FileWatcher::FileChangeType::RenamedOldName)
            RemoveAsset(path, extension);

        ProjectMetadata::Serialize(assetFiles);


    }

    const UUID& GetAssetFromPath(const fs::path& path) {
        
        CUP_FUNCTION();

        if (assetFiles.find(path) == assetFiles.end()) {

            LogError("Can't get an asset that isn't loaded.\n\tPath: {}", path);
            return UUID::GetInvalid();

        }

        return assetFiles.at(path);
    
    }
    const std::string& GetAssetName(const Copper::UUID& uuid) {

        CUP_FUNCTION();
        
        if (assetNames.find(uuid) == assetNames.end()) {

            LogError("No asset with uuid '{}' exists, or is not loaded", uuid.ToString());
            return emptyString;

        }
        return assetNames.at(uuid);

    }



    bool CheckExtension(const std::string& extension) {

        return extension == ".png" || extension == ".jpg" || // Textures
               extension == ".mat" || // Materials
               extension == ".fbx"; // Models

    }

}
