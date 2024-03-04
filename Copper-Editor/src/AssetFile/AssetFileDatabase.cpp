#include "AssetFileDatabase.h"

#include "Core/ProjectFileWatcher.h"
#include "Core/EditorApp.h"

#include "AssetFile/Deserializer.h"

#include "Engine/AssetStorage/AssetList.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include <yaml-cpp/yaml.h>

namespace Editor::AssetFileDatabase {

	using namespace Copper;

	std::unordered_map<fs::path, void*> assetFiles;

	void LoadAsset(const fs::path& path, const std::string& extension);

	void FileWatchCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType type);

	bool CheckExtension(const std::string& extension);

	void Initialize() {

		CUP_FUNCTION();

		ProjectFileWatcher::AddFileChangeCallback(FileWatchCallback);
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

	void* GetAssetFromPath(const fs::path& path) {
		
		CUP_FUNCTION();
		if (assetFiles.find(path) == assetFiles.end()) {

			LogError("No asset at path '{}' exists, or is not loaded", path);
			return nullptr;

		}

		return assetFiles.at(path);
	
	}

	void LoadAsset(const fs::path& path, const std::string& extension) {

		CUP_FUNCTION();

		void* asset = nullptr;
		if (extension == ".png" || extension == ".jpg")
			asset = AssetStorage::CreateAsset<Texture>(path.string());
		else if (extension == ".mat")
			asset = AssetFile::DeserializeMaterial(path);

		CU_ASSERT(asset, "Didn't load the Asset at path '{}'", path.string());

		assetFiles[path] = asset;

	}

	void FileWatchCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType type) {

		CUP_FUNCTION();

		if (type == ProjectFileWatcher::FileChangeType::RenamedNewName || type == ProjectFileWatcher::FileChangeType::RenamedOldName) return;

		std::string extension = path.extension().string();
		if (extension != ".png" && extension != ".jpg") return;

		if (type == ProjectFileWatcher::FileChangeType::Created || type == ProjectFileWatcher::FileChangeType::Changed)
			LoadAsset(path, extension);
		else
			assetFiles.erase(path);

	}

	bool CheckExtension(const std::string& extension) {

		return extension == ".png" || extension == ".jpg" || // Textures
			   extension == ".mat"; // Materials

	}

}