#include "AssetFileDatabase.h"

#include "Core/ProjectFileWatcher.h"
#include "Core/EditorApp.h"

#include "Engine/AssetStorage/AssetList.h"
#include "Engine/AssetStorage/AssetStorage.h"

namespace Editor::AssetFileDatabase {

	using namespace Copper;

	std::unordered_map<fs::path, void*> assetFiles;

	void FileWatchCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType type);

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
		CU_ASSERT(dir != "", "Project has no Assets path set");

		for (const fs::directory_entry& entry : fs::recursive_directory_iterator(dir)) {

			// Filter all entries that aren't an Asset file

			if (entry.is_directory()) continue;
			fs::path path = entry.path();

			std::string extension = path.extension().string();
			if (extension != ".png" && extension != ".jpg") continue;

			// Check if this Asset already exists 

			AssetStorage::AssetList<Texture>::Node* node = AssetStorage::GetAssetList<Texture>().GetNode(0);
			while (node) {

				if (node->data.Path() == path) break;
				node = node->next;

			}

			Texture* asset = node ? &node->data : AssetStorage::CreateAsset<Texture>(path.string());

			// Add it to the map

			assetFiles[path] = asset;

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

	void FileWatchCallback(const fs::path& path, const ProjectFileWatcher::FileChangeType type) {

		CUP_FUNCTION();

		if (type == ProjectFileWatcher::FileChangeType::RenamedNewName || type == ProjectFileWatcher::FileChangeType::RenamedOldName) return;

		std::string extension = path.extension().string();
		if (extension != ".png" && extension != ".jpg") return;

		if (type == ProjectFileWatcher::FileChangeType::Created || type == ProjectFileWatcher::FileChangeType::Changed) {

			// Check if this Asset already exists 

			AssetStorage::AssetList<Texture>::Node* node = AssetStorage::GetAssetList<Texture>().GetNode(0);
			while (node) {

				if (node->data.Path() == path) break;
				node = node->next;

			}

			Texture* asset = node ? &node->data : AssetStorage::CreateAsset<Texture>(path.string());

			// Add it to the map

			assetFiles[path] = asset;

		} else
			assetFiles.erase(path);

	}

}