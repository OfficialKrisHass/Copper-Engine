#include "AssetFileDatabase.h"

#include "Core/ProjectFileWatcher.h"
#include "Core/EditorApp.h"

#include "AssetFile/Serializer.h"
#include "AssetFile/MetaFile.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include <yaml-cpp/yaml.h>

namespace Editor::AssetFileDatabase {

	using namespace Copper;

	std::unordered_map<fs::path, UUID> assetFiles;
	std::unordered_map<UUID, std::string> assetNames;

	void LoadAsset(const fs::path& path, const std::string& extension);

	bool CheckExtension(const std::string& extension);

	void Initialize() {

		CUP_FUNCTION();

		Refresh();

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
			return UUID("");

		}

		return assetFiles.at(path);
	
	}
	const std::string& GetAssetName(const Copper::UUID& uuid) {

		CUP_FUNCTION();
		
		if (assetNames.find(uuid) == assetNames.end()) return "";
		return assetNames.at(uuid);

	}

	void LoadAsset(const fs::path& path, const std::string& extension) {

		CUP_FUNCTION();

		MetaFile meta;
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

	bool CheckExtension(const std::string& extension) {

		return extension == ".png" || extension == ".jpg" || // Textures
			   extension == ".mat"; // Materials

	}

}