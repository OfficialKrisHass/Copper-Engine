#pragma once

#include <Copper.h>

namespace Editor::ProjectFileWatcher {

	enum class FileChangeType : Copper::uint32;

	void Start();
	void Stop();

	void SetDirectory(const Copper::fs::path& directory);
	
	void AddFileChangeCallback(std::function<void(const Copper::fs::path&, const FileChangeType& changeType)> func);

	enum class FileChangeType : Copper::uint32 {

		Created,
		Changed,
		Deleted,

		RenamedOldName,
		RenamedNewName,

	};

}