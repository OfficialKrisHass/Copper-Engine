#include "ProjectAssetDatabase.h"

#include "Core/FileWatcher.h"
#include "Core/EditorApp.h"

#include "Assets/Serializer.h"
#include "Assets/AssetMeta.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include <yaml-cpp/yaml.h>

namespace Editor::ProjectAssetDatabase {

	using namespace Copper;

	std::unordered_map<fs::path, UUID> assetFiles;
	std::unordered_map<UUID, std::string> assetNames;

	UUID invalidUUID = UUID("");
	std::string emptyString = "";

	static void FileChangeCallback(const fs::path& path, const FileWatcher::FileChangeType changeType);

	void LoadAsset(const fs::path& path, const std::string& extension);
	void RemoveAsset(const fs::path& path, const std::string& extension);

	bool CheckExtension(const std::string& extension);

	void Initialize() {

		CUP_FUNCTION();

		Refresh();
		FileWatcher::AddCallback(FileChangeCallback);

	}
	void Refresh() {

		CUP_FUNCTION();

		assetFiles.clear();

		CU_ASSERT(GetProject(), "Current project is invalid, make sure you called AssetFileDatabase::Refresh when there is a valid project");

		const fs::path& dir = GetProject().assetsPath;
		CU_ASSERT(dir != "", "Project has no Assets path");

		for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dir)) {

			// Filter all entries that aren't an Asset file

			if (entry.is_directory()) continue;
			fs::path path = entry.path();

			std::string extension = path.extension().string();
			if (!CheckExtension(extension)) continue;

			LoadAsset(path, extension);

		}

	}

	const UUID& GetAssetFromPath(const fs::path& path) {
		
		CUP_FUNCTION();

		if (assetFiles.find(path) == assetFiles.end()) {

			LogError("No asset at path '{}' exists, or is not loaded", path);
			return invalidUUID;

		}

		return assetFiles.at(path);
	
	}
	const std::string& GetAssetName(const Copper::UUID& uuid) {

		CUP_FUNCTION();
		
		if (assetNames.find(uuid) == assetNames.end()) {

			LogError("No asset with uuid '{}' exists, or is not loaded", uuid.str());
			return emptyString;

		}
		return assetNames.at(uuid);

	}

	void FileChangeCallback(const fs::path& path, const FileWatcher::FileChangeType changeType) {

		CUP_FUNCTION();

		const std::string extension = path.extension().string();
		if (!CheckExtension(extension)) return;

		if (changeType == FileWatcher::FileChangeType::Created || changeType == FileWatcher::FileChangeType::RenamedNewName)
			LoadAsset(path, extension);
		else if (changeType == FileWatcher::FileChangeType::Deleted || changeType == FileWatcher::FileChangeType::RenamedOldName)
			RemoveAsset(path, extension);


	}

	void LoadAsset(const fs::path& path, const std::string& extension) {

		CUP_FUNCTION();

		AssetMeta meta;
		meta.Deserialize(path.string() + ".cum");
		UUID assetUUID = meta.AssetUUID();

		if (extension == ".png" || extension == ".jpg")
			AssetStorage::InsertAsset<Texture>(assetUUID, path.string());
		else if (extension == ".mat")
			AssetFile::DeserializeMaterial(path, assetUUID);

		CU_ASSERT(assetUUID != UUID(""), "Didn't load the Asset at path '{}'", path.string());

		assetFiles[path] = assetUUID;
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

	bool CheckExtension(const std::string& extension) {

		return extension == ".png" || extension == ".jpg" || // Textures
			   extension == ".mat"; // Materials

	}

}