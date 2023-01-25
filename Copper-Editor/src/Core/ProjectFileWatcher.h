#pragma once

#include <Copper.h>

namespace Editor::ProjectFileWatcher {

	enum class FileChangeType : int;

	void Start();
	void Stop();

	void SetDirectory(const std::filesystem::path& directory);

	void AddFilter(const std::string& filter);
	void AddFileChangeCallback(std::function<void(const std::filesystem::path&, const FileChangeType& changeType)> func);

	enum class FileChangeType : int {

		Created,
		Changed,
		Deleted,

		RenamedOldName,
		RenamedNewName,

	};

}