#include "ProjectFileWatcher.h"

#include <FileWatch/FileWatch.hpp>

using namespace Copper;

// TODO : THIS SHIT IS NOT WORKING FOR SOME REASON, WTF

namespace Editor::ProjectFileWatcher {

	struct ProjectFileWatcherData {

		fs::path directory;
		bool running;

		Unique<filewatch::FileWatch<std::string>> fw;

		std::vector<std::function<void(const fs::path&, const FileChangeType& changeType)>> callbacks;

	};
	ProjectFileWatcherData data;

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType);

	void Start() {

		if (data.directory.empty()) { LogError("Can't FileWatch an empty Directory!"); return; }

		data.running = true;
		data.fw = CreateUnique<filewatch::FileWatch<std::string>>(data.directory.string(), FileChangeCallback);

	}
	void Stop() {

		data.running = false;
		data.fw.reset();

	}

	static void FileChangeCallback(const std::string& path, const filewatch::Event changeType) {

		if (!data.running) return;

		fs::path fsPath(path);
		
		if (fsPath.extension() != ".cs")
			return;

		FileChangeType type;
		switch (changeType) {

			case filewatch::Event::added: type = FileChangeType::Created; break;
			case filewatch::Event::removed: type = FileChangeType::Deleted; break;
			case filewatch::Event::modified: type = FileChangeType::Changed; break;
			case filewatch::Event::renamed_old: type = FileChangeType::RenamedOldName; break;
			case filewatch::Event::renamed_new: type = FileChangeType::RenamedNewName; break;

		}

		for (size_t i = 0; i < data.callbacks.size(); i++) { data.callbacks[i](data.directory / fsPath, type); }

	}

	void SetDirectory(const fs::path& directory) { data.directory = directory; }

	void AddFileChangeCallback(std::function<void(const fs::path&, const FileChangeType& changeType)> func) { data.callbacks.push_back(func); }

}