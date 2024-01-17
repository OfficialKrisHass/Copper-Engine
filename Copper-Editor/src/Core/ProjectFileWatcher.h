#pragma once

#include <Copper.h>

namespace Editor::ProjectFileWatcher {

	enum class FileChangeType : int;

	void Start();
	void Stop();

	void SetDirectory(const fs::path& directory);
	
	void AddFileChangeCallback(std::function<void(const fs::path&, const FileChangeType& changeType)> func);

	enum class FileChangeType : int {

		Created,
		Changed,
		Deleted,

		RenamedOldName,
		RenamedNewName,

	};

}