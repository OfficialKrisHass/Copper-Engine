#pragma once

#include <Copper.h>

namespace Editor::ProjectFileWatcher {

	enum class FileChangeType : Copper::uint32 {

		Created,
		Changed,
		Deleted,

		RenamedOldName,
		RenamedNewName,

	};

	typedef std::function<void(const Copper::fs::path&, const FileChangeType)> Callback;

	void Start();
	void PollCallbacks();
	void Stop();

	void SetDirectory(const Copper::fs::path& directory);
	
	void AddCallback(Callback callback);

}